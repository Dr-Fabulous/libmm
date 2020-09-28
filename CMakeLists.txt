cmake_minimum_required( VERSION 3.10.2 )
project( libmm VERSION 0.1.0 LANGUAGES C DESCRIPTION "jumble of C utilities" )

if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
	set( CMAKE_BUILD_TYPE Release )
endif()

if( NOT CMAKE_BUILD_TYPE MATCHES Release AND NOT CMAKE_BUILD_TYPE MATCHES Debug )
	message( FATAL_ERROR "Unknown build type '${CMAKE_BUILD_TYPE}'" )
else()
	message( STATUS "Set to '${CMAKE_BUILD_TYPE}' mode" )
endif()

add_subdirectory( mm )
add_subdirectory( unit EXCLUDE_FROM_ALL )
