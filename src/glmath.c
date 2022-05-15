#include "glmath.h"
#include "display.h"
#include "math.h"

//      mX_x_y
#define m2_1_1 0
#define m2_2_1 1
#define m2_1_2 2
#define m2_2_2 3

#define m3_1_1 0
#define m3_2_1 1
#define m3_3_1 2
#define m3_1_2 3
#define m3_2_2 4
#define m3_3_2 5
#define m3_1_3 6
#define m3_2_3 7 
#define m3_3_3 8

#define m4_1_1 0
#define m4_2_1 1
#define m4_3_1 2
#define m4_4_1 3
#define m4_1_2 4
#define m4_2_2 5
#define m4_3_2 6
#define m4_4_2 7
#define m4_1_3 8
#define m4_2_3 9
#define m4_3_3 10
#define m4_4_3 11
#define m4_1_4 12
#define m4_2_4 13
#define m4_3_4 14
#define m4_4_4 15

const mat2 MAT2_IDENTITY = (mat2) {{
  1.0, 0.0,
  0.0, 1.0,
}};

const mat3 MAT3_IDENTITY = (mat3) {{
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
}};

const mat4 MAT4_IDENTITY = (mat4) {{
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0,
}};

const vec2 VEC2_ZERO = (vec2) {0.0, 0.0};
const vec2 VEC2_ONE  = (vec2) {1.0, 1.0};

const vec3 VEC3_ZERO = (vec3) {0.0, 0.0, 0.0};
const vec3 VEC3_ONE  = (vec3) {1.0, 1.0, 1.0};

const vec4 VEC4_ZERO = (vec4) {0.0, 0.0, 0.0, 0.0};
const vec4 VEC4_ONE  = (vec4) {1.0, 1.0, 1.0, 1.0};

static float bound(float min, float v, float max) {
  return v < min ? min : v > max ? max : v;
}

void quaternion_to_axes(quat q, vec4 out[3]) {
    float x2 = q.x + q.x;
    float y2 = q.y + q.y;
    float z2 = q.z + q.z;
    float xx = q.x * x2;
    float xy = q.x * y2;
    float xz = q.x * z2;
    float yy = q.y * y2;
    float yz = q.y * z2;
    float zz = q.z * z2;
    float wx = q.w * x2;
    float wy = q.w * y2;
    float wz = q.w * z2;

    out[0] = (vec4) {1.0 - (yy + zz), xy + wz, xz - wy, 0.0};
    out[1] = (vec4) {xy - wz, 1.0 - (xx + zz), yz + wx, 0.0};
    out[2] = (vec4) {xz + wy, yz - wx, 1.0 - (xx + yy), 0.0};
}

mat4 mat4_from_cols(vec4 a, vec4 b, vec4 c, vec4 d) {
  return (mat4) {{
    a.x, b.x, c.x, d.x,
    a.y, b.y, c.y, d.y,
    a.z, b.z, c.z, d.z,
    a.w, b.w, c.w, d.w,
  }};
}

mat4 mat4_scale_rotation_translation(vec3 scale, quat rot, vec3 translation) {
  vec4 axes[3];
  quaternion_to_axes(rot, axes);
  axes[0] = vec4_mul_scalar(axes[0], scale.x);
  axes[1] = vec4_mul_scalar(axes[1], scale.y);
  axes[2] = vec4_mul_scalar(axes[2], scale.z);
  return mat4_from_cols(axes[0], axes[1], axes[2], vec4_from_vec3(translation, 1.0));
}

vec4 mat4_x_axis(mat4 mat) {
  return (vec4) {mat.m[m4_1_1], mat.m[m4_1_2], mat.m[m4_1_3], mat.m[m4_1_4]};
}                                                                    
vec4 mat4_y_axis(mat4 mat) {                                         
  return (vec4) {mat.m[m4_2_1], mat.m[m4_2_2], mat.m[m4_2_3], mat.m[m4_2_4]};
}                                                                    
vec4 mat4_z_axis(mat4 mat) {                                         
  return (vec4) {mat.m[m4_3_1], mat.m[m4_3_2], mat.m[m4_3_3], mat.m[m4_3_4]};
}                                                                    
vec4 mat4_w_axis(mat4 mat) {                                         
  return (vec4) {mat.m[m4_4_1], mat.m[m4_4_2], mat.m[m4_4_3], mat.m[m4_4_4]};
}

vec4 mat4_mul_vec(mat4 mat, vec4 v) {
  vec4 res =          vec4_mul(mat4_x_axis(mat), vec4_splat_x(v)) ;
  res = vec4_add(res, vec4_mul(mat4_y_axis(mat), vec4_splat_y(v)));
  res = vec4_add(res, vec4_mul(mat4_z_axis(mat), vec4_splat_z(v)));
  res = vec4_add(res, vec4_mul(mat4_w_axis(mat), vec4_splat_w(v)));
  return res;
}

