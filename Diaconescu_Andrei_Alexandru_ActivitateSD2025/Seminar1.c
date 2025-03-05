#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct Masina {
	int id;
	int kmRulati;
	char* model;
	float pret;
	char tipMotor;//b, d, e
};

struct Masina initializare(int _id, int _kmRulati, const char* _model, float _pret, char _tipMotor) {
	struct Masina m;
	m.id = _id;
	m.kmRulati = _kmRulati;

	m.model = (const char*)malloc((sizeof(const char) * strlen(_model) + 1));
	strcpy_s(m.model, strlen(_model) + 1, _model);

	m.pret = _pret;
	m.tipMotor = _tipMotor;

	return m;
}


void afisare(const struct Masina m)
{
	printf_s("ID: %d\nkmRulati: %d\nmodel: %s\npret: %f \ntipMotor: %c\n", m.id, m.kmRulati, m.model, m.pret, m.tipMotor);
}

void modificaPret(struct Masina* m, float pret)
{
	(*m).pret = pret;
}


void dezalocare(struct Masina* m)
{
	free(*m->model);
	(*m).model = NULL;

	free(m);
}


int main() {
	struct Masina masina;
	masina = initializare(1, 50, "Logan", 2499.99, 'd');
	afisare(masina);
	modificaPret(&masina, 5000);
	afisare(masina);
	dezalocare(&masina);


}