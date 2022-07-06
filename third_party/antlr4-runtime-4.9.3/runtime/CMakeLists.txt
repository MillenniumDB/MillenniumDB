#------------------- LIBRARY CONFIGURATION ------------------------------------

set(ANTLR4_CPP_RUNTIME antlr4_cpp_runtime CACHE INTERNAL "")
file(GLOB_RECURSE ANTLR4_SRCS src/*.cpp)
set(ANTLR4_CPP_RUNTIME_SRC ${ANTLR4_SRCS})

add_library(${ANTLR4_CPP_RUNTIME} ${ANTLR4_CPP_RUNTIME_SRC})

target_include_directories(${ANTLR4_CPP_RUNTIME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

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