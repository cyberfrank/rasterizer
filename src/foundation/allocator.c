#include "allocator.h"

int64_t allocated_bytes = 0;

static void *system_alloc(Allocator *a, void *old_ptr, uint64_t old_size, uint64_t new_size, 
    const char *file, uint32_t line)
{
    void *new_ptr = 0;
    if (new_size > 0) {
        new_ptr = realloc(old_ptr, new_size);
    } else {
        free(old_ptr);
    }

    int64_t bytes = new_size - old_size;
    allocated_bytes += bytes;

    return new_ptr;
}

Allocator *system_allocator = &(Allocator) {
    .alloc_cb = system_alloc,
};
