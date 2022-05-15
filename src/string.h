#ifndef STRING_H
#define STRING_H

#include "stddef.h"
#include "stdint.h"

void * memcpy(void * dst, const void * src, size_t n);
void * memset(void * b, int c, size_t len);
void * memmove(void * dst, const void * src, size_t n);
void * memset_8(uint8_t * b, uint8_t c, size_t len);
void * memset_16(uint16_t * b, uint16_t c, size_t len);
void * malloc(size_t size);
void * realloc(void *ptr, size_t size);
void * calloc(size_t count, size_t size);
void free(void* poiniter);

#endif