mat4 mat4_mul(mat4 a, mat4 b) {
  return mat4_from_cols(
    mat4_mul_vec(a, mat4_x_axis(b)),
    mat4_mul_vec(a, mat4_y_axis(b)),
    mat4_mul_vec(a, mat4_z_axis(b)),
    mat4_mul_vec(a, mat4_w_axis(b))
  );
}

mat4 mat4_perspective_rh_gl(float fov_y_radians, float aspect_ratio, float z_near, float z_far) {
    float inv_length = 1.0 / (z_near - z_far);
    float f = 1.0 / tanf(0.5 * fov_y_radians);
    float a = f / aspect_ratio;
    float b = (z_near + z_far) * inv_length;
    float c = (2.0 * z_near * z_far) * inv_length;
    return mat4_from_cols(
        (vec4) {a, 0.0, 0.0,  0.0},
        (vec4) {0.0, f, 0.0,  0.0},
        (vec4) {0.0, 0.0, b, -1.0},
        (vec4) {0.0, 0.0, c,  0.0}
    );
}

Point vec2_point(vec2 v) {
  return (Point) {
    (uint16_t)((bound(-1, v.x, 1) + 1) * DISPLAY_WIDTH / 2),
    (uint16_t)((bound(-1, v.y, 1) + 1) * DISPLAY_HEIGHT / 2),
  };
}

vec2 vec2_from_point(Point p) {
  return (vec2) {(float) p.x, (float) p.y};
}

vec2 vec3_xy(vec3 v) {
  return (vec2) {v.x, v.y};
}

Point vec3_point(vec3 v) {
  return vec2_point(vec3_xy(v));
}

vec3 vec3_from_point(Point p, float z) {
  return vec3_from_vec2(vec2_from_point(p), z);
}

vec3 vec3_sub(vec3 a, vec3 b) {
  return (vec3) {
    a.x - b.x,
    a.y - b.y,
    a.z - b.z,
  };
}

vec3 vec3_from_vec2(vec2 v, float z) {
  return (vec3) {v.x, v.y, z};
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3) {
        a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y,
    };
}

vec3 vec4_xyz(vec4 v) {
  return (vec3) {v.x, v.y, v.z};
}

vec3 vec4_perspective_divide(vec4 v) {
  float w = v.w == 0 ? 1.0 : 1.0 / v.w;
  return (vec3) {v.x * w, v.y * w, v.z * w};
}

vec4 vec4_splat(float s) {
  return (vec4) {s, s, s, s};
}

vec4 vec4_splat_x(vec4 v) { return vec4_splat(v.x); }
vec4 vec4_splat_y(vec4 v) { return vec4_splat(v.y); }
vec4 vec4_splat_z(vec4 v) { return vec4_splat(v.z); }
vec4 vec4_splat_w(vec4 v) { return vec4_splat(v.w); }

vec4 vec4_from_vec3(vec3 v, float w) {
  return (vec4) {v.x, v.y, v.z, w};
}

vec4 vec4_mul_scalar(vec4 v, float s) {
  return (vec4) {v.x * s, v.y * s, v.z * s, v.w * s};
}

vec4 vec4_mul(vec4 a, vec4 b) {
  return (vec4) {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

vec4 vec4_add(vec4 a, vec4 b) {
  return (vec4) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

quat quat_rotation_x(float angle) {
  float s = sinf(angle * 0.5);
  float c = cosf(angle * 0.5);
  return (quat) {s, 0, 0, c};
}

quat quat_rotation_y(float angle) {
  float s = sinf(angle * 0.5);
  float c = cosf(angle * 0.5);
  return (quat) {0, s, 0, c};
}

quat quat_rotation_z(float angle) {
  float s = sinf(angle * 0.5);
  float c = cosf(angle * 0.5);
  return (quat) {0, 0, s, c};
}

quat quat_mul(quat a, quat b) {
  return (quat) {
    a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
    a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
    a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
  };
}

#undef m2_11
#undef m2_21
#undef m2_12
#undef m2_22
#undef m3_11
#undef m3_21
#undef m3_31
#undef m3_12
#undef m3_22
#undef m3_32
#undef m3_13
#undef m3_23
#undef m3_33
#undef m4_11
#undef m4_21
#undef m4_31
#undef m4_41
#undef m4_12
#undef m4_22
#undef m4_32
#undef m4_42
#undef m4_13
#undef m4_23
#undef m4_33
#undef m4_43
#undef m4_14
#undef m4_24
#undef m4_34
#undef m4_44
