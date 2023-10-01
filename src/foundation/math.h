#pragma once
#include "basic.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265f
#define PI_D 3.14159265358979323846
#define TAU 6.28318530f
#define TAU_D 6.28318530717958647692
#define DEG_TO_RAD (PI / 180.f)
#define RAD_TO_DEG (180.f / PI)

static inline float math_lerp(float s, float e, float t)
{
    return s * (1.f - t) + e * t;
}

static inline bool math_equal_abs_eps(float a, float b, float eps)
{
    return a - eps < b && b < a + eps;
}

static inline uint32_t math_uint32_div_ceil(uint32_t v, uint32_t d)
{
    return v / d + (v % d ? 1 : 0);
}

static inline Vec2 vec2_add(Vec2 lhs, Vec2 rhs)
{
    const Vec2 res = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
    };
    return res;
}

static inline Vec2 vec2_sub(Vec2 lhs, Vec2 rhs)
{
    const Vec2 res = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
    };
    return res;
}

static inline float vec2_dot(Vec2 lhs, Vec2 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

static inline float vec2_cross(Vec2 lhs, Vec2 rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

static inline Vec2 vec2_mul(Vec2 lhs, float rhs)
{
    const Vec2 res = {
        lhs.x * rhs,
        lhs.y * rhs,
    };
    return res;
}

static inline Vec2 vec2_mul_add(Vec2 lhs, Vec2 rhs, float mul)
{
    const Vec2 res = {
        lhs.x + rhs.x * mul,
        lhs.y + rhs.y * mul,
    };
    return res;
}

static inline Vec2 vec2_element_mul(Vec2 lhs, Vec2 rhs)
{
    const Vec2 res = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
    };
    return res;
}

static inline Vec2 vec2_element_div(Vec2 lhs, Vec2 rhs)
{
    const Vec2 res = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
    };
    return res;
}

static inline float vec2_length(Vec2 v)
{
    return sqrtf(vec2_dot(v, v));
}

static inline Vec2 vec2_normalize(Vec2 v)
{
    const float len = vec2_length(v);
    if (len < 0.00001f) {
        Vec2 res = { 0 };
        return res;
    }
    const float inv_len = 1.0f / len;
    const Vec2 res = {
        v.x * inv_len,
        v.y * inv_len,
    };
    return res;
}

static inline bool vec2_equal(Vec2 lhs, Vec2 rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

static inline Vec2 vec2_lerp(Vec2 a, Vec2 b, float t)
{
    return vec2_mul_add(vec2_mul(a, 1 - t), b, t);
}

static inline Vec3 vec3_add(Vec3 lhs, Vec3 rhs)
{
    const Vec3 res = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
    };
    return res;
}

static inline Vec3 vec3_sub(Vec3 lhs, Vec3 rhs)
{
    const Vec3 res = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
    };
    return res;
}

static inline float vec3_dot(Vec3 lhs, Vec3 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

static inline Vec3 vec3_cross(Vec3 lhs, Vec3 rhs)
{
    const Vec3 res = {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
    return res;
}

static inline Vec3 vec3_mul(Vec3 lhs, float rhs)
{
    const Vec3 res = {
        lhs.x * rhs,
        lhs.y * rhs,
        lhs.z * rhs,
    };
    return res;
}

static inline Vec3 vec3_mul_add(Vec3 lhs, Vec3 rhs, float mul)
{
    const Vec3 res = {
        lhs.x + rhs.x * mul,
        lhs.y + rhs.y * mul,
        lhs.z + rhs.z * mul,
    };
    return res;
}

static inline Vec3 vec3_element_mul(Vec3 lhs, Vec3 rhs)
{
    const Vec3 res = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
    };
    return res;
}

static inline Vec3 vec3_element_div(Vec3 lhs, Vec3 rhs)
{
    const Vec3 res = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z,
    };
    return res;
}

static inline float vec3_length(Vec3 v)
{
    return sqrtf(vec3_dot(v, v));
}

static inline Vec3 vec3_normalize(Vec3 v)
{
    const float len = vec3_length(v);
    if (len < 0.00001f) {
        Vec3 res = { 0 };
        return res;
    }
    const float inv_len = 1.0f / len;
    const Vec3 res = {
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len,
    };
    return res;
}

