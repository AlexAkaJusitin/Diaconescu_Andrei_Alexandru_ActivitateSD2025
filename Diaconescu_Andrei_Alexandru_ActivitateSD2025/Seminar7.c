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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrElemente;
	Masina* masini;

};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;
	heap.lungime = lungime;
	heap.masini = malloc(lungime * sizeof(Masina));
	heap.nrElemente = 0;
	
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	int fiuStanga = pozitieNod * 2 + 1;
	int fiuDreapta = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;
	
	if (fiuStanga < heap.nrElemente && heap.masini[pozMax].id < heap.masini[fiuStanga].id) {
		pozMax = fiuStanga;
	}
	
	if (fiuStanga < heap.nrElemente && heap.masini[pozMax].id < heap.masini[fiuDreapta].id) {
		pozMax = fiuDreapta;
	}

	if (pozMax != pozitieNod) {
		Masina aux = heap.masini[pozMax];
		heap.masini[pozMax] = heap.masini[pozitieNod];
		heap.masini[pozitieNod] = aux;

		if (2 * pozMax + 1 < heap.nrElemente) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul
	Heap h = initializareHeap(10);
	Masina m;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		m = citireMasinaDinFisier(f);
		h.masini[h.nrElemente++] = m;
	}

	for (int i = ((h.nrElemente - 2) / 2); i >= 0; i--) {
		filtreazaHeap(h, i);
	}
	
	fclose(f);

	return h;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	printf("Elementele ascunse sunt:\n");
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	//folosim metoda paharului pentru a aduce elementul de pe ultima pozitie
	//pe prima pozitie si apoi filtram heap-ul
	Masina m = heap->masini[0];
	heap->masini[0] = heap->masini[heap->nrElemente - 1];
	heap->masini[heap->nrElemente - 1] = m;

	heap->nrElemente--;

	for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(*heap, i);
	}
	
	return m;
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
	for (int i = 0; i < heap->nrElemente; i++) {
		free(heap->masini[i].model);
		free(heap->masini[i].numeSofer);
	}
	free(heap->masini);
	heap->masini = NULL;
	heap->nrElemente = 0;
	heap->lungime = 0;
}

int main() {

	Heap h = citireHeapDeMasiniDinFisier("masini_arbore.txt");
	afisareHeap(h);
	//dezalocareHeap(&h);
	printf("--------------------\n");
	//afisareHeap(h);
	{
		Masina m = extrageMasina(&h);
		afisareMasina(m);
	}

	{
		Masina m = extrageMasina(&h);
		afisareMasina(m);
	}

	{
		Masina m = extrageMasina(&h);
		afisareMasina(m);
	}

	printf("----------------\n");

	afiseazaHeapAscuns(h);
	

	
	return 0;
}