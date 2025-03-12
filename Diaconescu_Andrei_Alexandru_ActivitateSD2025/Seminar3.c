#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	//afiseaza toate atributele unei masini

	printf("Id: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %5.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
	printf("\n");

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	//afiseaza toate elemente de tip masina din vector
	//prin apelarea functiei afisareMasina()

	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;

	Masina* copieMasini;
	copieMasini = malloc(((*nrMasini) + 1) * sizeof(Masina));

	for (int i = 0; i < *nrMasini; i++) {
		copieMasini[i] = (*masini)[i];
	}

	copieMasini[(*nrMasini)] = masinaNoua;
	(*nrMasini)++;

	free(*masini);

	*masini = copieMasini;


}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	Masina m;

	char linie[100];
	fgets(linie, 100, file);

	printf("Linie citita: %s\n", linie);
	// 1, 3, 5000, Astra, Ionescu, A
	char* token = strtok(linie, ",");
	m.id = atoi(token);

	token = strtok(NULL, ",");
	m.nrUsi = atoi(token);

	token = strtok(NULL, ",");
	m.pret = atof(token);

	token = strtok(NULL, ",");
	m.model = (char*)malloc((strlen(token) + 1) * sizeof(char));

	strcpy(m.model, token);

	token = strtok(NULL, ",");
	m.numeSofer = (char*)malloc((strlen(token) + 1) * sizeof(char));

	strcpy(m.numeSofer, token);

	token = strtok(NULL, ",");
	m.serie = *token;

	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	*nrMasiniCitite = 0;

	FILE* f = fopen(numeFisier, "r");
	
	Masina* masini = (Masina*)malloc(sizeof(Masina));

	while (!feof(f)) {
		Masina m = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}

	fclose(f);
	
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	//ATENTIE - se modifica numarul de masini din vector
	for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}

}

int main() {

	Masina m1 = { 1, 4, 10000, "Audi", "Andrei", 'A' };
	Masina m2 = { 2, 2, 20000, "BMW", "Mihai", 'B' };
	Masina m3 = { 3, 4, 30000, "Mercedes", "Ion", 'C' };
	Masina m4 = { 4, 4, 40000, "Logan", "Andrei", 'D' };
	
	Masina* masini = (Masina*)malloc(3 * sizeof(Masina));

	//adaugaMasinaInVector(&masini, 3, m4);

	FILE* f = fopen("Masini.txt", "r");

	Masina m = citireMasinaFisier(f);

	afisareMasina(m);

	printf("--------------------\n");
	int nrMasiniCitite = 0;
	Masina* masiniCitite = citireVectorMasiniFisier("Masini.txt", &nrMasiniCitite);

	afisareVectorMasini(masiniCitite, nrMasiniCitite);

	dezalocareVectorMasini(&masiniCitite, &nrMasiniCitite);

	afisareVectorMasini(masiniCitite, nrMasiniCitite);
	return 0;
}