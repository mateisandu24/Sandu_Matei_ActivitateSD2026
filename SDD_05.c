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

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;


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

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->masina);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {

	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->masina = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = NULL;
	if ((*cap) == NULL)
	{
		nodNou->prev = NULL;
		*cap = nodNou;
		return;
	}
	Nod* temp = *cap;
	while (temp->next) {
		temp = temp->next;
	}
	temp->next = nodNou;
	nodNou->prev = temp;

}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua)
{
	if ((*cap) == NULL) {
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->masina = masinaNoua;
		nodNou->next = NULL;
		nodNou->prev = NULL;
		*cap = nodNou;
		return;
	}

	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->masina = masinaNoua;
	nodNou->next = *cap;
	nodNou->prev = NULL;
	(*cap)->prev = nodNou;
	*cap = nodNou;

}

void* citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;
	while (!feof(file)) {
		Masina m1 = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&cap, m1);
	}
	return cap;
}



void dezalocareLDMasini(Nod** temp) {
	while ((*temp))
	{
		(*temp)->masina.model = NULL;
		(*temp)->masina.numeSofer = NULL;
		Nod* nou = *temp;
		nou = nou->next;
		free(*temp);
		(*temp) = nou;
	}

}

float calculeazaPretMediu(Nod* cap) {
	float pretMediu = 0;
	int nrMasini = 0;
	while (cap) {
		pretMediu += cap->masina.pret;
		nrMasini++;
		cap = cap->next;
	}
	return pretMediu / nrMasini;
}

void stergeMasinaDupaID(Nod** cap, int id) {
	Nod* curent = *cap;
	while (curent) {
		if (curent->masina.id == id) {

			if (curent->prev) {
				curent->prev->next = curent->next;
			}
			else {
				*cap = curent->next;
			}


			if (curent->next) {
				curent->next->prev = curent->prev;
			}

			free(curent->masina.model);
			free(curent->masina.numeSofer);
			free(curent);
			return;
		}
		curent = curent->next;
	}
}


char* getNumeSoferMasinaScumpa(Nod* cap) {
	char* numeSofer = NULL;
	float maxim = 0;
	while (cap) {
		if (cap->masina.pret > maxim) {
			maxim = cap->masina.pret;
			free(numeSofer);
			numeSofer = (char*)malloc(strlen(cap->masina.numeSofer) + 1);
			strcpy(numeSofer, cap->masina.numeSofer);
		}
		cap = cap->next;
	}
	return numeSofer;
}

int main() {

	printf("Citirea listei din fisier:\n");
	Nod* cap = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);

	printf("Adaugare la inceput:\n");
	adaugaLaInceputInLista(&cap, (Masina) { 100, 4, 12000.0f, "Sandero", "Ana", 'D' });
	afisareListaMasini(cap);

	printf("Adaugare la final:\n");
	adaugaMasinaInLista(&cap, (Masina) { 100, 4, 12000.0f, "Sandero", "Ana", 'D' });
	afisareListaMasini(cap);

	printf("Calcul pret mediu:\n");
	float pretMediu = calculeazaPretMediu(cap);
	printf("Pretul mediu este: %1.2f\n\n", pretMediu);

	printf("Nume sofer masina cea mai scumpa:\n");
	char* soferScump = getNumeSoferMasinaScumpa(cap);
	printf("Soferul cu masina cea mai scumpa este: %s\n\n", soferScump);
	free(soferScump);

	printf("Stergere masina dupa ID: %d\n", 1);
	stergeMasinaDupaID(&cap, 1);
	afisareListaMasini(cap);

	printf("Dezalocare lista:\n");
	dezalocareLDMasini(&cap);
	afisareListaMasini(cap);

	return 0;
}
