#pragma once
#include "foundation/basic.h"

typedef struct Allocator {
	void *(*alloc_cb)(struct Allocator *a, void *p, uint64_t old_size, uint64_t new_size, 
		const char *file, uint32_t line);
    void *user_data;
} Allocator;

#define c_alloc(a, sz)                     (a)->alloc_cb(a, 0, 0, sz, __FILE__, __LINE__)
#define c_alloc_at(a, sz, file, line)      (a)->alloc_cb(a, 0, 0, sz, file, line)
#define c_free(a, p, sz)                   (a)->alloc_cb(a, p, sz, 0, __FILE__, __LINE__)
#define c_realloc(a, p, old_sz, new_sz)    (a)->alloc_cb(a, p, old_sz, new_sz, __FILE__, __LINE__)

// System default allocator
extern struct Allocator *system_allocator;
