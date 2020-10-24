set( MM_INSTALL_LIB_DIR "lib/mm-${GIT_FULL_VERSION}/${CMAKE_BUILD_TYPE}" )
set( MM_INSTALL_INC_DIR "include/mm-${GIT_FULL_VERSION}" )
file( GLOB MM_SRC "src/*.c" )

add_library( mm SHARED ${MM_SRC} )
add_library( mm::mm ALIAS mm )

target_include_directories( mm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				      $<INSTALL_INTERFACE:${MM_INSTALL_INC_DIR}> )

option( MM_USE_C11 "enable C11 features. Otherwise C99 is used." ON )
option( MM_USE_BUILTINS "use compiler builtins." ON )
option( MM_USE_NATIVE "use native instructions over builtins when possible ( only applies to GCC )." ON )

macro( mm_define NAME )
	if( NOT "${ARGV1}" STREQUAL "" )
		list( APPEND MM_DEFINES "${NAME}=${ARGV1}" )
	else()
		list( APPEND MM_DEFINES "${NAME}" )
	endif()
endmacro()

mm_define( MM_MAJOR ${GIT_MAJOR} )
mm_define( MM_MINOR ${GIT_MINOR} )
mm_define( MM_PATCH ${GIT_PATCH} )
mm_define( MM_TWEAK "\"${GIT_TWEAK}\"" )
mm_define( MM_VERSION "\"${GIT_FULL_VERSION}\"" )

if( MM_USE_C11 )
	mm_define( MM_USE_C11 )
	mm_define( MM_STD 11 )
	target_compile_features( mm PUBLIC c_std_11 )
else()
	mm_define( MM_USE_C99 )
	mm_define( MM_STD 99 )
	target_compile_features( mm PUBLIC c_std_99 )
endif()

target_compile_features( mm PUBLIC c_function_prototypes
			          c_variadic_macros )

if( MM_USE_BUILTINS )
	mm_define( MM_USE_BUILTINS )
endif()

if( MM_USE_NATIVE )
	mm_define( MM_USE_NATIVE )
endif()

target_compile_definitions( mm PUBLIC ${MM_DEFINES} )

if( MSVC )
	target_compile_options( mm PRIVATE /W4 /WX )
else()
	target_compile_options( mm PRIVATE -pedantic -Wextra -Werror )
endif()

install( TARGETS mm EXPORT mm
	 LIBRARY DESTINATION ${MM_INSTALL_LIB_DIR}
	 ARCHIVE DESTINATION ${MM_INSTALL_LIB_DIR}
	 INCLUDES DESTINATION  ${MM_INSTALL_INC_DIR} )
