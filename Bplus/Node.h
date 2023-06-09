#pragma once
#include<iostream>

typedef struct Node Node; //in order for union to recognize fields
struct Data {
	size_t ca_id;
	size_t ca_b_id;
	size_t ca_c_id;
	std::string ca_name;
	size_t ca_tax_st;
	double ca_bal;

};
union ptr {
	Node** subtree;
	Data** data;
};


struct Node {
	size_t sz, m;
	size_t* keys = nullptr;
	ptr p;
	bool isLeaf;

	Node(size_t m, bool isLeaf = false, size_t height = 0);
	bool isFull() { return sz == m - 1; }
	//for root minimum number of keys is 1, and minimum number of subtrees is 2
	void printNodeKeys();
	void printNodeData(size_t idx);
	~Node() {
		delete []keys;
		if (isLeaf) {
			for (int i = 0; i < sz; i++) {
				delete p.data[i];
			}
			delete (p.data);
		}
		else delete[] (p.subtree);
	}
};

