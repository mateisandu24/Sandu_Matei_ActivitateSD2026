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

	printf("ID: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);	
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %d\n", masina.serie);
	printf("\n");

}

void afisareVectorMasini(Masina* masini, int nrMasini) {

	if (masini == NULL || nrMasini <= 0)
	{
		printf("Vectorul de masini este gol.\n");
		return;
	}

	for (int i = 0; i < nrMasini; i++) 
	{
		printf("Masina numarul %d:\n", i + 1);
		afisareMasina(masini[i]);
	}

}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {

	Masina* copieMasini = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
	if (!copieMasini)
	{
		printf("Eroare la alocarea memoriei!");
		return;
	}

	for (int i = 0; i < *nrMasini; i++) 
	{
		copieMasini[i] = (*masini)[i];
	}
	copieMasini[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = copieMasini;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	if (!file)
	{
		printf("Eroare la deschiderea fisierului!");
		return;
	}
	Masina masina;
	char linie[255];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";
	masina.id = atoi(strtok(linie, delimitator));
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atof(strtok(NULL, delimitator));
	char *aux = strtok(NULL, delimitator);
	masina.model = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);
	aux = strtok(NULL, delimitator);
	masina.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);
	masina.serie = strtok(NULL, delimitator)[0];
	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {

	FILE* file = fopen(numeFisier, "r");
	if (!file)
	{
		printf("Eroare la deschiderea fisierului!");
		return NULL;
	}
	else
	{
		Masina* vectorMasini = NULL;
		*nrMasiniCitite = 0;
		while (!feof(file))
		{
			Masina masinaCitita = citireMasinaFisier(file);
			if (masinaCitita.id != 0) 
			{
				adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masinaCitita);
			}
		}
		fclose(file);
		return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) 
{
	for (int i = 0; i < *nrMasini; i++) 
	{
		free((*vector)[i].model);
		(*vector)[i].model = NULL;
		free((*vector)[i].numeSofer);
		(*vector)[i].numeSofer = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;

}

int main() 
{
	int *nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);

}
