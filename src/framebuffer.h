#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "config.h"
#include "eadk.h"
#include "stdint.h"

typedef struct {
  Point p1;
  Point p2;
  Point p3;
} Triangle;

typedef struct {
#if FB_VIRT
#else
  Color pixels[DISPLAY_PIXELS];
  uint8_t depth[DISPLAY_PIXELS];
#endif
} Framebuffer;

Framebuffer framebuffer_new();
void framebuffer_set_pixel(Framebuffer * fb, Point pixel, Color color);
void framebuffer_line(Framebuffer * fb, Point a, Point b, Color color);
void framebuffer_set_raw(Framebuffer * fb, uint16_t y, uint16_t x0, uint16_t x1, Color color);
void _bounds_line(Point a, Point b, uint16_t yoffset);
void framebuffer_triangle(Framebuffer * fb, Triangle tri, Color color);
void framebuffer_clear(Framebuffer * fb, Color clear_color);
void framebuffer_push(Framebuffer * fb);

#endif
