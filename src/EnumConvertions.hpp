#ifndef ENUMCONV_HPP
#define ENUMCONV_HPP
#include "EnumerationClasses.hpp"
#include <iostream>

///Съхранява преобразувания от enum към std::string и обратното

class EnumConvertions {
    public:
        ///Преобразува студентски статус в std::string
        static std::string getStudentStatus (Student_Status);
        
        ///Преобразува тип на дисциплина към std::string
        static std::string getType (Type);
        
        ///Преобразува std::string към студентски статус, ако е възможно
        static Student_Status stringToStudentStatus (std::string);

        ///Преобразува std::string към тип на дисциплина, ако е възможно
        static Type stringToType (std::string);
};

#endif