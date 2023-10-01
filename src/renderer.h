#pragma once
#include "foundation/basic.h"
#include "foundation/math.h"
#include "gfx_api.h"
#include "shaders.h"
#include "mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct Renderer {
    Mesh mesh;
    gfx_id global_buffer;
    gfx_id light_buffer;
} Renderer;

static void projection_from_fov(Mat44 *m, float near, float far, float vertical_fov, float aspect)
{
    const float range = near - far;
    const float tan_half_fov = tanf(vertical_fov / 2.f);
    memset(m, 0, sizeof(*m));
    m->xx = 1.f / (tan_half_fov * aspect);
    m->yy = 1.f / tan_half_fov;
    m->zz = (-near - far) / range;
    m->zw = 1.f;
    m->wz = 2.f * far * near / range;
}

static gfx_id load_texture_from_file(const char *path)
{
    int w, h, c;
    stbi_set_flip_vertically_on_load(1);
    uint8_t *data = stbi_load(path, &w, &h, &c, 3);
    gfx_id id = gfx_api->create_texture(data, w, h, c);
    stbi_image_free(data);
    printf("Loaded texture '%s' (w=%i, h=%i, c=%i)\n", path, w, h, c);
    return id;
}

static void renderer_init(Renderer *r, int w, int h)
{
    gfx_api->bind_shaders(default_vertex_shader, default_pixel_shader);

    load_mesh_from_file(&r->mesh, "data/chest.triangle_mesh");

    gfx_id tex = load_texture_from_file("data/chest.jpg");
    gfx_api->bind_texture(0, tex);

    LightUniformBlock lights;
    lights.point_lights[0] = (struct PointLight) {
        .pos = make_vec3(10, 0, -10),
        .col = make_vec3(0.8f, 0.6f, 0.1f),
    };
    lights.point_lights[1] = (struct PointLight) {
        .pos = make_vec3(-10, 0, -10),
        .col = make_vec3(0.4f, 0.4f, 0.4f),
    };
    lights.num_point_lights = 2;
    r->light_buffer = gfx_api->create_buffer(&lights, sizeof(lights));
    gfx_api->bind_uniform_block(1, r->light_buffer);

    Mat44 projection, view, transform;
    projection_from_fov(&projection, 0.1f, 1000.0f, 50.f * DEG_TO_RAD, (float)w / (float)h);

    GlobalUniformBlock globals;
    globals.camera_pos = make_vec3(0, 0, -2);
    mat44_from_translation(&transform, globals.camera_pos);
    mat44_inverse(&view, &transform);
    mat44_mul(&globals.view_projection, &view, &projection);

    r->global_buffer = gfx_api->create_buffer(&globals, sizeof(globals));
    gfx_api->bind_uniform_block(0, r->global_buffer);
}

static void renderer_draw_scene(Renderer *r, float t)
{
    // Update model transform
    Mat44 tm;
    mat44_from_translation_rotation_scale(&tm,
        make_vec3(0, -0.25f, -0.5f),
        euler_to_quaternion(make_vec3(0, -t + PI, 0)),
        make_vec3(1, 1, 1)
    );
    gfx_api->update_buffer(r->global_buffer, &tm, sizeof(tm), offsetof(GlobalUniformBlock, transform));

    Mesh *mesh = &r->mesh;
    gfx_api->draw_triangles(mesh->vbuffer, mesh->ibuffer, 0, mesh->num_indices);
}