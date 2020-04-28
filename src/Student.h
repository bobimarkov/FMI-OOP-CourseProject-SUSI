#ifndef STUDENT_SUSI_HEADER
#define STUDENT_SUSI_HEADER
#include "Discipline.h"
#include <iostream>

enum class Specialty { INFORMATICS, COMPUTER_SCIENCE, SOFTWARE_ENGINEERING, MATH_AND_INFORMATICS};
enum class Student_Status { GRADUATED, SIGNED, INTERRUPTED};

class Student {
private:
    std::string name;
    int faculty_number;
    int course;
    Specialty specialty;
    int group;
    Student_Status status;
    int average_grade;
    Discipline* disciplines;
public:
    Student(){
    }
};  
 
#endif