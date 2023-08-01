cmake_minimum_required(VERSION 3.12)
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

# set the project name and version
project(MillenniumDB VERSION 0.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -O0 -g3  \
-pthread -march=native -funroll-loops \
-fsanitize=undefined -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -pedantic -O3 -g0 \
-pthread -march=native -funroll-loops")

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
if (BUILD_TYPE STREQUAL "RELEASE")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif (BUILD_TYPE STREQUAL "RELEASE")

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/antlr4-runtime-4.9.3)
include_directories(${CMAKE_SOURCE_DIR}/third_party/antlr4-runtime-4.9.3/runtime/src)
include_directories(${CMAKE_SOURCE_DIR}/third_party/boost_1_81/include)
link_directories(${CMAKE_SOURCE_DIR}/third_party/boost_1_81/lib)

file(GLOB_RECURSE SRCS src/*.cc)
file(GLOB_RECURSE BIN_SRCS src/bin/*.cc)
file(GLOB_RECURSE TEST_SRCS src/tests/*.cc)

set(sources ${SRCS})
list(REMOVE_ITEM sources ${BIN_SRCS})
list(REMOVE_ITEM sources ${TEST_SRCS})

add_library(millenniumdb STATIC ${sources})

set(BUILD_TARGETS
    # check_strings_sparql
    # try_bpt_compression
    create_db_mql
    create_db_sparql
    query_mql
    server_mql
    server_sparql
)

set(TEST_TARGETS
    compare_datetime
    compare_decimal_both_ext
    compare_decimal_both_inl
    compare_decimal_inl_ext
    decimal_operations
    normalize_decimal
    # trie
    variable_set
)

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
foreach(target ${BUILD_TARGETS})
    add_executable(${target} ${CMAKE_SOURCE_DIR}/src/bin/${target}.cc)
    target_link_libraries(${target}
        millenniumdb
        antlr4_cpp_runtime
        stdc++fs
        ssl
        crypto
    )
endforeach(target)

enable_testing()
foreach(target ${TEST_TARGETS})
    add_executable(${target} ${CMAKE_SOURCE_DIR}/src/tests/${target}.cc)
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    target_link_libraries(${target}
        millenniumdb
        antlr4_cpp_runtime
        stdc++fs
        ssl
        crypto
    )
    add_test(
        NAME ${target}
        COMMAND $<TARGET_FILE:${target}>
        WORKING_DIRECTORY tests
    )
endforeach(target)
