#ifndef ENTRY_H
#define ENTRY_H
#include <iostream>
using namespace std;

class Entry {
private:
    bool isDeleted;
    uint32_t fileSize;
    uint16_t startCluster;
    uint16_t nameLength;
    uint16_t numEntryChild;
    uint16_t numExtraEntry;
    char Null[6];

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
};

#endif