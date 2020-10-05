#include "mm/vector.h"
#include "mm/log.h"
#include "mm/unit.h"

MM_UNIT_CASE( empty_case, NULL, NULL ) {
	MM_VECTOR_DECLARE( v, int, NULL );
	MM_UNIT_ASSERT_EQ( mm_vector_null( &v ), true );

	return MM_UNIT_DONE;
}

MM_UNIT_CASE( push_case, NULL, NULL ) {
	MM_VECTOR_DECLARE( v, int, NULL );

	for ( int i = 0; i < 10; ++i ) {
		MM_UNIT_ASSERT_EQ( mm_vector_push( &v, &i ), true );
	}

	MM_UNIT_ASSERT_EQ( mm_vector_size( &v ), 10 );
	mm_vector_destroy( &v );

	return MM_UNIT_DONE;
}

MM_UNIT_CASE( stack_case, NULL, NULL ) {
	MM_VECTOR_DECLARE( v, int, NULL );

	for ( int i = 0; i < 10; ++i ) {
		MM_UNIT_ASSERT_EQ( mm_vector_push( &v, &i ), true );
	}

	for ( int i = 9, j = 0; i >= 0; --i ) {
		mm_vector_pop( &v, &j );
		MM_UNIT_ASSERT_EQ( i, j );
	}

	MM_UNIT_ASSERT_EQ( mm_vector_size( &v ), 0 );
	MM_UNIT_ASSERT_EQ( mm_vector_null( &v ), false );
	MM_UNIT_ASSERT_EQ( mm_vector_set_capacity( &v, 0 ), true );
	MM_UNIT_ASSERT_EQ( mm_vector_null( &v ), true );

	return MM_UNIT_DONE;
}

MM_UNIT_CASE( copy_case, NULL, NULL ) {
	MM_VECTOR_DECLARE( v1, int, NULL );
	MM_VECTOR_DECLARE( v2, int, NULL );

	for ( int i = 0; i < 100; ++i ) {
		MM_UNIT_ASSERT_EQ( mm_vector_push( &v1, &i ), true );
	}

	MM_UNIT_ASSERT_EQ( mm_vector_copy( &v2, &v1 ), true );

	for ( int i = 0; i < 100; ++i ) {
		MM_UNIT_ASSERT_EQ( *MM_VECTOR_AS( &v1, i, int ), *MM_VECTOR_AS( &v1, i, int ) );
	}

	mm_vector_destroy( &v1 );
	mm_vector_destroy( &v2 );

	return MM_UNIT_DONE;
}

MM_UNIT_CASE( move_case, NULL, NULL ) {
	MM_VECTOR_DECLARE( v1, int, NULL );
	MM_VECTOR_DECLARE( v2, int, NULL );

	for ( int i = 0; i < 100; ++i ) {
		MM_UNIT_ASSERT_EQ( mm_vector_push( &v1, &i ), true );
	}

	mm_vector_move( &v2, &v1 );

	MM_UNIT_ASSERT_EQ( mm_vector_null( &v1 ), true );
	MM_UNIT_ASSERT_EQ( mm_vector_null( &v2 ), false );

	mm_vector_destroy( &v2 );
}

MM_UNIT_CASE( insert_case, NULL, NULL ) {
	MM_VECTOR_DECLARE( v, int, NULL );

	int i = 0;

	for ( i = 0; i < 100; ++i ) {
		MM_UNIT_ASSERT_EQ( mm_vector_push( &v, &i ), true );
	}

	i = 100;
	MM_UNIT_ASSERT_EQ( mm_vector_insert( &v, mm_vector_at( &v, 10 ), &i ), true );

	mm_vector_destroy( &v );
}

MM_UNIT_SUITE( vector_suite ) {
	MM_UNIT_RUN( empty_case );
	MM_UNIT_RUN( push_case );
	MM_UNIT_RUN( stack_case );
	MM_UNIT_RUN( copy_case );
	MM_UNIT_RUN( move_case );
	MM_UNIT_RUN( insert_case );

	return MM_UNIT_DONE;
}
