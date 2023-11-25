#ifndef BOOTSECTOR_H
#define BOOTSECTOR_H
#include <iostream>
#include "SHA256.h"
#include <string>
#include <fstream>
using namespace std;

class BootSector {
private:
    uint16_t numBytePerSector;
    uint16_t numSectorOfBoot;
    uint16_t numSectorPerCluster;
    uint16_t numFatTable;
    uint64_t volumeSize;
    uint64_t fatSize;
    uint64_t numberOfEntry;
    uint16_t passSize;
    string password;
public:
    BootSector(uint64_t volumeSize);
    uint64_t getFatSize();

    void setPassword(string pass);
    string getPassword();

    void resetPass();
    bool checkPassword(string pass);

    //Read Write 
    void ReadBootSector(fstream& file);
    void WriteBootSector(fstream& file);
    
    //Get
    uint16_t getnumBytePerSector() {
        return this->numBytePerSector;
    }
    uint16_t getnumSectorOfBoot() {
        return this->numSectorOfBoot;
    }
    uint16_t getNumSectorPerCluster() {
        return this->numSectorPerCluster;
    }
    uint16_t getNumFatTable() {
        return this->numFatTable;
    }

    uint64_t getNumberOfEntry() {
        return this->numberOfEntry;
    }

    //Set
    void setPassSize(uint16_t passSize) {
        this->passSize = passSize;
    }
};

#endif