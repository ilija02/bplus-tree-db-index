//43000000001 | 4300000001 | 4300000001 | Joshua Fowle Savings Account | 1 | 6334235.05
//43000000002 | 4300000003 | 4300000001 | Joshua Fowle Play Money | 2 | 2731646.43
//43000000003 | 4300000004 | 4300000001 | Joshua Fowle Emergency Expenses | 2 | 7625683.59
//43000000004 | 4300000005 | 4300000001 | Joshua Fowle Vacation Account | 2 | 79823.44
//43000000005 | 4300000010 | 4300000001 | Joshua Fowle Healthcare Fund | 2 | 1104436.03
//43000000006 | 4300000008 | 4300000001 | Joshua Fowle New Car Account | 1 | 1594842.08
//43000000011 | 4300000004 | 4300000002 | Willie Swigert Joint Account | 1 | -9093725.95
//43000000012 | 4300000003 | 4300000002 | Willie Swigert Business Account | 1 | 9288893.44
//43000000013 | 4300000007 | 4300000002 | Willie Swigert College Fund | 1 | 8096128.80
//43000000014 | 4300000006 | 4300000002 | Willie Swigert Flexible Spending | 0 | 9638415.62
//43000000015 | 4300000001 | 4300000002 | Willie Swigert Play Money | 1 | -1877283.56
//43000000016 | 4300000002 | 4300000002 | Willie Swigert Emergency Expenses | 2 | 7588354.79
//43000000017 | 4300000005 | 4300000002 | Willie Swigert Vacation Account | 2 | 7102893.94
//43000000018 | 4300000008 | 4300000002 | Willie Swigert Healthcare Fund | 2 | 7079673.10
//43000000021 | 4300000001 | 4300000003 | Amos Labree Individual Account | 1 | -8591065.12
//43000000022 | 4300000006 | 4300000003 | Amos Labree Family Trust | 1 | 2859736.31
//43000000023 | 4300000007 | 4300000003 | Amos Labree Retirement Fund | 0 | 6271643.53
//43000000024 | 4300000002 | 4300000003 | Amos Labree Joint Account | 2 | -7002828.09
//43000000025 | 4300000009 | 4300000003 | Amos Labree Business Account | 1 | 4997031.51
//43000000031 | 4300000005 | 4300000004 | Charlotte Ciciora Healthcare Fund | 1 | 1787356.81
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
		middleIndex = m / 2;
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
			//continue in loop
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
