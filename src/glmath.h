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

Point vec2_point(vec2 v);
vec2 vec3_xy(vec3 v);
Point vec3_point(vec3 v);
#endif
