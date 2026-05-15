#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina m;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};


void inserareListaP(NodP** graf, Masina m) {
	NodP* nou = malloc(sizeof(NodP));
	nou->m = m;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*graf) {
		NodP* temp = *graf;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareListaS(NodS** cap, NodP* vecin) {
	NodS* nou = malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;
	if (*cap) {
		NodS* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		*cap = nou;
	}
}

NodP* cautaNodDupaID(NodP* graf, int id) {
	NodP* temp = graf;
	while (temp) {
		if (temp->m.id == id) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}


void inserareMuchie(NodP* graf, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(graf, idStart);
	NodP* stop = cautaNodDupaID(graf, idStop);

	if (start != NULL && stop != NULL) {
		inserareListaS(&start->vecini, stop);
		inserareListaS(&stop->vecini, start);
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	if (!f)
	{
		printf("A aparut o eroare la deschiderea fisierului.");
	}

	NodP* graf = NULL;

	while (!feof(f))
	{
		inserareListaP(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);

	return graf;

}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");

	while (!feof(f))
	{
		int idStart = 0;
		int idStop = 0;

		fscanf(f, "%d %d", &idStart, &idStop);

		inserareMuchie(graf, idStart, idStop);
	}
	fclose(f);
}

void dezalocareNoduriGraf(NodP** graf) {
	NodP* temp = *graf;
	while (temp)
	{
		NodS* cap = temp->vecini;
		while (cap)
		{
			NodS* tempS = cap;
			cap = cap->next;
			free(tempS);
		}

		NodP* tempP = temp;
		temp = temp->next;

		free(tempP->m.model);
		free(tempP->m.numeSofer);

		free(tempP);
	}

	*graf = NULL;
}

void afisareListaVecini(NodP* graf, int id)
{
	if (graf == NULL) {
		printf("Graful este gol.\n");
		return;
	}

	NodP* temp = cautaNodDupaID(graf, id);
	NodS* cap = temp->vecini;
	while (cap)
	{
		afisareMasina(cap->info->m);
		cap = cap->next;
	}
}

int main() {
	NodP* graf = NULL; 

	graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);

	printf("Vecinii nodului 3 inainte de dezalocare:\n");
	afisareListaVecini(graf, 3);


	dezalocareNoduriGraf(&graf);
	afisareListaVecini(graf, 3);

	return 0;
}
