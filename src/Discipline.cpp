#include "Discipline.hpp"

Discipline::Discipline(const Discipline& other) {
    this -> name = other.name;
    this -> type = other.type;
    this -> specialty = other.specialty;
    this -> availableForCourse = other.availableForCourse;
    this -> grade = other.grade;
}

Discipline::Discipline(std::string name, Type type, std::string specialty, int availableForCourse, double grade): name(name), type(type), specialty(specialty), availableForCourse(availableForCourse), grade(grade) {

}

Discipline& Discipline::operator = (const Discipline& other) {
    if (this == &other) return *this;
    this -> name = other.name;
    this -> type = other.type;
    this -> specialty = other.specialty;
    this -> availableForCourse = other.availableForCourse;
    this -> grade = other.grade;
    return *this;
}

std::ostream& operator << (std::ostream& out, const Discipline& other) {
    out << other.name << std::endl
        << static_cast<int>(other.type) << std::endl
        << other.specialty << std::endl
        << other.availableForCourse << std::endl
        << other.grade << std::endl;
    return out;
}