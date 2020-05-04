#ifndef ENUMS_HPP
#define ENUMS_HPP

enum class Student_Status { 
    GRADUATED = 1, 
    SIGNED = 2, 
    INTERRUPTED = 3,
    UNKNOWN
};

enum class Specialty { 
    INFORMATICS = 1, 
    COMPUTER_SCIENCE = 2, 
    SOFTWARE_ENGINEERING = 3, 
    MATH_AND_INFORMATICS = 4,
    UNKNOWN
};

enum class Type {
    OPTIONAL = 1, 
    COMPULSORY = 2,
    UNKNOWN
};
#endif