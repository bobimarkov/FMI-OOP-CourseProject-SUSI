#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Student.hpp"
#include "Discipline.hpp"
#include "Helpers/StringHelper.hpp"

#define SH StringHelper 

bool openedFile = false;
std::string openedFilePath;
std::string fileName;

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

    std::cout << "Successfully opened " << fileName << "!\n";
    in.close();
}

void close() {
    openedFile = false;
    std::cout << "Successfully closed " << fileName << "!\n";
}

void saveas(std::string filePath) {
    std::ofstream out(filePath, std::ios::binary | std::ios::out);

    if (!out.is_open()) {
        std::cerr << "Error: Invalid file path!\n";
        return;
    }

    openedFilePath = filePath;

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
              << "close - closes the current opened file\n"
              << "saveas <file path> - saves the current opened file in <file path>\n"
              << "save - saves the current opened file\n"
              << "exit - exits the program\n";
    
}

void exit(){
    std::cout << "Exiting the program...\n";
    std::exit(0);
}

void enroll(){}
void advance(int fn){}
void change(){}
void graduate(){}
void interrupt(){}
void resume(){}
void print(){}
void printall(){}
void enrollin(){}
void addgrade(){}
void protocol(){}
void report(){} 

int main () {
    std::cout << "Welcome to SUSI!\n";
    while(true) {
        std::string command;
        std::cout << "\n> "; 
        getline(std::cin, command);
        command = SH::clearAllConsecutiveSpacesQ(SH::stripBegin(SH::strip(command)));
        std::string* separatedCommand = SH::splitQ(command);

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
        else if(!command.empty()) std::cerr << "Invalid command! Please type 'help' to see all available commands!\n";
        
        delete[] separatedCommand;
    }
    return 0;
}