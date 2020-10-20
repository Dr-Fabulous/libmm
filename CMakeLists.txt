cmake_minimum_required( VERSION 3.10.2 FATAL_ERROR )

project( LIBMM VERSION 0.1
	       LANGUAGES C
	       DESCRIPTION "jumble of C utilities" )

option( LIBMM_UNIT_TESTS "build and run mm unit tests" ON )
option( LIBMM_DYNAMIC "build as a dynamic library" ON )
option( LIBMM_BUILD_DOCS "use doxygen to generate documentation" ON )

if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
	set( CMAKE_BUILD_TYPE Release )
endif()

if( NOT CMAKE_BUILD_TYPE MATCHES Release AND NOT CMAKE_BUILD_TYPE MATCHES Debug )
	message( FATAL_ERROR "Unknown build type '${CMAKE_BUILD_TYPE}'" )
else()
	message( STATUS "Set to '${CMAKE_BUILD_TYPE}' mode" )
endif()

add_subdirectory( mm )
add_subdirectory( unit )
add_subdirectory( docs )
