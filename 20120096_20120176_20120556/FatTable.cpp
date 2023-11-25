#include "FatTable.h"

FatTable::FatTable(uint32_t size) {
	this->size = size;
	this->status.resize(this->size, 0);
}