#ifndef STUDENT_SUSI_HPP
#define STUDENT_SUSI_HPP
#include "Discipline.hpp"
#include <iostream>
#include <vector>

enum class Student_Status { GRADUATED = 1, SIGNED = 2, INTERRUPTED = 3} ;
enum class Specialty { INFORMATICS = 1, COMPUTER_SCIENCE = 2, SOFTWARE_ENGINEERING = 3, MATH_AND_INFORMATICS = 4};

class Student {

private:
    std::string name;
    int faculty_number;
    int course;
    Specialty specialty;
    int group;
    Student_Status status;
    int average_grade;
    std::vector<Discipline> disciplines;
public:

    Student() = default;
    Student(int, Specialty, int, std::string);
    Student(const Student&);

    Student& operator = (const Student&);

    void advance ();
    void changeStatus(Student_Status);

    void write(std::ofstream&);
    void read(std::ifstream&);

    friend std::ostream& operator << (std::ostream&, const Student&);
};  
 
#endif