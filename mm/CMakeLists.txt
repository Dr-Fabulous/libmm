include( GNUInstallDirs )

set( MM_INSTALL_CONF_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/mm-${GIT_FULL_VERSION}" )
set( MM_INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR}/mm-${GIT_FULL_VERSION}" )
set( MM_INSTALL_INC_DIR "${CMAKE_INSTALL_INCLUDEDIR}/mm-${GIT_FULL_VERSION}" )

file( GLOB MM_SRC "src/*.c" )
file( GLOB_RECURSE MM_HDR "include/*.h" )

add_library( mm SHARED ${MM_SRC} )
add_library( mm::mm ALIAS mm )

set_target_properties( mm PROPERTIES
		       VERSION "${GIT_VERSION}"
		       SOVERSION "${GIT_VERSION}"
		       PUBLIC_HEADER "${MM_HDR}" )

target_include_directories( mm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				      $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
				      $<INSTALL_INTERFACE:${MM_INSTALL_INC_DIR}> )

option( MM_OPT_USE_C11 "Use C11 standard. Otherwise uses C99" ON )
option( MM_OPT_USE_BUILTIN "Use compiler provided builtins" ON )
option( MM_OPT_USE_NATIVE "Use native instructions where possible ( only applies to GCC )" ON )

if( MM_OPT_USE_C11 )
	set( MM_C_STD 11 )
	target_compile_features( mm PUBLIC c_std_11 )
else()
	set( MM_C_STD 99 )
	target_compile_features( mm PUBLIC c_std_99 )
endif()

set( MM_VERSION "\"${GIT_FULL_VERSION}\"" )
set( MM_MAJOR ${GIT_MAJOR} )
set( MM_MINOR ${GIT_MINOR} )
set( MM_PATCH ${GIT_PATCH} )
set( MM_TWEAK "\"${GIT_TWEAK}\"" )

if( MM_OPT_USE_BUILTIN )
	mm_cond_set( MM_USE_BUILTINS 1 )
endif()

if( MM_OPT_USE_NATIVE )
	mm_cond_set( MM_USE_NATIVE 1 )
endif()

mm_cond_set( MM_ALLOC_INC stdlib.h )
mm_cond_set( MM_MALLOC malloc )
mm_cond_set( MM_REALLOC realloc )
mm_cond_set( MM_CALLOC calloc )
mm_cond_set( MM_FREE free )
mm_cond_set( MM_STDIN stdin )
mm_cond_set( MM_STDERR stderr )
mm_cond_set( MM_STDOUT stdout )
mm_cond_set( MM_PRINTF printf )
mm_cond_set( MM_VPRINTF vprintf )
mm_cond_set( MM_VFPRINTF vfprintf )
mm_cond_set( MM_FPRINTF fprintf )
mm_cond_set( MM_SPRINTF sprintf )
mm_cond_set( MM_FFLUSH fflush )

configure_file( "config.h.in"
		"${CMAKE_CURRENT_BINARY_DIR}/config.h" )

configure_file( "alloc.h.in"
		"${CMAKE_CURRENT_BINARY_DIR}/alloc.h" )

target_compile_features( mm PUBLIC c_function_prototypes
				   c_variadic_macros )

if( MSVC )
	target_compile_options( mm PRIVATE /W4 /WX )
else()
	target_compile_options( mm PRIVATE -pedantic -Wextra -Werror )
endif()

install( TARGETS mm EXPORT mmTargets
	 LIBRARY DESTINATION ${MM_INSTALL_LIB_DIR}
	 ARCHIVE DESTINATION ${MM_INSTALL_LIB_DIR}
	 INCLUDES DESTINATION  ${MM_INSTALL_INC_DIR}
	 PUBLIC_HEADER DESTINATION ${MM_INSTALL_INC_DIR}/mm )

install( FILES "${CMAKE_CURRENT_BINARY_DIR}/config.h" DESTINATION "${MM_INSTALL_INC_DIR}/mm" )

include( CMakePackageConfigHelpers )

write_basic_package_version_file( "mmConfigVersion.cmake"
				  VERSION ${PROJECT_VERSION}
				  COMPATIBILITY SameMajorVersion )

configure_package_config_file( "mmConfig.cmake.in"
			       "${CMAKE_CURRENT_BINARY_DIR}/mmConfig.cmake"
			       INSTALL_DESTINATION "${MM_INSTALL_CONF_DIR}" )

install( EXPORT mmTargets
	 NAMESPACE mm::
	 FILE "mmTargets.cmake"
	 DESTINATION "${MM_INSTALL_CONF_DIR}" )

install( FILES "${CMAKE_CURRENT_BINARY_DIR}/mmConfigVersion.cmake"
	       "${CMAKE_CURRENT_BINARY_DIR}/mmConfig.cmake"
	 DESTINATION "${MM_INSTALL_CONF_DIR}" )
