#include "Specialty.hpp"
#include "Helpers/StringHelper.hpp"

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

Specialty Specialty::stringToSpecialty (std::string str) {
    std::string lowerStr = SH::toLowerCase(str);
    int strNum = 0;
    if (SH::isNumber(str)) strNum = std::stoi(str); 
    if (lowerStr == "informatics" || strNum == 1) return Specialty("Informatics",50);
    else if (lowerStr == "computer_science" || lowerStr == "computer science" || strNum == 2) return Specialty("Computer science", 52);
    else if (lowerStr == "software_engineering" || lowerStr == "software engineering" || strNum == 3) return Specialty("Software engineering", 60);
    else if (lowerStr == "math_and_informatics" || lowerStr == "math and informatics" || strNum == 4) return Specialty("Math and Informatics", 50);
    else return Specialty();
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
    std::cout << "Disciplines for specialty " << this -> name << ":\n"; 
    for(int i = 0; i < availableDisciplines.size(); i++) {
        std::cout << i+1 << ": " << availableDisciplines[i].getName() << std::endl;
    }
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
    char* nameStr = new char[nameSize];
    nameStr[nameSize] = 0;
    in.read(nameStr, sizeof(char)*nameSize);
    this -> name = nameStr;
    delete[] nameStr;
    in.read(reinterpret_cast<char*>(&minCredits), sizeof(minCredits));
    in.read(reinterpret_cast<char*>(&disciplineSize), sizeof(disciplineSize));
    for(int i = 0; i < disciplineSize; i++) {
        Discipline d;
        d.read(in);
        availableDisciplines.push_back(d);
    }
}

std::ostream& operator << (std::ostream& out, const Specialty& other) {
    out << "Name: " << other.name
        << "Min Credits: " << other.minCredits;

    for(Discipline d: other.availableDisciplines) {
        out << d << std::endl;
    }
    return out;
}