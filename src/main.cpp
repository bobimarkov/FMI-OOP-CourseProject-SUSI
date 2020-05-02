#include <iostream>
#include <string>
#include <fstream>

#include "Student.hpp"
#include "Discipline.hpp"
#include "Helpers/StringHelper.hpp"

#define SH StringHelper 

bool openedFile = false;
std::string openedFilePath;

void open(std::string filePath){
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

    std::cout << "Successfully opened the file!\n";
    in.close();
}

void close(){
    openedFile = false;
    std::cout << "Successfully closed the file\n";
}

void saveas(){}

void save(){

}

void help(){

}

void exit(){
    std::exit(0);
}

void enroll(){}
void advance(){}
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
        else if(!command.empty()) std::cerr << "Invalid command! Try again!\n";
        
        delete[] separatedCommand;
    }
    return 0;
}