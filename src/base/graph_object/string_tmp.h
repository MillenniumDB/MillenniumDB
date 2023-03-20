#pragma once

/*
Used to represent a string that cannot be inlined and it is not present in the graph
*/
class StringTmp {
public:
    // using pointer instead of reference so GraphObject is trivially assignable
    const std::string* str;

    StringTmp(const std::string& str) : str(&str) { }
};
