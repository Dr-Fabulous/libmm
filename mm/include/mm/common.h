#ifndef MM_COMMON_H
#define MM_COMMON_H
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#if defined( __GNUC__ )\
 || defined( __clang__ )\
 || defined( __EMSCRIPTEN__ )\
 || defined( __MINGW32__ )\
 || defined( __MINGW64__ )
#define MM_IMPORT
#define MM_EXPORT __attribute__(( visibility("default") )) extern
#elif defined( _MSC_VER )
#define MM_IMPORT __declspec( dllimport )
#define MM_EXPORT __declspec( dllexport ) extern
#else
#error "unsupported compiler"
#endif

#ifdef MM_BUILDING
#define MM_FUNCTION MM_EXPORT
#else
#define MM_FUNCTION MM_IMPORT
#endif

#ifdef __has_builtin
#define MM_HAS_BUILTIN __has_builtin
#else
#define MM_HAS_BUILTIN( name ) 0
#endif

#ifdef __has_include
#define MM_HAS_INCLUDE __has_include
#else
#define MM_HAS_INCLUDE( name ) 0
#endif

#define MM_ARR_SIZE( arr )\
	( ( sizeof( arr ) / sizeof( arr[ 0 ] ) / ( ( size_t ) ( !( sizeof( arr ) / sizeof( arr[ 0 ] ) ) ) ) ) )

#define MM_ARR_NEXT( arr, pos )\
	( ( void* ) ( ( unsigned char* ) ( pos ) + sizeof( ( arr )[ 0 ] ) ) )

#define MM_ARR_FOREACH( arr, pos )\
	for( ( pos ) = ( arr );\
	     ( pos ) != ( arr ) + MM_ARR_SIZE( arr );\
	     ( pos ) = MM_ARR_NEXT( arr, pos ) )

#define MM_INTERNAL_CAT( lhs, rhs )\
	lhs##rhs

#define MM_CAT( lhs, rhs )\
	MM_INTERNAL_CAT( lhs, rhs )

#ifdef offsetof
#define MM_OFFSET_OF offsetof
#else
#define MM_OFFSET_OF( type, name )\
	( ( size_t ) &( ( type* ) NULL )->name )
#endif

#define MM_CONTAINER_OF( ptr, type, member )\
	( ( type* ) ( ( unsigned char* ) ( ptr ) - MM_OFFSET_OF( type, member ) ) )

#include "mm/config.h"
#endif
