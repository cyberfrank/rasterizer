#pragma once
#include "foundation/basic.h"

#include <emmintrin.h>
#include <immintrin.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265f
#define DEG_TO_RAD (PI / 180.f)

static inline vec2_t vec2_add(vec2_t lhs, vec2_t rhs)
{
    const vec2_t res = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
    };
    return res;
}

static inline vec2_t vec2_sub(vec2_t lhs, vec2_t rhs)
{
    const vec2_t res = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
    };
    return res;
}

static inline float vec2_dot(vec2_t lhs, vec2_t rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

static inline float vec2_cross(vec2_t lhs, vec2_t rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

static inline vec2_t vec2_mul(vec2_t lhs, float rhs)
{
    const vec2_t res = {
        lhs.x * rhs,
        lhs.y * rhs,
    };
    return res;
}

static inline vec2_t vec2_mul_add(vec2_t lhs, vec2_t rhs, float mul)
{
    const vec2_t res = {
        lhs.x + rhs.x * mul,
        lhs.y + rhs.y * mul,
    };
    return res;
}

static inline vec2_t vec2_element_mul(vec2_t lhs, vec2_t rhs)
{
    const vec2_t res = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
    };
    return res;
}

static inline vec2_t vec2_element_div(vec2_t lhs, vec2_t rhs)
{
    const vec2_t res = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
    };
    return res;
}

static inline float vec2_length(vec2_t v)
{
    return sqrtf(vec2_dot(v, v));
}

static inline vec2_t vec2_normalize(vec2_t v)
{
    const float len = vec2_length(v);
    if (len < 0.00001f) {
        vec2_t res = { 0 };
        return res;
    }
    const float inv_len = 1.0f / len;
    const vec2_t res = {
        v.x * inv_len,
        v.y * inv_len,
    };
    return res;
}

static inline bool vec2_equal(vec2_t lhs, vec2_t rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

static inline vec3_t vec3_add(vec3_t lhs, vec3_t rhs)
{
    const vec3_t res = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
    };
    return res;
}

static inline vec3_t vec3_sub(vec3_t lhs, vec3_t rhs)
{
    const vec3_t res = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
    };
    return res;
}

static inline float vec3_dot(vec3_t lhs, vec3_t rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

static inline vec3_t vec3_cross(vec3_t lhs, vec3_t rhs)
{
    const vec3_t res = {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
    return res;
}

static inline vec3_t vec3_mul(vec3_t lhs, float rhs)
{
    const vec3_t res = {
        lhs.x * rhs,
        lhs.y * rhs,
        lhs.z * rhs,
    };
    return res;
}

static inline vec3_t vec3_mul_add(vec3_t lhs, vec3_t rhs, float mul)
{
    const vec3_t res = {
        lhs.x + rhs.x * mul,
        lhs.y + rhs.y * mul,
        lhs.z + rhs.z * mul,
    };
    return res;
}

static inline vec3_t vec3_element_mul(vec3_t lhs, vec3_t rhs)
{
    const vec3_t res = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
    };
    return res;
}

static inline vec3_t vec3_element_div(vec3_t lhs, vec3_t rhs)
{
    const vec3_t res = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z,
    };
    return res;
}

static inline float vec3_length(vec3_t v)
{
    return sqrtf(vec3_dot(v, v));
}

static inline vec3_t vec3_normalize(vec3_t v)
{
    const float len = vec3_length(v);
    if (len < 0.00001f) {
        vec3_t res = { 0 };
        return res;
    }
    const float inv_len = 1.0f / len;
    const vec3_t res = {
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len,
    };
    return res;
}

static inline bool vec3_equal(vec3_t lhs, vec3_t rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

static inline vec4_t vec4_add(vec4_t lhs, vec4_t rhs)
{
    const vec4_t res = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
        lhs.w + rhs.z,
    };
    return res;
}

static inline vec4_t vec4_sub(vec4_t lhs, vec4_t rhs)
{
    const vec4_t res = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
        lhs.w - rhs.w,
    };
    return res;
}

