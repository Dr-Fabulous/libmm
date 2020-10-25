cmake_minimum_required( VERSION 3.10.2 )
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )
include( mm_functions )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release )
endif()

message( STATUS "build type set to '${CMAKE_BUILD_TYPE}'" )

mm_git_version()

project( mm VERSION ${GIT_VERSION}
	    LANGUAGES C
	    DESCRIPTION "Jumble of useful C utilities" )

add_subdirectory( mm )
add_subdirectory( docs EXCLUDE_FROM_ALL )

include( CTest )
enable_testing()

if( BUILD_TESTING )
	add_subdirectory( unit EXCLUDE_FROM_ALL )
endif()
