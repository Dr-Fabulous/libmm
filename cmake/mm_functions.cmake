include( CMakeParseArguments )

function( mm_execute_process_required )
	cmake_parse_arguments( PARSE_ARGV 0 "ARG" "" "WORKING_DIRECTORY;OUTPUT" "COMMAND" )

	if( NOT ARG_WORKING_DIRECTORY} )
		set( ARG_WORKING_DIRECTORY "${CMAKE_SOURCE_DIRECTORY}" )
	endif()

	execute_process( COMMAND ${ARG_COMMAND}
			 WORKING_DIRECTORY "${ARG_WORKING_DIRECTORY}"
			 RESULT_VARIABLE RESULT
			 OUTPUT_VARIABLE OUTPUT
			 OUTPUT_STRIP_TRAILING_WHITESPACE )

	if( NOT "${RESULT}" STREQUAL "0" )
		message( FATAL_ERROR "command '${ARG_COMMAND}' failed with '${RESULT}'" )
	endif()

	if( ARG_OUTPUT )
		set( ${ARG_OUTPUT} "${OUTPUT}" PARENT_SCOPE )
	endif()
endfunction()

function( mm_git_version )
	macro( mm_set_git_version PREFIX MINOR MAJOR PATCH TWEAK )
		set( ${PREFIX}_MINOR ${MINOR} PARENT_SCOPE )
		set( ${PREFIX}_MAJOR ${MAJOR} PARENT_SCOPE )
		set( ${PREFIX}_PATCH ${PATCH} PARENT_SCOPE )
		set( ${PREFIX}_TWEAK ${TWEAK} PARENT_SCOPE )

		# safe to use with cmake
		set( VERSION ${MINOR} ${MAJOR} ${PATCH} ${TWEAK} )
		string( REPLACE ";" "." VERSION "${VERSION}" )
		set( ${PREFIX}_FULL_VERSION ${VERSION} PARENT_SCOPE )

		# patch should be short hash from latest feature commit
		set( VERSION ${MINOR} ${MAJOR} ${PATCH} )
		string( REPLACE ";" "." VERSION "${VERSION}" )
		set( ${PREFIX}_VERSION ${VERSION} PARENT_SCOPE )
	endmacro()

	cmake_parse_arguments( PARSE_ARGV 0 "ARG" 
			       ""
			       "PREFIX"
			       "" )

	if( NOT ARG_PREFIX )
		set( ARG_PREFIX GIT )
	endif()

	find_package( Git REQUIRED )

	mm_execute_process_required( COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
				     OUTPUT GIT_BRANCH )

	mm_execute_process_required( COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
				     OUTPUT GIT_TAG )


	string( REGEX MATCHALL "[0-9]+" GIT_TAG_VERSION "${GIT_TAG}" )
	list( GET GIT_TAG_VERSION 0 GIT_MAJOR )
	list( GET GIT_TAG_VERSION 1 GIT_MINOR )

	if( "${GIT_BRANCH}" STREQUAL "main" )
		mm_set_git_version( ${ARG_PREFIX} ${GIT_MAJOR} ${GIT_MINOR} "" "" )
		return()
	endif()

	mm_execute_process_required( COMMAND ${GIT_EXECUTABLE} rev-list --count ${GIT_TAG}..dev
				     OUTPUT GIT_PATCH )

	if( "${GIT_BRANCH}" STREQUAL "dev" )
		mm_set_git_version( ${ARG_PREFIX} ${GIT_MAJOR} ${GIT_MINOR} ${GIT_PATCH} "" )
		return()
	endif()

	mm_execute_process_required( COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
				     OUTPUT GIT_TWEAK )

	mm_set_git_version( ${ARG_PREFIX} ${GIT_MAJOR} ${GIT_MINOR} ${GIT_PATCH} ${GIT_TWEAK} )
endfunction()

macro( mm_cond_set VAR DEFAULT )
	if ( "${${VAR}}" STREQUAL "" )
		set( ${VAR} ${DEFAULT} )
	endif()
endmacro()
