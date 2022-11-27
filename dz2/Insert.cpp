#include "BPlusTree.h"

void BPlusTree::insertInternalNode(size_t key, Node* child, Node* father)
{
	
	while (true) {
		std::vector<size_t> keys;
		std::vector<Node*> v_subtree;
		std::vector<size_t>::iterator it;
		if (!father->isFull()) {
			int i = 0;
			int j;
			while (key > father->keys[i]) i++;
			for (j = father->sz - 1; j >= i; j--) {
				father->keys[i] = father->keys[j];
			}
			father->keys[i] = key;
			for (j = father->sz; j > i; j--) {
				father->p.subtree[j + 1] = father->p.subtree[j];
			}
			father->p.subtree[j + 1] = child;
			father->sz++;
			return;
		}
		#pragma region split
		keys.insert(keys.end(), &father->keys[0], &father->keys[father->sz]);
		v_subtree.insert(v_subtree.end(), &father->p.subtree[0], &father->p.subtree[m]);
		it = std::upper_bound(keys.begin(), keys.end(), key);
		int dist = std::distance(keys.begin(), it);
		keys.insert(it, key); //insert key in sorted order
		v_subtree.insert(v_subtree.begin() + dist + 1, child);//insert subtree at same position as the key
		size_t middleIndex;
		if (m % 2 == 0) middleIndex = m/2+1; //left subtree can have more nodes
		else middleIndex = m / 2;
		size_t newKey = keys[middleIndex];
		father->sz = middleIndex;
		Node* newNode = new Node(this->m, false);
		newNode->sz = 0;
		int i, j;
		//transfer keys
		for (i = middleIndex + 1, j = 0; i < this->m; i++, j++) {
			newNode->keys[j] = keys[i];
			newNode->sz++;
		}//transfer subtrees
		for (i = middleIndex + 1, j = 0; i < this->m + 1; i++, j++) {
			newNode->p.subtree[j] = v_subtree[i];
		}//check if splitting into root
		#pragma endregion
		if (father == this->root) {// we make a new root, because we are splitting the root node
			Node* newRoot = new Node(this->m, false);
			newRoot->sz = 1;
			this->root = newRoot;
			root->keys[0] = newKey;
			if (key > newKey) {
				root->p.subtree[0] = father;
				root->p.subtree[1] = newNode;
			}
			else {
				root->p.subtree[1] = father;
				root->p.subtree[0] = newNode;
			}
			return;
		}
		else {
			Node* grandparent = getParent(father->keys[0]);
			key = newKey;
			child = newNode;
			father = grandparent;

		}
	}
		
}

bool BPlusTree::Insert(size_t key, Data* data)
{
	Node* newLeaf = new Node(this->m, true);
	//check if BPlusTree isn't created
	if (this->root == nullptr) {
		root = newLeaf;
		root->keys[0] = key;
		root->p.data[0] = data;
		root->sz = 1;
		return true;
	}
	Node* leaf, * parent;
	if (this->SearchSingle(key, leaf, parent)) return false;
	if (!leaf->isFull()) {
		int i = 0;
		int j;
		while (key > leaf->keys[i]) i++;
		for (j = leaf->sz - 1; j >= i; j--) {
			leaf->p.data[j + 1] = leaf->p.data[j];
			leaf->keys[j + 1] = leaf->keys[j];
		}
		leaf->p.data[j + 1] = data;
		leaf->keys[j + 1] = key;
		leaf->sz++;
		return true;
	}
	//leaf node is full, we need to split the leaf
	//ceil(m/2) keys stays in the old leaf
	std::vector<size_t> keys;
	std::vector<Data*> v_data;
	std::vector<size_t>::iterator it;
	//insert new leaf and data into vectors
	keys.insert(keys.end(), &leaf->keys[0], &leaf->keys[leaf->sz]);
	v_data.insert(v_data.end(), &leaf->p.data[0], &leaf->p.data[m]);
	it = std::upper_bound(keys.begin(), keys.end(), key);
	int dist = std::distance(keys.begin(), it);
	keys.insert(it, key); //insert key in sorted order
	v_data.insert(v_data.begin() + dist, data);//insert subtree at same position as the key

	newLeaf->p.subtree[leaf->m - 1] = leaf->p.subtree[leaf->m - 1];
	leaf->p.subtree[leaf->m - 1] = newLeaf; //create a link between leafs
	size_t oldsz = ceil((double)m / 2);
	leaf->sz = oldsz; //leave ceil(m/2) nodes in old leaf
	newLeaf->sz = m - oldsz;
	//move part of keys and subtrees into new node
	for (int i = oldsz, j = 0; i < keys.size(); i++, j++) {
		newLeaf->p.data[j] = v_data[i];
		newLeaf->keys[j] = keys[i];
	}
	if (leaf == this->root) {
		//create new root
		Node* newRoot = new Node(m, false);
		newRoot->sz = 1;
		newRoot->p.subtree[0] = leaf;
		newRoot->p.subtree[1] = newLeaf;
		newRoot->keys[0] = leaf->keys[leaf->sz - 1]; //rightmost key in left subtree
		this->root = newRoot;
	}
	else this->insertInternalNode(leaf->keys[leaf->sz - 1], newLeaf, parent);
	return true;
}
