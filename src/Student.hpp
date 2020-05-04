#ifndef STUDENT_SUSI_HPP
#define STUDENT_SUSI_HPP
#include "Discipline.hpp"
#include "EnumerationClasses.hpp"

#include <iostream>
#include <vector>

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

    Student();
    Student(int, Specialty, int, std::string);
    Student(const Student&);

    int getFN();
    Student_Status getStatus();

    Student& operator = (const Student&);

    void advance ();
    void changeStatus(Student_Status);

    void write(std::ofstream&);
    void read(std::ifstream&);

    friend std::ostream& operator << (std::ostream&, const Student&);
};  
 
#endif