#include "SpecialtyList.hpp"
#include "Specialty.hpp"
#include "Student.hpp"
#include "Discipline.hpp"
#include "Helpers/StringHelper.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>

#define SH StringHelper

struct fileHeader {
    char fileLetters[3];
    int authID;
    int fileCode;
    long long classCode;
};

bool checkHeader (fileHeader restored) {
    return strcmp(restored.fileLetters,"SP") == 0 && restored.authID == 13094 && restored.fileCode == 201 && restored.classCode == 974;
}

std::vector<Specialty> SpecialtyList::specialties{Specialty()};

int SpecialtyList::findSpecialty(std::string str) {
    if(SH::isNumber(str) && std::atoi(str.c_str()) < SpecialtyList::specialties.size() && std::atoi(str.c_str()) >= 0) return std::atoi(str.c_str());
    for (int i = 0; i < SpecialtyList::specialties.size(); i++) {
        if (SH::toLowerCase(str) == SH::toLowerCase(specialties[i].getName())) return i;
    }
    return 0;
}

int SpecialtyList::findDisciplineInSpecialty(std::string specialty, std::string discipline) {
    size_t specialtyIndex = findSpecialty(specialty);
    if (SH::isNumber(discipline) && std::atoi(discipline.c_str()) > 0 && std::atoi(discipline.c_str()) <= specialties[specialtyIndex].getAvailableDisciplines().size()) return std::atoi(discipline.c_str())-1;
    for (size_t i = 0; i < specialties[specialtyIndex].getAvailableDisciplines().size(); i++) {
        if (SH::toLowerCase(discipline) == SH::toLowerCase(specialties[specialtyIndex].getAvailableDisciplines()[i].getName())) return i;
    }
    return -1;
}

int SpecialtyList::countNotPassedMutualCompDisciplines(Student& st, int otherSpecialtyIndex) {
    int countAllPreviousCompDisciplines = 0, countPassedPreviousCompDisciplines = 0;
    int studentSpecialtyIndex = findSpecialty(st.getSpecialty());
    for(StudentDiscipline d : st.getDisciplines()) {
        int disciplineIndex = findDisciplineInSpecialty(st.getSpecialty(), d.discipline);
        for(Discipline a : specialties[otherSpecialtyIndex].getAvailableDisciplines()) {
            if(d.discipline == a.getName() && d.enrolledCourse < st.getCourse() && a.getMinAvailableCourse() < st.getCourse() && a.getType() == Type::COMPULSORY && specialties[studentSpecialtyIndex].getAvailableDisciplines()[disciplineIndex].getType() == Type::COMPULSORY) {
                countAllPreviousCompDisciplines++;
                if(d.grade >= 3) countPassedPreviousCompDisciplines++;
            }
        }
    }
    return countAllPreviousCompDisciplines - countPassedPreviousCompDisciplines;
}

bool containsInVector(std::vector<int> vector, int element) {
    for (int i : vector) {
        if (i == element) return true;
    }
    return false;
}

std::vector<int> splitAvailableCourses (std::string availableCourses) {
    availableCourses = SH::strip(SH::stripBegin(SH::clearAllConsecutiveSpaces(availableCourses)));
    std::string* splittedCourses = SH::split(availableCourses);
    int countCourses = SH::count(availableCourses, ' ') + 1;
    
    std::vector<int> courses;

    for (int i = 0; i < countCourses; i++) {
        if (SH::isNumber(splittedCourses[i]) && !containsInVector(courses, std::atoi(splittedCourses[i].c_str()))) courses.push_back(std::atoi(splittedCourses[i].c_str()));
    }

    delete[] splittedCourses;
    std::sort(courses.begin(), courses.end());
    return courses;
}

void SpecialtyList::addSpecialty (std::string specialtyName, double minCredits) {
    specialtyName = SH::strip(SH::stripBegin(specialtyName));
    for(Specialty sp : specialties) {
        if(SH::toLowerCase(specialtyName) == SH::toLowerCase(sp.getName())) {
            std::cerr << "A specialty with this name already exists!\n";
            return;
        }
    }
    specialties.push_back(Specialty(specialtyName, minCredits));
    std::cout << "Specialty \"" << specialties.back().getName() << "\" added successfully!\n";
}

void SpecialtyList::removeSpecialty (std::string specialtyName) {
    specialtyName = SH::strip(SH::stripBegin(specialtyName));
    int specialtyIndex = findSpecialty(specialtyName);

    if (!(specialtyIndex >= 1 && specialtyIndex < specialties.size())) {
        std::cerr << "Invalid specialty!\n";
        return;
    }

    std::cout << "Specialty \"" << specialties[specialtyIndex].getName() << "\" removed successfully!\n";
    specialties.erase(specialties.begin()+specialtyIndex);
}

