#include "BPlusTree.h"

int main() {
	BPlusTree *bpt = BPlusTree::FromFile(5, "CustomerAccount20.txt");
	bpt->Print();
	return 0;
}