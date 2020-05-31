#include "CommandLine.hpp"
#include "Student.hpp"
#include "StudentsList.hpp"
#include "Specialty.hpp"
#include "SpecialtyList.hpp"
#include "Discipline.hpp"
#include "StringHelper.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>


bool CommandLine::openedFile = false;
std::string CommandLine::openedFilePath;
std::string CommandLine::fileName;

///Представлява header, който се записва в началото на бинарният файл.
///Целта е верифициране на файловете на самата програма.
struct fileHeader {
    ///2 букви, описващи типа на файла
    char fileLetters[3];
    ///Някакво ID, уж за автентификация
    int authID;
    ///Код на файла
    int fileCode;
    ///Код на класа
    long long classCode;
};

bool checkHeaderStudent (fileHeader restored) {
    return strcmp(restored.fileLetters,"ST") == 0 && restored.authID == 13094 && restored.fileCode == 202 && restored.classCode == 890;
}

void CommandLine::setFileName () {
    int countSlash = StringHelper::count(openedFilePath,'/');
    int countBackslash = StringHelper::count(openedFilePath, (char)92);

    if(countSlash == 0 && countBackslash == 0) fileName = openedFilePath;
    else if (countSlash > 0) {
        std::string* sepPath = StringHelper::split(openedFilePath,'/');
        fileName = sepPath[countSlash];
    }
    else if (countBackslash > 0) {
        std::string* sepPath = StringHelper::split(openedFilePath,(char)92);
        fileName = sepPath[countBackslash];
    }
}

void CommandLine::open(std::string filePath) {
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
            StudentsList::students.push_back(s);
        }

        std::cout << "Successfully opened " << fileName << "!\n";
        in.close();
    }
    else std::cerr << "There is already an opened file! You can close the file with 'close'\n"; 
}

