#ifndef ENUMCONV_HPP
#define ENUMCONV_HPP
#include "EnumerationClasses.hpp"
#include <iostream>

class EnumConvertions {
    public:
        static std::string getStudentStatus (Student_Status);
        static std::string getType (Type);
        
        static Student_Status stringToStudentStatus (std::string);
        static Type stringToType (std::string);
};

#endif