void SpecialtyList::addDiscipline (std::string specialtyName, std::string disciplineName, std::string typeName, std::string availableFor, double credits) {
    
    specialtyName = SH::strip(SH::stripBegin(specialtyName));
    int specialtyIndex = findSpecialty(specialtyName);
    if (!(specialtyIndex >= 1 && specialtyIndex < specialties.size())) {
        std::cerr << "Invalid specialty!\n";
        return;
    }

    disciplineName = SH::strip(SH::stripBegin(disciplineName));

    typeName = SH::strip(SH::stripBegin(typeName));
    Type t = EnumConvertions::stringToType(typeName);
    if (t == Type::UNKNOWN) {
        std::cerr << "Invalid type!\n";
        return;
    }

    std::vector<int> courses = splitAvailableCourses(availableFor);

    specialties[specialtyIndex].addDiscipline(Discipline(disciplineName,t,courses,credits));
    std::cout << "Discipline \"" << specialties[specialtyIndex].getAvailableDisciplines().back().getName() << "\" was added successfully to specialty \"" << specialties[specialtyIndex].getName() << "\"!\n";
    std::sort(specialties[specialtyIndex].getAvailableDisciplines().begin(), specialties[specialtyIndex].getAvailableDisciplines().end(), [](Discipline x, Discipline y){return x.getMinAvailableCourse() < y.getMinAvailableCourse();});
}

void SpecialtyList::removeDiscipline (std::string specialtyName, std::string disciplineName) {
    specialtyName = SH::strip(SH::stripBegin(specialtyName));
    int specialtyIndex = findSpecialty(specialtyName);
    if (!(specialtyIndex >= 1 && specialtyIndex < specialties.size())) {
        std::cerr << "Invalid specialty!\n";
        return;
    }
    disciplineName = SH::strip(SH::stripBegin(disciplineName));
    int disciplineIndex = findDisciplineInSpecialty(specialtyName, disciplineName);
    if (!(disciplineIndex >= 0 && disciplineIndex < specialties[specialtyIndex].getAvailableDisciplines().size())) {
        std::cerr << "Invalid discipline!\n";
        return;
    }
    std::cout << "Discipline \"" << specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].getName() << "\" was removed successfully from specialty \"" << specialties[specialtyIndex].getName() << "\"!\n";
    specialties[specialtyIndex].getAvailableDisciplines().erase(specialties[specialtyIndex].getAvailableDisciplines().begin()+disciplineIndex);
}

void SpecialtyList::listSpecialties() {
    for(int i = 1; i < specialties.size(); i++) {
        std::cout << i << ": " << specialties[i].getName() << " (Minimal credits: " << specialties[i].getMinCredits() << ")" << std::endl;
    }
}

void SpecialtyList::listDisciplines(std::string specialtyName) {
    specialtyName = SH::strip(SH::stripBegin(specialtyName));
    int specialtyIndex = findSpecialty(specialtyName);
    if (!(specialtyIndex >= 1 && specialtyIndex < specialties.size())) {
        std::cerr << "Invalid name or id!\n";
        return;
    }
    specialties[specialtyIndex].printAvailableDisciplines();
}

void SpecialtyList::listAll () {
    for(int i = 1; i < specialties.size(); i++) {
        std::cout << "\n-------------------------------\n\n" << specialties[i] << std::endl;
    }
}

void SpecialtyList::loadSpecialties() {
    std::ifstream in("specialties.susi", std::ios::binary | std::ios::in);
    
    if(!in.is_open()) {
        std::cerr << "Warning: File specialties.susi doesn't exist! Check 'help' to see how you can add new specialty and discipline.\n";
        return;
    }

    in.seekg(0, std::ios::end);
    std::streampos size = in.tellg();
    in.seekg(0, std::ios::beg);

    //Header
    fileHeader header;
    in.read(reinterpret_cast<char*>(&header),sizeof(header));
    if(!checkHeader(header)) {
        std::cerr << "Invalid specialties.susi imported!\n";
        in.close();
        return;
    }

    while(in.tellg() != size && !in.eof()) {
        Specialty sp;
        sp.read(in);
        specialties.push_back(sp);
    }

    in.close();
}

void SpecialtyList::writeSpecialties() {
    std::ofstream out("specialties.susi", std::ios::binary | std::ios::out);
    //Header
    fileHeader realHeader{"SP", 13094, 201, 974};
    out.write(reinterpret_cast<char*>(&realHeader), sizeof(realHeader));

    //Specialties
    for (int i = 1; i < specialties.size(); i++) {
        specialties[i].write(out);
    }
    std::cout << "specialties.susi has been written successfully!\n";
    out.close();
}