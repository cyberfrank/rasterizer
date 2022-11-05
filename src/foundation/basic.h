#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef uint32_t gfx_id;

typedef struct vec2_t {
    float x, y;
} vec2_t;

typedef struct vec3_t {
    float x, y, z;
} vec3_t;

typedef struct vec4_t {
    float x, y, z, w;
} vec4_t;

typedef struct mat44_t {
    float xx, xy, xz, xw;
    float yx, yy, yz, yw;
    float zx, zy, zz, zw;
    float wx, wy, wz, ww;
} mat44_t;

#define make_vec2(x, y) \
    (vec2_t) { x, y }

#define make_vec3(x, y, z) \
    (vec3_t) { x, y, z }

#define make_vec4(x, y, z, w) \
    (vec4_t) { x, y, z, w }

#define make_rect(x, y, w, h) \
    (rect_t) { x, y, w, h }

#define c_max(a, b) ((a) > (b) ? (a) : (b))
#define c_min(a, b) ((a) < (b) ? (a) : (b))
#define c_clamp(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : x)
