#pragma once
#include<iostream>
#include "BPlusTree.h"
#include<cstdio>
#define ZAD1_NR_OPTIONS 7
namespace helpers {
	int getSelectedMenuOption();
	void mainLoop();
	bool nullCheckAndError(BPlusTree* b);
}
