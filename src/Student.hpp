#ifndef STUDENT_SUSI_HPP
#define STUDENT_SUSI_HPP
#include "Discipline.hpp"
#include "EnumerationClasses.hpp"
#include "Specialty.hpp"

#include <iostream>
#include <vector>

class Student {
    private:
        std::string name;
        int faculty_number;
        int course;
        std::string specialty;
        int group;
        Student_Status status;
        int average_grade;
        std::vector<Discipline> disciplines;
    public:

        Student();
        Student(int, std::string, int, std::string);
        Student(const Student&);

        int getFN();
        int getCourse();
        std::string getSpecialty();
        Student_Status getStatus();
        std::vector<Discipline> getDisciplines ();

        Student& operator = (const Student&);

        void addDiscipline (Discipline);
        void advance ();
        void changeStatus(Student_Status);
        void setSpecialty(std::string);
        void setGroup(int);
        void calculateAvgGrade();

        void write(std::ofstream&);
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Student&);
};  
 
#endif