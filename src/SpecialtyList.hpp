#ifndef SPECIALTYLIST_HPP
#define SPECIALTYLIST_HPP
#include "Specialty.hpp"
#include "Student.hpp"

#include <iostream>
#include <vector>

class SpecialtyList {
    public:
        static std::vector<Specialty> specialties;

        static size_t findSpecialty(std::string);
        static bool checkPassedCompDisciplines(Student&);

        static void addSpecialty();
        static void removeSpecialty();
        
        static void addDiscipline();
        static void removeDiscipline();

        static void listSpecialties();
        static void listDisciplines();
        static void listAll();

        static void loadSpecialties();
        static void writeSpecialties();
};

#endif