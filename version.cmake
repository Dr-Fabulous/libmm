# version number
# < tag major >.< tag minor >.< num dev commits since tag >.< num feature commits >

find_package( Git REQUIRED )

execute_process( COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
		 OUTPUT_VARIABLE GIT_TAG
		 OUTPUT_STRIP_TRAILING_WHITESPACE )

string( REGEX MATCHALL "[0-9]+" GIT_TAG_VERSION "${GIT_TAG}" )

list( GET GIT_TAG_VERSION 0 PROJECT_VERSION_MAJOR )
list( GET GIT_TAG_VERSION 1 PROJECT_VERSION_MINOR )

execute_process( COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
		 OUTPUT_VARIABLE GIT_BRANCH
		 OUTPUT_STRIP_TRAILING_WHITESPACE )

if( "${GIT_BRANCH}" STREQUAL "main" )
	set( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}" )
	return()
endif()

# number of commits since last tag
execute_process( COMMAND ${GIT_EXECUTABLE} rev-list --count ${GIT_TAG}..dev
		 OUTPUT_VARIABLE PROJECT_VERSION_PATCH
		 OUTPUT_STRIP_TRAILING_WHITESPACE )

if( "${GIT_BRANCH}" STREQUAL "dev" )
	set( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )
	return()
endif()

# number of commits from branching off of dev
execute_process( COMMAND ${GIT_EXECUTABLE} rev-list --count dev..HEAD
		 OUTPUT_VARIABLE PROJECT_VERSION_TWEAK
		 OUTPUT_STRIP_TRAILING_WHITESPACE )

set( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}" )
