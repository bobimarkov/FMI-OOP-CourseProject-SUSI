#ifndef STRINGHELPER_HPP
#define STRINGHELPER_HPP
#include <iostream>

namespace StringHelper {
    int count (std::string str, const char c);
    int count (std::string str, std::string c);
    int countQ (std::string str, const char c);
    std::string* split (std::string str, const char c);
    std::string* split (std::string str);
    std::string* splitQ (std::string str, const char c);
    std::string* splitQ (std::string str);
    std::string strip (std::string str, const char c);
    std::string strip (std::string str);
    std::string stripBegin (std::string str, const char c);
    std::string stripBegin (std::string str);
    std::string reverse (std::string str);
    std::string toLowerCase (std::string str);
    std::string toUpperCase (std::string str);
    std::string clearAllConsecutiveSpaces (std::string str); 
    std::string clearAllConsecutiveSpacesQ (std::string str); 
    bool isAllLetters (std::string str);
    bool isNumber(std::string str);
    bool isLowercase (std::string str);
    bool isUppercase (std::string str);
    bool contains (std::string str, const char c);
    bool contains (std::string str, std::string c);
};

#endif