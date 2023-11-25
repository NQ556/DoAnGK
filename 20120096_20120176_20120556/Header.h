#define _CRT_SECURE_NO_WARNINGS
#ifndef HEADER_H
#define HEADER_H
#include "BootSector.h"
#include "FatTable.h"
#include "Entry.h"
#include <fstream>
#include <stdexcept>

bool createVolume(const string& volumeName, uint64_t volumeSize, BootSector& bootSector);
void formatVolume(const string& volumeName, const BootSector& bootSector, const FatTable& fatTable);
bool isListEmpty(const string& volumeName, const FatTable& fatTable);
void importFile(const string& volumeName, BootSector& bootSector, FatTable& fatTable, const string& fileName, const string& extension);
void exportFile(const string& volumeName, BootSector& bootSector, const FatTable& fatTable, const string& fileName, const string& extension);
void listFiles(const string& volumeName, const BootSector& bootSector, const FatTable& fatTable);

#endif