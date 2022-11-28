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
	size_t m;
	void insertInternalNode(size_t key, Node* child, Node* father);
	Node* getParent(size_t key);
	void clear();
public:
	size_t maxKey = LLONG_MIN;
	BPlusTree(size_t m) {
		this->m = m;
		this->root = nullptr;
	} //TODO: move constructor into private section
	~BPlusTree();
	Node* root;

	static BPlusTree* FromFile(size_t m, std::string fname);
	bool Insert(size_t key, Data* data);
	bool Delete(size_t key);
	bool SearchSingle(size_t key, Node*& leaf, Node*& parent, size_t *took_steps, size_t* idx);//last two pointer parameters are used when finding the data of a node with given key
	bool SearchSingle(size_t key, Node*& leaf, Node*& parent);
	void SearchMultiple(std::vector<size_t>& keys, size_t* tookSteps, std::string filename = "found.txt");
	void SearchMultipleSuccessive(size_t startKey, size_t k, size_t* tookSteps, std::string filename = "found.txt");
	void Print();
};

