#ifndef MM_COMMON_H
#define MM_COMMON_H
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdalign.h>

// Release/Debug mode controlled with NDEBUG macro
#if !defined( NDEBUG ) && !defined( MM_DEBUG )
#define MM_DEBUG
#elif defined( NDEBUG ) && defined( MM_DEBUG )
#undef MM_DEBUG
#endif

#if defined( __GNUC__ )\
 || defined( __clang__ )\
 || defined( __EMSCRIPTEN__ )\
 || defined( __MINGW32__ )\
 || defined( __MINGW64__ )
#define MM_IMPORT
#define MM_EXPORT __attribute__(( visibility( "default" ) )) extern
#elif defined( _MSC_VER )
#define MM_IMPORT __declspec( dllimport )
#define MM_EXPORT __declspec( dllexport ) extern
#else
#error "unsupported compiler"
#endif

#ifdef mm_EXPORTS
#define MM_API MM_EXPORT
#else
#define MM_API MM_IMPORT
#endif

#ifdef __has_builtin
#define MM_HAS_BUILTIN __has_builtin
#else
#define MM_HAS_BUILTIN( name ) 0
#endif

// added in GCC 10
#ifdef __has_include
#define MM_HAS_INCLUDE __has_include
#else
#define MM_HAS_INCLUDE( name ) 0
#endif

#include "mm/macro.h"
#include "mm/config.h"
#endif
