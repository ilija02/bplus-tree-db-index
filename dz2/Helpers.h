#pragma once
#include<iostream>
#include<cstdio>
#include<iomanip>
#include "BPlusTree.h"

#define ZAD1_NR_OPTIONS 9
namespace helpers {
	int getSelectedMenuOption();
	void mainLoop();
	bool nullCheckAndError(BPlusTree* b);
	void writeNodeToFile(Data* d, std::ofstream& out);
}
