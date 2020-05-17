#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

#include "Student.hpp"
#include "Specialty.hpp"
#include "SpecialtyList.hpp"
#include "Discipline.hpp"
#include "Helpers/StringHelper.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#define SH StringHelper 

bool openedFile = false;
std::string openedFilePath;
std::string fileName;

std::vector<Student> students;

struct fileHeader {
    char fileLetters[3];
    int authID;
    int fileCode;
    long long classCode;
};

bool checkHeaderStudent (fileHeader restored) {
    return strcmp(restored.fileLetters,"ST") == 0 && restored.authID == 13094 && restored.fileCode == 202 && restored.classCode == 890;
}


bool checkAlreadyEnrolledDiscipline(int studentIndex, int specialtyIndex, int disciplineIndex) {
    for (StudentDiscipline d : students[studentIndex].getDisciplines()) {
        if (SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].getName() == d.discipline) return true;
    }
    return false;
}

bool alreadyEnrolledFN(int fn) { 
    for(Student s : students) {
        if (fn == s.getFN()) return true;
    }
    return false;
}

int findFN(int fn) {
    for(int i = 0; i < students.size(); i++) {
        if (students[i].getFN() == fn) return i;
    }
    return -1;
} 

int findStudentDisciplineIndex(int studentIndex, std::string discipline) {
    if (SH::isNumber(discipline)) if(std::atoi(discipline.c_str()) > 0 && std::atoi(discipline.c_str()) <= students[studentIndex].getDisciplines().size()) return std::atoi(discipline.c_str())-1;
    for (int i = 0; i < students[studentIndex].getDisciplines().size(); i++) {
        if (SH::toLowerCase(students[studentIndex].getDisciplines()[i].discipline) == SH::toLowerCase(discipline)) return i;
    }
    return -1;
}

void setFileName () {
    int countSlash = SH::count(openedFilePath,'/');
    int countBackslash = SH::count(openedFilePath, (char)92);

    if(countSlash == 0 && countBackslash == 0) fileName = openedFilePath;
    else if (countSlash > 0) {
        std::string* sepPath = SH::split(openedFilePath,'/');
        fileName = sepPath[countSlash];
    }
    else if (countBackslash > 0) {
        std::string* sepPath = SH::split(openedFilePath,(char)92);
        fileName = sepPath[countBackslash];
    }
}

void open(std::string filePath) {
    if(!openedFile) {
        std::ifstream in (filePath, std::ios::binary | std::ios::in);

        if(!in.is_open()) {
            std::ofstream out (filePath, std::ios::binary | std::ios::out);
            fileHeader realHeader{"ST", 13094, 202, 890};
            out.write(reinterpret_cast<char*>(&realHeader), sizeof(realHeader));
            out.close();    
            in.open(filePath, std::ios::binary | std::ios::in);
        }

        if(!in.is_open()) {
            std::cout << "Error: Invalid file path!\n";
            return;
        }

        in.seekg(0,std::ios::end);
        std::streampos size = in.tellg();
        in.seekg(0,std::ios::beg);

        fileHeader header;
        in.read(reinterpret_cast<char*>(&header),sizeof(header));
        if(!checkHeaderStudent(header)) {
            std::cerr << "Invalid file opened!\n";
            in.close();
            return;
        }

        openedFile = true;
        openedFilePath = filePath;
        setFileName();
        
        while(in.tellg() != size && !in.eof()) {
            Student s;
            s.read(in);
            students.push_back(s);
        }

        std::cout << "Successfully opened " << fileName << "!\n";
        in.close();
    }
    else std::cerr << "There is already an opened file! You can close the file with 'close'\n"; 
}

