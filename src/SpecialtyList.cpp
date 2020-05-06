#include "SpecialtyList.hpp"
#include "Specialty.hpp"
#include "Helpers/StringHelper.hpp"
#include "EnumerationClasses.hpp"
#include "EnumConvertions.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>

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

void SpecialtyList::addSpecialty () {
    std::string name;
    double minCredits;
    std::cout << "Enter specialty name: ";
    std::getline(std::cin, name);
    for(Specialty sp : specialties) {
        if(SH::toLowerCase(name) == SH::toLowerCase(sp.getName())) {
            std::cerr << "A specialty with this name already exists!\n";
            return;
        }
    }
    std::cout << "Enter minimal credits: ";
    std::cin >> minCredits;
    std::cin.ignore();
    specialties.push_back(Specialty(name, minCredits));
    std::cout << "Specialty added successfully!\n";
}

void SpecialtyList::removeSpecialty () {
    bool removed = false;
    std::string name;
    std::cout << "Enter specialty name or id: ";
    std::getline(std::cin, name);
    if (StringHelper::isNumber(name) && std::stoi(name) < specialties.size()) {
        specialties.erase(specialties.begin()+std::stoi(name));
        std::cout << "Specialty removed successfully!\n";
        removed = true;
    }
    else {
        for (int i = 1; i < specialties.size(); i++) {
            if (SH::toLowerCase(name) == SH::toLowerCase(specialties[i].getName())) {
                specialties.erase(specialties.begin()+i);
                std::cout << "Specialty removed successfully!\n";
                removed = true;
            }
        }
    }
    if (!removed) std::cerr << "Invalid name or id!\n";
}

void SpecialtyList::addDiscipline () {
    std::string name;
    std::cout << "Enter specialty name or id: ";
    std::getline(std::cin, name);
    int spID = 0;
    if (StringHelper::isNumber(name)) spID = std::stoi(name);
    else {
        for (int i = 1; i < specialties.size(); i++) {
            if (SH::toLowerCase(name) == SH::toLowerCase(specialties[i].getName())) spID = i;
        }
    }
    if (!(spID >= 1 && spID < specialties.size())) {
        std::cerr << "Invalid name or id!\n";
        return;
    }
    std::string disName;
    std::cout << "Selected specialty \"" << specialties[spID].getName() << "\"." <<  std::endl;
    std::cout << "Enter discipline name: ";
    std::getline(std::cin, disName);
    std::string typeName;
    std::cout << "Enter discipline type (1 - Optional, 2 - Compulsory): ";
    std::getline(std::cin, typeName);
    Type t = EnumConvertions::stringToType(typeName);
    int availableFor = 0;
    std::cout << "Available for course: ";
    std::cin >> availableFor;
    int credits = 0;
    std::cout << "Discipline credits: ";
    std::cin >> credits;
    std::cin.ignore();
    specialties[spID].addDiscipline(Discipline(disName,t,availableFor,credits));
    std::cout << "Discipline added successfully to specialty \"" << specialties[spID].getName() << "\"!\n";
}

void SpecialtyList::removeDiscipline () {
    std::string name;
    std::cout << "Enter specialty name or id: ";
    std::getline(std::cin, name);
    int spID = 0;
    if (StringHelper::isNumber(name)) spID = std::stoi(name);
    else {
        for (int i = 1; i < specialties.size(); i++) {
            if (SH::toLowerCase(name) == SH::toLowerCase(specialties[i].getName())) spID = i;
        }
    }
    if (!(spID >= 1 && spID < specialties.size())) {
        std::cerr << "Invalid name or id!\n";
        return;
    }
    std::string disName;
    std::cout << "Enter discipline name or id: ";
    std::getline(std::cin, disName);
    int dsID = 0;
    if (StringHelper::isNumber(disName)) dsID = std::stoi(disName);
    else {
        for (int i = 1; i < specialties[spID].getAvailableDisciplines().size(); i++) {
            if (SH::toLowerCase(disName) == SH::toLowerCase(specialties[spID].getAvailableDisciplines()[i].getName())) dsID = i;
        }
    }
    if (!(dsID >= 0 && dsID < specialties[spID].getAvailableDisciplines().size())) {
        std::cerr << "Invalid name or id!\n";
        return;
    }
    specialties[spID].getAvailableDisciplines().erase(specialties[spID].getAvailableDisciplines().begin()+dsID);
    std::cout << "Discipline removed successfully!\n";
}

void SpecialtyList::listSpecialties() {
    for(int i = 1; i < specialties.size(); i++) {
        std::cout << i << ". " << specialties[i].getName() << std::endl;
    }
}

void SpecialtyList::listDisciplines() {
    std::string name;
    std::cout << "Enter specialty name or id: ";
    std::getline(std::cin, name);
    int spID = 0;
    if (StringHelper::isNumber(name)) spID = std::stoi(name);
    else {
        for (int i = 1; i < specialties.size(); i++) {
            if (SH::toLowerCase(name) == SH::toLowerCase(specialties[i].getName())) spID = i;
        }
    }
    if (!(spID >= 1 && spID < specialties.size())) {
        std::cerr << "Invalid name or id!\n";
        return;
    }
    specialties[spID].printAvailableDisciplines();
}

void SpecialtyList::loadSpecialties() {
    std::ifstream in("specialties.dat", std::ios::binary | std::ios::in);
    
    if(!in.is_open()) {
        std::cerr << "Warning: File specialties.dat doesn't exist!\n";
        return;
    }

    //Header
    fileHeader header;
    in.read(reinterpret_cast<char*>(&header),sizeof(header));
    if(!checkHeader(header)) {
        std::cerr << "Invalid file!\n";
        in.close();
        return;
    }

    in.seekg(0, std::ios::end);
    std::streampos size = in.tellg();
    in.seekg(0, std::ios::beg);

    while(in.tellg() != size || !in.eof()) {
        Specialty sp;
        sp.read(in);
        specialties.push_back(sp);
    }

    in.close();
}

void SpecialtyList::writeSpecialties() {
    std::ofstream out("specialties.dat", std::ios::binary | std::ios::out);
    //Header
    fileHeader realHeader{"SP",13094,201,974}; 
    out.write(reinterpret_cast<char*>(&realHeader), sizeof(realHeader));

    //Specialties
    for (Specialty sp : specialties) {
        sp.write(out);
    }
    out.close();
}