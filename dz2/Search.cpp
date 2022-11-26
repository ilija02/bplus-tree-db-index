#include "BPlusTree.h"
#include "Helpers.h"

bool BPlusTree::SearchSingle(size_t key, Node*& leaf, Node*& parent, size_t* took_steps, size_t* idx)
{
	*took_steps = 1;
	if (this->root == nullptr) return false;
	bool skip;
	Node* curr = this->root;
	while (!curr->isLeaf) {
		skip = false;
		size_t i;
		for (i = 0; i < curr->sz; i++)
		{
			if (key <= curr->keys[i]) {
				parent = curr;
				(*took_steps)++;
				curr = curr->p.subtree[i];
				skip = true;
				break;
			}
		}
		if (!skip && i == curr->sz)//if key > all keys in node
		{
			(*took_steps)++;
			parent = curr;
			curr = curr->p.subtree[i];
		}


	}
	leaf = curr;
	//we are at leaf level now, search inside leaf node for key	
	for (size_t i = 0; i < curr->sz; i++) if (key == curr->keys[i]) {
		*idx = i;
		return true;
	}
	*idx = -1;
	return false;
}

bool BPlusTree::SearchSingle(size_t key, Node*& leaf, Node*& parent)
{
	if (this->root == nullptr) return false;
	Node* curr = this->root;
	bool skip;
	while (!curr->isLeaf) {
		skip = false;
		size_t i;
		for (i = 0; i < curr->sz; i++)
		{
			if (key <= curr->keys[i]) {
				parent = curr;
				curr = curr->p.subtree[i];
				skip = true;
				break;
			}
		}
		if (!skip && i == curr->sz)//if key > all keys in node
		{
			parent = curr;
			curr = curr->p.subtree[i];
		}

	}
	leaf = curr;
	//we are at leaf level now, search inside leaf node for key	
	for (size_t i = 0; i < curr->sz; i++) if (key == curr->keys[i]) return true;
	return false;
}

Node* BPlusTree::getParent(size_t key)
{
	Node* parent = nullptr;
	Node* curr = this->root;
	if (this->root == nullptr) return nullptr;
	while (!curr->isLeaf) {
		size_t i;
		for (i = 0; i < curr->sz; i++)
		{
			if (key == curr->keys[i]) {
				return parent;
			}
			if (key < curr->keys[i]) {
				parent = curr;
				curr = curr->p.subtree[i];
				break;
			}
		}
		if (i == curr->sz)//if key > all keys in node
		{
			parent = curr;
			curr = curr->p.subtree[i];
		}


	}
	return nullptr;


}

void BPlusTree::SearchMultiple(std::vector<size_t>& keys, size_t* tookSteps, std::string filename)
{
	std::ofstream out(filename, std::ios_base::out | std::ios_base::trunc);
	*tookSteps = 0;
	for (size_t i = 0; i < keys.size(); i++)
	{
		size_t took_steps_i, idx, key;
		Node* leaf, * junk;
		if (SearchSingle(keys[i], leaf, junk, &took_steps_i, &idx)) {
			*tookSteps += took_steps_i;
			helpers::writeNodeToFile(leaf->p.data[idx], out);
		}
	}
	out.close();
}
void BPlusTree::SearchMultipleSuccessive(size_t startKey, size_t k, size_t* took_steps, std::string filename)
{
	Node* curr, * junk;
	size_t idx;
	std::ofstream out(filename, std::ios_base::out | std::ios_base::trunc);
	SearchSingle(startKey, curr, junk, took_steps, &idx);
	size_t i = 0;
	while (i < k && curr != nullptr) {
		while (idx < curr->sz && i < k) {


			helpers::writeNodeToFile(curr->p.data[idx], out);
			idx++;
			i++;
		}
		idx = 0;
		curr = curr->p.subtree[curr->m - 1];//next in linked list
		(*took_steps)++;
	}
	return;
}