void CommandLine::close() {
    if(openedFile) {
        openedFile = false;
        StudentsList::students.clear();
        std::cout << "Successfully closed " << fileName << "!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::saveas(std::string filePath) {
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

        for(Student s: StudentsList::students) {
            s.write(out);
        }

        std::cout << "Successfully saved " << fileName << "!\n"; 

        out.close();
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::save() {
    if(openedFile) saveas(openedFilePath);
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::help(){
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
              << "printall <specialty name/id> <course> - prints information about all StudentsList::students in given specialty and course\n"
              << "enrollin <faculty number> <discipline name/id> - enrolls a student in given discipline if he meets the requirements\n"
              << "addgrade <faculty number> <discipline name/id> <grade> - adds a grade to given discipline\n"
              << "protocol <discipline name> - prints protocols of all StudentsList::students enrolled in a given discipline\n"
              << "report <faculty number> - prints academic report of a student's grades\n"
              << "\nDatabase commands:\n"
              << "add specialty <specialty name> <minimal credits> - adds a specialty\n"
              << "remove specialty <specialty name/id> - removes a specialty\n"
              << "add discipline <specialty name/id> <discipline name> <compulsory/optional> <available for courses> <credits (OPTIONAL DISCIPLINES ONLY)> - adds a discipline to an existing specialty\n"
              << "remove discipline <specialty name/id> <discipline name/id> - removes a discipline from an existing specialty\n" 
              << "list specialties - prints all available specialties\n"
              << "list disciplines <specialty name/id> - prints all available disciplines for a specific specialty\n"
              << "list all - prints all specialties along with the registered disciplines for each specialty\n"
              << "savespecialties - save all registered specialties along with the disciplines for each specialty in specialties.susi\n";
              
}

void CommandLine::exit(){
    std::cout << "Exiting the program...\n";
    std::exit(0);
}

void CommandLine::enroll(int fn, std::string specialty, int group, std::string name) {
    if(openedFile) {
        int specialtyIndex = SpecialtyList::findSpecialty(specialty);
        std::string spName = SpecialtyList::specialties[specialtyIndex].getName();
        if (StudentsList::alreadyEnrolledFN(fn)) {
            std::cerr << "A student with this faculty number already exists!\n";
            return;
        }
        if (spName == "UNKNOWN") std::cerr << "Invalid specialty\n";
        else {
            StudentsList::students.push_back(Student(fn, spName, group, name));

            for (int i = 0; i < SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines().size(); i++) {
                Discipline currentDiscipline = SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[i];
                if (currentDiscipline.getType() == Type::COMPULSORY) StudentsList::students.back().addDiscipline(currentDiscipline);
            }
            std::sort(StudentsList::students.begin(), StudentsList::students.end(), [] (Student x, Student y) {return x.getFN() < y.getFN();});
            std::cout << "The student is enrolled successfully!\n";
        }
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::advance(int fn) {
    if(openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    StudentsList::students[studentIndex].advance();
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

void CommandLine::change(int fn, std::string option, std::string value) {
    if(openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {     
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    if (StringHelper::toLowerCase(option) == "specialty") {
                        int specialtyIndex = SpecialtyList::findSpecialty(value);
                        if (specialtyIndex != 0) {
                            if(SpecialtyList::countNotPassedMutualCompDisciplines(StudentsList::students[studentIndex],specialtyIndex) == 0) {
                                StudentsList::students[studentIndex].setSpecialty(SpecialtyList::specialties[specialtyIndex].getName());
                                std::cout << "The student's specialty was changed successfully!\n";
                            }
                            else std::cerr << "The student doesn't meet the requierements to change the specialty!\n";
                        }
                        else std::cerr << "Not valid value!\n";
                    }
                    else if (StringHelper::toLowerCase(option) == "course") {
                        if (StringHelper::isNumber(value)) {
                            if(StudentsList::students[studentIndex].getCourse() == std::atoi(value.c_str()) - 1 && SpecialtyList::countNotPassedMutualCompDisciplines(StudentsList::students[studentIndex], SpecialtyList::findSpecialty(StudentsList::students[studentIndex].getSpecialty()) <= 2)) {
                                std::cout << "The student's course was changed successfully!\n";
                                StudentsList::students[studentIndex].advance();
                            }
                            else std::cerr << "The student doesn't meet the requierements to change the course!\n";
                        }
                        else std::cerr << "Not valid value!\n";
                    }
                    else if (StringHelper::toLowerCase(option) == "group") {
                        if (StringHelper::isNumber(value)) {
                            int group = std::atoi(value.c_str());
                            if (group > 0) {
                                StudentsList::students[studentIndex].setGroup(group);
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

void CommandLine::graduate(int fn) {
    if(openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    int countPassedExams = 0;
                    for (StudentDiscipline d : StudentsList::students[studentIndex].getDisciplines()) {
                        if (d.grade >= 3) countPassedExams++;
                    }
                    if (countPassedExams == StudentsList::students[studentIndex].getDisciplines().size()) {
                        double creditsDiff = SpecialtyList::specialties[SpecialtyList::findSpecialty(StudentsList::students[studentIndex].getSpecialty())].getMinCredits() - StudentsList::students[studentIndex].countCredits();
                        if (creditsDiff > 0) {
                            std::cerr << "The student needs " << creditsDiff << " credits to graduate!\n";
                            return; 
                        }
                        StudentsList::students[studentIndex].changeStatus(Student_Status::GRADUATED);
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

void CommandLine::interrupt(int fn) {
    if(openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() == Student_Status::SIGNED) {
                    StudentsList::students[studentIndex].changeStatus(Student_Status::INTERRUPTED);
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

void CommandLine::resume(int fn) {
    if(openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() == Student_Status::INTERRUPTED) {
                    StudentsList::students[studentIndex].changeStatus(Student_Status::SIGNED);
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

void CommandLine::print(int fn) {
    if(openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            std::cout << StudentsList::students[studentIndex] << std::endl;
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::printall(std::string specialty, int course) {
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
        for (Student s : StudentsList::students) {
            if(specialtyIndex == SpecialtyList::findSpecialty(s.getSpecialty()) && course == s.getCourse()) std::cout << s << std::endl;
        }
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::enrollin(int fn, std::string discipline) {
    if (openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    int specialtyIndex = SpecialtyList::findSpecialty(StudentsList::students[studentIndex].getSpecialty());
                    int disciplineIndex = SpecialtyList::findDisciplineInSpecialty(StudentsList::students[studentIndex].getSpecialty(), discipline);
                    if (disciplineIndex == -1) {
                        std::cerr << "This discipline doesn't exist!\n";
                        return;
                    }
                    if (StudentsList::checkAlreadyEnrolledDiscipline(studentIndex, specialtyIndex, disciplineIndex)) {
                        std::cerr << "This discipline is already enrolled for this student!\n";
                        return;
                    }
                    if(SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex].checkMatchCurrentCourse(StudentsList::students[studentIndex].getCourse())) {
                        StudentsList::students[studentIndex].addDiscipline(SpecialtyList::specialties[specialtyIndex].getAvailableDisciplines()[disciplineIndex]);
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

void CommandLine::addgrade(int fn, std::string discipline, double grade) {
    if (openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            if (StudentsList::students[studentIndex].getStatus() != Student_Status::GRADUATED) {
                if(StudentsList::students[studentIndex].getStatus() != Student_Status::INTERRUPTED) {
                    int specialtyIndex = SpecialtyList::findSpecialty(StudentsList::students[studentIndex].getSpecialty());
                    int studentDisciplineIndex = StudentsList::findStudentDisciplineIndex(studentIndex,discipline);
                    if (studentDisciplineIndex == -1) {
                        std::cerr << "This discipline doesn't exist!\n";
                        return;
                    }
                    if (grade >= 2 && grade <=6) {
                        std::cout << "The grade was added successfully!\n";
                       StudentsList::students[studentIndex].getDisciplines()[studentDisciplineIndex].grade = grade;
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

void CommandLine::protocol(std::string discipline) {
    if (openedFile) {
        std::cout << "\nDiscipline \"" << discipline << "\":\n";
        for(int i = 1; i < SpecialtyList::specialties.size(); i++) {
            int disciplineIndex = SpecialtyList::findDisciplineInSpecialty(std::to_string(i), discipline);
            if (disciplineIndex == -1) continue;
            std::cout << "\nSpecialty \"" << SpecialtyList::specialties[i].getName() << "\":\n";
            for(int c : SpecialtyList::specialties[i].getAvailableDisciplines()[disciplineIndex].getAvailableForCourses()) {
                std::cout << "Course: " << c << std::endl;
                for(Student s : StudentsList::students) {
                    if (s.getSpecialty() == SpecialtyList::specialties[i].getName() && s.isDisciplineEnrolled(discipline) && s.getDisciplineEnrolledCourse(discipline) == c) {
                        std::cout << s.getName() << " - FN: " << s.getFN() << ", Group: " << s.getGroup() << std::endl; 
                    }       
                }
            }
        }
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
}

void CommandLine::report(int fn) {
    if (openedFile) {
        if(StudentsList::alreadyEnrolledFN(fn)) {
            int studentIndex = StudentsList::findFN(fn);
            StudentsList::students[studentIndex].calculateAvgGrade();
            double creditsDiff = SpecialtyList::specialties[SpecialtyList::findSpecialty(StudentsList::students[studentIndex].getSpecialty())].getMinCredits() - StudentsList::students[studentIndex].countCredits();
            std::cout << "\nStudent name: " << StudentsList::students[studentIndex].getName() << std::endl
                      << "Average grade: " << StudentsList::students[studentIndex].getAverageGrade() << std::endl
                      << "Credits left: " << creditsDiff << std::endl;

            std::cout << "\nPassed disciplines:\n";
            for (int i = 0; i < StudentsList::students[studentIndex].getDisciplines().size(); i++) {
                if (StudentsList::students[studentIndex].getDisciplines()[i].grade >= 3) {
                    std::cout << "- " << StudentsList::students[studentIndex].getDisciplines()[i].discipline << " - "
                    << "Grade: " << StudentsList::students[studentIndex].getDisciplines()[i].grade
                    << std::endl;
                }
            }
            
            std::cout << "\nNot passed disciplines:\n";
            for (int i = 0; i < StudentsList::students[studentIndex].getDisciplines().size(); i++) {
                if (StudentsList::students[studentIndex].getDisciplines()[i].grade < 3) {
                    std::cout << "- " << StudentsList::students[studentIndex].getDisciplines()[i].discipline << " - "
                    << "Grade: " << StudentsList::students[studentIndex].getDisciplines()[i].grade
                    << std::endl;
                }
            }
        }
        else std::cerr << "A student with this faculty number hasn't been found!\n";
    }
    else std::cerr << "There is not opened file! You can open file with 'open <file path>'\n";
} 

void CommandLine::run () {
    SpecialtyList::loadSpecialties();
    std::cout << "Welcome to SUSI!\n";
    while(true) {
        std::string command;
        std::cout << "\n> "; 
        getline(std::cin, command);
        command = StringHelper::clearAllConsecutiveSpacesQ(StringHelper::stripBegin(StringHelper::strip(command)));
        std::string* separatedCommand = StringHelper::splitQ(command);
        int commandArguments = StringHelper::countQ(command, ' ') + 1;

        if (StringHelper::toLowerCase(separatedCommand[0]) == "exit") {
           if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else {
                delete[] separatedCommand;
                exit();
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "open") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                separatedCommand[1] = StringHelper::strip(StringHelper::stripBegin(separatedCommand[1],'"'),'"');
                open(separatedCommand[1]);
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "close") {
            if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else close();
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "help") {
            if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else help();
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "save") {
            if (commandArguments > 1) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 1) std::cerr << "Too few arguments for this command!\n";
            else save();
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "saveas") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                separatedCommand[1] = StringHelper::strip(StringHelper::stripBegin(separatedCommand[1],'"'),'"');
                saveas(separatedCommand[1]);
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "print") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) print(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "printall") {
            if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[2])) printall(StringHelper::strip(StringHelper::stripBegin(separatedCommand[1],'"'),'"'), std::atoi(separatedCommand[2].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "enroll") {
            if (commandArguments > 5) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 5) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1]) && StringHelper::isNumber(separatedCommand[3])) {
                    enroll(std::atoi(separatedCommand[1].c_str()),StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'),std::atoi(separatedCommand[3].c_str()),StringHelper::strip(StringHelper::stripBegin(separatedCommand[4], '"'),'"'));
                }
                else std::cerr << "Invalid arguments!\n";
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "advance") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) advance(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "graduate") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) graduate(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "change") {
            if (commandArguments > 4) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 4) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) change(std::atoi(separatedCommand[1].c_str()), StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[3],'"'),'"'));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "interrupt") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) interrupt(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "resume") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) resume(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "enrollin") {
            if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) enrollin(std::atoi(separatedCommand[1].c_str()),StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "addgrade") {
            if (commandArguments > 4) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 4) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1]) && StringHelper::isNumber(separatedCommand[3])) addgrade(std::atoi(separatedCommand[1].c_str()), StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'), std::atof(separatedCommand[3].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "protocol") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else protocol(StringHelper::strip(StringHelper::stripBegin(separatedCommand[1],'"'),'"'));
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "report") {
            if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
            else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
            else {
                if (StringHelper::isNumber(separatedCommand[1])) report(std::atoi(separatedCommand[1].c_str()));
                else std::cerr << "Invalid arguments!\n"; 
            }
        }
        //Database management
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "add") {
            if (commandArguments == 1) {
                std::cerr << "Too few arguments for this command!\n";
                continue;
            }
            if (StringHelper::toLowerCase(separatedCommand[1]) == "specialty") {
                if (commandArguments > 4) std::cerr << "Too much arguments for this command!\n";
                else if (commandArguments < 4) std::cerr << "Too few arguments for this command!\n";
                else {
                    if (StringHelper::isNumber(separatedCommand[3])) SpecialtyList::addSpecialty(StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'), std::atof(separatedCommand[3].c_str()));
                    else std::cerr << "Invalid arguments!\n";
                }
            }
            else if (StringHelper::toLowerCase(separatedCommand[1]) == "discipline") {
                if (commandArguments < 6) {
                    std::cerr << "Too few arguments for this command!\n";
                    continue;
                }
                if (EnumConvertions::stringToType(separatedCommand[4]) == Type::OPTIONAL) {
                    if (commandArguments > 7) std::cerr << "Too much arguments for this command!\n";
                    else if (commandArguments < 7) std::cerr << "Too few arguments for this command!\n";
                    else {
                        if (StringHelper::isNumber(separatedCommand[6])) SpecialtyList::addDiscipline(StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[3],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[4],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[5],'"'),'"'), std::atof(separatedCommand[6].c_str()));
                        else std::cerr << "Invalid arguments!\n";
                    }
                }
                else if (EnumConvertions::stringToType(separatedCommand[4]) == Type::COMPULSORY) {
                    if (commandArguments > 6) std::cerr << "Too much arguments for this command!\n";
                    else if (commandArguments < 6) std::cerr << "Too few arguments for this command!\n";
                    else SpecialtyList::addDiscipline(StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[3],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[4],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[5],'"'),'"'), 0);
                }
                else std::cerr << "Invalid type!\n";
            }
            else std::cerr << "Invalid option!\n";
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "remove") {
            if (StringHelper::toLowerCase(separatedCommand[1]) == "specialty") {
                if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
                else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
                else SpecialtyList::removeSpecialty(StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'));
            }
            else if (StringHelper::toLowerCase(separatedCommand[1]) == "discipline") {
                if (commandArguments > 4) std::cerr << "Too much arguments for this command!\n";
                else if (commandArguments < 4) std::cerr << "Too few arguments for this command!\n";
                else {
                    SpecialtyList::removeDiscipline(StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'), StringHelper::strip(StringHelper::stripBegin(separatedCommand[3],'"'),'"'));
                }
            }
            else std::cerr << "Invalid option!\n";
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "list") {
            if (StringHelper::toLowerCase(separatedCommand[1]) == "specialties") {
                if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
                else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
                else SpecialtyList::listSpecialties();
            }
            else if (StringHelper::toLowerCase(separatedCommand[1]) == "disciplines") {
                if (commandArguments > 3) std::cerr << "Too much arguments for this command!\n";
                else if (commandArguments < 3) std::cerr << "Too few arguments for this command!\n";
                else SpecialtyList::listDisciplines(StringHelper::strip(StringHelper::stripBegin(separatedCommand[2],'"'),'"'));
            }
            else if (StringHelper::toLowerCase(separatedCommand[1]) == "all") {
                if (commandArguments > 2) std::cerr << "Too much arguments for this command!\n";
                else if (commandArguments < 2) std::cerr << "Too few arguments for this command!\n";
                else SpecialtyList::listAll();
            }
            else std::cerr << "Invalid option!\n";
        }
        else if (StringHelper::toLowerCase(separatedCommand[0]) == "savespecialties") {
            SpecialtyList::writeSpecialties();
        }
        else if(!command.empty()) std::cerr << "Invalid command! Please type 'help' to see all available commands!\n";
        
        delete[] separatedCommand;
    }
}