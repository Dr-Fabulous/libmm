#ifndef LIBMM_COMMON_H
#define LIBMM_COMMON_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#if defined( __GNUC__ ) || defined( __clang__ ) || defined( __EMSCRIPTEN__ ) || defined( __MINGW32__ ) || defined( __MINGW64__ )

#define LIBMM_IMPORT
#define LIBMM_EXPORT __attribute__(( visibility("default") )) extern

#elif defined( _MSC_VER )

#define LIBMM_IMPORT __declspec( dllimport )
#define LIBMM_EXPORT __declspec( dllexport ) extern

#else
#error "compiler is missing import/export attributes"
#endif

#ifdef LIBMM_EXPORTING
#define LIBMM_FUNCTION LIBMM_EXPORT
#else
#define LIBMM_FUNCTION LIBMM_IMPORT
#endif

// force expansion of macros before concat
#define LIBMM_INTERNAL_CAT( lhs, rhs ) lhs##rhs
#define LIBMM_CAT( lhs, rhs ) LIBMM_INTERNAL_CAT( lhs, rhs )

#include "libmm/config.h"
#endif
