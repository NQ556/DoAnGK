#include "Entry.h"

Entry::Entry() {
    std::memset(MainName, 0, sizeof(MainName)); 
    std::memset(Extension, 0, sizeof(Extension));
    isDeleted = false; 
    fileSize = 0; 
    startCluster = 0;
    numEntryChild = 0; 
    numExtraEntry = 0; 
    password = "";
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

void Entry::setPassword(string pass) {
    SHA256 sha;
    sha.update(pass);
    std::array<uint8_t, 32> digest = sha.digest();
    string passhash = SHA256::toString(digest);

    int size_rubbish;
    size_rubbish = rand() % 62;
    string characters = "rstuvwxyzabcEFGHIJKL123456defghijklmnoMNOPQRSTUVWXYZ0pqABCD789";

    string subString = characters.substr(0, size_rubbish);
    this->password = to_string(size_rubbish + 11) + subString + passhash;
    this->passSize = this->password.length();
    this->passSize = this->password.length();
}

string Entry::getPassword() {
    return this->password;
}

void Entry::resetPass() {
    this->password = "";
    this->passSize = 0;
}

void Entry::setPassSize(uint16_t passSize) {
    this->passSize = passSize;
}