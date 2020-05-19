#include "Specialty.hpp"
#include "Helpers/StringHelper.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define SH StringHelper

Specialty::Specialty(): name("UNKNOWN"), minCredits(0){
    availableDisciplines.clear();
}

Specialty::Specialty(const Specialty& other): name(other.name), minCredits(other.minCredits), availableDisciplines(other.availableDisciplines) {

}

Specialty::Specialty(std::string name, double minCredits): name(name), minCredits(minCredits) {
    availableDisciplines.clear();
}

Specialty::Specialty(std::string name, double minCredits, std::vector<Discipline> availableDisciplines):name(name), minCredits(minCredits), availableDisciplines(availableDisciplines) {

}

Specialty& Specialty::operator = (const Specialty& other) {
    if(this == &other) return *this;
    this -> name = other.name;
    this -> minCredits = other.minCredits; 
    this -> availableDisciplines = other.availableDisciplines;
    return *this;
}

void Specialty::addDiscipline(Discipline discipline) {
    this -> availableDisciplines.push_back(discipline);
}

std::string Specialty::getName() const {
    return this -> name;
}

double Specialty::getMinCredits() const {
    return this -> minCredits;
}

std::vector<Discipline>& Specialty::getAvailableDisciplines() {
    return this -> availableDisciplines;
}

void Specialty::printAvailableDisciplines() const{
    std::cout << "Disciplines for specialty \"" << this -> name << "\":\n"; 
    for(int i = 0; i < availableDisciplines.size(); i++) {
        std::cout << i+1 << ": " << availableDisciplines[i].getName() << " (" << EnumConvertions::getType(availableDisciplines[i].getType());
        std::cout << ", " << ((availableDisciplines[i].getAvailableForCourses().size() > 1) ? "Courses: " : "Course: ") << availableDisciplines[i].getAvailableForCourses()[0];
        for(int j = 1; j < availableDisciplines[i].getAvailableForCourses().size(); j++) {
            std::cout << " " << availableDisciplines[i].getAvailableForCourses()[j];
        }
        std::cout << ")" <<  std::endl;
    }
}

int Specialty::checkMaxAvailableCourse() const {
    int course = 1;
    for(Discipline d : availableDisciplines) {
        int currentDisciplineCourse = d.getMaxAvailableCourse();
        if (currentDisciplineCourse > course) course = currentDisciplineCourse;
    } 
    return course;
}

void Specialty::write(std::ofstream& out) {
    int nameSize = name.length();
    int disciplineSize = availableDisciplines.size();
    out.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    out.write(name.c_str(), sizeof(char)*nameSize);
    out.write(reinterpret_cast<char*>(&minCredits), sizeof(minCredits));
    out.write(reinterpret_cast<char*>(&disciplineSize), sizeof(disciplineSize));
    for(int i = 0; i < disciplineSize; i++) { 
        availableDisciplines[i].write(out);
    }
}

void Specialty::read(std::ifstream& in) {
    int nameSize;
    int disciplineSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    name.resize(nameSize);
    in.read(&name[0], sizeof(char)*nameSize);

    in.read(reinterpret_cast<char*>(&minCredits), sizeof(minCredits));
    in.read(reinterpret_cast<char*>(&disciplineSize), sizeof(disciplineSize));

    for(int i = 0; i < disciplineSize; i++) {
        Discipline d;
        d.read(in);
        availableDisciplines.push_back(d);
    }
}

std::ostream& operator << (std::ostream& out, const Specialty& other) {
    out << "Specialty name: " << other.name << std::endl
        << "Min Credits: " << other.minCredits << std::endl << std::endl
        << "Disciplines:" << std::endl;
    
    for(int i = 0; i < other.availableDisciplines.size(); i++) {
        out << i+1 << ": " << other.availableDisciplines[i] << std::endl;
    }
    return out;
}