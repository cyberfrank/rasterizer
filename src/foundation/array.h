#pragma once
#include "foundation/basic.h"
#include "foundation/allocator.h"

#include <string.h>

typedef struct Array_Header {
    uint64_t capacity;
    uint64_t size;
} Array_Header;

#define array_header(a) \
    ((Array_Header *)((uint8_t *)(a) - sizeof(Array_Header)))

#define array_capacity(a) \
    ((a) ? array_header(a)->capacity : 0)

#define array_size(a) \
    ((a) ? array_header(a)->size : 0)

#define array_free_at(a, allocator, file, line) \
    ((*(void **)&(a)) = array__set_capacity_internal((void *)a, 0, sizeof(*(a)), allocator, file, line))

#define array_free(a, allocator) \
    array_free_at(a, allocator, __FILE__, __LINE__)

#define array_push_at(a, v, allocator, file, line) \
    (array_ensure_at(a, array_size(a) + 1, allocator, file, line), (a)[array_header(a)->size++] = (v))

#define array_push(a, v, allocator) \
    array_push_at(a, v, allocator, __FILE__, __LINE__)

#define array_join_at(a, v, n, allocator, file, line) \
    ((n) ? ((array_ensure_at(a, array_size(a) + n, allocator, file, line), memcpy(a + array_size(a), v, n * sizeof(*(a))), array_header(a)->size += n), 0) : 0)
    
#define array_join(a, v, n, allocator) \
    array_join_at(a, v, n, allocator, __FILE__, __LINE__)
    
#define array_pop(a) \
    (array_size(a) > 0 ? (a)[--array_header(a)->size] : 0)

#define array_reset(a) \
    ((a) ? (array_header(a)->size = 0) : 0)

#define array_end(a) \
    ((a) ? (a) + array_size(a) : 0)

#define array_needs_to_grow(a, n) \
    ((n) > array_capacity(a))

#define array_ensure_at(a, n, allocator, file, line) \
    (array_needs_to_grow(a, n) ? array_grow_at(a, n, allocator, file, line) : 0)

#define array_ensure(a, n, allocator) \
    array_ensure_at(a, n, allocator, __FILE__, __LINE__)

#define array_grow_at(a, n, allocator, file, line) \
    (*((void **)&(a)) = array__grow_internal((void *)a, (n), sizeof(*(a)), allocator, file, line))

#define array_grow(a, n, allocator) \
    array_grow_at(a, n, allocator, __FILE__, __LINE__)

static inline void *array__set_capacity_internal(void *arr, uint64_t new_capacity, uint64_t item_size,
    struct Allocator *allocator, const char *file, uint32_t line)
{
    uint8_t *p = arr ? (uint8_t *)array_header(arr) : 0;
    const uint64_t extra = sizeof(Array_Header);
    const uint64_t size = array_size(arr);
    const uint64_t bytes_before = arr ? item_size * array_capacity(arr) + extra : 0;
    const uint64_t bytes_after = new_capacity ? item_size * new_capacity + extra : 0;
    p = (uint8_t *)allocator->alloc_cb(allocator, p, bytes_before, bytes_after, file, line);
    void *new_a = p ? p + extra : p;
    if (new_a) {
        array_header(new_a)->size = size;
        array_header(new_a)->capacity = new_capacity;
    }
    return new_a;
}

static inline void *array__grow_internal(void *arr, uint64_t to_at_least, uint64_t item_size,
    struct Allocator *allocator, const char *file, uint32_t line)
{
    const uint64_t capacity = arr ? array_capacity(arr) : 0;
    if (capacity >= to_at_least)
        return arr;
    const uint64_t min_capacity = capacity ? capacity * 2 : 16;
    const uint64_t new_capacity = min_capacity > to_at_least ? min_capacity : to_at_least;
    return array__set_capacity_internal(arr, new_capacity, item_size, allocator, file, line);
}
