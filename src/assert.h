#ifndef ASSERT_H
#define ASSERT_H
#include "display.h"
#include "palette.h"
#include "eadk.h"

// This is the best way to show an error, because crashing resets the calculator.
static inline void assert(bool cond) {
  if (!cond) {
    eadk_display_push_rect_uniform(DISPLAY_RECT, CLR_RED);
    eadk_timing_msleep(500);
  }
}
#endif
