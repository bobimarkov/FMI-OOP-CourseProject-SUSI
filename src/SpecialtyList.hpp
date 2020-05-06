#ifndef SPECIALTYLIST_HPP
#define SPECIALTYLIST_HPP
#include "Specialty.hpp"

#include <iostream>
#include <vector>

class SpecialtyList {
    public:
        static std::vector<Specialty> specialties;

        static void addSpecialty ();
        static void removeSpecialty ();
        static void addDiscipline ();
        static void removeDiscipline ();
        static void listSpecialties();
        static void listDisciplines();

        static void loadSpecialties();
        static void writeSpecialties();
};

#endif