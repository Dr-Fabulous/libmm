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

	mm_execute_process_required( COMMAND ${GIT_EXECUTABLE} describe --tags --long
				     OUTPUT GIT_VERSION )

	string( REGEX REPLACE "^([0-9]+).([0-9]+).([0-9]+)-([0-9]+)-([a-zA-Z0-9]+)$" "\\1;\\2;\\3;\\4;\\5" TOKENS "${GIT_VERSION}" )
	list( LENGTH TOKENS TOKENS_LENGTH )

	if( NOT TOKENS_LENGTH )
		message( FATAL_ERROR "Invalid git version ( check your tags )" )
	endif()

	list( GET TOKENS 0 MAJOR )
	list( GET TOKENS 1 MINOR )
	list( GET TOKENS 2 PATCH )
	list( GET TOKENS 3 COMMITS )
	list( GET TOKENS 4 HASH )

	if( "${GIT_BRANCH}" STREQUAL "dev" OR "${GIT_BRANCH}" STREQUAL "main" )
		list( REMOVE_AT TOKENS 4 )
	endif()

	if( "${GIT_BRANCH}" STREQUAL "main" )
		list( REMOVE_AT TOKENS 3 )
	endif()

	string( REPLACE ";" "." VERSION "${TOKENS}" )

	set( ${ARG_PREFIX}_MAJOR ${MAJOR} PARENT_SCOPE )
	set( ${ARG_PREFIX}_MINOR ${MINOR} PARENT_SCOPE )
	set( ${ARG_PREFIX}_PATCH ${PATCH} PARENT_SCOPE )
	set( ${ARG_PREFIX}_COMMITS ${COMMITS} PARENT_SCOPE )
	set( ${ARG_PREFIX}_HASH ${HASH} PARENT_SCOPE )
	set( ${ARG_PREFIX}_VERSION ${MAJOR}.${MINOR}.${PATCH} PARENT_SCOPE )
	set( ${ARG_PREFIX}_FULL_VERSION ${VERSION} PARENT_SCOPE )
endfunction()

macro( mm_cond_set VAR DEFAULT )
	if ( "${${VAR}}" STREQUAL "" )
		set( ${VAR} ${DEFAULT} )
	endif()
endmacro()
