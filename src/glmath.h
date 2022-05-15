#ifndef GLMATH_H
#define GLMATH_H
#include "eadk.h"

// theses are structs because I want to be able to assign them

typedef struct {
  float m[4];
} mat2;

typedef struct {
  float m[9];
} mat3;

typedef struct {
  float m[16];
} mat4;

typedef struct {
  float x;
  float y;
} vec2;

typedef struct {
  float x;
  float y;
  float z;
} vec3;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} vec4;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} quat;

extern const mat2 MAT2_IDENTITY;
extern const mat3 MAT3_IDENTITY;
extern const mat4 MAT4_IDENTITY;
extern const vec2 VEC2_ZERO;
extern const vec2 VEC2_ONE;
extern const vec3 VEC3_ZERO;
extern const vec3 VEC3_ONE;
extern const vec4 VEC4_ZERO;
extern const vec4 VEC4_ONE;

mat4 mat4_from_cols(vec4 a, vec4 b, vec4 c, vec4 d);
mat4 mat4_scale_rotation_translation(vec3 scale, quat rot, vec3 translation);
mat4 mat4_perspective_rh_gl(float fov_y_radians, float aspect_ratio, float z_near, float z_far);
vec4 mat4_x_axis(mat4 mat);
vec4 mat4_y_axis(mat4 mat);
vec4 mat4_z_axis(mat4 mat);
vec4 mat4_w_axis(mat4 mat);
vec4 mat4_mul_vec(mat4 mat, vec4 v);
mat4 mat4_mul(mat4 a, mat4 b);

Point vec2_point(vec2 v);
vec2 vec2_from_point(Point p);

vec2 vec3_xy(vec3 v);
Point vec3_point(vec3 v);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_from_vec2(vec2 v, float z);
vec3 vec3_cross(vec3 a, vec3 b);
vec3 vec3_from_point(Point p, float z);

vec4 vec4_from_vec3(vec3 v, float w);
vec4 vec4_mul_scalar(vec4 v, float scalar);
vec3 vec4_xyz(vec4 v);
vec3 vec4_perspective_divide(vec4 v);
vec4 vec4_mul(vec4 a, vec4 b);
vec4 vec4_add(vec4 a, vec4 b);
vec4 vec4_splat(float s);
vec4 vec4_splat_x(vec4 v);
vec4 vec4_splat_y(vec4 v);
vec4 vec4_splat_z(vec4 v);
vec4 vec4_splat_w(vec4 v);

quat quat_rotation_x(float angle);
quat quat_rotation_y(float angle);
quat quat_rotation_z(float angle);
quat quat_mul(quat a, quat b);
#endif
