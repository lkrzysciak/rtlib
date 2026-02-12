#pragma once

#include <stddef.h>
#include <stdint.h>

#define RTLIB__SPREAD_1(v) ((v) | ((v) >> 1))
#define RTLIB__SPREAD_2(v) (RTLIB__SPREAD_1(v) | (RTLIB__SPREAD_1(v) >> 2))
#define RTLIB__SPREAD_4(v) (RTLIB__SPREAD_2(v) | (RTLIB__SPREAD_2(v) >> 4))
#define RTLIB__SPREAD_8(v) (RTLIB__SPREAD_4(v) | (RTLIB__SPREAD_4(v) >> 8))
#define RTLIB__SPREAD_16(v) (RTLIB__SPREAD_8(v) | (RTLIB__SPREAD_8(v) >> 16))
#define RTLIB__SPREAD_32(v) (RTLIB__SPREAD_16(v) | (RTLIB__SPREAD_16(v) >> 32))

#if SIZE_MAX > 0xFFFFFFFFu
#define RTLIB__SPREAD_SIZE_T(v) RTLIB__SPREAD_32((size_t)(v))
#else
#define RTLIB__SPREAD_SIZE_T(v) RTLIB__SPREAD_16((size_t)(v))
#endif

#define RTLIB_NEXT_POW2_SIZE_T_CONST(x) \
    ((size_t)(x) <= (size_t)1 ? (size_t)1 : (RTLIB__SPREAD_SIZE_T((size_t)(x) - 1) + (size_t)1))
