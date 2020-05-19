#ifndef STUDENT_SUSI_HPP
#define STUDENT_SUSI_HPP
#include "Discipline.hpp"
#include "EnumerationClasses.hpp"
#include "Specialty.hpp"

#include <iostream>
#include <vector>

struct StudentDiscipline {
    std::string discipline;
    double grade;
    int enrolledCourse;

    StudentDiscipline(std::string discipline, double grade, int enrolledCourse) {
        this -> discipline = discipline;
        this -> grade = grade;
        this -> enrolledCourse = enrolledCourse;
    }
};

class Student {
    private:
        std::string name;
        int faculty_number;
        int course;
        std::string specialty;
        int group;
        Student_Status status;
        double average_grade;
        std::vector<StudentDiscipline> disciplines;

    public:

        Student(); 
        Student(int, std::string, int, std::string);
        Student(const Student&);

        int getFN() const;
        int getCourse() const;
        int getGroup() const;
        std::string getName() const;
        std::string getSpecialty() const;
        Student_Status getStatus() const;
        std::vector<StudentDiscipline>& getDisciplines();
        double getGrade(std::string) const;
        double getAverageGrade() const;
        int getDisciplineEnrolledCourse(std::string) const;

        Student& operator = (const Student&);

        double countCredits() const; 
        void addDiscipline (Discipline&);
        void advance ();
        void changeStatus(Student_Status);
        void setSpecialty(std::string);
        void setGroup(int);
        void addGrade(std::string, double);
        void calculateAvgGrade();
        bool isDisciplineEnrolled(std::string);

        void write(std::ofstream&);
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Student&);
};  
 
#endif