#include "BPlusTree.h"






void BPlusTree::insertInternalNode(size_t key, Node* child, Node* father)
{
	child->father = father;
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
	std::cout << "Splitting internal node when inserting " << key << std::endl;
	std::vector<size_t> keys;
	std::vector<Node*> v_subtree;
	std::vector<size_t>::iterator it;
	//insert new leaf and subtrees into vectors
	keys.insert(keys.end(), &father->keys[0], &father->keys[father->sz]);
	v_subtree.insert(v_subtree.end(), &father->p.subtree[0], &father->p.subtree[m]);
	it = std::upper_bound(keys.begin(), keys.end(), key);
	int dist = std::distance(keys.begin(), it);
	keys.insert(it, key); //insert key in sorted order
	v_subtree.insert(v_subtree.begin() + dist + 1, child);//insert subtree at same position as the key
	//TODO: najverovatnije postoji bug sa mestom insertovanja u subtree vektor
	size_t middleIndex = ceil((double)m / 2); //left subtree can have more nodes
	size_t newKey = keys[middleIndex];
	father->sz = middleIndex;
	Node* newNode = new Node(this->m, false);
	newNode->sz = 0;
	int i, j;
	for (i = middleIndex + 1, j = 0; i < this->m; i++, j++) {
		newNode->keys[j] = keys[i];
		newNode->sz++;
	}
	for (i = middleIndex + 1, j = 0; i < this->m + 1; i++, j++) {
		newNode->p.subtree[j] = v_subtree[i];
	}
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
		newNode->father = newRoot;
		this->Print();
	}


}

BPlusTree* BPlusTree::FromFile(size_t m, const char* fname)
{
	BPlusTree* b = new BPlusTree(m);
	std::ifstream infile;
	infile.open(fname);
	std::string line = "";
	while (std::getline(infile, line)) {
		Data* d = new Data();
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
		b->Print();
		std::cout << "################################################\n\n";
	}
	infile.close();
	return b;
}

bool BPlusTree::Insert(size_t key, Data* data)
{
	this->maxKey = this->maxKey < key ? key : this->maxKey;
	Node* newLeaf = new Node(this->m, true);
	//check if BPlusTree isn't created
	if (this->root == nullptr) {
		root = newLeaf;
		root->keys[0] = key;
		root->p.data[0] = data;
		root->sz = 1;
		return true;
	}
	Node* leaf;
	if (this->SearchSingle(key, leaf)) return false;
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
	v_data.insert(v_data.begin() + dist+1, data);//insert subtree at same position as the key

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
		leaf->father = newRoot;
		newLeaf->father = newRoot;
	}
	else this->insertInternalNode(leaf->keys[leaf->sz - 1], newLeaf, leaf->father);
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
				curr = curr->p.subtree[i];
				break;
			}
		}
		if (i == curr->sz) curr = curr->p.subtree[i]; //if key > all keys in node

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
	while (!q.empty()) {
		int cnt = q.size();
		while (cnt) {
			Node* t = q.front();
			q.pop();
			for (size_t i = 0; i < t->sz; i++)
			{
				std::cout << t->keys[i] << "|";
				if (!t->isLeaf) q.push((t->p.subtree[i]));
			}
			if (!t->isLeaf) q.push(t->p.subtree[t->sz]);

			std::cout << "\t";
			cnt--;
		}
		std::cout << std:: endl;
		
	}
}
