#pragma once
#include<string>
#include<cmath>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<iostream>
#include<queue>

#include "Node.h"
//kod preloma, ceil(m/2) se zadrzava u starom listu
//kod preloma u lsitovima dolazi do replikacije srednjeg kljuca
// imamo m podstabala i m-1 kljuceva

class BPlusTree
{
private:
#pragma region fields
	size_t m;
#pragma endregion
#pragma region methods
	void insertInternalNode(size_t key, Node* child, Node* father);
	Node* getParent(size_t key);
#pragma endregion
public:
	size_t maxKey = LLONG_MIN;
	BPlusTree(size_t m) {
		this->m = m;
		this->root = nullptr;
	} //TODO: move constructor into private section
	Node* root;
	static BPlusTree* FromFile(size_t m, const char* fname);
	bool Insert(size_t key, Data* data);
	bool SearchSingle(size_t key, Node*& leaf, Node*& parent);
	void Print();
};

