#include "Student.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <vector>
#include <fstream>

Student::Student(): name(""), faculty_number(0), course(0), group(0), status(Student_Status::UNKNOWN), average_grade(0) {
    disciplines.clear();
}

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

Student::Student(int faculty_number, std::string specialty, int group, std::string name):faculty_number(faculty_number), specialty(specialty), group(group), name(name), course(1), status(Student_Status::SIGNED), average_grade(0) {
    this -> disciplines.clear();
}

int Student::getFN () {
    return this -> faculty_number;
}

int Student::getCourse () {
    return this -> course;
}

std::string Student::getSpecialty () {
    return this -> specialty;
}

Student_Status Student::getStatus() {
    return this -> status;
}

std::vector<Discipline> Student::getDisciplines () {
    return this -> disciplines;
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

void Student::addDiscipline (Discipline discipline) {
    this -> disciplines.push_back(discipline);
}

void Student::advance () {
    this -> course++;
}

void Student::changeStatus (Student_Status status) {
    this -> status = status;
}

void Student::setSpecialty (std::string name) {
    this -> specialty = name;
}

void Student::setGroup (int group) {
    this -> group = group;
}

void Student::calculateAvgGrade () {
    int sumGrades = 0, countNotTaken = 0;
    for (Discipline d : this -> disciplines) {
        if(d.getHadExam()) sumGrades += d.getGrade();
        else countNotTaken++;
    }
    this -> average_grade = sumGrades/(this -> disciplines.size() - countNotTaken);
}

void Student::write(std::ofstream& out) {
    int nameSize = name.length();
    out.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    out.write(name.c_str(), sizeof(char)*nameSize);
    
    out.write(reinterpret_cast<char*>(&faculty_number), sizeof(faculty_number));
    out.write(reinterpret_cast<char*>(&course), sizeof(course));
    
    int specialtyNameSize = specialty.length();
    out.write(reinterpret_cast<char*>(&specialtyNameSize), sizeof(specialtyNameSize));
    out.write(specialty.c_str(), sizeof(char)*specialtyNameSize);

    out.write(reinterpret_cast<char*>(&group), sizeof(group));
    out.write(reinterpret_cast<char*>(&status), sizeof(status));
    out.write(reinterpret_cast<char*>(&average_grade), sizeof(average_grade));
    out.write(reinterpret_cast<char*>(&disciplines), sizeof(disciplines));
}

void Student::read(std::ifstream& in) {
    int nameSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    char* nameStr = new char[nameSize + 1];
    in.read(nameStr, sizeof(char)*nameSize);
    nameStr[nameSize] = 0;
    this -> name = nameStr;
    delete[] nameStr;

    in.read(reinterpret_cast<char*>(&faculty_number), sizeof(faculty_number));
    in.read(reinterpret_cast<char*>(&course), sizeof(course));

    int specialtyNameSize;
    in.read(reinterpret_cast<char*>(&specialtyNameSize), sizeof(specialtyNameSize));
    char* specialtyNameStr = new char[specialtyNameSize + 1];
    in.read(specialtyNameStr, sizeof(char)*specialtyNameSize);
    specialtyNameStr[specialtyNameSize] = 0;
    this -> specialty = specialtyNameStr;
    delete[] specialtyNameStr;

    in.read(reinterpret_cast<char*>(&group), sizeof(group));
    in.read(reinterpret_cast<char*>(&status), sizeof(status));
    in.read(reinterpret_cast<char*>(&average_grade), sizeof(average_grade));
    in.read(reinterpret_cast<char*>(&disciplines), sizeof(disciplines));
}

std::ostream& operator << (std::ostream& out, const Student& other) {
    out << "Name: " << other.name << std::endl
        << "Faculty number: " << other.faculty_number << std::endl
        << "Course: " << other.course << std::endl 
        << "Specialty: " << other.specialty << std::endl
        << "Group: " << other.group << std::endl
        << "Status: " << EnumConvertions::getStudentStatus(other.status) << std::endl
        << "Average grade: " << other.average_grade << std::endl
        << "\nDisciplines:\n";
    for (Discipline d : other.disciplines) {
        out << d << std::endl;
    }
    return out;
}