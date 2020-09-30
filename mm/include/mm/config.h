#ifndef MM_CONFIG_H
#define MM_CONFIG_H

// these macros can be changed to build libmm with a different allocator/logging utility
#define MM_MALLOC malloc
#define MM_CALLOC calloc
#define MM_REALLOC realloc
#define MM_FREE free
#define MM_STDIN stdin
#define MM_STDOUT stdout
#define MM_STDERR stderr
#define MM_PRINTF printf
#define MM_VPRINTF vprintf
#define MM_VFPRINTF vfprintf
#define MM_FPRINTF fprintf
#define MM_SPRINTF sprintf
#define MM_FFLUSH fflush

#endif
