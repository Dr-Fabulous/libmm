file( GLOB mm_src "src/*.c" )
file( GLOB_RECURSE mm_hdr "include/*.h" )

add_library( mm SHARED "${mm_src}" )
target_include_directories( mm PUBLIC include )

set_target_properties( mm PROPERTIES PUBLIC_HEADER "${mm_hdr}"
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
	
	include( GNUInstallDirs )
	install( TARGETS mm LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mm" )
endif()
