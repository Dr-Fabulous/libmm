file( GLOB MM_UNIT_SRC "src/*.c" )

set( TEST_BINARIES "" )

foreach( PATH ${MM_UNIT_SRC} )
	string( REGEX MATCH "[a-zA-Z0-9-_]+\.c$" FILE "${PATH}" )
	string( REGEX MATCH "[a-zA-Z0-9-_]+" NAME "${FILE}" )
	set( TEST "test-${NAME}" )
	add_executable( ${NAME} ${PATH} )
	target_link_libraries( ${NAME} PUBLIC mm::mm )
	add_test( NAME ${TEST} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${NAME} )
	list( APPEND TEST_BINARIES ${NAME} )
endforeach()

add_custom_target( unit DEPENDS ${TEST_BINARIES} )
