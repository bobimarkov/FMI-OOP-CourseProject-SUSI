#ifndef SPECIALTYLIST_HPP
#define SPECIALTYLIST_HPP
#include "Specialty.hpp"
#include "Student.hpp"

#include <iostream>
#include <vector>

///Съхранява списък и действия относно всички специалности.

class SpecialtyList {
    public:
        ///Вектор, съхраняващ всички специалности.
        static std::vector<Specialty> specialties;

        ///Намира специалност от вектора със специалности.
        static int findSpecialty(std::string);

        ///Намира дисциплина от дадена специалност.
        static int findDisciplineInSpecialty(std::string, std::string);

        ///Брои колко са невзетите общи дисциплини между 2 специалности.
        static int countNotPassedMutualCompDisciplines(Student&, int);

        ///Добавя специалност.
        static void addSpecialty(std::string, double);

        ///Премахва специалност.
        static void removeSpecialty(std::string);
        
        /**Добавя дисциплина в дадена специалност.
         * @param специалност
         * @param име на дисциплината
         * @param тип на дисциплината
         * @param курсове, в които може да бъде записана
         * @param кредити, даваща дисциплината (само ако е избираема дисциплина)
         */
        
        static void addDiscipline(std::string, std::string, std::string, std::string, double);

        ///Премахва дисциплина от дадена специалност.
        static void removeDiscipline(std::string, std::string);

        ///Извежда всички специалности.
        static void listSpecialties();

        ///Извежда всички дисциплини към дадена специалност.
        static void listDisciplines(std::string);

        ///Извежда цялата информация относно всички специалности и дисциплините към тях.
        static void listAll();

        ///Чете списъка от бинарен файл.
        static void loadSpecialties();

        ///Записва списъка в бинарен файл.
        static void writeSpecialties();
};

#endif