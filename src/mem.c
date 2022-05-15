#include "stddef.h"
#include "string.h"
#include "config.h"

void * __attribute__((noinline)) memset(void * b, int c, size_t len) {
  char * destination = (char *)b;
  while (len--) {
    *destination++ = (unsigned char)c;
  }
  return b;
}

void * __attribute__((noinline)) memcpy(void * dst, const void * src, size_t n) {
  char * destination = (char *)dst;
  char * source = (char *)src;

  while (n--) {
    *destination++ = *source++;
  }

  return dst;
}

void * memmove(void * dst, const void * src, size_t n) {
  char * destination = (char *)dst;
  char * source = (char *)src;

  if (source < destination && destination < source + n) {
    /* Copy backwards to avoid overwrites */
    source += n;
    destination += n;
    while (n--) {
      *--destination = *--source;
    }
  } else {
    while (n--) {
      *destination++ = *source++;
    }
  }

  return dst;
}

void * memset_8(uint8_t * buffer, uint8_t pattern, size_t count) {
#if OPT_32
  uint32_t count_32 = count >> 2;
  count -= count_32 << 2;
  uint32_t pattern_32 = (uint32_t) pattern;
  pattern_32 |= pattern_32 << 8;
  pattern_32 |= pattern_32 << 16;
  uint32_t * buffer_32 = (uint32_t *) buffer;
  while (count_32--) {
    *buffer_32++ = pattern_32;
  }
  buffer = (uint8_t *)(buffer_32);
  while (count--) {
    *buffer++ = pattern;
  }
  return buffer;
#else
  while (count--) {
    *buffer++ = pattern;
  }
  return buffer;
#endif
}

void * memset_16(uint16_t * buffer, uint16_t pattern, size_t count) {
#if OPT_32
  uint32_t count_32 = count >> 1;
  uint32_t pattern_32 = (uint32_t) pattern;
  pattern_32 |= pattern_32 << 16;
  uint32_t * buffer_32 = (uint32_t *) buffer;
  while (count_32--) {
    *buffer_32++ = pattern_32;
  }
  count -= count >> 1 << 1;
  buffer = (uint16_t *)(buffer_32);
  while (count--) {
    *buffer++ = pattern;
  }
  return buffer;
#else
  while (count--) {
    *buffer++ = pattern;
  }
  return buffer;
#endif
}
