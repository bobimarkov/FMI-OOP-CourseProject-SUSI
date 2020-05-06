#ifndef SPECIALTY_HPP
#define SPECIALTY_HPP
#include <iostream>
#include <vector>
#include "Discipline.hpp" 

class Specialty {
    private:
        std::string name;
        double minCredits;
        std::vector<Discipline> availableDisciplines;
    public:
        Specialty();
        Specialty(const Specialty&);
        Specialty(std::string, double);
        Specialty(std::string, double, std::vector<Discipline>);

        Specialty& operator = (const Specialty&);
        
        void addDiscipline(Discipline);

        static Specialty stringToSpecialty (std::string);

        std::string getName() const;
        double getMinCredits() const;
        std::vector<Discipline>& getAvailableDisciplines();

        void printAvailableDisciplines() const; 

        void write(std::ofstream&);
        void read(std::ifstream&);

        friend std::ostream& operator << (std::ostream&, const Specialty&);
};

#endif