#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
    std::ifstream in (filePath, std::ios::binary | std::ios::in);

    if(!in.is_open()) {
        //Ще създаде нов файл
        std::ofstream out (filePath, std::ios::binary | std::ios::out);
        out.close();
        in.open(filePath, std::ios::binary | std::ios::in);
    }

    if(!in.is_open()) {
        std::cout << "Error: Invalid file path!\n";
        return;
    }

    openedFile = true;
    openedFilePath = filePath;
    setFileName();

    in.seekg(0,std::ios::end);
    std::streampos size = in.tellg();
    in.seekg(0,std::ios::beg);
    
    while(in.tellg() != size && !in.eof()) {
        Student s;
        s.read(in);
        students.push_back(s);
    }

    std::cout << "Successfully opened " << fileName << "!\n";
    in.close();
}

void close() {
    openedFile = false;
    students.clear();
    std::cout << "Successfully closed " << fileName << "!\n";
}

void saveas(std::string filePath) {
    std::ofstream out(filePath, std::ios::binary | std::ios::out);

    if (!out.is_open()) {
        std::cerr << "Error: Invalid file path!\n";
        return;
    }

    openedFilePath = filePath;
    setFileName();

    for(Student s: students) {
        s.write(out);
    }

    std::cout << "Successfully saved " << fileName << "!\n"; 

    out.close();
}

void save() {
    saveas(openedFilePath);
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
              << "\nAdministrator commands:\n"
              << "add specialty - adds a specialty\n"
              << "remove specialty - removes a specialty\n"
              << "add discipline - adds a discipline to an existing specialty\n"
              << "remove discipline - removes a discipline from an existing specialty\n" 
              << "list specialties - prints all available specialties\n"
              << "list disciplines - prints all available disciplines for a specific specialty\n"
              << "list all - prints all specialties along with the registered disciplines for each specialty\n"
              << "savespecialties - save all registered specialties along with the disciplines for each specialty in specialties.dat\n";
              
}

void exit(){
    std::cout << "Exiting the program...\n";
    std::exit(0);
}

void enroll(int fn, std::string specialty, int group, std::string name) {
    if(openedFile) {
        std::string spName = SpecialtyList::specialties[SpecialtyList::findSpecialty(specialty)].getName();
        if (alreadyEnrolledFN(fn)) {
            std::cerr << "A student with this faculty number already exists!\n";
            return;
        }
        if (spName == "UNKNOWN") std::cerr << "Invalid specialty\n";
        else {
            students.push_back(Student(fn, spName, group, name));
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
                if(students[studentIndex].getStatus() == Student_Status::SIGNED) {
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
            size_t studentIndex = findFN(fn);
            if (students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if (SH::toLowerCase(option) == "specialty") {
                    if (SH::isNumber(value)) {
                        int specialtyIndex = std::stoi(value);
                        if (specialtyIndex < SpecialtyList::specialties.size() && specialtyIndex > 0) students[studentIndex].setSpecialty(SpecialtyList::specialties[specialtyIndex].getName());
                        else std::cerr << "Not valid value!\n";
                    }
                    else {
                        size_t specialtyIndex = SpecialtyList::findSpecialty(value);
                        if(specialtyIndex != 0) students[studentIndex].setSpecialty(SpecialtyList::specialties[specialtyIndex].getName());
                        else std::cerr << "Not valid value!\n";
                    }
                }
                else if (SH::toLowerCase(option) == "year") {

                }
                else if (SH::toLowerCase(option) == "group") {
                    if (SH::isNumber(value)) {
                        int group = std::stoi(value);
                        if (group > 0) students[studentIndex].setGroup(group);
                        else std::cerr << "Not valid value!\n";
                    }
                    else std::cerr << "Not valid value!\n";
                }
                else std::cerr << "Invalid option!\n";
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
                int countPassedExams = 0;
                for (Discipline d : students[studentIndex].getDisciplines()) {
                    if (d.getGrade() >= 3 && d.getHadExam()) countPassedExams++;
                }
                if (countPassedExams == students[studentIndex].getDisciplines().size()) {
                    students[studentIndex].changeStatus(Student_Status::GRADUATED);
                    std::cout << "The student graduated successfully!\n";
                }
                else {
                    std::cout << "This student hasn't passed all his exams!\n";
                }
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
void enrollin(){}
void addgrade(){}
void protocol(){}
void report(){} 

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
            delete[] separatedCommand;
            exit();
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "open") {
            separatedCommand[1] = SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"');
            if(!openedFile) open(separatedCommand[1]);
            else std::cerr << "There is already an opened file! You can close the file with 'close'\n"; 
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "close") {
            if(openedFile) close();
            else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "help") help();
        else if (SH::toLowerCase(separatedCommand[0]) == "save") {
            if(openedFile) save();
            else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "saveas") {
            separatedCommand[1] = SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"');
            if(openedFile) saveas(separatedCommand[1]);
            else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "print") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) print(std::stoi(separatedCommand[1]));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "printall") {
            if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[2])) printall(SH::strip(SH::stripBegin(separatedCommand[1],'"'),'"'), std::stoi(separatedCommand[2]));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "enroll") {
            if (commandArguments > 5) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 5) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1]) && SH::isNumber(separatedCommand[3])) {
                    enroll(std::stoi(separatedCommand[1]),SH::strip(SH::stripBegin(separatedCommand[2],'"'),'"'),std::stoi(separatedCommand[3]),SH::strip(SH::stripBegin(separatedCommand[4], '"'),'"'));
                }
                else std::cerr << "Invalid arguments!\n";
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "advance") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) advance(std::stoi(separatedCommand[1]));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "graduate") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) graduate(std::stoi(separatedCommand[1]));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "interrupt") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) interrupt(std::stoi(separatedCommand[1]));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "resume") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (SH::isNumber(separatedCommand[1])) resume(std::stoi(separatedCommand[1]));
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