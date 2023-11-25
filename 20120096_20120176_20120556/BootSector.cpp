#include "BootSector.h"

BootSector::BootSector(uint64_t volumeSize) {
    this->password = "";
    this->passSize = 0;
    this->numBytePerSector = 512;
    this->numSectorOfBoot = 1;
    this->numSectorPerCluster = 4;
    this->numFatTable = 1;
    this->volumeSize = volumeSize;
    this->fatSize = volumeSize / this->numBytePerSector / this->numSectorPerCluster;
    this->numberOfEntry = 1;
}

uint64_t BootSector::getFatSize() {
    return this->fatSize;
}

void BootSector::setPassword(string pass) {
    SHA256 sha;
    sha.update(pass);
    std::array<uint8_t, 32> digest = sha.digest();
    string passhash = SHA256::toString(digest);

    int size_rubbish;
    size_rubbish = rand() % 62;
    string characters = "rstuvwxyzabcEFGHIJKL123456defghijklmnoMNOPQRSTUVWXYZ0pqABCD789";
    
    string subString = characters.substr(0, size_rubbish);
    this->password = to_string(size_rubbish + 11)  + subString+ passhash;
    this->passSize = this->password.length();
}
string BootSector::getPassword() {
    return this->password;
}
void BootSector::resetPass() {
    this->password = "";
    this->passSize = 0;
}
bool BootSector::checkPassword(string pass) {
    SHA256 sha;
    sha.update(pass);
    std::array<uint8_t, 32> digest = sha.digest();
    string passhash = SHA256::toString(digest);


    string rubbish = this->password.substr(0, 2);
    int location = stoi(rubbish) - 11;
    string passlocal = this->password.substr(2+location ,this->passSize);
    if (passhash.compare(passlocal) == 0) {
        return true;
    }
    else {
        return false;
    }
}

void BootSector::ReadBootSector(fstream& file)
{
    file.read((char*)&this->numBytePerSector, sizeof(this->numBytePerSector));
    file.read((char*)&this->numSectorOfBoot, sizeof(this->numSectorOfBoot));
    file.read((char*)&this->numSectorPerCluster, sizeof(this->numSectorPerCluster));
    file.read((char*)&this->numFatTable, sizeof(this->numFatTable));
    file.read((char*)&this->volumeSize, sizeof(this->volumeSize));
    file.read((char*)&this->fatSize, sizeof(this->fatSize));
    file.read((char*)&this->numberOfEntry, sizeof(this->numberOfEntry));
    file.read((char*)&this->passSize, sizeof(this->passSize));
    this->password.resize(this->passSize);
    file.read((char*)this->password.c_str(), this->passSize);
}