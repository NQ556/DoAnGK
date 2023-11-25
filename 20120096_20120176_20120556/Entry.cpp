#include "Entry.h"

Entry::Entry() {
    std::memset(MainName, 0, sizeof(MainName)); 
    std::memset(Extension, 0, sizeof(Extension));
    isDeleted = false; 
    fileSize = 0; 
    startCluster = 0;
    numEntryChild = 0; 
    numExtraEntry = 0; 
    passSize = 0;
}

void Entry::setFileSize(uint32_t fileSize) {
    this->fileSize = fileSize;
}

uint16_t Entry::getStartCluster() {
    return this->startCluster;
}

uint32_t Entry::getFileSize() {
    return this->fileSize;
}

bool Entry::getIsDeleted() {
    return this->isDeleted;
}

uint16_t Entry::getNumEntryChild() {
    return this->numEntryChild;
}

uint16_t Entry::getNumExtraEntry() {
    return this->numExtraEntry;
}

void Entry::setNumEntryChild(uint16_t numEntryChild) {
    this->numEntryChild = numEntryChild;
}

void Entry::setStartCluster(uint16_t startCluster) {
    this->startCluster = startCluster;
}


void Entry::resetPass() {
    this->passSize = 0;
}

void Entry::setPassSize(uint16_t passSize) {
    this->passSize = passSize;
}

uint16_t Entry::getPassSize() {
    return this->passSize;
}