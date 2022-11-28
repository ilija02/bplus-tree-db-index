#include "BPlusTree.h"

bool BPlusTree::Delete(size_t key) {
	if (root == nullptr) return false;
	Node* leaf, * parent;
	size_t steps, idx, parent_idx = -1;
	if (!this->SearchSingle(key, leaf, parent, &steps, &idx)) return false;
	Node* r_brother = leaf->p.subtree[m - 1], * l_brother; //koristimo ulancanu listu
	if (idx > 0 && parent->sz > 1) l_brother = parent->p.subtree[idx - 1];
	else l_brother = nullptr;
	for (int i = 0; i < parent->sz; i++) if (parent->keys[i] == key) parent_idx = i;
	if (leaf->sz >= m / 2) { //ako cvor nije minimalno popunjen TODO: pogresan je uslov, treba da bude vece od m/2
		delete leaf->p.data[idx];
		if (idx == leaf->sz - 1 && r_brother != nullptr) {
			//ako je najdesniji cvor
			if (parent) parent->keys[parent_idx] = leaf->keys[idx - 1];
		}
		for (int i = idx; i < leaf->sz; i++) {
			leaf->keys[i] = leaf->keys[i + 1];
			leaf->p.data[i] = leaf->p.data[i + 1];
		}
		leaf->sz--;

	}
	else if (r_brother && r_brother->sz >= m / 2) { //pozajica od desnog brata TODO: pogresan je uslov, treba da bude vece od m/2
		//uzimamo najlevlji kljuc i najlevlje podstablo
		parent->keys[parent_idx] = r_brother->keys[0];
		for (int i = idx; i < leaf->sz; i++) {
			leaf->keys[i] = leaf->keys[i + 1];
			leaf->p.data[i] = leaf->p.data[i + 1];
		}
		leaf->keys[leaf->sz - 1] = r_brother->keys[0];
		leaf->p.data[leaf->sz - 1] = r_brother->p.data[0];
		for (int i = 0; i < r_brother->sz; i++) {
			r_brother->keys[i] = r_brother->keys[i + 1];
			r_brother->p.data[i] = r_brother->p.data[i + 1];
		}
		r_brother->sz--;
	}
	else if (l_brother && l_brother->sz >= m / 2) {
		parent->keys[parent_idx] = l_brother->keys[l_brother->sz - 1]; //uzmemo najdesniji kljuc

		for (int i = 0; i < leaf->sz; i++) {
			leaf->keys[i + 1] = leaf->keys[i];
			leaf->p.data[i + 1] = leaf->p.data[i];
		}

		leaf->keys[0] = l_brother->keys[l_brother->sz - 1];
		leaf->p.data[0] = l_brother->p.data[l_brother->sz - 1];

		l_brother->sz--;
	}

}