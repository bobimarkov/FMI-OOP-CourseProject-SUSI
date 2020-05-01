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

    openedFile = true;
    openedFilePath = filePath;

    if(!in.is_open()) {
        //Ще създаде нов файл
        std::ofstream out (filePath, std::ios::binary | std::ios::out);
        out.close();
    }

    std::cout << "Successfully opened the file!\n";

    in.close();
}

void close(){
    openedFile = false;
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
        command = SH::stripBegin(SH::strip(command));  
        std::string* separatedCommand = SH::split(command);

        if (SH::toLowerCase(separatedCommand[0]) == "exit") {
            delete[] separatedCommand;
            exit();
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "open") {
            if(!openedFile) open("D:/VS/FMI-OOP-CourseProject-SUSI/FMI-OOP-CourseProject-SUSI/src/test.bin/");
            else std::cerr << "There is already an opened file! You can close the file with 'close'\n"; 
        }
        else if (SH::toLowerCase(separatedCommand[0]) == "close") {
            close();
        }
        else std::cerr << "Invalid command! Try again!\n";
        
        delete[] separatedCommand;
    }
    return 0;
}