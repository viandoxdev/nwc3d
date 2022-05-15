#ifndef EADK_H
#define EADK_H

#include "stdint.h"
#include "stdbool.h"

typedef uint16_t Color;
inline Color color_new(uint32_t rgb) {
  return (rgb >> 3 & 0x1F) | (rgb >> 5 & 0x7E0) | ( rgb >> 8 & 0xF800);
}
inline Color color_rgb(uint8_t r, uint8_t g, uint8_t b) {
  return (r >> 3) | (g >> 2 << 5) | (b >> 3 << 11);
}

struct Point {
  uint16_t x;
  uint16_t y;
};
typedef struct Point Point;

struct Rect {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};
typedef struct Rect Rect;

enum Key {
  Left = 0,
  Up = 1,
  Down = 2,
  Right = 3,
  OK = 4,
  Back = 5,
  Home = 6,
  Shift = 12,
  Alpha = 13,
  XNT = 14,
  Var = 15,
  Toolbox = 16,
  Backspace = 17,
  Exp = 18,
  Ln = 19,
  Log = 20,
  Imaginary = 21,
  Comma = 22,
  Power = 23,
  Sine = 24,
  Cosine = 25,
  Tangent = 26,
  Pi = 27,
  Sqrt = 28,
  Square = 29,
  Seven = 30,
  Eight = 31,
  Nine = 32,
  LeftParenthesis = 33,
  RightParenthesis = 34,
  Four = 36,
  Five = 37,
  Six = 38,
  Multiplication = 39,
  Division = 40,
  One = 42,
  Two = 43,
  Three = 44,
  Plus = 45,
  Minus = 46,
  Zero = 48,
  Dot = 49,
  EE = 50,
  Ans = 51,
  EXE = 52,
};
typedef enum Key Key;

typedef uint64_t State;

inline bool state_key_down(State state, Key k) {
  return state >> (uint8_t)k & 1;
}

uint8_t eadk_backlight_brightness();
void eadk_backlight_set_brightness(uint8_t brightness);

bool eadk_battery_is_charging();
uint8_t eadk_battery_level();
float eadk_battery_voltage();

void eadk_display_pull_rect(Rect r, Color * pixels);
void eadk_display_push_rect(Rect rect, const Color * pixels);
void eadk_display_push_rect_uniform(Rect rect, Color color);
void eadk_display_draw_string(const char * text, Point p, bool largeFont, Color textColor, Color backgroundColor);
bool eadk_display_wait_for_vblank();

State eadk_keyboard_scan();

uint64_t eadk_timing_millis();
void eadk_timing_msleep(uint32_t ms);
void eadk_timing_usleep(uint32_t us);

uint32_t eadk_random();

#endif
