
if( NOT ${MM_UNIT_TESTS} )
	return()
endif()

file( GLOB MM_UNIT_SRC "src/*.c" )
add_executable( mm-unit "${MM_UNIT_SRC}" )
target_link_libraries( mm-unit PUBLIC mm )
add_custom_target( check ALL COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mm-unit DEPENDS mm-unit )
