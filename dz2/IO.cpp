#include "BPlusTree.h"
#include "Helpers.h"


BPlusTree* BPlusTree::FromFile(size_t m, std::string fname)
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
		d->ca_id = std::stoll(tmp_s)%10000;
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
		//b->Print();
	}
	infile.close();
	return b;
}

void BPlusTree::Print()
{
	if (this->root == nullptr) return;
	std::queue<Node*> q;
	q.push(this->root);
	while (!q.empty()) {
		int cnt = q.size();
		while (cnt) {
			Node* t = q.front();
			q.pop();
			for (size_t i = 0; i < t->sz; i++)
			{
				if (!t->isLeaf) q.push((t->p.subtree[i]));
			}
			if (!t->isLeaf) q.push(t->p.subtree[t->sz]);
			t->printNodeKeys();
			std::cout << "-";
			cnt--;
		}
		std::cout << std::endl << std::endl;

	}
}

void helpers::writeNodeToFile(Data* d, std::ofstream& out)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << d->ca_id << "|" << d->ca_b_id << "|" << d->ca_c_id << "|" << d->ca_name << "|" << d->ca_tax_st << "|" << d->ca_bal << std::endl;
	out << ss.rdbuf();
	return;
}
