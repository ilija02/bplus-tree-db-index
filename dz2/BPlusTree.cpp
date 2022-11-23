#include "BPlusTree.h"


void BPlusTree::insertInternalNode(size_t key, Data* data)
{
	std::cout << "Not yet implemented";
}

BPlusTree* BPlusTree::FromFile(size_t m, const char* fname)
{
	BPlusTree* b = new BPlusTree(m);
	std::ifstream infile;
	infile.open(fname);
	std::string line = "";
	while (std::getline(infile, line)) {
		Data* d = new Data();
		int tmp_i;
		std::string tmp_s = "";
		std::stringstream ss(line);
		std::getline(ss, tmp_s, '|');
		d->ca_id = std::stoll(tmp_s);
		std::getline(ss, tmp_s, '|');
		d->ca_b_id = std::stoll(tmp_s);
		std::getline(ss, tmp_s, '|');
		d->ca_c_id = std::stoll(tmp_s);
		std::getline(ss, d->ca_name, '|');
		std::getline(ss, tmp_s, '|');
		d->ca_tax_st = std::stoll(tmp_s);
		std::getline(ss, tmp_s, '|');
		d->ca_bal = std::stod(tmp_s);
		b->Insert(d->ca_id, d);
	}
	infile.close();
	return b;
}

bool BPlusTree::Insert(size_t key, Data* data)
{
	Node* newLeaf = new Node(this->m, true);
	//check if BPlusTree isn't created
	if (this->root == nullptr) {
		root = newLeaf;
		root->keys[0] = key;
		root->children[0] = data;
		root->sz = 1;
		return true;
	}

	Node* curr = this->root;
	Node* leaf;
	if (this->SearchSingle(key, leaf)) return false;
	if (!leaf->isFull()) {
		int i = 0;
		int j;
		while (key > leaf->keys[i]) i++;
		for (j = curr->sz - 1; j >= i; j--) {
			curr->children[j + 1] = curr->children[j];
			curr->keys[j + 1] = curr->keys[j];
		}
		curr->children[j + 1] = data;
		curr->keys[j + 1] = key;
		curr->sz++;
		return true;
	}
	//leaf node is full, we need to split the leaf
	//ceil(m/2) keys stays in the old leaf
	std::vector<size_t> keys;
	std::vector<void*> subtrees;
	std::vector<size_t>::iterator it;
	//insert new leaf and data into vectors
	keys.insert(keys.end(), &leaf->keys[0], &leaf->keys[leaf->sz]);
	subtrees.insert(subtrees.end(), &leaf->children[0], &leaf->children[m]);
	it = std::upper_bound(keys.begin(), keys.end(), key);
	int dist = std::distance(keys.begin(), it);
	keys.insert(it, key); //insert key in sorted order
	subtrees.insert(subtrees.begin() + dist, data);//insert subtree at same position as the key
	leaf->children[leaf->m - 1] = newLeaf; //create a link between leafs
	int oldsz = ceil((double)m / 2);
	leaf->sz = oldsz; //leave ceil(m/2) nodes in old leaf
	newLeaf->sz = m - oldsz;
	//move part of keys and subtrees into new node
	for (int i = oldsz, j = 0; i < keys.size(); i++, j++) {
		newLeaf->children[j] = subtrees[i];
		newLeaf->keys[j] = keys[i];
	}
	if (leaf == this->root) {
		//create new root
		Node* newRoot = new Node(m, false);
		newRoot->sz = 1;
		newRoot->children[0] = leaf;
		newRoot->children[1] = newLeaf;
		newRoot->keys[0] = leaf->keys[leaf->sz - 1]; //rightmost key in left subtree
		this->root = newRoot;
		leaf->father = newRoot;
		newLeaf->father = newRoot;
	}
	else this->insertInternalNode(key, data);

	return true;
}

bool BPlusTree::SearchSingle(size_t key, Node*& leaf)
{
	if (this->root == nullptr) return false;
	Node* curr = this->root;
	while (!curr->isLeaf) {
		size_t i;
		for (i = 0; i < curr->sz; i++)
		{
			if (key < curr->keys[i]) {
				curr = static_cast<Node*>(curr->children[i]); //cast from void* to node*
				break;
			}
		}
		if (i == curr->sz) curr = static_cast<Node*>(curr->children[i]); //if key > all keys in node

	}
	leaf = curr;
	//we are at leaf level now, search inside leaf node for key	
	for (size_t i = 0; i < curr->sz; i++) if (key == curr->keys[i]) return true;
	return false;
}

void BPlusTree::Print()
{
	std::queue<Node*> q;
	q.push(this->root);
	q.push(nullptr);
	while (!q.empty()) {
		Node* t = q.front();
		q.pop();
		if (t == nullptr) {
			std::cout << std::endl;
			q.push(nullptr);
			continue;
		}
		for (size_t i = 0; i < t->sz; i++)
		{
			std::cout << " " << t->keys[i];
			q.push(static_cast<Node*>(t->children[i]));
		}
		q.push(static_cast<Node*>(t->children[t->sz]));

		std::cout << "\t";
	}
}
