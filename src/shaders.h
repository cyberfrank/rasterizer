#pragma once
#include "foundation/math.h"
#include "gfx_api.h"

typedef struct GlobalUniformBlock {
    Mat44 transform;
    Mat44 view_projection;
    Vec3 camera_pos;
} GlobalUniformBlock;

typedef struct LightUniformBlock {
    struct PointLight {
        Vec3 pos;
        Vec3 col;
    } point_lights[4];
    uint32_t num_point_lights;
} LightUniformBlock;

static Vec3 sample_texture(const Texture *texture, Vec2 uv)
{
    int x = abs((int)(uv.x * texture->width) % texture->width);
    int y = abs((int)(uv.y * texture->height) % texture->height);
    int idx = (x + y * texture->width) * texture->channels;
    return (Vec3) {
        .x = texture->data[idx + 0] / 255.f,
        .y = texture->data[idx + 1] / 255.f,
        .z = texture->data[idx + 2] / 255.f,
    };
}

static Vec4 default_vertex_shader(const Vertex *in, Vertex *out, const Shader_Bindings *bindings)
{
    const GlobalUniformBlock *globals = bindings->uniform_blocks[0];

    out->position = mat44_transform(&globals->transform, in->position);
    out->uv = in->uv;
    out->normal = mat44_transform(&globals->transform, in->normal);
    out->tangent = in->tangent;

    Vec4 world_pos = make_vec4(out->position.x, out->position.y, out->position.z, 1.0f);
    return mat44_transform_vec4(&globals->view_projection, world_pos);
}

static inline float distance_attenuation(Vec3 unormalized_light_vec) 
{
    float dist2 = vec3_dot(unormalized_light_vec, unormalized_light_vec);
    float att = 1.0f / c_max(dist2, 0.01f * 0.01f);
    return att;
}

static Vec3 default_pixel_shader(const Vertex *in, const Shader_Bindings *bindings)
{
    const Texture *albedo = bindings->textures[0];
    const GlobalUniformBlock *globals = bindings->uniform_blocks[0];
    const LightUniformBlock *lights = bindings->uniform_blocks[1];

    Vec3 diffuse_acc = { 0 };
    Vec3 specular_acc = { 0 };
    Vec3 view_dir = vec3_normalize(vec3_sub(globals->camera_pos, in->position));

    for (uint32_t i = 0; i < lights->num_point_lights; ++i) {
        const struct PointLight *light = &lights->point_lights[i];
        Vec3 light_dir = vec3_sub(light->pos, in->position);
        Vec3 light_dir_n = vec3_normalize(light_dir);
        Vec3 halfway_dir = vec3_normalize(vec3_add(view_dir, light_dir_n));

        float attenuation = distance_attenuation(light_dir);
        float n_dot_l = c_clamp(vec3_dot(in->normal, light_dir_n), 0.f, 1.f);

        float specular = powf(c_max(vec3_dot(halfway_dir, in->normal), 0.0f), 16);

        Vec3 attenuated_col = vec3_mul(light->col, n_dot_l * attenuation);
        diffuse_acc = vec3_add(diffuse_acc, attenuated_col);
        specular_acc = vec3_add(specular_acc, vec3_mul(attenuated_col, specular));
    }

    Vec3 ambient = make_vec3(0.2f, 0.2f, 0.2f);
    Vec3 tex_color = sample_texture(albedo, in->uv);
    return vec3_element_mul(tex_color, vec3_add(vec3_add(ambient, diffuse_acc), specular_acc));
}