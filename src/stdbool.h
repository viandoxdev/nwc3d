#ifndef STDBOOL_H
#define STDBOOL_H
// Just here to make my editor shut up about redefining bool.
#ifndef __cplusplus
#include "stdint.h"
typedef uint32_t bool;
#define true 1
#define false 0
#endif
#endif
