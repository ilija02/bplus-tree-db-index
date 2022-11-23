#pragma once
struct Node {
	size_t height, sz, m;
	size_t* keys;
	Node* father;
	void** children;
	bool isLeaf;

	Node(size_t m, bool isLeaf=false, size_t height = 0)
	{
		this->m = m;
		this->isLeaf = isLeaf;
		this->father = nullptr;
		this->sz = 0;
		this->children = new void* [m]; //for leaf nodes, children[m-1] is the pointer for linked list between leaf nodes
		this->keys = new size_t[m - 1];
		for (size_t i = 0; i < m; this->children[i++] = nullptr);
	}
	bool isFull() { return sz == m - 1; }
	bool szInternalNode() {
		//returns if number of keys in node satisfies the condition for internal nodes
		return sz >= (ceil(static_cast<double>(m) / 2) - 1);
	}
	bool szLeafNode() {
		//returns if number of keys in node satisfies the condition for leaf nodes
		return sz >= m / 2;
	}
	//for root minimum number of keys is 1, and minimum number of subtrees is 2

};

struct Data {
	size_t ca_id;
	size_t ca_b_id;
	size_t ca_c_id;
	std::string ca_name;
	size_t ca_tax_st;
	double ca_bal;
};