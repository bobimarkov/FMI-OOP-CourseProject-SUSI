#ifndef DISCIPLINE_HPP
#define DISCIPLINE_HPP
#include <iostream>
#include "EnumerationClasses.hpp"

class Discipline{
    private:
        std::string name;
        Type type; 
        Specialty specialty;
        int availableForCourse;
        double grade;
        double credits;
    public:
        Discipline();
        Discipline(const Discipline&);
        Discipline(std::string, Type, Specialty, int, double, double);

        Discipline& operator = (const Discipline&);

        friend std::ostream& operator << (std::ostream&, const Discipline&);
};

#endif