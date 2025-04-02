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

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod nod;
struct Nod {
	Masina info;
	nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	nod** vector;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}

}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) {
	nod* temp = (nod*)malloc(sizeof(nod));
	temp->info = masinaNoua; //shallow copy
	temp->next = NULL;

	if ((*cap) == NULL) {
		*cap = temp;
	}
	else {
		nod* aux = (*cap);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = temp;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (nod**)malloc(sizeof(nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* cheieNumeSofer, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	// suma
	if (dimensiune < 0 || dimensiune == 0) {
		return -1;
	}

	unsigned int sum = 0;

	for (int i = 0; i < strlen(cheieNumeSofer); i++) {
		sum += cheieNumeSofer[i];
	}

	sum %= dimensiune;

	return sum;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.vector[pozitie] == NULL) {
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina); // nu avem coliziune, adaugam direct
	}
	else {
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina); // avem coliziune, adaugam la capat (se ocupa functia)
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dim);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			printf("Clusterul %d-----------------------------\n", i);
			afisareListaMasini(ht.vector[i]);
			printf("End of cluster-----------------------------\n\n");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	int pozitie = calculeazaHash(numeSofer, ht.dim);

	Masina m;
	m.id = -1;

	if (ht.vector[pozitie] == NULL || pozitie < 0 || pozitie >= ht.dim) {
		printf("Nu exista masina cu numele %s\n", numeSofer);
		return m;
	}
	
	//cautam in lista de pe pozitia pozitie masina cu numele numeSofer
	nod* temp = ht.vector[pozitie];

	while (temp && strcmp(temp->info.numeSofer, numeSofer) != 0) {
		temp = temp->next;
	}

	if (temp != NULL) {
		//deep copy
		m.id = temp->info.id;
		m.nrUsi = temp->info.nrUsi;
		m.pret = temp->info.pret;
		m.model = (char*)malloc(strlen(temp->info.model) + 1);
		strcpy_s(m.model, strlen(temp->info.model) + 1, temp->info.model);
		m.numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(temp->info.numeSofer) + 1, temp->info.numeSofer);
		m.serie = temp->info.serie;
	}

	return m;

}

int main() {
	HashTable hash = citireMasiniDinFisier("Masini.txt", 10);
	afisareTabelaDeMasini(hash);
	
	Masina m = getMasinaDupaCheie(hash, "Popescu");
	afisareMasina(m);

	return 0;
}
