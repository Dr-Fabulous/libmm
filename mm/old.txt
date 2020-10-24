set( LIBMM_INSTALL_LIB_DEST "${CMAKE_INSTALL_PREFIX}/lib/mm-${LIBMM_VERSION}" )
set( LIBMM_INSTALL_INC_DEST "${CMAKE_INSTALL_PREFIX}/include/mm-${LIBMM_VERSION}" )

file( GLOB LIBMM_SRC "src/*.c" )
file( GLOB_RECURSE LIBMM_HDR "include/*.h" )

if( ${LIBMM_DYNAMIC} )
	add_library( mm SHARED "${LIBMM_SRC}" )
else()
	add_library( mm STATIC "${LIBMM_SRC}" )
endif()

# generator expressions determine set include path when building vs installing
target_include_directories( mm PUBLIC 
			    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			    $<INSTALL_INTERFACE:${LIBMM_INSTALL_INC_DEST}> )

set_target_properties( mm PROPERTIES PUBLIC_HEADER "${LIBMM_HDR}"
				     C_STANDARD 11
				     C_STANDARD_REQUIRED ON )

add_definitions( -DMM_BUILDING )

if( CMAKE_BUILD_TYPE MATCHES Release )
	add_definitions( -DNDEBUG )
endif()

if( MSVC )
	target_compile_options( mm PRIVATE /W4 /WX )
else()
	target_compile_options( mm PRIVATE -Wextra -pedantic -Werror )
endif()

install( TARGETS mm EXPORT mm
	 LIBRARY DESTINATION "${LIBMM_INSTALL_LIB_DEST}"
	 PUBLIC_HEADER DESTINATION "${LIBMM_INSTALL_INC_DEST}" )

install( EXPORT mm DESTINATION "${LIBMM_INSTALL_LIB_DEST}" )
