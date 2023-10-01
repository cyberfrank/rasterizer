#pragma once
#include "basic.h"

#define PAGE_SIZE (4096)
#define ALIGN_SIZE(size, align) (((size + align - 1) / align) * align)

typedef struct Allocator {
    void *(*allocate_func)(struct Allocator *allocator, void *p, uint64_t old_size, uint64_t new_size, const char *file, uint32_t line);
    void *user_data;
} Allocator;

void record_allocation(void *old_ptr, uint64_t old_size, void *new_ptr, uint64_t new_size,
    const char *file, uint32_t line);

int64_t total_bytes_allocated();

#define c_alloc(a, size)                      (a)->allocate_func(a, 0, 0, size, __FILE__, __LINE__)
#define c_alloc_at(a, size, file, line)       (a)->allocate_func(a, 0, 0, size, file, line)
#define c_free(a, p, size)                    (a)->allocate_func(a, p, size, 0, __FILE__, __LINE__)
#define c_realloc(a, p, old_size, new_size)   (a)->allocate_func(a, p, old_size, new_size, __FILE__, __LINE__) 

// System default allocator
extern struct Allocator *system_allocator;