static inline float vec4_dot(vec4_t lhs, vec4_t rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

static inline vec4_t vec4_mul(vec4_t lhs, float rhs)
{
    const vec4_t res = {
        lhs.x * rhs,
        lhs.y * rhs,
        lhs.z * rhs,
        lhs.w * rhs,
    };
    return res;
}

static inline vec4_t vec4_mul_add(vec4_t lhs, vec4_t rhs, float mul)
{
    const vec4_t res = {
        lhs.x + rhs.x * mul,
        lhs.y + rhs.y * mul,
        lhs.z + rhs.z * mul,
        lhs.w + rhs.w * mul,
    };
    return res;
}

static inline vec4_t vec4_element_mul(vec4_t lhs, vec4_t rhs)
{
    const vec4_t res = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
        lhs.w * rhs.w,
    };
    return res;
}

static inline vec4_t vec4_element_div(vec4_t lhs, vec4_t rhs)
{
    const vec4_t res = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z,
        lhs.w / rhs.w,
    };
    return res;
}

static inline float vec4_length(vec4_t v)
{
    return sqrtf(vec4_dot(v, v));
}

static inline vec4_t vec4_normalize(vec4_t v)
{
    const float len = vec4_length(v);
    if (len < 0.00001f) {
        vec4_t res = { 0 };
        return res;
    }
    const float inv_len = 1.0f / len;
    const vec4_t res = {
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len,
        v.w * inv_len,
    };
    return res;
}

