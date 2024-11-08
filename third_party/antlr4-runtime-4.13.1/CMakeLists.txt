cmake_minimum_required(VERSION 3.12)

project(antlr4-cpp-runtime VERSION 4.13.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE RELEASE)
else()
    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
endif()

message(STATUS "Building ANTLR4-CPP-RUNTIME in ${CMAKE_BUILD_TYPE} mode")

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT supported OUTPUT error)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif (CMAKE_BUILD_TYPE STREQUAL "RELEASE")

set(ANTLR4_CPP_RUNTIME antlr4_cpp_runtime CACHE INTERNAL "")
file(GLOB_RECURSE ANTLR4_SRCS src/*.cpp)
set(ANTLR4_CPP_RUNTIME_SRC ${ANTLR4_SRCS})

add_library(${ANTLR4_CPP_RUNTIME} STATIC ${ANTLR4_CPP_RUNTIME_SRC})

target_include_directories(${ANTLR4_CPP_RUNTIME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(${ANTLR4_CPP_RUNTIME} PUBLIC ANTLR4CPP_STATIC)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    target_compile_options(${ANTLR4_CPP_RUNTIME} PRIVATE
        -g -Wall -O0
    )
else()
    target_compile_options(${ANTLR4_CPP_RUNTIME} PRIVATE
        -w -O3
    )
    target_compile_options(${ANTLR4_CPP_RUNTIME} PUBLIC
        -Wno-attributes
    )
endif()