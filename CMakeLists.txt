cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "g++")

set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)

# set the project name and version
project(MillenniumDB VERSION 0.1)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -O0 -g3 -pthread \
-fPIC -msse4.2 -mpclmul -march=native -funroll-loops -Wstrict-overflow -Wstrict-aliasing -pedantic \
-fsanitize=undefined -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3 -g0 -pthread \
-fPIC -msse4.2 -mpclmul -march=native -funroll-loops -Wstrict-overflow -Wstrict-aliasing -pedantic")

find_package(Boost 1.71.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SRCS src/*.cc)

set(sources ${SRCS})
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*)
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/*)
add_library(millenniumdb STATIC ${sources})

set(BUILD_TARGETS
    create_db
    query
    server
)

set(TEST_TARGETS
    create_bpt
    check_bpts
    check_extendible_hash
)

foreach(target ${BUILD_TARGETS})
    add_executable(${target} ${CMAKE_SOURCE_DIR}/src/main/${target}.cc)
    target_link_libraries(${target} millenniumdb stdc++fs boost_program_options boost_system)
endforeach(target)

foreach(target ${TEST_TARGETS})
    add_executable(${target} ${CMAKE_SOURCE_DIR}/src/tests/${target}.cc)
    target_link_libraries(${target} millenniumdb stdc++fs boost_program_options boost_system)
    # add_test(NAME ${target} COMMAND ${target})
endforeach(target)
