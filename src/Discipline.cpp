#include "Discipline.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <fstream>
#include <tuple>

Discipline::Discipline(): name(""), type(Type::UNKNOWN), credits(0) {

}

Discipline::Discipline(const Discipline& other) {
    this -> name = other.name;
    this -> type = other.type;
    this -> availableForCourses = other.availableForCourses;
    this -> credits = other.credits;
}

Discipline::Discipline(std::string name, Type type, std::vector<int> availableForCourses, double credits): name(name), type(type), availableForCourses(availableForCourses), credits(credits) {

}


Type Discipline::getType () const {
    return this -> type;
}

double Discipline::getCredits () const {
    return this -> credits;
}


std::vector<int> Discipline::getAvailableForCourses() const {
    return this -> availableForCourses;
}

std::string Discipline::getName () const {
    return this -> name;
}

int Discipline::getMinAvailableCourse () const {
    int minimal = availableForCourses[0];
    for(int i = 1; i < availableForCourses.size(); i++) {
        if (availableForCourses[i] < minimal) minimal = availableForCourses[i];
    }
    return minimal;
}

int Discipline::getMaxAvailableCourse () const {
    int maximal = availableForCourses[0];
    for(int i = 1; i < availableForCourses.size(); i++) {
        if (availableForCourses[i] > maximal) maximal = availableForCourses[i];
    }
    return maximal;
}

bool Discipline::checkMatchCurrentCourse (int course) const {
    for(int i : availableForCourses) {
        if (i == course) return true;
    }
    return false;
}

Discipline& Discipline::operator = (const Discipline& other) {
    if (this == &other) return *this;
    this -> name = other.name;
    this -> type = other.type;
    this -> availableForCourses = other.availableForCourses;
    this -> credits = other.credits;
    return *this;
}

void Discipline::write (std::ofstream& out) {
    int nameSize = name.length();
    out.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    out.write(name.c_str(), sizeof(char)*nameSize);
    
    out.write(reinterpret_cast<char*>(&type), sizeof(type));
    
    int availableCoursesCount = availableForCourses.size();
    out.write(reinterpret_cast<char*>(&availableCoursesCount), sizeof(availableCoursesCount));
    for(int i = 0; i < availableCoursesCount; i++) {
        out.write(reinterpret_cast<char*>(&availableForCourses[i]),sizeof(availableForCourses[i]));
    }
    
    out.write(reinterpret_cast<char*>(&credits), sizeof(credits));
}

void Discipline::read (std::ifstream& in) {
    int nameSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    name.resize(nameSize);
    in.read(&name[0], sizeof(char)*nameSize);

    in.read(reinterpret_cast<char*>(&type), sizeof(type));

    int availableCoursesCount;
    in.read(reinterpret_cast<char*>(&availableCoursesCount), sizeof(availableCoursesCount));
    for(int i = 0; i < availableCoursesCount; i++) {
        int course;
        in.read(reinterpret_cast<char*>(&course), sizeof(course));
        availableForCourses.push_back(course);
    }
    
    in.read(reinterpret_cast<char*>(&credits), sizeof(credits));
}

std::ostream& operator << (std::ostream& out, const Discipline& other) {
    out << other.name
        << " - Type: " << EnumConvertions::getType(other.type)
        << ", Available for courses: " << other.availableForCourses[0];
    for(int i = 1; i < other.availableForCourses.size(); i++) {
            std::cout << " " << other.availableForCourses[i];
    }
    out << ", Credits: " << other.credits;
    return out;
}