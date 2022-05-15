#include "framebuffer.h"
#include "display.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

Framebuffer framebuffer_new() {
  Framebuffer fb = {};
#if FB_VIRT
  eadk_display_push_rect_uniform(DISPLAY_RECT, 0);
#endif
  return fb;
}

void framebuffer_set_pixel(Framebuffer * fb, Point pixel, Color color) {
#if FB_VIRT
  eadk_display_push_rect_uniform((Rect) {pixel.x, pixel.y, 1, 1}, color);
#else
  fb->pixels[pixel.y * DISPLAY_WIDTH + pixel.x] = color;
#endif
}

// Shamelesly stolen code
void framebuffer_line(Framebuffer * fb, Point a, Point b, Color color) {
  int x0 = a.x, y0 = a.y;
  int x1 = b.x, y1 = b.y;
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = dx+dy, e2; /* error value e_xy */

  for(;;){
    framebuffer_set_pixel(fb, (Point) {x0, y0}, color);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

// efficiently set a row of the framebuffer to one color
void framebuffer_set_row(Framebuffer * fb, uint16_t y, uint16_t x0, uint16_t x1, Color color) {
  assert(x0 <= x1);
  uint16_t width = x1 - x0;
#if FB_VIRT
  eadk_display_push_rect_uniform((Rect) {x0, y, width, 1}, color);
#else
  memset_16((uint16_t) color, fb->pixels + y * DISPLAY_WIDTH + x0, width);
#endif
}

// buffers used to rasterize triangles
static uint16_t TRIANGLE_BOUNDS_MIN[DISPLAY_HEIGHT] = {};
static uint16_t TRIANGLE_BOUNDS_MAX[DISPLAY_HEIGHT] = {};

void _bounds_line(Point a, Point b, uint16_t yoffset) {
  int x0 = a.x, y0 = a.y - yoffset;
  int x1 = b.x, y1 = b.y - yoffset;
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = dx+dy, e2; /* error value e_xy */

  for(;;){  /* loop */
    TRIANGLE_BOUNDS_MIN[y0] = min(TRIANGLE_BOUNDS_MIN[y0], x0);
    TRIANGLE_BOUNDS_MAX[y0] = max(TRIANGLE_BOUNDS_MAX[y0], x0);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

void framebuffer_triangle(Framebuffer * fb, Triangle tri, Color color) {
  uint16_t min_y = min(min(tri.p1.y, tri.p2.y), tri.p3.y);
  uint16_t max_y = max(max(tri.p1.y, tri.p2.y), tri.p3.y);
  uint16_t height = max_y - min_y;

  // reset bounds
  memset_16(TRIANGLE_BOUNDS_MAX, 0, DISPLAY_HEIGHT);
  memset_16(TRIANGLE_BOUNDS_MIN, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT);
  
  _bounds_line(tri.p1, tri.p2, min_y);
  _bounds_line(tri.p2, tri.p3, min_y);
  _bounds_line(tri.p1, tri.p3, min_y);

  for (uint16_t y = 0; y < height; y++) {
    uint16_t x0 = TRIANGLE_BOUNDS_MIN[y];
    uint16_t x1 = TRIANGLE_BOUNDS_MAX[y];
    framebuffer_set_row(fb, y + min_y, x0, x1, color);
  }
}

void framebuffer_clear(Framebuffer * fb, Color clear_color) {
#if FB_VIRT
  eadk_display_push_rect_uniform(DISPLAY_RECT, clear_color);
#else
  memset_16(fb->pixels, (uint16_t) clear_color, DISPLAY_PIXELS);
  memset_8(fb->depth, 0, DISPLAY_PIXELS);
#endif
}

void framebuffer_push(Framebuffer * fb) {
#if FB_VIRT
#else
  eadk_display_push_rect(DISPLAY_RECT, fb->pixels);
#endif
}

//Triangle triangle_sort(Triangle tri) {
//  Triangle stri;
//  if (tri.p1.y < tri.p2.y) {
//    if (tri.p1.y < tri.p3.y) {
//      stri.p1 = tri.p1;
//      if (tri.p2.y < tri.p3.y) {
//        stri.p2 = tri.p2;
//        stri.p3 = tri.p3;
//      } else {
//        stri.p2 = tri.p2;
//        stri.p3 = tri.p3;
//      }
//    } else {
//      stri.p1 = tri.p3;
//      stri.p2 = tri.p1;
//      stri.p3 = tri.p2;
//    }
//  } else {
//    if (tri.p2.y < tri.p3.y) {
//      stri.p1 = tri.p2;
//      if (tri.p1.y < tri.p3.y) {
//        stri.p2 = tri.p1;
//        stri.p3 = tri.p3;
//      } else {
//        stri.p2 = tri.p3;
//        stri.p3 = tri.p1;
//      }
//    } else {
//      stri.p1 = tri.p3;
//      stri.p2 = tri.p2;
//      stri.p3 = tri.p1;
//    }
//  }
//  return stri;
//}
