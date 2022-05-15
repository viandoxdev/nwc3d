#include "stdint.h"
#include "stddef.h"
#include "string.h"
// For errors
#include "eadk.h"
#include "display.h"
#include "palette.h"

#define ALLOCATIONS_MAX 128
#define POOL_SIZE 512

typedef struct {
  void * ptr;
  size_t size;
} Allocation;

static Allocation allocations[ALLOCATIONS_MAX];
static size_t allocations_length = 0;
static uint8_t pool[POOL_SIZE];
static void* pool_ptr = (void *)pool;
static void* pool_end = NULL; // will be lazy intialized, because it isn't compile time constant.

inline static void init() {
  if (pool_end == NULL) {
    pool_end = pool_ptr + POOL_SIZE;
  }
}

static void * add_allocation(Allocation alloc, size_t index) {
  if (index >= allocations_length) { // I write >= but it shouldn't ever be >
    allocations[allocations_length++] = alloc;
    return alloc.ptr;
  }

  // shift all following elements
  memmove(allocations + index + 1, allocations + index, (allocations_length - index) * sizeof(Allocation));
  allocations[index] = alloc;
  allocations_length++;
  return alloc.ptr;
}

static void remove_allocation(size_t index) {
  if (index >= allocations_length) { // I write >= but it shouldn't ever be >
    allocations_length--;
  }
  memmove(allocations + index, allocations + index + 1, (allocations_length - index - 1) * sizeof(Allocation));
  allocations_length--;
}

static size_t find_allocation(void * ptr) {
  for(size_t i = 0; i < allocations_length; i++) {
    if(allocations[i].ptr == ptr)
      return i;
  }
  return ALLOCATIONS_MAX;
}

void * malloc(size_t size) {
  init();

  if (size == 0)
    return pool_end;
  if (allocations_length >= ALLOCATIONS_MAX || size > POOL_SIZE)
    return NULL;

  // The room from the pool start to the first allocation, usually zero.
  size_t room = allocations[0].ptr - pool_ptr;
  // if nothing is allocated or there is enough room at the begining of the pool
  if (allocations_length == 0 || room >= size)
    return add_allocation((Allocation) {pool_ptr, size}, 0);

  Allocation last_alloc = allocations[allocations_length - 1];
  // The room from the last allocation to the end of the pool
  room = pool_end - last_alloc.ptr - last_alloc.size;
  if (room >= size)
    return add_allocation((Allocation) {last_alloc.ptr + last_alloc.size, size}, allocations_length);

  // Iterate over every allocation (length - 1 because we look at i and i+1)
  for(size_t i = 0; i < allocations_length - 1; i++) {
    // start of unallocated space, end of allocation[i]
    void * start = allocations[i].ptr + allocations[i].size;
    // end of unallocated space, start of next allocation
    void * end = allocations[i+1].ptr;
    room = end - start;

    if (room >= size)
      return add_allocation((Allocation) {start, size}, i + 1);
  }
  // return NULL if we couldn't find the space.
  return NULL;
}

void free(void * ptr) {
  init();
  // pool_end is returned on malloc(0), and free must accept any pointer malloc returns
  if (ptr == pool_end)
    return;
  
  if (ptr >= pool_ptr && ptr < pool_end) {
    size_t index = find_allocation(ptr);
    if(index != ALLOCATIONS_MAX)
      return remove_allocation(index);
  }
  // We couldn't remove the allocation, error out
  eadk_display_push_rect_uniform(DISPLAY_RECT, CLR_RED);
  char error[] = "ERR FREE";
  eadk_display_draw_string(error, (Point) {0, 0}, true, CLR_WHITE, CLR_RED);
  eadk_timing_msleep(1000);
}

void * calloc(size_t count, size_t size) {
  void * ptr = malloc(size);
  if (!ptr)
    return NULL;
  if (!size) // don't memset if size is 0
    return ptr;
  memset(ptr, 0, size);
  return ptr;
}

void * realloc(void *ptr, size_t size) {
  init();

  if(ptr == NULL || ptr == pool_end)
    return malloc(size);

  size_t index = find_allocation(ptr);
  // if allocation is invalid
  if(index == ALLOCATIONS_MAX)
    return NULL;

  Allocation * alloc = &allocations[index];

  void * curr_start = alloc->ptr;
  void * next_start = index != allocations_length - 1 ?
    allocations[index + 1].ptr : // next alloc start if there's a next
    pool_end; // the end of the pool if there isn't

  // just resize if we realloc to a smaller size, or if there's enough space after
  if(size <= alloc->size || next_start - curr_start >= size) {
    alloc->size = size;
    return alloc->ptr;
  }

  void * curr_end = curr_start + alloc->size;
  void * last_end = index != 0 ?
    allocations[index - 1].ptr + allocations[index - 1].size :
    pool_ptr;
  // if there's place before
  if(curr_end - last_end >= size) {
    memmove(last_end, curr_start, alloc->size);
    alloc->size = size;
    alloc->ptr = last_end;
    return alloc->ptr;
  }

  // We couldn't find place arround the current allocation, se we alloc, copy and free.
  void * new_ptr = malloc(size);
  if(new_ptr == NULL)
    return NULL;

  memcpy(new_ptr, alloc->ptr, alloc->size);
  // same as free, but faster since we already know the index
  remove_allocation(index);
  return new_ptr;
}