static inline void vec3_orthonormal_basis(Vec3 n, Vec3 *b1, Vec3 *b2)
{
    // https://graphics.pixar.com/library/OrthonormalB/paper.pdf
    if (n.z < 0) {
        const float a = 1.0f / (1.0f - n.z);
        const float b = n.x * n.y * a;
        *b1 = (Vec3) { 1.0f - n.x * n.x * a, -b, n.x };
        *b2 = (Vec3) { b, n.y * n.y * a - 1.0f, -n.y };
    } else {
        const float a = 1.0f / (1.0f + n.z);
        const float b = -n.x * n.y * a;
        *b1 = (Vec3) { 1.0f - n.x * n.x * a, b, -n.x };
        *b2 = (Vec3) { b, 1.0f - n.y * n.y * a, -n.y };
    }
}

static inline bool vec3_equal(Vec3 lhs, Vec3 rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

static inline Vec3 vec3_lerp(Vec3 a, Vec3 b, float t)
{
    return vec3_mul_add(vec3_mul(a, 1 - t), b, t);
}

static inline Vec4 vec4_add(Vec4 lhs, Vec4 rhs)
{
    const Vec4 res = {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
        lhs.w + rhs.z,
    };
    return res;
}

static inline Vec4 vec4_sub(Vec4 lhs, Vec4 rhs)
{
    const Vec4 res = {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
        lhs.w - rhs.w,
    };
    return res;
}

static inline float vec4_dot(Vec4 lhs, Vec4 rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

static inline Vec4 vec4_mul(Vec4 lhs, float rhs)
{
    const Vec4 res = {
        lhs.x * rhs,
        lhs.y * rhs,
        lhs.z * rhs,
        lhs.w * rhs,
    };
    return res;
}

static inline Vec4 vec4_mul_add(Vec4 lhs, Vec4 rhs, float mul)
{
    const Vec4 res = {
        lhs.x + rhs.x * mul,
        lhs.y + rhs.y * mul,
        lhs.z + rhs.z * mul,
        lhs.w + rhs.w * mul,
    };
    return res;
}

static inline Vec4 vec4_element_mul(Vec4 lhs, Vec4 rhs)
{
    const Vec4 res = {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
        lhs.w * rhs.w,
    };
    return res;
}

static inline Vec4 vec4_element_div(Vec4 lhs, Vec4 rhs)
{
    const Vec4 res = {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z,
        lhs.w / rhs.w,
    };
    return res;
}

static inline float vec4_length(Vec4 v)
{
    return sqrtf(vec4_dot(v, v));
}

static inline Vec4 vec4_normalize(Vec4 v)
{
    const float len = vec4_length(v);
    if (len < 0.00001f) {
        Vec4 res = { 0 };
        return res;
    }
    const float inv_len = 1.0f / len;
    const Vec4 res = {
        v.x * inv_len,
        v.y * inv_len,
        v.z * inv_len,
        v.w * inv_len,
    };
    return res;
}

static inline bool vec4_equal(Vec4 lhs, Vec4 rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

static inline Vec4 quaternion_from_rotation(Vec3 axis, float angle)
{
    const float half_angle = angle * 0.5f;
    const float cosha = cosf(half_angle);
    const float sinha = sinf(half_angle);
    const Vec4 res = {
        axis.x * sinha,
        axis.y * sinha,
        axis.z * sinha,
        cosha,
    };
    return res;
}

static inline Vec3 quaternion_to_rotation(Vec4 q, float *angle)
{
    const Vec3 v = { q.x, q.y, q.z };
    const float s = vec3_length(v);
    *angle = 2.0f * atan2f(s, q.w);
    return s ? vec3_mul(v, 1.0f / s) : make_vec3(1, 0, 0);
}

static inline Vec4 quaternion_mul(Vec4 lhs, Vec4 rhs)
{
    const Vec4 res = {
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
    };
    return res;
}

static inline Vec4 quaternion_inverse(Vec4 q)
{
    const Vec4 res = {
        -q.x,
        -q.y,
        -q.z,
        q.w,
    };
    return res;
}

static inline Vec3 quaternion_rotate_vec3(Vec4 q, Vec3 v)
{
    const Vec4 v4 = { v.x, v.y, v.z };
    const Vec4 q_inv = quaternion_inverse(q);
    const Vec4 v4_rot = quaternion_mul(q, quaternion_mul(v4, q_inv));
    const Vec3 res = {
        v4_rot.x,
        v4_rot.y,
        v4_rot.z,
    };
    return res;
}

static inline Vec3 quaternion_to_euler(Vec4 q)
{
    const float sinr = 2 * (q.w * q.x + q.y * q.z);
    const float cosr = 1 - 2 * (q.x * q.x + q.y * q.y);
    const float roll = atan2f(sinr, cosr);

    const float sinp = 2 * (q.w * q.y - q.z * q.x);
    const float pitch = sinp >= 0.999f ? PI / 2 : sinp <= -0.999f ? -PI / 2 : asinf(sinp);

    const float siny = 2 * (q.w * q.z + q.x * q.y);
    const float cosy = 1 - 2 * (q.y * q.y + q.z * q.z);
    const float yaw = atan2f(siny, cosy);
    const Vec3 res = {
        roll,
        pitch,
        yaw,
    };
    return res;
}

static inline Vec4 euler_to_quaternion(Vec3 xyz)
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

    const Vec4 res = {
        cy * sr * cp - sy * cr * sp,
        cy * cr * sp + sy * sr * cp,
        sy * cr * cp - cy * sr * sp,
        cy * cr * cp + sy * sr * sp,
    };
    return res;
}

static inline void quaternion_to_xyz(Vec3 *x, Vec3 *y, Vec3 *z, Vec4 q)
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
static inline Vec4 quaternion_nlerp(Vec4 a, Vec4 b, float t)
{
    const float dot = vec4_dot(a, b);
    const Vec4 q = vec4_mul_add(vec4_mul(a, 1 - t), b, dot >= 0 ? t : -t);
    return vec4_normalize(q);
}

static inline const Mat44 *mat44_identity()
{
    static Mat44 identity = { 
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    return &identity;
}

static inline void mat44_from_translation(Mat44 *res, Vec3 t)
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

static inline void mat44_from_quaternion(Mat44 *res, Vec4 q)
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

static inline void mat44_from_scale(Mat44 *res, Vec3 s)
{
    *res = *mat44_identity();
    res->xx = s.x;
    res->yy = s.y;
    res->zz = s.z;
}

static inline void xyz_from_quaternion(Vec3 *x, Vec3 *y, Vec3 *z, Vec4 q)
{
    const float d = vec4_dot(q, q);
    const float s = (d != 0.0f) ? 2.f / d : 1.f;

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

static inline void mat44_from_translation_rotation_scale(Mat44 *res, Vec3 t, Vec4 r, Vec3 s)
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

static inline void mat44_from_transform(Mat44 *res, const Transform *tm)
{
    mat44_from_translation_rotation_scale(res, tm->pos, tm->rot, tm->scl);
}

static inline void mat44_mul(Mat44 *res, const Mat44 *lhs, const Mat44 *rhs)
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

static inline void mat44_inverse(Mat44 *res44, const Mat44 *m44)
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

static inline Vec3 mat44_transform(const Mat44 *m, Vec3 v)
{
    Vec3 res;
    res.x = m->xx * v.x + m->yx * v.y + m->zx * v.z + m->wx;
    res.y = m->xy * v.x + m->yy * v.y + m->zy * v.z + m->wy;
    res.z = m->xz * v.x + m->yz * v.y + m->zz * v.z + m->wz;
    return res;
}

static inline Vec3 mat44_transform_no_translation(const Mat44 *m, Vec3 v)
{
    Vec3 res;
    res.x = m->xx * v.x + m->yx * v.y + m->zx * v.z;
    res.y = m->xy * v.x + m->yy * v.y + m->zy * v.z;
    res.z = m->xz * v.x + m->yz * v.y + m->zz * v.z;
    return res;
}

static inline Vec4 mat44_transform_vec4(const Mat44 *m, Vec4 v)
{
    Vec4 res;
    res.x = m->xx * v.x + m->yx * v.y + m->zx * v.z + v.w * m->wx;
    res.y = m->xy * v.x + m->yy * v.y + m->zy * v.z + v.w * m->wy;
    res.z = m->xz * v.x + m->yz * v.y + m->zz * v.z + v.w * m->wz;
    res.w = m->xw * v.x + m->yw * v.y + m->zw * v.z + v.w * m->ww;
    return res;
}

static inline float mat44_determinant(const Mat44 *m)
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

static inline float mat44_determinant33(const Mat44 *m)
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

static inline Vec4 mat44_to_quaternion(const Mat44 *m)
{
    // reference code from https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
    // Converting a Rotation Matrix to a Quaternion, Mike Day @ Insomniac
    const float m00 = m->xx, m10 = m->yx, m20 = m->zx;
    const float m01 = m->xy, m11 = m->yy, m21 = m->zy;
    const float m02 = m->xz, m12 = m->yz, m22 = m->zz;

    float t;
    Vec4 res;
    if (m22 < 0.f) {
        if (m00 > m11) {
            t = 1.f + m00 - m11 - m22;
            res.x = t;
            res.y = m01 + m10;
            res.z = m20 + m02;
            res.w = m12 - m21;
        } else {
            t = 1.f - m00 + m11 - m22;
            res.x = m01 + m10;
            res.y = t;
            res.z = m12 + m21;
            res.w = m20 - m02;
        }
    } else {
        if (m00 < -m11) {
            t = 1.f - m00 - m11 + m22;
            res.x = m20 + m02;
            res.y = m12 + m21;
            res.z = t;
            res.w = m01 - m10;
        } else {
            t = 1.f + m00 + m11 + m22;
            res.x = m12 - m21;
            res.y = m20 - m02;
            res.z = m01 - m10;
            res.w = t;
        }
    }

    float s = 0.5f / sqrtf(t);
    res.x *= s;
    res.y *= s;
    res.z *= s;
    res.w *= s;

    return res;
}

static inline void mat44_to_translation_rotation_scale(Vec3 *t, Vec4 *r, Vec3 *s, const Mat44 *m)
{
    memcpy(t, &m->wx, sizeof(*t));

    s->x = sqrtf(m->xx * m->xx + m->xy * m->xy + m->xz * m->xz);
    s->y = sqrtf(m->yx * m->yx + m->yy * m->yy + m->yz * m->yz);
    s->z = sqrtf(m->zx * m->zx + m->zy * m->zy + m->zz * m->zz);

    Mat44 tmp;
    memcpy(&tmp, m, sizeof(tmp));
    tmp.xx *= 1.f / s->x, tmp.xy *= 1.f / s->x, tmp.xz *= 1.f / s->x;
    tmp.yx *= 1.f / s->y, tmp.yy *= 1.f / s->y, tmp.yz *= 1.f / s->y;
    tmp.zx *= 1.f / s->z, tmp.zy *= 1.f / s->z, tmp.zz *= 1.f / s->z;

    bool is_mirrored = mat44_determinant33(&tmp) < 0.0f;
    if (!is_mirrored) {
        *r = mat44_to_quaternion(&tmp);
    } else {
        s->x = -s->x;
        s->y = -s->y;
        s->z = -s->z;

        Mat44 mirror = {
            -tmp.xx, -tmp.xy, -tmp.xz, tmp.xw,
            -tmp.yx, -tmp.yy, -tmp.yz, tmp.yw,
            -tmp.zx, -tmp.zy, -tmp.zz, tmp.zw,
            +tmp.wx, +tmp.wy, +tmp.wz, tmp.ww
        };
        *r = mat44_to_quaternion(&mirror);
    }
}

static inline Vec3 *mat44_x(Mat44 *m) { return (Vec3 *)&m->xx; };
static inline Vec3 *mat44_y(Mat44 *m) { return (Vec3 *)&m->yx; };
static inline Vec3 *mat44_z(Mat44 *m) { return (Vec3 *)&m->zx; };
static inline Vec3 *mat44_w(Mat44 *m) { return (Vec3 *)&m->wx; };

static inline Vec4 *mat44_x_vec4(Mat44 *m) { return (Vec4 *)&m->xx; };
static inline Vec4 *mat44_y_vec4(Mat44 *m) { return (Vec4 *)&m->yx; };
static inline Vec4 *mat44_z_vec4(Mat44 *m) { return (Vec4 *)&m->zx; };
static inline Vec4 *mat44_w_vec4(Mat44 *m) { return (Vec4 *)&m->wx; };
