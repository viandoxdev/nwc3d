#include "glmath.h"
#include "display.h"

#define m2_11 0
#define m2_21 1
#define m2_12 2
#define m2_22 3

#define m3_11 0
#define m3_21 1
#define m3_31 2
#define m3_12 3
#define m3_22 4
#define m3_32 5
#define m3_13 6
#define m3_23 7 
#define m3_33 8

#define m4_11 0
#define m4_21 1
#define m4_31 2
#define m4_41 3
#define m4_12 4
#define m4_22 5
#define m4_32 6
#define m4_42 7
#define m4_13 8
#define m4_23 9
#define m4_33 10
#define m4_43 11
#define m4_14 12
#define m4_24 13
#define m4_34 14
#define m4_44 15

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

Point vec2_point(vec2 v) {
  return (Point) {
    (uint16_t)(bound(0, v.x, 1) * DISPLAY_WIDTH),
    (uint16_t)(bound(0, v.y, 1) * DISPLAY_HEIGHT),
  };
}

vec2 vec3_xy(vec3 v) {
  return (vec2) {v.x, v.y};
}

Point vec3_point(vec3 v) {
  return vec2_point(vec3_xy(v));
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
