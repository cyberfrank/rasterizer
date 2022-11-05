#pragma once
#include "foundation/basic.h"

typedef struct Mesh_Group {
    uint32_t first;
    uint32_t count;
    uint32_t material_idx;
} Mesh_Group;

typedef struct Mesh {
    gfx_id vbuffer;
    gfx_id ibuffer;
    uint32_t num_vertices;
    uint32_t num_indices;
    // Unused
    // Mesh_Group *groups;
    uint32_t num_groups;
    uint32_t num_wanted_materials;
} Mesh;

// Load a mesh from the file path
void load_mesh_from_file(Mesh *mesh, const char *path);