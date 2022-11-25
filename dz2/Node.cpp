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

void Node::printNodeData(size_t idx)
{
	Data* d = this->p.data[idx];
	std::cout << "\tca_id: " << d->ca_id << std::endl;
	std::cout << "\tca_b_id: " << d->ca_b_id << std::endl;;
	std::cout << "\tca_c_id: " << d->ca_c_id << std::endl;;
	std::cout << "\tca_name: " << d->ca_name << std::endl;;
	std::cout << "\tca_tax_st: " << d->ca_tax_st << std::endl;;
	std::cout << "\tca_bal: " << d->ca_bal << std::endl;;
}
