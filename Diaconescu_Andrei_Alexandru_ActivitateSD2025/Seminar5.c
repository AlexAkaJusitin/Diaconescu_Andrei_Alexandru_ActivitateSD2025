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

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};

typedef struct Nod Nod;

//creare structura pentru Lista Dubla 
struct ListaDubluInlantuita {
	Nod* prim;
	Nod* ultim;
};

typedef struct ListaDubluInlantuita LD;

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

void afisareListaMasiniDeLaInceput(LD list) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* aux = list.prim;
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->next;
	}
}

void adaugaMasinaInListaLaSfarsit(LD* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->ultim;

	if (lista->ultim) {
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}

	lista->ultim = nou;

}

void adaugaLaInceputInLista(LD* list, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru

}

LD citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	while (!feof(f))
	{
		Masina aux = citireMasinaDinFisier(f);
		adaugaMasinaInListaLaSfarsit(&lista, aux);
	}

	fclose(f);
	return lista;
}

void dezalocareLDMasini(LD* list) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	while (list->prim != NULL) {
		Nod* aux = list->prim;
		free(aux->info.numeSofer);
		free(aux->info.model);
		list->prim = list->prim->next;
		free(aux);
	}
	list->ultim = NULL;

}

float calculeazaPretMediu(LD list) {
	//calculeaza pretul mediu al masinilor din lista.

	float sum = 0;
	int contor = 0;

	Nod* temp = list.prim;
	while (temp)
	{
		sum += temp->info.pret;
		contor++;
		temp = temp->next;
	}
	
	if (contor == 0)return 0;
	return (float)sum / contor;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(LD list) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	if (list.prim == NULL) return NULL;

	Nod* max = list.prim;
	list.prim = list.prim->next;
	
	while (list.prim)
	{
		if (list.prim->info.pret > max->info.pret)
		{
			max = list.prim;
		}
		list.prim = list.prim->next;
	}

	char* nume = (char*)malloc(strlen(max->info.numeSofer) + 1);
	strcpy_s(nume, strlen(max->info.numeSofer) + 1, max->info.numeSofer);

	return nume;
}

int main() {
	LD L = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(L);

	printf("%f", calculeazaPretMediu(L));
	printf("%s", getNumeSoferMasinaScumpa(L));

	dezalocareLDMasini(&L); 

	return 0;
}