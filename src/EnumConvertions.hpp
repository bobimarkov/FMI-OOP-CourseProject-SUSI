#ifndef ENUMCONV_HPP
#define ENUMCONV_HPP
#include "EnumerationClasses.hpp"
#include <iostream>

namespace EnumConvertions {
    std::string getSpecialty (Specialty);
    std::string getStudentStatus (Student_Status);
    std::string getType (Type);
    
    Specialty stringToSpecialty (std::string);
    Student_Status stringToStudentStatus (std::string);
    Type stringToType (std::string);
};

#endif