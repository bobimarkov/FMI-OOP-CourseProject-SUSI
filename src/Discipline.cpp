#include "Discipline.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <fstream>

Discipline::Discipline(): name(""), type(Type::UNKNOWN), availableForCourse(0), hadExam(false), grade(0), credits(0) {

}

Discipline::Discipline(const Discipline& other) {
    this -> name = other.name;
    this -> type = other.type;
    this -> availableForCourse = other.availableForCourse;
    this -> hadExam = other.hadExam;
    this -> grade = other.grade;
    this -> credits = other.credits;
}

Discipline::Discipline(std::string name, Type type, int availableForCourse, double credits): name(name), type(type), availableForCourse(availableForCourse), grade(2), credits(credits), hadExam(false) {

}

void Discipline::setExam (bool flag) {
    this -> hadExam = flag;
}

void Discipline::setGrade (double grade) {
    this -> grade = grade;
}

double Discipline::getGrade () const {
    return this -> grade;
}

double Discipline::getCredits () const {
    return this -> credits;
}

bool Discipline::getHadExam () const {
    return this -> hadExam;
}

int Discipline::getAvailableForCourse() const {
    return this -> availableForCourse;
}

std::string Discipline::getName () const {
    return this -> name;
}

Discipline& Discipline::operator = (const Discipline& other) {
    if (this == &other) return *this;
    this -> name = other.name;
    this -> type = other.type;
    this -> availableForCourse = other.availableForCourse;
    this -> hadExam = other.hadExam;
    this -> grade = other.grade;
    this -> credits = other.credits;
    return *this;
}

void Discipline::write (std::ofstream& out) {
    int nameSize = name.length();
    out.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    out.write(name.c_str(), sizeof(char)*nameSize);
    out.write(reinterpret_cast<char*>(&type), sizeof(type));
    out.write(reinterpret_cast<char*>(&availableForCourse), sizeof(availableForCourse));
    out.write(reinterpret_cast<char*>(&hadExam), sizeof(hadExam));
    out.write(reinterpret_cast<char*>(&grade), sizeof(grade));
    out.write(reinterpret_cast<char*>(&credits), sizeof(credits));
}

void Discipline::read (std::ifstream& in) {
    int nameSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    char* nameStr = new char[nameSize + 1];
    in.read(nameStr, sizeof(char)*nameSize);
    nameStr[nameSize] = 0;
    this -> name = nameStr;
    delete[] nameStr;
    in.read(reinterpret_cast<char*>(&type), sizeof(type));
    in.read(reinterpret_cast<char*>(&availableForCourse), sizeof(availableForCourse));
    in.read(reinterpret_cast<char*>(&hadExam), sizeof(hadExam));
    in.read(reinterpret_cast<char*>(&grade), sizeof(grade));
    in.read(reinterpret_cast<char*>(&credits), sizeof(credits));
}

std::ostream& operator << (std::ostream& out, const Discipline& other) {
    out << "\nDiscipline name: " << other.name << std::endl
        << "Type: " << EnumConvertions::getType(other.type) << std::endl
        << "Available for course: " << other.availableForCourse << std::endl
        << "Had exam: " << other.hadExam << std::endl
        << "Grade: " << other.grade << std::endl
        << "Credits: " << other.credits << std::endl;
    return out;
}