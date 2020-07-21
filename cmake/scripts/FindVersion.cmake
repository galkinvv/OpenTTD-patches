cmake_minimum_required(VERSION 3.5)

#
# Finds the current version of the current folder.
#

find_package(Git QUIET)
# ${CMAKE_SOURCE_DIR}/.git may be a directory or a regular file
if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    # Make sure LC_ALL is set to something desirable
    set(SAVED_LC_ALL "$ENV{LC_ALL}")
    set(ENV{LC_ALL} C)

    # Assume the dir is not modified
    set(REV_MODIFIED 0)

    # Refresh the index to make sure file stat info is in sync, then look for modifications
    execute_process(COMMAND ${GIT_EXECUTABLE} update-index --refresh
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_QUIET
    )

    # See if git tree is modified
    execute_process(COMMAND ${GIT_EXECUTABLE} diff-index HEAD
                    OUTPUT_VARIABLE IS_MODIFIED
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    if (NOT IS_MODIFIED STREQUAL "")
        set(REV_MODIFIED 2)
    endif()

    # Get last commit hash
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --verify HEAD
                    OUTPUT_VARIABLE FULLHASH
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    ERROR_QUIET
    )
    set(REV_HASH "${FULLHASH}")

    string(SUBSTRING "${FULLHASH}" 0 10 SHORTHASH)

    # Get the last commit date
    execute_process(COMMAND ${GIT_EXECUTABLE} show -s --pretty=format:%ci HEAD
                    OUTPUT_VARIABLE COMMITDATE
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    string(REGEX REPLACE "([0-9]+)-([0-9]+)-([0-9]+).*" "\\1\\2\\3" COMMITDATE "${COMMITDATE}")
    set(REV_ISODATE "${COMMITDATE}")
    string(SUBSTRING "${REV_ISODATE}" 0 4 REV_YEAR)

    # Get the branch
    execute_process(COMMAND ${GIT_EXECUTABLE} symbolic-ref -q HEAD
                    OUTPUT_VARIABLE BRANCH
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    ERROR_QUIET
    )
    string(REGEX REPLACE ".*/" "" BRANCH "${BRANCH}")

    # Get the tag
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags
                    OUTPUT_VARIABLE TAG
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    ERROR_QUIET
    )
    string(REGEX REPLACE "\^0$" "" TAG "${TAG}")

    if (REV_MODIFIED EQUAL 0)
        set(HASHPREFIX "-g")
    elseif (REV_MODIFIED EQUAL 2)
        set(HASHPREFIX "-m")
    else ()
        set(HASHPREFIX "-u")
    endif()

    # Set the version string
    if (NOT TAG STREQUAL "")
        set(REV_VERSION "${TAG}")
        set(REV_ISTAG 1)

        string(REGEX REPLACE "^[0-9.]+$" "" STABLETAG "${TAG}")
        if (NOT STABLETAG STREQUAL "")
            set(REV_ISSTABLETAG 1)
        else ()
            set(REV_ISSTABLETAG 0)
        endif ()
    else ()
        set(REV_VERSION "${REV_ISODATE}-${BRANCH}${HASHPREFIX}${SHORTHASH}")
        set(REV_ISTAG 0)
        set(REV_ISSTABLETAG 0)
    endif ()

    # Restore LC_ALL
    set(ENV{LC_ALL} "${SAVED_LC_ALL}")
elseif (EXISTS "${CMAKE_SOURCE_DIR}/.ottdrev-vc")
    file(READ "${CMAKE_SOURCE_DIR}/.ottdrev-vc" OTTDREVVC)
    string(REPLACE "\n" ";" OTTDREVVC "${OTTDREVVC}")
     list(GET OTTDREVVC 0 OTTDREV)
     list(GET OTTDREVVC 1 SRCHASH)
    string(REPLACE "\t" ";" OTTDREV "${OTTDREV}")
    list(GET OTTDREV 0 REV_VERSION)
    list(GET OTTDREV 1 REV_ISODATE)
    list(GET OTTDREV 2 REV_MODIFIED)
    list(GET OTTDREV 3 REV_HASH)
    list(GET OTTDREV 4 REV_ISTAG)
    list(GET OTTDREV 5 REV_ISSTABLETAG)
    list(GET OTTDREV 6 REV_YEAR)
    if (REV_MODIFIED EQUAL 2)
        string(REGEX REPLACE "M$" "" REV_VERSION "${REV_VERSION}")
    endif ()
    execute_process(COMMAND ./version_utils.sh -o
                    RESULT_VARIABLE CAN_CHECK_MODIFIED
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    if (CAN_CHECK_MODIFIED EQUAL 0)
        execute_process(COMMAND ./version_utils.sh -s
                        OUTPUT_VARIABLE CURRENT_HASH
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        if (NOT CURRENT_HASH STREQUAL SRCHASH)
            set(REV_MODIFIED 2)
            string(SUBSTRING "${CURRENT_HASH}" 0 8 SHORT_CURRENT_HASH)
            set(REV_VERSION "${REV_VERSION}-H${SHORT_CURRENT_HASH}")
            set(REV_MODIFIED 2)
        endif ()
    else ()
        set(REV_MODIFIED 1)
    endif ()
elseif (EXISTS "${CMAKE_SOURCE_DIR}/.ottdrev")
    file(READ "${CMAKE_SOURCE_DIR}/.ottdrev" OTTDREV)
    string(REPLACE "\n" "" OTTDREV "${OTTDREV}")
    string(REPLACE "\t" ";" OTTDREV "${OTTDREV}")
    list(GET OTTDREV 0 REV_VERSION)
    list(GET OTTDREV 1 REV_ISODATE)
    list(GET OTTDREV 2 REV_MODIFIED)
    list(GET OTTDREV 3 REV_HASH)
    list(GET OTTDREV 4 REV_ISTAG)
    list(GET OTTDREV 5 REV_ISSTABLETAG)
    list(GET OTTDREV 6 REV_YEAR)
else ()
    message(WARNING "No version detected; this build will NOT be network compatible")
    set(REV_VERSION "norev0000")
    set(REV_ISODATE "19700101")
    set(REV_MODIFIED 1)
    set(REV_HASH "unknown")
    set(REV_ISTAG 0)
    set(REV_ISSTABLETAG 0)
    set(REV_YEAR "1970")
endif ()

message(STATUS "Version string: ${REV_VERSION}")

if (GENERATE_OTTDREV)
    message(STATUS "Generating ${GENERATE_OTTDREV}")
    file(WRITE ${CMAKE_SOURCE_DIR}/${GENERATE_OTTDREV} "${REV_VERSION}\t${REV_ISODATE}\t${REV_MODIFIED}\t${REV_HASH}\t${REV_ISTAG}\t${REV_ISSTABLETAG}\t${REV_YEAR}\n")
else (GENERATE_OTTDREV)
    message(STATUS "Generating rev.cpp")
    configure_file("${CMAKE_SOURCE_DIR}/src/rev.cpp.in"
            "${FIND_VERSION_BINARY_DIR}/rev.cpp")

    if (WIN32)
        message(STATUS "Generating ottdres.rc")
        configure_file("${CMAKE_SOURCE_DIR}/src/os/windows/ottdres.rc.in"
                "${FIND_VERSION_BINARY_DIR}/ottdres.rc")
    endif (WIN32)

    message(STATUS "Generating CPackProperties.cmake")
    configure_file("${CMAKE_SOURCE_DIR}/CPackProperties.cmake.in"
            "${CPACK_BINARY_DIR}/CPackProperties.cmake" @ONLY)
endif (GENERATE_OTTDREV)
