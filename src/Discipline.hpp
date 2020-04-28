#ifndef DISCIPLINE_HPP
#define DISCIPLINE_HPP
#include <iostream>

class Discipline{
    private:
        std::string name;
        std::string type; 
        std::string specialty;
        int availableForCourse;
        double grade;
    public:
        Discipline() = default;

};

#endif