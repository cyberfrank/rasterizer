#include "gfx_api.h"
#include "foundation/array.h"
#include "foundation/math.h"
#include "mesh.h"
#include <float.h> // FLT_MAX

typedef struct Projected_Vertex {
    vec2_t screen_pos;
    float depth;
    float inv_w;
    vec3_t position;
    vec2_t uv;
    vec3_t normal;
    vec3_t tangent;
    float _pad;
} Projected_Vertex;

struct Graphics_Context {
    int width;
    int height;
    uint32_t *color_buf;
    float *depth_buf;
    Vertex_Shader vertex_shader;
    Pixel_Shader pixel_shader;
    mat44_t viewport_transform;
    // Raw data
    Allocator *allocator;
    Buffer *buffers;
    Texture *textures;
    // Bindings
    const void *uniform_bindings[MAX_NUM_UNIFORM_BLOCKS];
    Texture *texture_bindings[MAX_NUM_TEXTURES];
    // Runtime
    Projected_Vertex *projected_vertices;
} *ctx = &(struct Graphics_Context) {
    0
};

static void init(int width, int height)
{
    ctx->allocator = system_allocator;

    const int n = width * height;
    ctx->color_buf = c_alloc(ctx->allocator, n * sizeof(*ctx->color_buf));
    ctx->depth_buf = c_alloc(ctx->allocator, n * sizeof(*ctx->depth_buf));
    ctx->width = width;
    ctx->height = height;

    for (int i = 0; i < n; ++i) {
        ctx->color_buf[i] = 0;
        ctx->depth_buf[i] = FLT_MAX;
    }

    float ww = (float)width * 0.5f;
    float hh = (float)height * 0.5f;
    ctx->viewport_transform = (mat44_t) {
        ww, 0, 0, 0,
        0, -hh, 0, 0,
        0, 0, 1, 0,
        ww, hh, 0, 1,
    };
}

static void shutdown()
{
    Allocator *a = ctx->allocator;
    for (Buffer *it = ctx->buffers; it != array_end(ctx->buffers); ++it) {
        c_free(a, it->data, it->size);
    }
    for (Texture *it = ctx->textures; it != array_end(ctx->textures); ++it) {
        c_free(a, it->data, it->width * it->height * it->channels);
    }
    array_free(ctx->buffers, a);
    array_free(ctx->textures, a);
    array_free(ctx->projected_vertices, a);

    int num_pixels = ctx->width * ctx->height;
    c_free(a, ctx->color_buf, num_pixels * sizeof(*ctx->color_buf));
    c_free(a, ctx->depth_buf, num_pixels * sizeof(*ctx->depth_buf));
}

static void bind_shaders(Vertex_Shader vs, Pixel_Shader ps)
{
    ctx->vertex_shader = vs;
    ctx->pixel_shader = ps;
}

static void bind_uniform_block(uint32_t slot, gfx_id id)
{
    if (slot < MAX_NUM_UNIFORM_BLOCKS) {
        Buffer *buf = &ctx->buffers[id - 1];
        ctx->uniform_bindings[slot] = buf->data;
    }
}

static void bind_texture(uint32_t slot, gfx_id id)
{
    if (slot < MAX_NUM_TEXTURES) {
        Texture *tex = &ctx->textures[id - 1];
        ctx->texture_bindings[slot] = tex;
    }
}

static gfx_id create_texture(const void *data, int width, int height, int channels)
{
    uint64_t bytes = width * height * channels;
    void *mem = c_alloc(ctx->allocator, bytes);
    memcpy(mem, data, bytes);
    Texture texture = {
        .data = mem,
        .width = width,
        .height = height,
        .channels = channels,
    };
    array_push(ctx->textures, texture, ctx->allocator);
    return (gfx_id)array_size(ctx->textures);
}

static gfx_id create_buffer(const void *data, uint64_t size)
{
    void *mem = c_alloc(ctx->allocator, size);
    if (data)
        memcpy(mem, data, size);

    Buffer buffer = {
        .data = mem,
        .size = size,
    };
    array_push(ctx->buffers, buffer, ctx->allocator);
    return (gfx_id)array_size(ctx->buffers);
}

