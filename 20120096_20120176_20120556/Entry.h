#ifndef ENTRY_H
#define ENTRY_H
#include <iostream>
#include "SHA256.h"
using namespace std;

class Entry {
private:
    bool isDeleted;
    uint32_t fileSize;
    uint16_t startCluster;
    uint16_t numEntryChild;
    uint16_t numExtraEntry;
    uint16_t passSize;
    string password;

public:
    char MainName[6];
    char Extension[6];
    Entry();
    
    void setFileSize(uint32_t fileSize);

    uint16_t getStartCluster();
    uint32_t getFileSize();
    bool getIsDeleted();
    uint16_t getNumEntryChild();
    uint16_t getNumExtraEntry();

    void setNumEntryChild(uint16_t numEntryChild);
    void setStartCluster(uint16_t startCluster);

    void setPassword(string pass);
    string getPassword();
    void resetPass();
    void setPassSize(uint16_t passSize);
};

#endif