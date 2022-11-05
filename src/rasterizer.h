#pragma once
#include "foundation/basic.h"

#define MAX_NUM_UNIFORM_BLOCKS (4)
#define MAX_NUM_TEXTURES (4)

typedef struct Texture {
    int width;
    int height;
    int channels;
    uint8_t *data;
} Texture;

typedef struct Vertex {
    vec3_t position;
    vec2_t uv;
    vec3_t normal;
    vec3_t tangent;
} Vertex;

typedef struct Buffer {
    void *data;
    uint64_t size;
} Buffer;

typedef struct Shader_Bindings {
    const void **uniform_blocks;
    const Texture **textures;
} Shader_Bindings;

typedef vec4_t (*Vertex_Shader)(const Vertex *in, Vertex *out, const Shader_Bindings *bindings);
typedef vec3_t (*Pixel_Shader)(const Vertex *in, const Shader_Bindings *bindings);

struct gfx_api {
    void (*init)(int width, int height);
    void (*shutdown)();

    // Set active shaders
    void (*bind_shaders)(Vertex_Shader vs, Pixel_Shader ps);

    // Bind a uniform block to the given `slot`
    void (*bind_uniform_block)(uint32_t slot, gfx_id id);

    // Bind a texture to the given `slot`
    void (*bind_texture)(uint32_t slot, gfx_id id);

    // Create a texture and return a handle
    gfx_id (*create_texture)(const void *data, int width, int height, int channels);
    
    // Create a persistent buffer and return a handle
    gfx_id (*create_buffer)(const void *data, uint64_t size);

    // Update buffer with `data` of given `size`
    void (*update_buffer)(gfx_id id, const void *data, uint64_t size, uint64_t offset);

    // Draw triangles using vertex and index buffers
    void (*draw_triangles)(gfx_id vbuf, gfx_id ibuf, uint32_t first, uint32_t count);

    // Copy internal color buffer to `buffer`
    void (*swap_buffers)(uint32_t *buffer);
};

extern struct gfx_api *gfx_api;