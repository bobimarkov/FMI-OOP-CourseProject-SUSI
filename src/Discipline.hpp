#ifndef DISCIPLINE_HPP
#define DISCIPLINE_HPP
#include <iostream>
#include <vector>
#include "EnumerationClasses.hpp"

class Discipline{
    private:
        std::string name;
        Type type; 
        std::vector<int> availableForCourses;
        double credits;

    public:
        Discipline();
        Discipline(const Discipline&);
        Discipline(std::string, Type, std::vector<int>, double);

        Type getType() const;
        double getCredits() const;
        std::vector<int> getAvailableForCourses() const;
        std::string getName() const;
        int getMinAvailableCourse() const;
        int getMaxAvailableCourse() const;
        
        bool checkMatchCurrentCourse(int) const;

        Discipline& operator = (const Discipline&);

        void write(std::ofstream&);
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Discipline&);
};

#endif