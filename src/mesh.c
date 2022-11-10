#include "mesh.h"
#include "foundation/allocator.h"
#include "gfx_api.h"

#define MESH_FORMAT_VERSION (4)

static void *read_entire_file(const char *path, uint64_t *sz, Allocator *a)
{
    uint64_t size = 0;
    FILE *f = fopen(path, "rb");
    if (f == NULL)
        return 0;

    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    void *data = c_alloc(a, size);
    fread(data, 1, size, f);
    *sz = size;
    fclose(f);
    return data;
}

static inline void report_error(const char *file, const char *msg)
{
    fprintf(stderr, "Failed to load mesh '%s': %s.\n", file, msg);
}

void load_mesh_from_file(Mesh *mesh, const char *path)
{
    Allocator *a = system_allocator;

    uint64_t size = 0;
    void *data = read_entire_file(path, &size, a);
    if (data == 0) {
        report_error(path, "unable to open file");
        return;
    }

    const uint32_t *start = data;
    const uint32_t *p = start;
    const uint32_t u32_size = (uint32_t)(size >> 2);
    const uint32_t *end = start + u32_size;

    uint32_t check_size = 4;
    if (check_size > u32_size) {
        report_error(path, "invalid header");
        return;
    }

    // Start parsing header
    const uint32_t magic = *p++;
    if (magic != 'MESH') {
        report_error(path, "invalid file format");
        return;
    }

    if ((size & 3) != 0) {
        report_error(path, "file size is not multiple of 4");
        return;
    }

    const uint32_t version = *p++;
    if (version != MESH_FORMAT_VERSION) {
        report_error(path, "incorrect format version");
        return;
    }
    mesh->num_vertices = *p++;
    mesh->num_indices = *p++;
    mesh->num_groups = *p++;
    mesh->num_wanted_materials = *p++;

    const uint32_t vertex_stride = sizeof(Vertex);
    const uint32_t index_stride = sizeof(uint32_t);

    // Read vertices
    const uint32_t vertex_data_size = mesh->num_vertices * vertex_stride;
    check_size += vertex_data_size >> 2;
    if (check_size > u32_size) {
        report_error(path, "too little vertex data");
        return;
    }
    const float *vertices = (float *)p;
    p += vertex_data_size >> 2;

    // Read indices
    const uint32_t index_data_size = mesh->num_indices * index_stride;
    check_size += index_data_size >> 2;
    if (check_size > u32_size) {
        report_error(path, "too little index data");
        return;
    }
    const uint32_t *indices = (uint32_t *)p;
    p += index_data_size >> 2;


    // Read triangle groups
    const uint32_t group_data_size = mesh->num_groups * sizeof(Mesh_Group);
    check_size += group_data_size >> 2;
    if (check_size > u32_size) {
        report_error(path, "too little group data");
        return;
    }
    // mesh->groups = c_alloc(a, group_data_size);
    // memcpy(mesh->groups, p, group_data_size);
    p += group_data_size >> 2;

    // Verify that end of file was reached
    if (p != end) {
        report_error(path, "unable to reach end of file");
        return;
    }

    mesh->vbuffer = gfx_api->create_buffer(vertices, vertex_data_size);
    mesh->ibuffer = gfx_api->create_buffer(indices, index_data_size);

    printf("Loaded mesh '%s' (v=%i i=%i g=%i m=%i format=%i)\n", path, 
        mesh->num_vertices, mesh->num_indices, mesh->num_groups, mesh->num_wanted_materials, version);

    c_free(a, data, size);
}
