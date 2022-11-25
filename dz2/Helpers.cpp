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
	0) Glavni meni\n";
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
	BPlusTree *b = nullptr;
	while (selectedOption = getSelectedMenuOption()) {
		switch (selectedOption) {
		case 1: {
			size_t m;
			std::string fname="CustomerAccount20.txt";
			do {
				std::cout << "\tRed stabla m: ";
				std::cin >> m;
			} while (m < 3 || m>10);
			//std::cout << "\tIme fajla: "; TODO: omoguciti unos imena fajla
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
				std::cout << std::endl<< "\033[1;31mGreska, kljuc vec postoji\033[0m" << std::endl;
				continue;
			}
			b->Print();
			break;
		}
		case 4: {
			std::cout << "Not implemented" << std::endl;
			break;
		}
		case 5: {
			
			break;
		}
		case 6: {
			
			break;
		}
		case 7: {
			
			break;
		}
		}
	}
}


bool helpers::nullCheckAndError(BPlusTree *b)
{
	if (b == nullptr) {
		std::cout << "\033[1;31mGreska, morate prvo ucitati/generisati B+ stablo\033[0m" << std::endl;
		return false;
	}
	return true;
}