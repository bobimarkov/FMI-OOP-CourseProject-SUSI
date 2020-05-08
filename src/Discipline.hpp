#ifndef DISCIPLINE_HPP
#define DISCIPLINE_HPP
#include <iostream>
#include "EnumerationClasses.hpp"

class Discipline{
    private:
        std::string name;
        Type type; 
        int availableForCourse;
        bool hadExam;
        double grade;
        double credits;
    public:
        Discipline();
        Discipline(const Discipline&);
        Discipline(std::string, Type, int, double);

        void setExam(bool);

        double getGrade() const;
        double getCredits() const;
        bool getHadExam() const;
        int getAvailableForCourse() const;
        std::string getName() const;

        Discipline& operator = (const Discipline&);

        void write(std::ofstream&);
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Discipline&);
};

#endif