#pragma once
typedef struct Node Node; //in order for union to recognize fields
typedef struct Data Data;//in order for union to recognize fields
union ptr {
	Node** subtree;
	Data** data;
};

struct Node {
	size_t sz, m;
	size_t* keys;
	ptr p;
	bool isLeaf;

	Node(size_t m, bool isLeaf=false, size_t height = 0)
	{
		this->m = m;
		this->isLeaf = isLeaf;
		this->sz = 0;
		this->p.subtree = new Node* [m]; //for leaf nodes, children[m-1] is the pointer for linked list between leaf nodes
		this->keys = new size_t[m - 1];
		for (size_t i = 0; i < m; this->p.subtree[i++] = nullptr);
	}
	bool isFull() { return sz == m - 1; }
	//for root minimum number of keys is 1, and minimum number of subtrees is 2
	void printNode() {
		std::cout << "|";
		for (size_t i = 0; i <sz; i++) std::cout << " " << keys[i]%100 << " |";
		for (size_t i = 0; i < m - sz-1; i++) std::cout << " #### |";
	}
};

struct Data {
	size_t ca_id;
	size_t ca_b_id;
	size_t ca_c_id;
	std::string ca_name;
	size_t ca_tax_st;
	double ca_bal;
};