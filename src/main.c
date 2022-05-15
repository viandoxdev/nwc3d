#include "eadk.h"
#include "display.h"
#include "palette.h"
#include "string.h"
#include "stdlib.h"
#include "framebuffer.h"
#include "glmath.h"
#include "vector.h"

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
    {-0.5,  0.5, -0.5},
    { 0.5,  0.5, -0.5},
    { 0.5, -0.5, -0.5},
    {-0.5, -0.5, -0.5},
    {-0.5,  0.5,  0.5},
    { 0.5,  0.5,  0.5},
    { 0.5, -0.5,  0.5},
    {-0.5, -0.5,  0.5},
  };
  Index i[] = {
    {3, 0, 1}, {1, 2, 3},
    {6, 5, 1}, {1, 2, 6},
    {3, 0, 4}, {4, 7, 3},
    {4, 0, 1}, {1, 5, 4},
    {3, 7, 6}, {6, 2, 3},
    {7, 4, 5}, {5, 6, 7},
  };
  Mesh m = mesh_new(v, 8, i, 12);

  Vector tris = vector_of(Triangle);
  for(size_t i = 0; i < m.indices.used; i++) {
    vec3 * verts = m.vertices.data;
    Index idx = *(Index*)vector_get_ptr(&m.indices, i);
    Triangle t = (Triangle) {
      vec3_point(verts[idx.a]),
      vec3_point(verts[idx.b]),
      vec3_point(verts[idx.c]),
    };
    vector_push(&tris, &t);
  }

  for(size_t i = 0; i < tris.used; i++) {
    Triangle * tri = vector_get_ptr(&tris, i);
    framebuffer_line(&fb, tri->p1, tri->p2, CLR_WHITE);
    framebuffer_line(&fb, tri->p2, tri->p3, CLR_WHITE);
    framebuffer_line(&fb, tri->p1, tri->p3, CLR_WHITE);
  }
  
  while (1) {
  }
}
