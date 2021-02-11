#ifndef MM_COMMON_H
#define MM_COMMON_H
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdalign.h>
#include <assert.h>

// Release/Debug mode controlled with NDEBUG macro
#if !defined( NDEBUG ) && !defined( MM_DEBUG )
#define MM_DEBUG
#elif defined( NDEBUG ) && defined( MM_DEBUG )
#undef MM_DEBUG
#endif

// compiler detection
#if defined( __GNUC__ )\
 || defined( __clang__ )\
 || defined( __EMSCRIPTEN__ )\
 || defined( __MINGW32__ )\
 || defined( __MINGW64__ )
#define MM_USING_GCC 1
#elif defined( _MSC_VER )
#define MM_USING_MSVC 1
#else
#error "unsupported compiler"
#endif

// os detection
#if defined( unix )\
 || defined( __unix )\
 || defined( __unix__ )\
 || defined( __linux__ )\
 || defined( __FREEBSD__ )\
 || defined( __APPLE__ )\
 || defined( __MACH__ )
#define MM_USING_POSIX 1
#elif defined( WIN32 )\
   || defined( _WIN32 )\
   || defined( __WIN32__ )\
   || defined( _WIN64 )
#define MM_USING_WINDOWS 1
#else
#error "unsupported os"
#endif

// inline
#ifdef MM_USING_GCC
#define MM_FORCE_INLINE __attribute__(( always_inline ))
#elif defined( MM_USING_MSVC )
#define MM_FORCE_INLINE __forceinline
#endif

// exports
#ifdef MM_USING_GCC
#define MM_IMPORT
#define MM_EXPORT __attribute__(( visibility( "default" ) )) extern
#elif defined( MM_USING_MSVC )
#define MM_IMPORT __declspec( dllimport )
#define MM_EXPORT __declspec( dllexport ) extern
#endif

// defined by cmake
#ifdef mm_EXPORTS
#define MM_API MM_EXPORT
#else
#define MM_API MM_IMPORT
#endif

#include "mm/macro.h"
#include "mm/config.h"

#define MM_DO_IS_VERSION_OR_LATER( c_major, c_minor, c_patch, major, minor, patch )\
	   ( c_major > major\
	|| ( c_major == major && c_minor > minor )\
	|| ( c_major == major && c_minor == minor && c_patch >= patch ) )

#define MM_IS_VERSION_OR_LATER( major, minor, patch )\
	MM_DO_IS_VERSION_OR_LATER( MM_MAJOR, MM_MINOR, MM_PATCH, major, minor, patch )

#ifdef MM_USING_GCC
#define MM_GCC_IS_VERSION_OR_LATER( major, minor, patch )\
	MM_DO_IS_VERSION_OR_LATER( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, major, minor, patch )

#if __has_builtin
#define MM_GCC_HAS_BUILTIN( name ) __has_builtin( name )
#else
#define MM_GCC_HAS_BUILTIN( name ) 0
#endif

#ifdef __has_include
#define MM_GCC_HAS_INCLUDE( name ) __has_include( name )
#else
#define MM_GCC_HAS_INCLUDE( name ) 0
#endif

#else
#define MM_GCC_IS_VERSION_OR_LATER( major, minor, patch) 0
#define MM_GCC_HAS_BUILTIN( name ) 0
#define MM_GCC_HAS_INCLUDE( name ) 0
#endif

#ifdef MM_USING_MSVC
#else
#endif

#endif
