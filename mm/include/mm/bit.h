#ifndef MM_BIT_H
#define MM_BIT_H
#include "mm/common.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef _MSC_VER
#define mm_popcount_u16 __popcnt16
#define mm_popcount_u32 __popcnt
#define mm_popcount_u64 __popcnt64
#elif ( __GNUC__ > 4 )\
   || MM_HAS_BUILTIN( __builtin_popcount )
#define mm_popcount_u16 __bultin_popcount
#define mm_popcount_u32 __bultin_popcountl
#define mm_popcount_u64 __bultin_popcountll
#else
#endif

// parity

// count leading zeros

// count trailing zeros

// rank

// select

#endif
