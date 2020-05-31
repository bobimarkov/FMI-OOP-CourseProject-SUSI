#include "Student.hpp"
#include "SpecialtyList.hpp"
#include "StringHelper.hpp"
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

int Student::getFN () const {
    return this -> faculty_number;
}

int Student::getCourse () const {
    return this -> course;
}

int Student::getGroup() const {
    return this -> group;
}

std::string Student::getName() const {
    return this -> name;
}

std::string Student::getSpecialty () const {
    return this -> specialty;
}

Student_Status Student::getStatus() const {
    return this -> status;
}

std::vector<StudentDiscipline>& Student::getDisciplines () {
    return this -> disciplines;
}

double Student::getGrade(std::string discipline) const {
    for (StudentDiscipline d : disciplines) {
        if (StringHelper::toLowerCase(d.discipline) == StringHelper::toLowerCase(discipline)) return d.grade;
    }
    return -1;
}

double Student::getAverageGrade() const {
    return this -> average_grade;
}

int Student::getDisciplineEnrolledCourse(std::string discipline) const {
    for (StudentDiscipline d : disciplines) {
        if (StringHelper::toLowerCase(d.discipline) == StringHelper::toLowerCase(discipline)) return d.enrolledCourse;
    }
    return -1;
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

double Student::countCredits () const {
    double counter = 0;
    for(int i = 0; i < disciplines.size(); i++) {
        if (disciplines[i].grade >= 3) counter += SpecialtyList::specialties[SpecialtyList::findSpecialty(specialty)].getAvailableDisciplines()[SpecialtyList::findDisciplineInSpecialty(specialty, disciplines[i].discipline)].getCredits();
    }
    return counter;
}

void Student::addDiscipline (Discipline& discipline) {
    int course = discipline.getType() == Type::COMPULSORY ? discipline.getMinAvailableCourse() : this -> course;
    this -> disciplines.push_back(StudentDiscipline(discipline.getName(), 2, course));
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
    double sumGrades = 0;
    for (StudentDiscipline d : disciplines) {
        sumGrades += d.grade;
    }
    this -> average_grade = sumGrades/disciplines.size();
}

bool Student::isDisciplineEnrolled (std::string discipline) {
    int specialtyIndex = SpecialtyList::findSpecialty(specialty);
    int disciplineIndex = SpecialtyList::findDisciplineInSpecialty(specialty, discipline);
    for (StudentDiscipline d : disciplines) {
        if (StringHelper::toLowerCase(d.discipline) == StringHelper::toLowerCase(discipline) && SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].checkMatchCurrentCourse(d.enrolledCourse)) return true;
    }
    return false;
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

    int disciplinesSize = this -> disciplines.size();
    out.write(reinterpret_cast<char*>(&disciplinesSize), sizeof(disciplinesSize));
    for(int i = 0; i < disciplinesSize; i++) {
        int disciplineNameSize = disciplines[i].discipline.length();
        out.write(reinterpret_cast<char*>(&disciplineNameSize), sizeof(disciplineNameSize));
        out.write(disciplines[i].discipline.c_str(), sizeof(char)*disciplineNameSize);

        out.write(reinterpret_cast<char*>(&disciplines[i].grade), sizeof(disciplines[i].grade));
        out.write(reinterpret_cast<char*>(&disciplines[i].enrolledCourse), sizeof(disciplines[i].enrolledCourse));
    }
}

void Student::read(std::ifstream& in) {
    int nameSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    name.resize(nameSize);
    in.read(&name[0], sizeof(char)*nameSize);

    in.read(reinterpret_cast<char*>(&faculty_number), sizeof(faculty_number));
    in.read(reinterpret_cast<char*>(&course), sizeof(course));

    int specialtyNameSize;
    in.read(reinterpret_cast<char*>(&specialtyNameSize), sizeof(specialtyNameSize));
    specialty.resize(specialtyNameSize);
    in.read(&specialty[0], sizeof(char)*specialtyNameSize);

    in.read(reinterpret_cast<char*>(&group), sizeof(group));
    in.read(reinterpret_cast<char*>(&status), sizeof(status));
    in.read(reinterpret_cast<char*>(&average_grade), sizeof(average_grade));

    int disciplinesSize;
    in.read(reinterpret_cast<char*>(&disciplinesSize), sizeof(disciplinesSize));
    for(int i = 0; i < disciplinesSize; i++) {
        std::string disciplineName;
        int disciplineNameSize, disciplineEnrolledCourse;
        double disciplineGrade;

        in.read(reinterpret_cast<char*>(&disciplineNameSize), sizeof(disciplineNameSize));
        disciplineName.resize(disciplineNameSize);
        in.read(&disciplineName[0], sizeof(char)*disciplineNameSize);

        in.read(reinterpret_cast<char*>(&disciplineGrade), sizeof(disciplineGrade));
        in.read(reinterpret_cast<char*>(&disciplineEnrolledCourse), sizeof(disciplineEnrolledCourse));

        int specialtyIndex = SpecialtyList::findSpecialty(specialty);
        this -> disciplines.push_back(StudentDiscipline(disciplineName, disciplineGrade, disciplineEnrolledCourse));
    }
}

std::ostream& operator << (std::ostream& out, const Student& other) {
    out << "Name: " << other.name << std::endl
        << "Faculty number: " << other.faculty_number << std::endl
        << "Course: " << other.course << std::endl 
        << "Specialty: " << other.specialty << std::endl
        << "Group: " << other.group << std::endl
        << "Status: " << EnumConvertions::getStudentStatus(other.status) << std::endl
        << "\nDisciplines:\n";

    for (int i = 0; i < other.disciplines.size(); i++) {
        int disciplineIndex = SpecialtyList::findDisciplineInSpecialty(other.specialty, other.disciplines[i].discipline);
        int specialtyIndex = SpecialtyList::findSpecialty(other.specialty);
        out << i+1 << ": " 
            << other.disciplines[i].discipline << " - "
            << "Type: " << EnumConvertions::getType(SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].getType())
            << ", Course: " << other.disciplines[i].enrolledCourse
            << ", Grade: " << other.disciplines[i].grade
            << std::endl;
    }
    return out;
}