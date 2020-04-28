#ifndef DISCIPLINE_H
#define DISCIPLINE_H
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