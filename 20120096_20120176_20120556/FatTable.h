#ifndef FATTABLE_H
#define FATTABLE_H
#include <iostream>
#include <vector>
using namespace std;

class FatTable {
private:
public:
	vector<uint16_t> status;
	uint32_t size;
	FatTable(uint32_t size);
};

#endif