static inline bool vec4_equal(vec4_t lhs, vec4_t rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

static inline vec4_t quaternion_from_rotation(vec3_t axis, float angle)
{
    const float half_angle = angle * 0.5f;
    const float cosha = cosf(half_angle);
    const float sinha = sinf(half_angle);
    const vec4_t res = {
        axis.x * sinha,
        axis.y * sinha,
        axis.z * sinha,
        cosha,
    };
    return res;
}

static inline vec3_t quaternion_to_rotation(vec4_t q, float *angle)
{
    const vec3_t v = { q.x, q.y, q.z };
    const float s = vec3_length(v);
    *angle = 2.0f * atan2f(s, q.w);
    return s ? vec3_mul(v, 1.0f / s) : make_vec3(1, 0, 0);
}

static inline vec4_t quaternion_mul(vec4_t lhs, vec4_t rhs)
{
    const vec4_t res = {
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
    };
    return res;
}

static inline vec4_t quaternion_inverse(vec4_t q)
{
    const vec4_t res = {
        -q.x,
        -q.y,
        -q.z,
        q.w,
    };
    return res;
}

static inline vec3_t quaternion_rotate_vec3(vec4_t q, vec3_t v)
{
    const vec4_t v4 = { v.x, v.y, v.z };
    const vec4_t q_inv = quaternion_inverse(q);
    const vec4_t v4_rot = quaternion_mul(q, quaternion_mul(v4, q_inv));
    const vec3_t res = {
        v4_rot.x,
        v4_rot.y,
        v4_rot.z,
    };
    return res;
}

static inline vec3_t quaternion_to_euler(vec4_t q)
{
    const float sinr = 2 * (q.w * q.x + q.y * q.z);
    const float cosr = 1 - 2 * (q.x * q.x + q.y * q.y);
    const float roll = atan2f(sinr, cosr);

    const float sinp = 2 * (q.w * q.y - q.z * q.x);
    const float pitch = sinp >= 0.999f ? PI / 2 : sinp <= -0.999f ? -PI / 2 : asinf(sinp);

    const float siny = 2 * (q.w * q.z + q.x * q.y);
    const float cosy = 1 - 2 * (q.y * q.y + q.z * q.z);
    const float yaw = atan2f(siny, cosy);
    const vec3_t res = {
        roll,
        pitch,
        yaw,
    };
    return res;
}

static inline vec4_t euler_to_quaternion(vec3_t xyz)
{
    const float roll = xyz.x;
    const float pitch = xyz.y;
    const float yaw = xyz.z;

    const float cy = cosf(yaw * 0.5f);
    const float sy = sinf(yaw * 0.5f);
    const float cr = cosf(roll * 0.5f);
    const float sr = sinf(roll * 0.5f);
    const float cp = cosf(pitch * 0.5f);
    const float sp = sinf(pitch * 0.5f);

    const vec4_t res = {
        cy * sr * cp - sy * cr * sp,
        cy * cr * sp + sy * sr * cp,
        sy * cr * cp - cy * sr * sp,
        cy * cr * cp + sy * sr * sp,
    };
    return res;
}

static inline void quaternion_to_xyz(vec3_t *x, vec3_t *y, vec3_t *z, vec4_t q)
{
    const float d = vec4_dot(q, q);
    const float s = (d != 0.f) ? 2.f / d : 1.f;

    const float xs = q.x * s;
    const float ys = q.y * s;
    const float zs = q.z * s;
    const float wx = q.w * xs;
    const float wy = q.w * ys;
    const float wz = q.w * zs;
    const float xx = q.x * xs;
    const float xy = q.x * ys;
    const float xz = q.x * zs;
    const float yy = q.y * ys;
    const float yz = q.y * zs;
    const float zz = q.z * zs;

    if (x) {
        x->x = 1.f - yy - zz;
        x->y = xy + wz;
        x->z = xz - wy;
    }

    if (y) {
        y->x = xy - wz;
        y->y = 1.f - xx - zz;
        y->z = yz + wx;
    }

    if (z) {
        z->x = xz + wy;
        z->y = yz - wx;
        z->z = 1.f - xx - yy;
    }
}

static inline const mat44_t *mat44_identity()
{
    static mat44_t identity = { 
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    return &identity;
}

static inline void mat44_from_translation(mat44_t *res, vec3_t t)
{
    memset(res, 0, sizeof(*res));
    res->xx = 1.0f;
    res->yy = 1.0f;
    res->zz = 1.0f;
    res->ww = 1.0f;
    res->wx = t.x;
    res->wy = t.y;
    res->wz = t.z;
}

static inline void mat44_from_quaternion(mat44_t *res, vec4_t q)
{
    const float d = vec4_dot(q, q);
    const float s = (d != 0.f) ? 2.f / d : 1.f;

    const float xs = q.x * s;
    const float ys = q.y * s;
    const float zs = q.z * s;
    const float wx = q.w * xs;
    const float wy = q.w * ys;
    const float wz = q.w * zs;
    const float xx = q.x * xs;
    const float xy = q.x * ys;
    const float xz = q.x * zs;
    const float yy = q.y * ys;
    const float yz = q.y * zs;
    const float zz = q.z * zs;

    res->xx = 1.f - yy - zz;
    res->xy = xy + wz;
    res->xz = xz - wy;
    res->xw = 0.f;

    res->yx = xy - wz;
    res->yy = 1.f - xx - zz;
    res->yz = yz + wx;
    res->yw = 0.f;

    res->zx = xz + wy;
    res->zy = yz - wx;
    res->zz = 1.f - xx - yy;
    res->zw = 0.f;

    res->wx = 0;
    res->wy = 0;
    res->wz = 0;
    res->ww = 1.f;
}

static inline void mat44_from_scale(mat44_t *res, vec3_t s)
{
    *res = *mat44_identity();
    res->xx = s.x;
    res->yy = s.y;
    res->zz = s.z;
}

static inline void mat44_from_translation_rotation_scale(mat44_t *res, vec3_t t, vec4_t r, vec3_t s)
{
    mat44_from_quaternion(res, r);
    res->xx *= s.x;
    res->xy *= s.x;
    res->xz *= s.x;
    res->yx *= s.y;
    res->yy *= s.y;
    res->yz *= s.y;
    res->zx *= s.z;
    res->zy *= s.z;
    res->zz *= s.z;
    res->wx = t.x;
    res->wy = t.y;
    res->wz = t.z;
}

static inline void mat44_mul(mat44_t *res, const mat44_t *lhs, const mat44_t *rhs)
{
    __m128 vx = _mm_loadu_ps(&rhs->xx);
    __m128 vy = _mm_loadu_ps(&rhs->yx);
    __m128 vz = _mm_loadu_ps(&rhs->zx);
    __m128 vw = _mm_loadu_ps(&rhs->wx);
    // row 0
    __m128 sx = _mm_set1_ps(lhs->xx);
    __m128 sy = _mm_set1_ps(lhs->xy);
    __m128 sz = _mm_set1_ps(lhs->xz);
    __m128 sw = _mm_set1_ps(lhs->xw);
    sx = _mm_mul_ps(sx, vx);
    sy = _mm_mul_ps(sy, vy);
    sz = _mm_mul_ps(sz, vz);
    sw = _mm_mul_ps(sw, vw);
    sx = _mm_add_ps(sx, sz);
    sy = _mm_add_ps(sy, sw);
    sx = _mm_add_ps(sx, sy);
    _mm_storeu_ps(&res->xx, sx);
    // row 1
    sx = _mm_set1_ps(lhs->yx);
    sy = _mm_set1_ps(lhs->yy);
    sz = _mm_set1_ps(lhs->yz);
    sw = _mm_set1_ps(lhs->yw);
    sx = _mm_mul_ps(sx, vx);
    sy = _mm_mul_ps(sy, vy);
    sz = _mm_mul_ps(sz, vz);
    sw = _mm_mul_ps(sw, vw);
    sx = _mm_add_ps(sx, sz);
    sy = _mm_add_ps(sy, sw);
    sx = _mm_add_ps(sx, sy);
    _mm_storeu_ps(&res->yx, sx);
    // row 2
    sx = _mm_set1_ps(lhs->zx);
    sy = _mm_set1_ps(lhs->zy);
    sz = _mm_set1_ps(lhs->zz);
    sw = _mm_set1_ps(lhs->zw);
    sx = _mm_mul_ps(sx, vx);
    sy = _mm_mul_ps(sy, vy);
    sz = _mm_mul_ps(sz, vz);
    sw = _mm_mul_ps(sw, vw);
    sx = _mm_add_ps(sx, sz);
    sy = _mm_add_ps(sy, sw);
    sx = _mm_add_ps(sx, sy);
    _mm_storeu_ps(&res->zx, sx);
    // row 3
    sx = _mm_set1_ps(lhs->wx);
    sy = _mm_set1_ps(lhs->wy);
    sz = _mm_set1_ps(lhs->wz);
    sw = _mm_set1_ps(lhs->ww);
    sx = _mm_mul_ps(sx, vx);
    sy = _mm_mul_ps(sy, vy);
    sz = _mm_mul_ps(sz, vz);
    sw = _mm_mul_ps(sw, vw);
    sx = _mm_add_ps(sx, sz);
    sy = _mm_add_ps(sy, sw);
    sx = _mm_add_ps(sx, sy);
    _mm_storeu_ps(&res->wx, sx);
}

static inline void mat44_inverse(mat44_t *res44, const mat44_t *m44)
{
    // https://github.com/niswegmann/small-matrix-inverse/blob/master/invert4x4_sse.h
    // Creative Common
    float *res = &res44->xx;
    const float *m = &m44->xx;

    __m128 minor0, minor1, minor2, minor3;
    __m128 row0, row1, row2, row3;
    __m128 det, tmp1;

    tmp1 = _mm_set_ps(0, 0, 0, 0);
    row1 = _mm_set_ps(0, 0, 0, 0);
    row3 = _mm_set_ps(0, 0, 0, 0);

    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64 *)(m)), (__m64 *)(m + 4));
    row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64 *)(m + 8)), (__m64 *)(m + 12));

    row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64 *)(m + 2)), (__m64 *)(m + 6));
    row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64 *)(m + 10)), (__m64 *)(m + 14));

    row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

    tmp1 = _mm_mul_ps(row2, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor0 = _mm_mul_ps(row1, tmp1);
    minor1 = _mm_mul_ps(row0, tmp1);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

    tmp1 = _mm_mul_ps(row1, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
    minor3 = _mm_mul_ps(row0, tmp1);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
    minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

    tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);

    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
    minor2 = _mm_mul_ps(row0, tmp1);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

    tmp1 = _mm_mul_ps(row0, row1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

    tmp1 = _mm_mul_ps(row0, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

    tmp1 = _mm_mul_ps(row0, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

    det = _mm_mul_ps(row0, minor0);
    det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
    det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);

    tmp1 = _mm_rcp_ss(det);

    det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
    det = _mm_shuffle_ps(det, det, 0x00);

    minor0 = _mm_mul_ps(det, minor0);
    _mm_storel_pi((__m64 *)(res), minor0);
    _mm_storeh_pi((__m64 *)(res + 2), minor0);

    minor1 = _mm_mul_ps(det, minor1);
    _mm_storel_pi((__m64 *)(res + 4), minor1);
    _mm_storeh_pi((__m64 *)(res + 6), minor1);

    minor2 = _mm_mul_ps(det, minor2);
    _mm_storel_pi((__m64 *)(res + 8), minor2);
    _mm_storeh_pi((__m64 *)(res + 10), minor2);

    minor3 = _mm_mul_ps(det, minor3);
    _mm_storel_pi((__m64 *)(res + 12), minor3);
    _mm_storeh_pi((__m64 *)(res + 14), minor3);
}

static inline vec3_t mat44_transform(const mat44_t *m, vec3_t v)
{
    vec3_t res;
    res.x = m->xx * v.x + m->yx * v.y + m->zx * v.z + m->wx;
    res.y = m->xy * v.x + m->yy * v.y + m->zy * v.z + m->wy;
    res.z = m->xz * v.x + m->yz * v.y + m->zz * v.z + m->wz;
    return res;
}

static inline vec4_t mat44_transform_vec4(const mat44_t *m, vec4_t v)
{
    vec4_t res;
    res.x = m->xx * v.x + m->yx * v.y + m->zx * v.z + v.w * m->wx;
    res.y = m->xy * v.x + m->yy * v.y + m->zy * v.z + v.w * m->wy;
    res.z = m->xz * v.x + m->yz * v.y + m->zz * v.z + v.w * m->wz;
    res.w = m->xw * v.x + m->yw * v.y + m->zw * v.z + v.w * m->ww;
    return res;
}

static inline float mat44_determinant(const mat44_t *m)
{
    float det = 0;
    det += m->xw * m->yz * m->zy * m->wx - m->xz * m->yw * m->zy * m->wx - m->xw * m->yy * m->zz * m->wx + m->xy * m->yw * m->zz * m->wx;
    det += m->xz * m->yy * m->zw * m->wx - m->xy * m->yz * m->zw * m->wx - m->xw * m->yz * m->zx * m->wy + m->xz * m->yw * m->zx * m->wy;
    det += m->xw * m->yx * m->zz * m->wy - m->xx * m->yw * m->zz * m->wy - m->xz * m->yx * m->zw * m->wy + m->xx * m->yz * m->zw * m->wy;
    det += m->xw * m->yy * m->zx * m->wz - m->xy * m->yw * m->zx * m->wz - m->xw * m->yx * m->zy * m->wz + m->xx * m->yw * m->zy * m->wz;
    det += m->xy * m->yx * m->zw * m->wz - m->xx * m->yy * m->zw * m->wz - m->xz * m->yy * m->zx * m->ww + m->xy * m->yz * m->zx * m->ww;
    det += m->xz * m->yx * m->zy * m->ww - m->xx * m->yz * m->zy * m->ww - m->xy * m->yx * m->zz * m->ww + m->xx * m->yy * m->zz * m->ww;
    return det;
}

static inline float mat44_determinant33(const mat44_t *m)
{
    const float mxx = m->xx, mxy = m->xy, mxz = m->xz;
    const float myx = m->yx, myy = m->yy, myz = m->yz;
    const float mzx = m->zx, mzy = m->zy, mzz = m->zz;

    float det = 0;
    det += mxx * myy * mzz - mxx * myz * mzy;
    det += mxy * myz * mzx - mxy * myx * mzz;
    det += mxz * myx * mzy - mxz * myy * mzx;
    return det;
}

static inline vec3_t *mat44_x(mat44_t *m) { return (vec3_t *)&m->xx; };
static inline vec3_t *mat44_y(mat44_t *m) { return (vec3_t *)&m->yx; };
static inline vec3_t *mat44_z(mat44_t *m) { return (vec3_t *)&m->zx; };
static inline vec3_t *mat44_w(mat44_t *m) { return (vec3_t *)&m->wx; };

static inline vec4_t *mat44_x_vec4(mat44_t *m) { return (vec4_t *)&m->xx; };
static inline vec4_t *mat44_y_vec4(mat44_t *m) { return (vec4_t *)&m->yx; };
static inline vec4_t *mat44_z_vec4(mat44_t *m) { return (vec4_t *)&m->zx; };
static inline vec4_t *mat44_w_vec4(mat44_t *m) { return (vec4_t *)&m->wx; };
