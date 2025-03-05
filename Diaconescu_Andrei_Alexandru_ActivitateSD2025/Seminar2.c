#include<stdio.h>
#include<malloc.h>
#include <memory.h>
struct Masina {
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.pret = pret;
	m.tipMotor = tipMotor;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d\nnr km: %d\nmodel: %s\npret: %f\ntip motor: %c\n", m.id, m.nrKm, m.model, m.pret, m.tipMotor);

}

void modifica_Pret(struct Masina* m, float pretNou) {
	if (pretNou > 0)
		m->pret = pretNou;

}
void afisareVector(struct Masina* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int* nrElementeCopiate) {

	struct Masina* vectorNou = NULL;

	if (vector == NULL || nrElemente <= 0)
		return vectorNou;

	if ((*nrElementeCopiate) >= nrElemente) {
		*nrElementeCopiate = nrElemente;
	}

	vectorNou = malloc(*nrElementeCopiate * sizeof(struct Masina));
	
	for (int i = 0; i < *nrElementeCopiate; i++) {
		vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
	}

	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	if (*vector != NULL) {
		for (int i = 0; i < *nrElemente; i++) {
			free((*vector)[i].model);
		}

		free(*vector);
		*vector = NULL;
		*nrElemente = 0;
	}
}

void copiazaMasiniIeftine(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	//dimensiunea noului vector este returnata prin intermediul parametrului dimensiune
	
	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++){
		if (vector[i].pret < prag) {
			(*dimensiune)++;
		}
	}

	*vectorNou = malloc(*dimensiune * sizeof(struct Masina));

	int k = 0;

	for (int i = 0; i < nrElemente; i++){
		if (vector[i].pret < prag) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = (char*)malloc(strlen(vector[i].model) + 1);
			strcpy((*vectorNou)[k].model, vector[i].model);
			k++;
		}
	}




}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Masina s;
	s.id = 1;

	return s;
}
	


int main() {
	int nrMasini = 3;
	struct Masina* vectorMasini = (struct Masina*)malloc(nrMasini * sizeof(struct Masina));
	struct Masina* vectorMasiniIeftine = (struct Masina*)malloc(nrMasini * sizeof(struct Masina));

	vectorMasini[0] = initializare(1, 100, "Logan", 1000, 'd');
	vectorMasini[1] = initializare(2, 200, "Audi", 2000, 'b');
	vectorMasini[2] = initializare(3, 300, "BMW", 4000, 'e');
	
	afisareVector(vectorMasini, nrMasini);

	//int nrElementeCopiate = 2;
	//vectorMasini = copiazaPrimeleNElemente(vectorMasini, nrMasini, &nrElementeCopiate);

	printf("------------------------------------------------------\n");

	//afisareVector(vectorMasini, nrElementeCopiate);
	int nrMasiniIeftine = 0;
	copiazaMasiniIeftine(vectorMasini, nrMasini, 3000, &vectorMasiniIeftine, &nrMasiniIeftine);
	printf("%d\n", nrMasiniIeftine);
	afisareVector(vectorMasiniIeftine, nrMasiniIeftine);

	return 0;
}