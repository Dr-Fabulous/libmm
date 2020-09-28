file( GLOB mm_unit_src "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" )
add_executable( mm-unit "${mm_unit_src}" )
target_link_libraries( mm-unit PUBLIC mm )
add_custom_target( check COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mm-unit DEPENDS mm-unit )
