#ifndef DISCIPLINE_HPP
#define DISCIPLINE_HPP
#include <iostream>
#include <vector>
#include "EnumerationClasses.hpp"

///Съхранява данните относно една дисциплина.

class Discipline{
    private:
        ///Име на дисциплината.
        std::string name;

        ///Тип на дисциплината.
        ///Избираема или задължителна.
        Type type; 

        ///Вектор, складиращ за кои курсове е достъпна дисциплината.
        std::vector<int> availableForCourses;

        ///Кредити от дисциплината.
        double credits;

    public:
        Discipline();
        Discipline(const Discipline&);
        Discipline(std::string, Type, std::vector<int>, double);

        Type getType() const;
        double getCredits() const;
        std::vector<int> getAvailableForCourses() const;
        std::string getName() const;
        
        ///Взима минималният курс, в който може да бъде записана дисциплината.
        int getMinAvailableCourse() const;

        ///Взима максималният курс, в който може да бъде записана дисциплината.
        int getMaxAvailableCourse() const;
        
        ///Проверява дали дисциплината може да бъде записана в даден курс.
        bool checkMatchCurrentCourse(int) const;

        Discipline& operator = (const Discipline&);

        ///Записва дисциплината в бинарен файл.
        void write(std::ofstream&);

        ///Чете дисциплина от бинарен файл.
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Discipline&);
};

#endif