static void update_buffer(gfx_id id, const void *data, uint64_t size, uint64_t offset)
{
    Buffer *buf = &ctx->buffers[id - 1];
    if ((size + offset) <= buf->size)
        memcpy((uint8_t *)buf->data + offset, data, size);
}

static vec3_t calc_barycentric_coords(int x, int y, const vec2_t *p)
{
    const vec3_t a = (vec3_t) {
        .x = p[2].x - p[0].x,
        .y = p[1].x - p[0].x,
        .z = p[0].x - x,
    };
    const vec3_t b = (vec3_t) {
        .x = p[2].y - p[0].y,
        .y = p[1].y - p[0].y,
        .z = p[0].y - y,
    };
    const vec3_t u = vec3_cross(a, b);
    const float s = 1.0f / u.z;
    return (vec3_t) {
        s * (u.z - (u.x + u.y)),
        s * u.y,
        s * u.x,
    };
}

static inline void vec3_interpolate_3(vec3_t *out, const vec3_t p0, const vec3_t p1, const vec3_t p2, vec3_t uvw)
{
    *out = (vec3_t) {
        .x = p0.x * uvw.x + p1.x * uvw.y + p2.x * uvw.z,
        .y = p0.y * uvw.x + p1.y * uvw.y + p2.y * uvw.z,
        .z = p0.z * uvw.x + p1.z * uvw.y + p2.z * uvw.z,
    };
}

static inline void vec2_interpolate_3(vec2_t *out, vec2_t p0, vec2_t p1, vec2_t p2, vec3_t uvw)
{
    *out = (vec2_t) {
        .x = p0.x * uvw.x + p1.x * uvw.y + p2.x * uvw.z,
        .y = p0.y * uvw.x + p1.y * uvw.y + p2.y * uvw.z,
    };
}

static void rasterize_triangle(
    const Projected_Vertex v0, const Projected_Vertex v1, const Projected_Vertex v2, 
    const Shader_Bindings *bindings)
{
    struct Bounding_Box {
        int x0, y0;
        int x1, y1;
    } bb = {
        INT_MAX, INT_MAX,
        INT_MIN, INT_MIN,
    };

    const vec2_t points[3] = {
        v0.screen_pos, v1.screen_pos, v2.screen_pos,
    };
    for (int i = 0; i < 3; ++i) {
        bb.x0 = c_max(0, c_min(bb.x0, (int)points[i].x));
        bb.y0 = c_max(0, c_min(bb.y0, (int)points[i].y));
        bb.x1 = c_min(ctx->width - 1, c_max(bb.x1, (int)points[i].x));
        bb.y1 = c_min(ctx->height - 1, c_max(bb.y1, (int)points[i].y));
    }

    if (bb.x1 < bb.x0 || bb.y1 < bb.y0)
        return;

    for (int x = bb.x0; x <= bb.x1; ++x) {
        for (int y = bb.y0; y <= bb.y1; ++y) {
            vec3_t uvw = calc_barycentric_coords(x, y, points);
            if (uvw.x < 0 || uvw.y < 0 || uvw.z < 0)
                continue;

            // Perspective correction factor
            float pc = 1.f / (uvw.x * v0.inv_w + uvw.y * v1.inv_w + uvw.z * v2.inv_w);
            uvw = vec3_mul(uvw, pc);

            // Calculate depth value
            float depth = uvw.x * v0.depth + uvw.y * v1.depth + uvw.z * v2.depth;
            if (depth < 0)
                continue;

            // Check against depth buffer
            int idx = x + y * ctx->width;
            if (depth < ctx->depth_buf[idx]) {
                ctx->depth_buf[idx] = depth;

                // Interpolate attributes
                Vertex v;
                vec3_interpolate_3(&v.position, v0.position, v1.position, v2.position, uvw);
                vec2_interpolate_3(&v.uv, v0.uv, v1.uv, v2.uv, uvw);
                vec3_interpolate_3(&v.normal, v0.normal, v1.normal, v2.normal, uvw);
                vec3_interpolate_3(&v.tangent, v0.tangent, v1.tangent, v2.tangent, uvw);

                // Calculate final output color
                vec3_t out_color = ctx->pixel_shader(&v, bindings);
                uint8_t r = (uint8_t)c_min(out_color.x * 255, 255);
                uint8_t g = (uint8_t)c_min(out_color.y * 255, 255);
                uint8_t b = (uint8_t)c_min(out_color.z * 255, 255);
                ctx->color_buf[idx] = r << 16 | g << 8 | b;
            }
        }
    }
}

