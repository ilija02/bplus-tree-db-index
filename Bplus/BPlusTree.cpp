#include "BPlusTree.h"
void BPlusTree::clear()
{
	std::queue<Node*> q;
	q.push(this->root);
	while (!q.empty()) {
		Node* t = q.front();
		q.pop();
		if (!t->isLeaf) {
			for (size_t i = 0; i < t->sz+1; i++)
			{
				q.push((t->p.subtree[i]));
			}
		}
		delete t;
	}
}

BPlusTree::~BPlusTree()
{
	clear();
}