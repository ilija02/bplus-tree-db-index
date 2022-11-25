#include "Helpers.h"

int getSelectedMenuOption()
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
	std::cout << "Unos: ";
	std::cin >> selectedOption;
	while (selectedOption < 0 || selectedOption > ZAD1_NR_OPTIONS) {
		std::cout << "Neispravan unos, pokusajte opet ili unesite 0 za kraj\n";
		std::cout << "Unos: ";
		std::cin >> selectedOption;
	}
	//system("cls");
	return selectedOption;

}