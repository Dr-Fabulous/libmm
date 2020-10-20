if( NOT ${LIBMM_BUILD_DOCS} )
	return()
endif()

find_package( Doxygen REQUIRED doxygen )

set( DOXYGEN_OUTPUT_LANGUAGE "English" )
set( DOXYGEN_RECURSIVE "YES" )
set( DOXYGEN_GENERATE_HTML "YES" )
set( DOXYGEN_GENERATE_LATEX "NO" )
set( DOXYGEN_EXTRACT_ALL "NO" )
set( DOXYGEN_EXTRACT_STATIC "YES" )

doxygen_add_docs( docs "../mm/include" )
