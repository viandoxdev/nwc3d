#include "eadk.h"
#include "display.h"
#include "palette.h"
#include "string.h"
#include "stdlib.h"
#include "framebuffer.h"
#include "glmath.h"
#include "vector.h"
#include "math.h"

const char eadk_app_name[] = "la babajie";

typedef struct {
  Vector vertices;
  Vector indices;
} Mesh;

typedef struct {
  uint16_t a;
  uint16_t b;
  uint16_t c;
} Index;

typedef enum {
  Clockwise,
  CounterClockwise,
} Winding;

Winding triangle_get_winding(Triangle tri) {
  vec3 v21 = vec3_sub(vec3_from_point(tri.p1, 0), vec3_from_point(tri.p2, 0));
  vec3 v23 = vec3_sub(vec3_from_point(tri.p3, 0), vec3_from_point(tri.p2, 0));
  vec3 cross = vec3_cross(v21, v23);
  return cross.z < 0 ? Clockwise : CounterClockwise;
}

static const Winding CULL_WINDING = CounterClockwise;

Mesh mesh_new(vec3 * vertices, size_t vert_count, Index * indices, size_t idx_count) {
  return (Mesh) {
    vector_from_array(vertices, vert_count, sizeof(vec3)),
    vector_from_array(indices, idx_count, sizeof(Index)),
  };
}

void mesh_free(Mesh * mesh) {
  vector_free(&mesh->vertices);
  vector_free(&mesh->indices);
}

void eadk_main() {
  Framebuffer fb = framebuffer_new();
  vec3 v[] = {
    {-0.5, 0.5, 0.5},
    {-0.5,-0.5, 0.5},
    { 0.5, 0.5, 0.5},
    { 0.5,-0.5, 0.5},
    {-0.5, 0.5,-0.5},
    {-0.5,-0.5,-0.5},
    { 0.5, 0.5,-0.5},
    { 0.5,-0.5,-0.5}
  };
  Index i[] = {
    {0, 2, 3}, {0, 3, 1}, // Front
    {2, 6, 7}, {2, 7, 3}, // Right
    {6, 4, 5}, {6, 5, 7}, // Back
    {4, 0, 1}, {4, 1, 5}, // Left
    {0, 4, 6}, {0, 6, 2}, // Top
    {1, 3, 7}, {1, 7, 5}, // Bottom
  };
  Mesh m = mesh_new(v, 8, i, 12);
  Vector tris = vector_of(Triangle);
  float t = 0;

  while (1) {
    vector_clear(&tris);
    quat rot = quat_rotation_y(t);
    rot = quat_mul(rot, quat_rotation_x(sinf(t) * 2));
    rot = quat_mul(rot, quat_rotation_z(sinf(t * 2) * 2));
    mat4 model_mat = mat4_scale_rotation_translation(VEC3_ONE, rot, (vec3) {0, 0, 3.0});
    mat4 perspective_mat = mat4_perspective_rh_gl(M_PI_4, DISPLAY_ASPECT, 0.1, 10.0);
    mat4 mp = mat4_mul(perspective_mat, model_mat);

    for(size_t i = 0; i < m.indices.used; i++) {
      vec3 * verts = m.vertices.data;
      Index idx = *(Index*)vector_get_ptr(&m.indices, i);
      Triangle t = (Triangle) {
        vec3_point(vec4_perspective_divide(mat4_mul_vec(mp, vec4_from_vec3(verts[idx.a], 1.0)))),
        vec3_point(vec4_perspective_divide(mat4_mul_vec(mp, vec4_from_vec3(verts[idx.b], 1.0)))),
        vec3_point(vec4_perspective_divide(mat4_mul_vec(mp, vec4_from_vec3(verts[idx.c], 1.0)))),
        //vec3_point(verts[idx.a]),
        //vec3_point(verts[idx.b]),
        //vec3_point(verts[idx.c]),
      };
      Winding w = triangle_get_winding(t);

      if(w != CULL_WINDING)
        vector_push(&tris, &t);
    }
    framebuffer_clear(&fb, CLR_BLACK);
    Color clrs[4] = {CLR_RED, CLR_GREEN, CLR_ORANGE, CLR_LIGHT_BLUE};
    for(size_t i = 0; i < tris.used; i++) {
      Triangle * tri = vector_get_ptr(&tris, i);
      framebuffer_triangle(&fb, *tri, clrs[i & 3]);
      //framebuffer_line(&fb, tri->p1, tri->p2, CLR_WHITE);
      //framebuffer_line(&fb, tri->p2, tri->p3, CLR_WHITE);
      //framebuffer_line(&fb, tri->p1, tri->p3, CLR_WHITE);
    }
    t += 0.01;
    eadk_display_wait_for_vblank();
  }
}
