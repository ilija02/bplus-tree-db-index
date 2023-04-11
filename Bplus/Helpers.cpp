#include "Helpers.h"

int helpers::getSelectedMenuOption()
{

	int selectedOption;
	std::cout << "\n----------------------------------------------------------------------------------------------\n";
	std::cout << "\t1) Create/Destroy index over CUSTOMER_ACCOUNT table\n\
	2) Print index\n\
	3) Add new record\n\
	4) Delete record\n\
	5) Search record\n\
	6) Search k records\n\
	7) Search k successive records\n\
	8) Delete index from memory\n\
	9) Create empty B+ tree\n\
	0) Exit\n";
	std::cout << "----------------------------------------------------------------------------------------------\n";
	std::cout << "\033[1;33mInput: \033[0m";
	std::cin >> selectedOption;
	while (selectedOption < 0 || selectedOption > ZAD1_NR_OPTIONS) {
		std::cout << "Invalid input, try again or enter 0 to exit\n";
		std::cout << "\033[1;31m Input: \033[0m";
		std::cin >> selectedOption;
	}
	//system("cls");
	return selectedOption;

}

void helpers::mainLoop() {
	int selectedOption;
	BPlusTree* b = nullptr;
	while (selectedOption = getSelectedMenuOption()) {
		switch (selectedOption) {
		case 1: {
			size_t m;
			std::string fname;
			do {
				std::cout << "\tB+ tree order: ";
				std::cin >> m;
			} while (m < 3 || m>10);
			//std::cout << "\tIme fajla: "; TODO: omoguciti unos imena fajla
			
			while (true) {
				std::ifstream in;
				std::cout << "\tFilename (Filepath): ";
				std::cin >> fname;
				in.open(fname);
				if (in) break;
				std::cout << "\033[1;31m\tError, file doesn't exist\033[0m" << std::endl;
			}
			b = BPlusTree::FromFile(m, fname);
			
			break;
		}
		case 2: {
			if (!nullCheckAndError(b)) continue;
			std::cout << "Tree: " << std::endl;
			b->Print();
			break;
		}
		case 3: {
			if (!nullCheckAndError(b)) continue;
			Data* d = new Data();
			std::cout << "\tca_id: ";
			std::cin >> d->ca_id;
			std::cout << "\tca_b_id: ";
			std::cin >> d->ca_b_id;
			std::cout << "\tca_c_id: ";
			std::cin >> d->ca_c_id;
			std::cout << "\tca_name: ";
			std::cin >> d->ca_name;
			std::cout << "\tca_tax_st: ";
			std::cin >> d->ca_tax_st;
			std::cout << "\tca_bal: ";
			std::cin >> d->ca_bal;
			if (!b->Insert(d->ca_id, d)) {
				std::cout << std::endl << "\033[1;31mError, key already exists\033[0m" << std::endl;
				continue;
			}
			//b->Print();
			break;
		}
		case 4: {
			size_t key;
			if (!nullCheckAndError(b)) continue;
			std::cout << "\tPrimary key of the record you want to delete: ";
			std::cin >> key;
			if (!b->Delete(key)) {
				std::cout << std::endl << "\033[1;31m\tError, nonexistent key\033[0m" << std::endl;
				continue;
			}
			break;
		}
		case 5: {
			if (!nullCheckAndError(b)) continue;
			size_t took_steps, idx, key;
			Node* leaf, * junk;
			std::cout << "\tKey to search: ";
			std::cin >> key;
			if (!b->SearchSingle(key, leaf, junk, &took_steps, &idx)) {

				std::cout << std::endl << "\033[1;31mError, key doesn't exist\033[0m" << std::endl;
				continue;
			}
			std::cout << "\tSearch took " << took_steps << " steps. Record: "<<std::endl;
			leaf->printNodeData(idx);
			break;
		}
		case 6: {
			if (!nullCheckAndError(b)) continue;
			size_t k, key, took_steps;
			std::vector<size_t> keys;
			std::cout << "\tNumber of keys to search: ";
			std::cin >> k;
			keys.resize(k);
			std::cout <<"\t"<<k << " space separated values: ";
			for (int i = 0; i < k; i++) {
				std::cin >> key;
				keys[i] = key;
			}
			b->SearchMultiple(keys, &took_steps);
			std::cout << "\tSearch took " << took_steps << " steps." << std::endl;
			break;
		}
		case 7: {
			if (!nullCheckAndError(b)) continue;
			size_t k, startKey, took_steps;
			std::cout << "\tStarting record primary key: ";
			std::cin >> startKey;
			std::cout << "\tNumber of primary keys k: ";
			std::cin >> k;
			b->SearchMultipleSuccessive(startKey, k, &took_steps);
			std::cout << "\tData successfully written to file" << std::endl;
			std::cout << "\tSearch took "<<took_steps<<" steps" << std::endl;
			break;
		}
		case 8: {
			if (!nullCheckAndError(b)) continue;
			delete b;
			b = nullptr;
			break;
		}
		case 9: {
			delete b;
			size_t m;
			do {
				std::cout << "\t B+ tree order: ";
				std::cin >> m;
			} while (m < 3 || m>10);
			b = new BPlusTree(m);
			break;
		}

		}
	}
	delete b;
}

bool helpers::nullCheckAndError(BPlusTree* b)
{
	if (b == nullptr) {
		std::cout << "\033[1;31mGreska, morate prvo ucitati/generisati B+ stablo\033[0m" << std::endl;
		return false;
	}
	return true;
}

