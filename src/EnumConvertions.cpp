#include "EnumConvertions.hpp"
#include "Helpers/StringHelper.hpp"
#include <iostream>
#include <algorithm>

#define SH StringHelper

std::string EnumConvertions::getSpecialty (Specialty sp) {
    switch(static_cast<int>(sp)) {
        case 1: return "Informatics"; break;
        case 2: return "Computer science"; break;
        case 3: return "Software engineering"; break;
        case 4: return "Math and infomatics"; break;
        default: return "UNKNOWN"; break;
    }
}

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

Specialty EnumConvertions::stringToSpecialty (std::string str) {
    std::string lowerStr = SH::toLowerCase(str);
    int strNum = 0;
    if (SH::isNumber(str)) strNum = std::stoi(str); 
    if (lowerStr == "informatics" || strNum == 1) return Specialty::INFORMATICS;
    else if (lowerStr == "computer_science" || lowerStr == "computer science" || strNum == 2) return Specialty::COMPUTER_SCIENCE;
    else if (lowerStr == "software_engineering" || lowerStr == "software engineering" || strNum == 3) return Specialty::SOFTWARE_ENGINEERING;
    else if (lowerStr == "math_and_informatics" || lowerStr == "math and informatics" || strNum == 4) return Specialty::MATH_AND_INFORMATICS;
    else return Specialty::UNKNOWN;
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