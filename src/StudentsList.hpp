#ifndef STUDENTSLIST_HPP
#define STUDENTSLIST_HPP

#include "Student.hpp"

#include <iostream>
#include <vector>
#include <string>

///Съхранява списък с всички записани студенти

class StudentsList {
    public:
        ///Вектор с всички записани студенти
        static std::vector<Student> students;

        ///Проверява дали студент е записал вече дадена дисциплина
        static bool checkAlreadyEnrolledDiscipline(int, int, int);

        ///Проверява дали даден факултетен номер е вече записан
        static bool alreadyEnrolledFN(int);

        ///Връща индекса в списъка на студента с определен факултетен номер
        static int findFN(int);

        ///Намира индекса на дадена дисциплината, към която е записан студента
        static int findStudentDisciplineIndex(int, std::string);
};

#endif