void close() {
    if(openedFile) {
        openedFile = false;
        students.clear();
        std::cout << "Successfully closed " << fileName << "!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void saveas(std::string filePath) {
    if (openedFile) {
        std::ofstream out(filePath, std::ios::binary | std::ios::out);

        if (!out.is_open()) {
            std::cerr << "Error: Invalid file path!\n";
            return;
        }

        fileHeader realHeader{"ST", 13094, 202, 890};
        out.write(reinterpret_cast<char*>(&realHeader), sizeof(realHeader));


        openedFilePath = filePath;
        setFileName();

        for(Student s: students) {
            s.write(out);
        }

        std::cout << "Successfully saved " << fileName << "!\n"; 

        out.close();
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void save() {
    if(openedFile) saveas(openedFilePath);
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void help(){
    std::cout << "Available commands:\n"
              << "help - shows all available commands\n"
              << "open <file path> - opens <file path>\n"
              << "exit - exits the program\n"
              << "\nThe following commands work only when a file is opened:\n"
              << "close - closes the current opened file\n"
              << "saveas <file path> - saves the current opened file in <file path>\n"
              << "save - saves the current opened file\n"
              << "enroll <faculty number> <specialty id/name> <group> <student name> - enrolls a student\n"
              << "advance <faculty number> - advance a student\n"
              << "change <faculty number> <option> <value> - transfer a student to a different specialty/group/year\n"
              << "graduate <faculty number> - a student will graduate if he has passed all his exams\n"
              << "interrupt <faculty number> - interrupts a student\n"
              << "resume <faculty number> - resumes a student\n"
              << "print <faculty number> - prints information about a student\n"
              << "printall <specialty name/id> <course> - prints information about all students in given specialty and course\n"
              << "enrollin <faculty number> <discipline name/id> - enrolls a student in given discipline if he meets the requirements\n"
              << "addgrade <faculty number> <discipline name/id> <grade> - adds a grade to given discipline\n"
              << "protocol <discipline name> - prints protocols of all students enrolled in a given discipline\n"
              << "report <faculty number> - prints academic report of a student's grades\n"
              << "\nAdministrator commands:\n"
              << "add specialty - adds a specialty\n"
              << "remove specialty - removes a specialty\n"
              << "add discipline - adds a discipline to an existing specialty\n"
              << "remove discipline - removes a discipline from an existing specialty\n" 
              << "list specialties - prints all available specialties\n"
              << "list disciplines - prints all available disciplines for a specific specialty\n"
              << "list all - prints all specialties along with the registered disciplines for each specialty\n"
              << "savespecialties - save all registered specialties along with the disciplines for each specialty in specialties.susi\n";
              
}

void exit(){
    std::cout << "Exiting the program...\n";
    std::exit(0);
}

void enroll(int fn, std::string specialty, int group, std::string name) {
    if(openedFile) {
        int specialtyIndex = SpecialtyList::findSpecialty(specialty);
        std::string spName = SpecialtyList::specialties[specialtyIndex].getName();
        if (alreadyEnrolledFN(fn)) {
            std::cerr << "A student with this faculty number already exists!\n";
            return;
        }
        if (spName == "UNKNOWN") std::cerr << "Invalid specialty\n";
        else {
            students.push_back(Student(fn, spName, group, name));

            for (int i = 0; i < SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines().size(); i++) {
                Discipline currentDiscipline = SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[i];
                if (currentDiscipline.getType() == Type::COMPULSORY) students.back().addDiscipline(currentDiscipline);
            }
            std::sort(students.begin(), students.end(), [] (Student x, Student y) {return x.getFN() < y.getFN();});
            std::cout << "The student is enrolled successfully!\n";
        }
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void advance(int fn) {
    if(openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    students[studentIndex].advance();
                    std::cout << "The student has been advanced successfully!\n";
                }
                else std::cerr << "You cannot do that while the student is interrupted!\n";
            }
            else std::cerr << "You can't advance a graduated student!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void change(int fn, std::string option, std::string value) {
    if(openedFile) {
        if(alreadyEnrolledFN(fn)) {     
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    if (SH::toLowerCase(option) == "specialty") {
                        int specialtyIndex = SpecialtyList::findSpecialty(value);
                        if (specialtyIndex != 0) {
                            if(SpecialtyList::countNotPassedMutualCompDisciplines(students[studentIndex],specialtyIndex) == 0) {
                                students[studentIndex].setSpecialty(SpecialtyList::specialties[specialtyIndex].getName());
                                std::cout << "The student's specialty was changed successfully!\n";
                            }
                            else std::cerr << "The student doesn't meet the requierements to change the specialty!\n";
                        }
                        else std::cerr << "Not valid value!\n";
                    }
                    else if (SH::toLowerCase(option) == "course") {
                        if (SH::isNumber(value)) {
                            if(students[studentIndex].getCourse() == std::atoi(value.c_str()) - 1 && SpecialtyList::countNotPassedMutualCompDisciplines(students[studentIndex], SpecialtyList::findSpecialty(students[studentIndex].getSpecialty()) <= 2)) {
                                std::cout << "The student's course was changed successfully!\n";
                                students[studentIndex].advance();
                            }
                            else std::cerr << "The student doesn't meet the requierements to change the course!\n";
                        }
                        else std::cerr << "Not valid value!\n";
                    }
                    else if (SH::toLowerCase(option) == "group") {
                        if (SH::isNumber(value)) {
                            int group = std::atoi(value.c_str());
                            if (group > 0) {
                                students[studentIndex].setGroup(group);
                                std::cout << "The student's group was changed successfully!\n";
                            }
                            else std::cerr << "Not valid value!\n";
                        }
                        else std::cerr << "Not valid value!\n";
                    }
                    else std::cerr << "Invalid option!\n";
                }
                else std::cerr << "You can't change the information of a interrupted student!\n";
            }
            else std::cerr << "You can't change the information of a graduated student!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void graduate(int fn) {
    if(openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    int countPassedExams = 0;
                    for (StudentDiscipline d : students[studentIndex].getDisciplines()) {
                        if (d.grade >= 3) countPassedExams++;
                    }
                    if (countPassedExams == students[studentIndex].getDisciplines().size()) {
                        double creditsDiff = SpecialtyList::specialties[SpecialtyList::findSpecialty(students[studentIndex].getSpecialty())].getMinCredits() - students[studentIndex].countCredits();
                        if (creditsDiff > 0) {
                            std::cerr << "The student needs " << creditsDiff << " credits to graduate!\n";
                            return; 
                        }
                        students[studentIndex].changeStatus(Student_Status::GRADUATED);
                        std::cout << "The student graduated successfully!\n";
                    }
                    else std::cerr << "This student hasn't passed all his exams!\n";
                }
                else std::cerr << "An interrupted student can't graduate!\n";
            }
            else std::cerr << "This student is already graduated!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void interrupt(int fn) {
    if(openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() == Student_Status::SIGNED) {
                    students[studentIndex].changeStatus(Student_Status::INTERRUPTED);
                    std::cout << "The student has been interrupted successfully!\n";
                }
                else std::cerr << "The student is already interrupted!\n";
            }
            else std::cerr << "You can't interrupt a graduated student!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void resume(int fn) {
    if(openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() == Student_Status::INTERRUPTED) {
                    students[studentIndex].changeStatus(Student_Status::SIGNED);
                    std::cout << "The student has been resumed successfully!\n";
                }
                else std::cerr << "The student isn't interrupted!\n";
            }
            else std::cerr << "You can't resume a graduated student!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void print(int fn) {
    if(openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            std::cout << students[studentIndex] << std::endl;
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void printall(std::string specialty, int course) {
    if(openedFile) {
        int specialtyIndex = SpecialtyList::findSpecialty(specialty);
        if(specialtyIndex == 0) {
            std::cerr << "Invalid specialty!\n";
            return;
        }
        if(course < 1) {
            std::cerr << "Invalid course!\n";
            return;
        }
        for (Student s : students) {
            if(specialtyIndex == SpecialtyList::findSpecialty(s.getSpecialty()) && course == s.getCourse()) std::cout << s << std::endl;
        }
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void enrollin(int fn, std::string discipline) {
    if (openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    int specialtyIndex = SpecialtyList::findSpecialty(students[studentIndex].getSpecialty());
                    int disciplineIndex = SpecialtyList::findDisciplineInSpecialty(students[studentIndex].getSpecialty(), discipline);
                    if (disciplineIndex == -1) {
                        std::cerr << "This discipline doesn't exist!\n";
                        return;
                    }
                    if (checkAlreadyEnrolledDiscipline(studentIndex, specialtyIndex, disciplineIndex)) {
                        std::cerr << "This discipline is already enrolled for this student!\n";
                        return;
                    }
                    if(SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].checkMatchCurrentCourse(students[studentIndex].getCourse())) {
                        students[studentIndex].addDiscipline(SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex]);
                        std::cout << "The discipline was added successfully!\n";
                    }   
                    else std::cerr << "The student doesn't meet the requierements to add this discipline!\n";
                }
                else std::cerr << "You can't enroll an interrupted student to a discipline!\n";
            }
            else std::cerr << "You can't enroll a graduated student to a discipline!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void addgrade(int fn, std::string discipline, double grade) {
    if (openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    int specialtyIndex = SpecialtyList::findSpecialty(students[studentIndex].getSpecialty());
                    int studentDisciplineIndex = findStudentDisciplineIndex(studentIndex,discipline);
                    if (studentDisciplineIndex == -1) {
                        std::cerr << "This discipline doesn't exist!\n";
                        return;
                    }
                    if (grade >= 2 && grade <=6) {
                        std::cout << "The grade was added successfully!\n";
                       students[studentIndex].getDisciplines()[studentDisciplineIndex].grade = grade;
                    }
                    else std::cerr << "Invalid grade!\n";
                }
                else std::cerr << "You can't add grade to an interrupted student!\n";
            }
            else std::cerr << "You can't add grade to a graduated student!\n";
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void protocol(std::string discipline) {
    if (openedFile) {
        std::cout << "\nDiscipline \"" << discipline << "\":\n";
        for(int i = 1; i < SpecialtyList::specialties.size(); i++) {
            int disciplineIndex = SpecialtyList::findDisciplineInSpecialty(std::to_string(i), discipline);
            if (disciplineIndex == -1) continue;
            std::cout << "\nSpecialty \"" << SpecialtyList::specialties[i].getName() << "\":\n";
            for(int c : SpecialtyList::specialties[i].getAvailableDisciplines()[disciplineIndex].getAvailableForCourses()) {
                std::cout << "Course: " << c << std::endl;
                for(Student s : students) {
                    if (s.getSpecialty() == SpecialtyList::specialties[i].getName() && s.isDisciplineEnrolled(discipline) && s.getDisciplineEnrolledCourse(discipline) == c) {
                        std::cout << s.getName() << " - FN: " << s.getFN() << ", Group: " << s.getGroup() << std::endl; 
                    }       
                }
            }
        }
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void report(int fn) {
    if (openedFile) {
        if(alreadyEnrolledFN(fn)) {
            int studentIndex = findFN(fn);
            students[studentIndex].calculateAvgGrade();
            double creditsDiff = SpecialtyList::specialties[SpecialtyList::findSpecialty(students[studentIndex].getSpecialty())].getMinCredits() - students[studentIndex].countCredits();
            std::cout << "\nStudent name: " << students[studentIndex].getName() << std::endl
                      << "Average grade: " << students[studentIndex].getAverageGrade() << std::endl
                      << "Credits left: " << creditsDiff << std::endl;

            std::cout << "\nPassed disciplines:\n";
            for (int i = 0; i < students[studentIndex].getDisciplines().size(); i++) {
                if (students[studentIndex].getDisciplines()[i].grade >= 3) {
                    std::cout << "- " << students[studentIndex].getDisciplines()[i].discipline << " - "
                    << "Grade: " << students[studentIndex].getDisciplines()[i].grade
                    << std::endl;
                }
            }
            
            std::cout << "\nNot passed disciplines:\n";
            for (int i = 0; i < students[studentIndex].getDisciplines().size(); i++) {
                if (students[studentIndex].getDisciplines()[i].grade < 3) {
                    std::cout << "- " << students[studentIndex].getDisciplines()[i].discipline << " - "
                    << "Grade: " << students[studentIndex].getDisciplines()[i].grade
                    << std::endl;
                }
            }
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
} 

int main () {
    SpecialtyList::loadSpecialties();
    std::cout << "Welcome to SUSI!\n";
    while(true) {
        std::string command;
        std::cout << "\n> "; 
        getline(std::cin, command);
        command = SH::clearAllConsecutiveSpacesQ(SH::stripBegin(SH::strip(command)));
        std::string* separatedCommand = SH::splitQ(command);
        int commandArguments = SH::countQ(command, ' ') + 1;

        if (SH::toLowerCase(separatedCommand[0]) == "exit") {
           if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else {
                delete[] separatedCommand;
                exit();
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "open") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                separatedCommand[1] = SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"');
                open(separatedCommand[1]);
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "close") {
            if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else close();
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "help") {
            if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else help();
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "save") {
            if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else save();
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "saveas") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                separatedCommand[1] = SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"');
                saveas(separatedCommand[1]);
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "print") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) print(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "printall") {
            if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[2])) printall(SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"'), std::atoi(separatedCommand[2].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "enroll") {
            if (commandArguments > 5) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 5) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1]) && SH::isNumber(separatedCommand[3])) {
                    enroll(std::atoi(separatedCommand[1].c_str()),SH::strip(SH::stripBegin(separatedCommand[2],'"'),'"'),std::atoi(separatedCommand[3].c_str()),SH::strip(SH::stripBegin(separatedCommand[4], '"'),'"'));
                }
                else std::cerr << "Invalid arguments!\n";
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "advance") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) advance(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "graduate") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) graduate(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "change") {
            if (commandArguments > 4) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 4) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) change(std::atoi(separatedCommand[1].c_str()), SH::strip(SH::stripBegin(separatedCommand[2],'"'),'"'), SH::strip(SH::stripBegin(separatedCommand[3],'"'),'"'));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "interrupt") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) interrupt(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "resume") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) resume(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "enrollin") {
            if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) enrollin(std::atoi(separatedCommand[1].c_str()),SH::strip(SH::stripBegin(separatedCommand[2],'"'),'"'));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "addgrade") {
            if (commandArguments > 4) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 4) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1]) && SH::isNumber(separatedCommand[3])) addgrade(std::atoi(separatedCommand[1].c_str()), SH::strip(SH::stripBegin(separatedCommand[2],'"'),'"'), std::stod(separatedCommand[3]));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "protocol") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else protocol(SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"'));
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "report") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) report(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        //Раздел Администратор
        else if (SH::toLowerCase(separatedCommand[0]) == "add") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::toLowerCase(separatedCommand[1]) == "specialty") SpecialtyList::addSpecialty();
                else if (SH::toLowerCase(separatedCommand[1]) == "discipline") SpecialtyList::addDiscipline();
                else std::cerr << "Invalid argument!\n";
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "remove") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::toLowerCase(separatedCommand[1]) == "specialty") SpecialtyList::removeSpecialty();
                else if (SH::toLowerCase(separatedCommand[1]) == "discipline") SpecialtyList::removeDiscipline();
                else std::cerr << "Invalid argument!\n";
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "list") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::toLowerCase(separatedCommand[1]) == "specialties") SpecialtyList::listSpecialties();
                else if (SH::toLowerCase(separatedCommand[1]) == "disciplines") SpecialtyList::listDisciplines();
                else if (SH::toLowerCase(separatedCommand[1]) == "all") SpecialtyList::listAll();
                else std::cerr << "Invalid argument!\n";
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "savespecialties") {
            SpecialtyList::writeSpecialties();
        }
        else if(!command.empty()) std::cerr << "Invalid command! Please type 'help' to see all available commands!\n";
        
        delete[] separatedCommand;
    }
    return 0;
}