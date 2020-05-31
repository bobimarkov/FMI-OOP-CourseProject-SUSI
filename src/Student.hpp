#ifndef STUDENT_SUSI_HPP
#define STUDENT_SUSI_HPP
#include "Discipline.hpp"
#include "EnumerationClasses.hpp"
#include "Specialty.hpp"

#include <iostream>
#include <vector>

///Съхранява информация относно записана дисциплина на студента.
struct StudentDiscipline {
    ///Име на дисциплината.
    std::string discipline;
    ///Оценка на дисциплината.
    double grade;
    ///В кой курс е записана дисциплината.
    int enrolledCourse;

    StudentDiscipline(std::string discipline, double grade, int enrolledCourse) {
        this -> discipline = discipline;
        this -> grade = grade;
        this -> enrolledCourse = enrolledCourse;
    }
};

///Съхранява информацията относно студент.

class Student {
    private:
        ///Име на студента.
        std::string name;
        ///Факултетен номер.
        int faculty_number;
        ///Курс на студента.
        int course;
        ///Име на записаната специалност.
        std::string specialty;
        ///Група.
        int group;
        ///Статус на студента.
        Student_Status status;
        ///Средната му оценка.
        double average_grade;
        ///Всички записани дисциплини.
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

        ///Взима курса, в който студент е записал дадена дисциплина.
        int getDisciplineEnrolledCourse(std::string) const;

        Student& operator = (const Student&);

        ///Брои събраните домомента кредити на студента.
        double countCredits() const; 
        
        ///Добавя дисциплина към студента.
        void addDiscipline (Discipline&);

        ///Записва студента в горен курс.
        void advance ();

        ///Променя статуса на студент.
        void changeStatus(Student_Status);

        ///Задава му специалност.
        void setSpecialty(std::string);

        ///Задава група на студента.
        void setGroup(int);

        ///Добавя оценка към дадена дисциплина.
        void addGrade(std::string, double);

        ///Пресмята средната оценка.
        void calculateAvgGrade();

        ///Проверява дали дадена дисциплина е записана за студента.
        bool isDisciplineEnrolled(std::string);

        ///Записва студента в бинарен файл.
        void write(std::ofstream&);
        
        ///Чете студент от бинарен файл.
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Student&);
};  
 
#endif