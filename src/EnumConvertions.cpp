#include "EnumConvertions.hpp"
#include "Helpers/StringHelper.hpp"
#include <iostream>
#include <string>

#define SH StringHelper

std::string EnumConvertions::getStudentStatus (Student_Status ss) {
    switch(static_cast<int>(ss)) {
        case 1: return "Graduated"; break;
        case 2: return "Signed"; break;
        case 3: return "Interrupted"; break;
        default: return "UNKNOWN"; break;
    }
}

std::string EnumConvertions::getType (Type t) {
    switch(static_cast<int>(t)) {
        case 1: return "Optional"; break;
        case 2: return "Compulsory"; break;
        default: return "UNKNOWN"; break;
    }
}

Student_Status EnumConvertions::stringToStudentStatus (std::string str) {
    std::string lowerStr = SH::toLowerCase(str);
    int strNum = 0;
    if (SH::isNumber(str)) strNum = std::stoi(str); 
    if (lowerStr == "graduated" || strNum == 1) return Student_Status::GRADUATED;
    else if (lowerStr == "signed" || strNum == 2) return Student_Status::SIGNED;
    else if (lowerStr == "interrupted" || strNum == 3) return Student_Status::INTERRUPTED;
    else return Student_Status::UNKNOWN;
}

Type EnumConvertions::stringToType (std::string str) {
    std::string lowerStr = SH::toLowerCase(str);
    int strNum = 0;
    if (SH::isNumber(str)) strNum = std::stoi(str); 
    if (lowerStr == "optional" || strNum == 1) return Type::OPTIONAL;
    else if (lowerStr == "compulsory" || strNum == 2) return Type::COMPULSORY;
    else return Type::UNKNOWN;
}