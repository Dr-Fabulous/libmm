#ifndef MM_SBF_H
#define MM_SBF_H
#include "mm/common.h"
#include "mm/vector.h"
#include <stdio.h>

/*! \file */

typedef enum mm_sbf_type {
	MM_SBF_I8, //!< signed 8 bit int
	MM_SBF_I16, //!< signed 16 bit int
	MM_SBF_I32, //!< signed 32 bit int
	MM_SBF_I64, //!< signed 64 bit int
	MM_SBF_U8, //!< unsigned 8 bit int
	MM_SBF_U16, //!< unsigned 16 bit int
	MM_SBF_U32, //!< unsigned 32 bit int
	MM_BBF_I64, //!< unsigned 64 bit int
	MM_SBF_F32, //!< single floating point
	MM_SBF_F64, //!< double floating point
	MM_SBF_STRING, //!< c-string
	MM_SBF_BLOB, //!< binary object
	MM_SBF_ARRAY, //!< array of values
	MM_SBF_OBJECT //!< key/value store
} mm_sbf_type_t;

typedef enum mm_sbf_serial_type {
	MM_SBF_USE_VEC, //!< sbf is stored using an mm_vector
	MM_SBF_USE_BUF, //!< sbf is stored using a static buf
	MM_SBF_USE_FILE //!< sbf is stored using a FILE ptr
} mm_sbf_serial_type;

typedef union mm_sbf_data {
	struct mm_vector vec;
	FILE *file;
} mm_sbf_data_t;

typedef union mm_sbf_pos {
	long fpos;
	unsigned char *vpos; // has to be relative ( vec could relocate )
} mm_sbf_pos_t;

typedef struct mm_sbf_value {
	enum mm_sbf_type type;
	union mm_sbf_pos pos;
	size_t size;
} mm_sbf_value_t;

typedef struct mm_sbf {
	enum mm_sbf_serial_type stype;
	union mm_sbf_data data;
} mm_sbf_t;

MM_API bool mm_sbf_construct( struct mm_sbf *this, size_t size ); // use dynamic vec
MM_API void mm_sbf_construct_with_buf( struct mm_sbf *this, void *buf, size_t size ); // use static vec
MM_API void mm_sbf_construct_with_file( struct mm_sbf *this, FILE *f ); // use file
MM_API void mm_sbf_destroy( struct mm_sbf *this );

MM_API bool mm_sbf_open_object( struct mm_sbf *this, const char *name, union mm_sbf_pos *pos );
MM_API bool mm_sbf_close_object( struct mm_sbf *this, union mm_sbf_pos pos );
MM_API bool mm_sbf_open_array( struct mm_sbf *this, const char *name, union mm_sbf_pos *pos );
MM_API bool mm_sbf_close_array( struct mm_sbf *this, union mm_sbf_pos pos );
MM_API bool mm_sbf_set_root( struct mm_sbf *this, enum mm_sbf_type type, struct mm_sbf_value *value );

MM_API bool mm_sbf_value_is_type( struct mm_sbf_value *this );
MM_API int8_t mm_sbf_value_get_i8( struct mm_sbf_value *this );
MM_API int16_t mm_sbf_value_get_i16( struct mm_sbf_value *this );
MM_API int32_t mm_sbf_value_get_i32( struct mm_sbf_value *this );
MM_API int64_t mm_sbf_value_get_i64( struct mm_sbf_value *this );
MM_API uint8_t mm_sbf_value_get_u8( struct mm_sbf_value *this );
MM_API uint16_t mm_sbf_value_get_u16( struct mm_sbf_value *this );
MM_API uint32_t mm_sbf_value_get_u32( struct mm_sbf_value *this );
MM_API uint64_t mm_sbf_value_get_u64( struct mm_sbf_value *this );
MM_API float mm_sbf_value_get_f32( struct mm_sbf_value *this );
MM_API double mm_sbf_value_get_f64( struct mm_sbf_value *this );
MM_API void mm_sbf_value_get_string( struct mm_sbf_value *this, char *buf, size_t size );

/*
	If I use fseek/foffset to zip around FILE streams, need to
	keep in mind the limitation of a long's garunteed numerical range ( 2GB ).
	I could use fsetpos/fgetpos to overcome this. However I'd lose the ability
	to easily calculate sizes of objects & arrays.
*/

#define MM_SBF_ARRAY_FOR_EACH( arr, value )
#define MM_SBF_OBJECT_FOR_EACH( obj, key, value )

#endif
