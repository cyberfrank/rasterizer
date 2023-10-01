#pragma once
#include <stdint.h>
#include <stdbool.h>

/* Math types */

typedef struct Vec2 {
    float x, y;
} Vec2;

typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct Rect {
    float x, y, w, h;
} Rect;

typedef struct Mat44 {
    float xx, xy, xz, xw;
    float yx, yy, yz, yw;
    float zx, zy, zz, zw;
    float wx, wy, wz, ww;
} Mat44;

typedef struct Transform {
    Vec3 pos;
    Vec4 rot;
    Vec3 scl;
} Transform;

/* String types */

typedef struct String8 {
    uint8_t *str;
    uint32_t len;
} String8;

typedef struct String16 {
    uint16_t *str;
    uint32_t len;
} String16;

typedef struct String32 {
    uint32_t *str;
    uint32_t len;
} String32;

typedef struct Time_Stamp {
    uint64_t opaque;
} Time_Stamp;

#define make_vec2(x, y)       (Vec2) { x, y }
#define make_vec3(x, y, z)    (Vec3) { x, y, z }
#define make_vec4(x, y, z, w) (Vec4) { x, y, z, w }
#define make_rect(x, y, w, h) (Rect) { x, y, w, h }

/* Macros */

#define c_min(a, b)         ((a) < (b) ? (a) : (b))
#define c_max(a, b)         ((a) > (b) ? (a) : (b))
#define c_clamp(x, lo, hi)  ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

#define KB(n) (((uint64_t)(n)) << 10)
#define MB(n) (((uint64_t)(n)) << 20)
#define GB(n) (((uint64_t)(n)) << 30)

#define STATIC_ASSERT(x) static_assert(x, #x)

// Static array count
#define ARRAY_COUNT(a) (sizeof(a) / sizeof(a[0]))

// Static string hashing
#define STRHASH(x) (x)
#define STATIC_HASH(s, v) STRHASH(sizeof("" s "") > 0 ? v : v)

// Graphics resource identifier
typedef uint32_t gfx_id;