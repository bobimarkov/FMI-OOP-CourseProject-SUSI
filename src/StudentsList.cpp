#include "StudentsList.hpp"
#include "SpecialtyList.hpp"
#include "StringHelper.hpp"

std::vector<Student> StudentsList::students;

bool StudentsList::checkAlreadyEnrolledDiscipline(int studentIndex, int specialtyIndex, int disciplineIndex) {
    for (StudentDiscipline d : students[studentIndex].getDisciplines()) {
        if (SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].getName() == d.discipline) return true;
    }
    return false;
}

bool StudentsList::alreadyEnrolledFN(int fn) { 
    for(Student s : students) {
        if (fn == s.getFN()) return true;
    }
    return false;
}

int StudentsList::findFN(int fn) {
    for(int i = 0; i < students.size(); i++) {
        if (students[i].getFN() == fn) return i;
    }
    return -1;
} 

int StudentsList::findStudentDisciplineIndex(int studentIndex, std::string discipline) {
    if (StringHelper::isNumber(discipline)) if(std::atoi(discipline.c_str()) > 0 && std::atoi(discipline.c_str()) <= students[studentIndex].getDisciplines().size()) return std::atoi(discipline.c_str())-1;
    for (int i = 0; i < students[studentIndex].getDisciplines().size(); i++) {
        if (StringHelper::toLowerCase(students[studentIndex].getDisciplines()[i].discipline) == StringHelper::toLowerCase(discipline)) return i;
    }
    return -1;
}