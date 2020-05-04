#include "Discipline.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

Discipline::Discipline(): name(""), type(Type::UNKNOWN), specialty(Specialty::UNKNOWN), availableForCourse(0), grade(0), credits(0) {

}

Discipline::Discipline(const Discipline& other) {
    this -> name = other.name;
    this -> type = other.type;
    this -> specialty = other.specialty;
    this -> availableForCourse = other.availableForCourse;
    this -> grade = other.grade;
    this -> credits = other.credits;
}

Discipline::Discipline(std::string name, Type type, Specialty specialty, int availableForCourse, double grade, double credits): name(name), type(type), specialty(specialty), availableForCourse(availableForCourse), grade(grade), credits(credits) {

}

Discipline& Discipline::operator = (const Discipline& other) {
    if (this == &other) return *this;
    this -> name = other.name;
    this -> type = other.type;
    this -> specialty = other.specialty;
    this -> availableForCourse = other.availableForCourse;
    this -> grade = other.grade;
    this -> credits = other.credits;
    return *this;
}

std::ostream& operator << (std::ostream& out, const Discipline& other) {
    out << other.name << std::endl
        << EnumConvertions::getType(other.type) << std::endl
        << EnumConvertions::getSpecialty(other.specialty) << std::endl
        << other.availableForCourse << std::endl
        << other.grade << std::endl
        << other.credits << std::endl;
    return out;
}