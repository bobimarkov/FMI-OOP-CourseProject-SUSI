#ifndef DISCIPLINE_HPP
#define DISCIPLINE_HPP
#include <iostream>

enum class Type {OPTIONAL = 1, COMPULSORY = 2};

class Discipline{
    private:
        std::string name;
        Type type; 
        std::string specialty;
        int availableForCourse;
        double grade;
    public:
        Discipline() = default;
        Discipline(const Discipline&);
        Discipline(std::string, Type, std::string, int, double);

        Discipline& operator = (const Discipline&);

        friend std::ostream& operator << (std::ostream&, const Discipline&);
};

#endif