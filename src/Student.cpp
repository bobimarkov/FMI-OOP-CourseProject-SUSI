#include "Student.hpp"
#include <iostream>
#include <fstream>

Student::Student(const Student& other) {
    this -> faculty_number = other.faculty_number;
    this -> course = other.course;
    this -> name = other.name;
    this -> specialty = other.specialty;
    this -> group = other.group;
    this -> status = other.status;
    this -> average_grade = other.average_grade;
    this -> disciplines = other.disciplines;
}

Student::Student(int faculty_number, Specialty specialty, int group, std::string name):faculty_number(faculty_number), specialty(specialty), group(group), name(name) {

}

Student& Student::operator = (const Student& other) {
    if (this == &other) return *this;
    this -> faculty_number = other.faculty_number;
    this -> course = other.course;
    this -> name = other.name;
    this -> specialty = other.specialty;
    this -> group = other.group;
    this -> status = other.status;
    this -> average_grade = other.average_grade;
    this -> disciplines = other.disciplines;
    return *this;
}

void Student::advance () {
    this -> course++;
}

void Student::changeStatus(Student_Status status) {
    this -> status = status;
}

void Student::write(std::ofstream& out) {
    out.write(reinterpret_cast<char*>(&name), sizeof(name));
    out.write(reinterpret_cast<char*>(&faculty_number), sizeof(faculty_number));
    out.write(reinterpret_cast<char*>(&course), sizeof(course));
    out.write(reinterpret_cast<char*>(&specialty), sizeof(specialty));
    out.write(reinterpret_cast<char*>(&group), sizeof(group));
    out.write(reinterpret_cast<char*>(&status), sizeof(status));
    out.write(reinterpret_cast<char*>(&average_grade), sizeof(average_grade));
    out.write(reinterpret_cast<char*>(&disciplines), sizeof(disciplines));
}

void Student::read(std::ifstream& in) {
    in.read(reinterpret_cast<char*>(&name), sizeof(name));
    in.read(reinterpret_cast<char*>(&faculty_number), sizeof(faculty_number));
    in.read(reinterpret_cast<char*>(&course), sizeof(course));
    in.read(reinterpret_cast<char*>(&specialty), sizeof(specialty));
    in.read(reinterpret_cast<char*>(&group), sizeof(group));
    in.read(reinterpret_cast<char*>(&status), sizeof(status));
    in.read(reinterpret_cast<char*>(&average_grade), sizeof(average_grade));
    in.read(reinterpret_cast<char*>(&disciplines), sizeof(disciplines));
}

std::ostream& operator << (std::ostream& out, const Student& other) {
    out << other.name << std::endl
        << other.faculty_number << std::endl
        << other.course << std::endl 
        << static_cast<int>(other.specialty) << std::endl
        << other.group << std::endl
        << static_cast<int>(other.status) << std::endl
        << other.average_grade << std::endl
        << "Disciplines:\n";
    for (Discipline d : other.disciplines) {
        out << d << std::endl;
    }
    return out;
}