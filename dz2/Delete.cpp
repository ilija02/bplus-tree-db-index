#include "BPlusTree.h"

bool BPlusTree::Delete(size_t key) {
	if (root == nullptr) return false;
	Node* leaf, * parent;
	size_t steps, idx;
	if (!this->SearchSingle(key, leaf, parent, &steps, &idx)) return false;
	if (leaf->sz >= m / 2) { //ako cvor nije minimalno popunjen
		if (idx == leaf->sz - 1) {
			//ako je najdesniji cvor
			parent->keys[0] = leaf->keys[idx - 1];
		}
		for (int i = idx; i < leaf->sz; i++) {
			leaf->keys[i] = leaf->keys[i + 1];
			leaf->p.data[i] = leaf->p.data[i + 1];
		}
		leaf->sz--;
	}
}