#include <iostream>
#include "StringHelper.hpp"

int StringHelper::count (std::string str, const char c) {
    int counter = 0;
    for(int i = 0; i < str.length(); i++) {
        if (str[i] == c) counter++;
    }
    return counter;
}

int StringHelper::count (std::string str, std::string c) {
    int counter = 0, it = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == c[it]) {
            it++;
            for(int j = i + 1; j < str.length(); j++) {
                if(str[j] == c[it]) it++;
                else break;
                if(it == c.length()) counter++;
            }
        }
    }
    return counter;
}

std::string* StringHelper::split (std::string str, const char c) {
    int arrSize = count(str,c) + 1;
    std::string* array = new std::string[arrSize];
    if(arrSize == 1) {
        array[0] = str;
        return array;    
    };
    std::string tempString;
    int h = 0;
    for (int i = 0; i < str.length(); i++) {
        if(str[i] == c) {
            array[h] = tempString;
            tempString.erase();
            h++;
        }
        else {
            tempString += str[i];
        }
    }   
    array[h] = tempString;
    return array;
}

std::string* StringHelper::split (std::string str) {
    std::string* newString = split(str, ' ');
    return newString;
}

std::string StringHelper::strip (std::string str, const char c) {
    int stopIndex = str.length()-1;
    for(int i = str.length()-2;i>=0;i--) { 
        if(str[i] == c) {
            stopIndex--;
        }
        else break;
    }
    std::string newString = str.substr(0,stopIndex);
    return newString;
}

std::string StringHelper::strip (std::string str) {
    std::string newString = strip(str, ' ');
    return newString;
}

std::string StringHelper::stripBegin (std::string str, const char c) {
    int beginIndex = 0;
    for(int i = 1; i < str.length(); i++) { 
        if(str[i] == c) {
            beginIndex++;
        }
        else break;
    }
    std::string newString = str.substr(beginIndex + 1,str.length()-beginIndex);
    return newString;
}

std::string StringHelper::stripBegin (std::string str) {
    std::string newString = stripBegin(str, ' ');
    return newString;
}

std::string StringHelper::reverse (std::string str) {
    std::string newString;
    for(int i=str.length()-1; i>=0; --i) {
        newString += str[i];
    }
    return newString;
}

std::string StringHelper::toLowerCase (std::string str) {
    std::string newString = str;
    for(int i=0;i<newString.length();i++) {
        if(newString[i]>='A' && newString[i]<='Z') newString[i]+=('a'-'A');
    }
    return newString;
}

std::string StringHelper::toUpperCase (std::string str) {
    std::string newString = str;
    for(int i=0;i<newString.length();i++) {
        if(newString[i]>='a' && newString[i]<='z') newString[i]-=('a'-'A');
    }
    return newString;
}

bool StringHelper::isAllLetters (std::string str) {
    for(int i = 0; i < str.length(); i++) {
        if(!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))) return false;
    }
    return true;
}

bool StringHelper::isNumber(std::string str) {
    for(int i = 0; i < str.length(); i++) {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool StringHelper::isLowercase (std::string str) {
    return str.length() != 0 && toLowerCase(str) == str;
}

bool StringHelper::isUppercase (std::string str) {
    return str.length() != 0 && toUpperCase(str) == str;
}

bool StringHelper::contains (std::string str, const char c) {
    return count(str,c) > 0;
}

bool StringHelper::contains (std::string str, std::string c) {
    int it = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == c[it]) {
            it++;
            for(int j = i + 1; j < str.length(); j++) {
                if(str[j] == c[it]) it++;
                else break;
                if(it == c.length()) return true;
            }
        }
    }
    return false;
}
