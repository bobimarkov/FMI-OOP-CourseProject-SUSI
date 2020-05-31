#ifndef SPECIALTY_HPP
#define SPECIALTY_HPP
#include <iostream>
#include <vector>
#include "Discipline.hpp" 

///Съхранява данните относно специалност.

class Specialty {
    private:
        ///Име на специалността.
        std::string name;
        ///Минимално изкарани кредити, за да можеш да завършиш специалността.
        double minCredits;
        ///Всички дисциплини към съответната специалност.
        std::vector<Discipline> availableDisciplines;
    public:
        Specialty();
        Specialty(const Specialty&);
        Specialty(std::string, double);
        Specialty(std::string, double, std::vector<Discipline>);

        Specialty& operator = (const Specialty&);
        
        ///Добавя нова дисциплина.
        void addDiscipline(Discipline);

        std::string getName() const;
        double getMinCredits() const;
        std::vector<Discipline>& getAvailableDisciplines();

        ///Извежда всички дисциплини за специалността.
        void printAvailableDisciplines() const; 
        
        ///Проверява максималният курс за съответната специалност.
        int checkMaxAvailableCourse() const;

        ///Записва специалност в бинарен файл.
        void write(std::ofstream&);
        ///Чете специалност в бинарен файл.
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Specialty&);
};

#endif