#include "Node.h"

Node::Node(size_t m, bool isLeaf, size_t height)
{
	this->m = m;
	this->isLeaf = isLeaf;
	this->sz = 0;
	this->p.subtree = new Node * [m]; //for leaf nodes, children[m-1] is the pointer for linked list between leaf nodes
	this->keys = new size_t[m - 1];
	for (size_t i = 0; i < m; this->p.subtree[i++] = nullptr);
}

void Node::printNodeKeys()
{
	std::cout << "|";
	for (size_t i = 0; i < sz; i++) std::cout << keys[i] << "|";
	for (size_t i = 0; i < m - sz - 1; i++) std::cout << "#|";
}
