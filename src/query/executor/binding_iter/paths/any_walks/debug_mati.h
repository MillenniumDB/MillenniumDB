// debug_mati.h
#ifndef DEBUG_MATI_H
#define DEBUG_MATI_H

#include <iostream>

#if defined(__GNUC__) || defined(__clang__)
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define FUNCTION_NAME __FUNCSIG__
#else
    #define FUNCTION_NAME __func__
#endif

// Extracts only the function name (without class/namespace)
static inline std::string getSimpleFunctionName(const std::string& fullName) {
    size_t start = fullName.rfind(" ")+1;   // Find last space (before function name)
    size_t end = fullName.find("(");        // Find first '(' (start of parameters)
    size_t colons = fullName.rfind("::", end); // Find last "::" (if inside a class)

    if (colons != std::string::npos) {
        start = colons + 2;  // Move past "::"
    }

    return fullName.substr(start, end - start);
}

#define SIMPLE_FUNCTION_NAME getSimpleFunctionName(FUNCTION_NAME)

// Extracts only the function name from __PRETTY_FUNCTION__ or __FUNCSIG__
// #define SIMPLE_FUNCTION_NAME (std::string(FUNCTION_NAME).substr(std::string(FUNCTION_NAME).find(" ")+1, std::string(FUNCTION_NAME).find("(")-std::string(FUNCTION_NAME).find(" ")-1))

// Use __PRETTY_FUNCTION__ for GCC/Clang or __FUNCSIG__ for MSVC
#define _debug_mati() std::cout << "MATI: " << SIMPLE_FUNCTION_NAME << ": "

#define _debug_mati_simple() std::cout
#endif // DEBUG_MATI_H