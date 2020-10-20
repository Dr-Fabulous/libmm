if( NOT ${LIBMM_BUILD_DOCS} )
	return()
endif()

set( DOXYGEN_CONFIG_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" )
set( DOXYGEN_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )

find_program( DOXYGEN_BIN doxygen REQUIRED )

configure_file( "${DOXYGEN_CONFIG_IN}" "${DOXYGEN_CONFIG}" @ONLY )
add_custom_target( docs ALL
		   COMMAND "${DOXYGEN_BIN}" "${DOXYGEN_CONFIG}"
		   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" )