static void process_vertices(Projected_Vertex **output, 
    const Vertex *vertices, uint32_t num_vertices, const Shader_Bindings *bindings)
{
    array_reset(*output);
    array_ensure(*output, num_vertices, ctx->allocator);

    for (uint32_t i = 0; i < num_vertices; ++i) {
        Vertex vs_out;

        vec4_t clip_pos = ctx->vertex_shader(&vertices[i], &vs_out, bindings);
        float inv_w = 1.f / clip_pos.w;
         // Perspective divide
        clip_pos = vec4_mul(clip_pos, inv_w);
        // Viewport to screen space
        vec4_t screen_pos = mat44_transform_vec4(&ctx->viewport_transform, clip_pos);

        Projected_Vertex v = {
            .screen_pos = (vec2_t) { screen_pos.x, screen_pos.y },
            .inv_w = inv_w,
            .depth = clip_pos.z,
            // Copy vertex attributes
            .position = vec3_mul(vs_out.position, inv_w),
            .uv = vec2_mul(vs_out.uv, inv_w),
            .normal = vec3_mul(vs_out.normal, inv_w),
            .tangent = vec3_mul(vs_out.tangent, inv_w),
        };
        array_push(*output, v, ctx->allocator);
    }
}

static void draw_triangles(gfx_id vbuf, gfx_id ibuf, uint32_t first, uint32_t count)
{
    const Buffer *vbuffer = &ctx->buffers[vbuf - 1];
    const Buffer *ibuffer = &ctx->buffers[ibuf - 1];

    const Shader_Bindings bindings = { 
        .uniform_blocks = ctx->uniform_bindings,
        .textures = ctx->texture_bindings,
    };

    // We only need to run the vertex transformation stage on the raw vertex data
    // This is to avoid running potentially duplicate indexed vertices
    uint32_t num_vertices = (uint32_t)vbuffer->size / sizeof(Vertex);
    process_vertices(&ctx->projected_vertices, vbuffer->data, num_vertices, &bindings);
    
    // Use the indices to construct the triangles to be rasterized
    const uint32_t *indices = ibuffer->data;
    for (uint32_t i = first; i < count; i += 3) {
        rasterize_triangle(
            ctx->projected_vertices[indices[i + 0]],
            ctx->projected_vertices[indices[i + 1]],
            ctx->projected_vertices[indices[i + 2]],
            &bindings
        );
    }
}

static void swap_buffers(uint32_t *buffer)
{
    const int count = ctx->width * ctx->height;
    memcpy(buffer, ctx->color_buf, count * sizeof(*ctx->color_buf));

    for (int i = 0; i < count; ++i) {
        ctx->color_buf[i] = 0x11111111;
        ctx->depth_buf[i] = FLT_MAX;
    }
}

struct gfx_api *gfx_api = &(struct gfx_api) {
    .init = init,
    .shutdown = shutdown,
    .bind_shaders = bind_shaders,
    .bind_uniform_block = bind_uniform_block,
    .bind_texture = bind_texture,
    .create_texture = create_texture,
    .create_buffer = create_buffer,
    .update_buffer = update_buffer,
    .draw_triangles = draw_triangles,
    .swap_buffers = swap_buffers,
};