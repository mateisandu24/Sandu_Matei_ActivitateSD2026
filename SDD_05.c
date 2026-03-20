#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;


struct Nod
{
	Masina masina;

	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct LDI
{
	Nod* first;
	Nod* last;
};
typedef struct LDI LDI;

Masina citireMasinaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	char* aux;
	aux = strtok(buffer, sep);

	Masina m1;
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

void afisareMasina(Masina masina)
{
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(LDI lista)
{

	Nod* temp = lista.first;

	while (temp)
	{
		afisareMasina(temp->masina);
		temp = temp->next;
	}
}

void afisareListaMasiniDeLaSfarsit(LDI lista) {

	Nod* temp = lista.last;

	while (temp)
	{
		afisareMasina(temp->masina);
		temp = temp->prev;
	}
}

void adaugaMasinaInLista(LDI* lista, Masina masinaNoua) {

	Nod* nodNou = (Nod*)malloc(sizeof(Nod));

	nodNou->masina = masinaNoua;
	nodNou->prev = lista->last;
	nodNou->next = NULL;

	if (lista->last)
		lista->last->next = nodNou;

	else
		lista->first = nodNou;
	lista->last = nodNou;

}

void adaugaLaInceputInLista(LDI* lista, Masina masinaNoua)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));

	nodNou->masina = masinaNoua;
	nodNou->prev = NULL;
	nodNou->next = lista->first;

	if (lista->first)
		lista->first->prev = nodNou;

	else
		lista->last = nodNou;

	lista->first = nodNou;

}

LDI citireLDMasiniDinFisier(const char* numeFisier) 
{

	FILE* file = fopen(numeFisier, "r");

	LDI lista;
	lista.first = NULL;
	lista.last = NULL;

	while (!feof(file)) 
	{
		Masina m1 = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&lista, m1);
	}

	fclose(file);
	return lista;
}

void dezalocareLDMasini(LDI* lista) 
{

	Nod* cap = lista->first;

	while (cap) 
	{

		free(cap->masina.model);
		free(cap->masina.numeSofer);

		Nod* nodGolit = cap;
		cap = cap->next;
		free(nodGolit);
	}

	lista->first = NULL;
	lista->last = NULL;
}


float calculeazaPretMediu(LDI lista) 
{

	float pretMediu = 0;
	int nrMasini = 0;

	Nod* cap = lista.first;

	while (cap) 
	{

		pretMediu += cap->masina.pret;
		nrMasini++;

		cap = cap->next;
	}

	if (nrMasini)
		return pretMediu / nrMasini;

	return 0;
}

void stergeMasinaDupaID(LDI* lista, int id) 
{

	Nod* curent = lista->first;

	while (curent) 
	{
		if (curent->masina.id == id) 
		{

			if (curent->prev)
				curent->prev->next = curent->next;
			else 
				lista->first = curent->next;
			
			if (curent->next) 
				curent->next->prev = curent->prev;
			else 
				lista->last = curent->prev;
			
			free(curent->masina.model);
			free(curent->masina.numeSofer);
			free(curent);
			
			return;
		}
		curent = curent->next;
	}
}


char* getNumeSoferMasinaScumpa(LDI lista) 
{
	char* numeSofer = NULL;
	float maxim = 0;
	Nod* cap = lista.first;

	while (cap) 
	{
		if (cap->masina.pret > maxim) 
		{
			maxim = cap->masina.pret;

			free(numeSofer);

			numeSofer = (char*)malloc(strlen(cap->masina.numeSofer) + 1);
			strcpy(numeSofer, cap->masina.numeSofer);
		}
		cap = cap->next;
	}
	return numeSofer;
}

Masina creareMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
	//am creeat functia asta pentru ca nu mai suport sa primesc erori de copiere de tip shallow si ca nu se realizeaza deep copy :,)
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;

	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);

	m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
	strcpy(m.numeSofer, numeSofer);

	m.serie = serie;
	return m;
}

int main() {

	printf("Citirea listei din fisier:\n");
	LDI lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	printf("\n");

	printf("Afisare inversa:\n");
	afisareListaMasiniDeLaSfarsit(lista);

	Masina m1 = creareMasina(100, 4, 12000.0f, "Sandero", "Ana", 'D');
	printf("Adaugare la inceput:\n");
	adaugaLaInceputInLista(&lista, m1);
	afisareListaMasiniDeLaInceput(lista);

	Masina m2 = creareMasina(100, 4, 12000.0f, "Sandero", "Ana", 'D');
	printf("Adaugare la final:\n");
	adaugaMasinaInLista(&lista, m2);
	afisareListaMasiniDeLaInceput(lista);

	float pretMediu = calculeazaPretMediu(lista);
	printf("Pretul mediu este: %1.2f\n\n", pretMediu);

	char* soferScump = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu masina cea mai scumpa este: %s\n\n", soferScump);
	free(soferScump);

	printf("Stergere masina dupa ID: %d\n", 1);
	stergeMasinaDupaID(&lista, 1);
	afisareListaMasiniDeLaInceput(lista);

	printf("Dezalocare lista:\n");
	dezalocareLDMasini(&lista);
	afisareListaMasiniDeLaInceput(lista);

	return 0;
}
