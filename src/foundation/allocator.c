#include "allocator.h"
#include <stdlib.h>

static void *system_alloc(Allocator *allocator, void *old_ptr, uint64_t old_size, uint64_t new_size, 
        const char *file, uint32_t line)
{
    void *new_ptr = 0;
    if (new_size != 0)
    {
        new_ptr = realloc(old_ptr, new_size);
    }
    else
    {
        free(old_ptr);
    }

    record_allocation(old_ptr, old_size, new_ptr, new_size, file, line);

    return new_ptr;
}

static int64_t bytes_allocated = 0;

void record_allocation(void *old_ptr, uint64_t old_size, 
    void *new_ptr, uint64_t new_size, const char *file, uint32_t line)
{
    bytes_allocated += (new_size - old_size);
}

int64_t total_bytes_allocated()
{
    return bytes_allocated;
}

Allocator *system_allocator = &(Allocator)
{
    .allocate_func = system_alloc,
    .user_data = 0,
};