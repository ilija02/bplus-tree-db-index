#include "Helpers.h"

int helpers::getSelectedMenuOption()
{

	int selectedOption;
	std::cout << "\n----------------------------------------------------------------------------------------------\n";
	std::cout << "\t1) Stvaranje i unistavanje indeksa nad tabelom CUSTOMER_ACCOUNT\n\
	2) Ispis indeksa na standardni izlaz\n\
	3) Dodavanje novog zapisa u tabelu\n\
	4) Brisanje zapisa iz tabele\n\
	5) Pretrazivanje jednog podatka\n\
	6) Pretrazivanje k podataka\n\
	7) Pretrazivanje k sukcesivnih podataka\n\
	8) Brisanje indeksa iz memorije\n\
	9) Kreiranje praznog stabla\n\
	0) Kraj rada\n";
	std::cout << "----------------------------------------------------------------------------------------------\n";
	std::cout << "\033[1;33mUnos: \033[0m";
	std::cin >> selectedOption;
	while (selectedOption < 0 || selectedOption > ZAD1_NR_OPTIONS) {
		std::cout << "Neispravan unos, pokusajte opet ili unesite 0 za kraj\n";
		std::cout << "\033[1;31m Unos: \033[0m";
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
				std::cout << "\tRed stabla m: ";
				std::cin >> m;
			} while (m < 3 || m>10);
			//std::cout << "\tIme fajla: "; TODO: omoguciti unos imena fajla
			
			while (true) {
				std::ifstream in;
				std::cout << "\tIme (putanja) fajla: ";
				std::cin >> fname;
				in.open(fname);
				if (in) break;
				std::cout << "\033[1;31m\tGreska, fajl ne postoji\033[0m" << std::endl;
			}
			b = BPlusTree::FromFile(m, fname);
			
			break;
		}
		case 2: {
			if (!nullCheckAndError(b)) continue;
			std::cout << "Ispis stabla: " << std::endl;
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
				std::cout << std::endl << "\033[1;31mGreska, kljuc vec postoji\033[0m" << std::endl;
				continue;
			}
			//b->Print();
			break;
		}
		case 4: {
			size_t key;
			if (!nullCheckAndError(b)) continue;
			std::cout << "\tKljuc koji zelite da obrisete: ";
			std::cin >> key;
			if (!b->Delete(key)) {
				std::cout << std::endl << "\033[1;31m\tGreska, kljuc ne postoji\033[0m" << std::endl;
				continue;
			}
			break;
		}
		case 5: {
			if (!nullCheckAndError(b)) continue;
			size_t took_steps, idx, key;
			Node* leaf, * junk;
			std::cout << "\tKljuc na osnovu kojeg se vrsi pretraga: ";
			std::cin >> key;
			if (!b->SearchSingle(key, leaf, junk, &took_steps, &idx)) {

				std::cout << std::endl << "\033[1;31mGreska, kljuc ne postoji\033[0m" << std::endl;
				continue;
			}
			std::cout << "\tZa pretragu je bilo potrebno " << took_steps << " koraka. Podaci iz cvora: "<<std::endl;
			leaf->printNodeData(idx);
			break;
		}
		case 6: {
			if (!nullCheckAndError(b)) continue;
			size_t k, key, took_steps;
			std::vector<size_t> keys;
			std::cout << "\tBroj kljuceva na osnovu kojih se vrsi pretraga: ";
			std::cin >> k;
			keys.resize(k);
			std::cout <<"\t"<<k << " vrednosti odvojenih blanko znakom: ";
			for (int i = 0; i < k; i++) {
				std::cin >> key;
				keys[i] = key;
			}
			b->SearchMultiple(keys, &took_steps);
			std::cout << "\tZa pretragu je bilo potrebno " << took_steps << " koraka." << std::endl;
			break;
		}
		case 7: {
			if (!nullCheckAndError(b)) continue;
			size_t k, startKey, took_steps;
			std::cout << "\tPrimarni kljuc pocetnog zapisa: ";
			std::cin >> startKey;
			std::cout << "\tBroj primarnih kljuceva k: ";
			std::cin >> k;
			b->SearchMultipleSuccessive(startKey, k, &took_steps);
			std::cout << "\tPodaci uspesno upisani u fajl" << std::endl;
			std::cout << "\tZa pretragu je bilo potrebno "<<took_steps<<" koraka" << std::endl;
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
				std::cout << "\tRed stabla m: ";
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

