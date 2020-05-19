#ifndef SPECIALTYLIST_HPP
#define SPECIALTYLIST_HPP
#include "Specialty.hpp"
#include "Student.hpp"

#include <iostream>
#include <vector>

class SpecialtyList {
    public:
        static std::vector<Specialty> specialties;

        static int findSpecialty(std::string);
        static int findDisciplineInSpecialty(std::string, std::string);
        static int countNotPassedMutualCompDisciplines(Student&, int);

        static void addSpecialty(std::string, double);
        static void removeSpecialty(std::string);
        
        static void addDiscipline(std::string, std::string, std::string, std::string, double);
        static void removeDiscipline(std::string, std::string);

        static void listSpecialties();
        static void listDisciplines(std::string);
        static void listAll();

        static void loadSpecialties();
        static void writeSpecialties();
};

#endif