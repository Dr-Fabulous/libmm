#include "mm/sbf.h"

bool mm_sbf_construct( struct mm_sbf *this, size_t size ) {
	this->stype = MM_SBF_USE_VEC;
	return mm_vector_construct( &this->data.vec, 1, size );
}

void mm_sbf_construct_with_buf( struct mm_sbf *this, void *buf, size_t size ) {
	this->stype = MM_SBF_USE_BUF;
	mm_vector_construct_with_buf( &this->data.vec, buf, size );
}

void mm_sbf_construct_with_file( struct mm_sbf *this, FILE *file ) {
	this->stype = MM_SBF_USE_FILE;
	this->data.file = file;
}

void mm_sbf_destroy( struct mm_sbf *this ) {
	switch( this->stype ) {
		case MM_SBF_USE_VEC:
		case mm_SBF_USE_BUF:
			mm_vector_destroy( &this->data.vec );
		break;
	}
}
