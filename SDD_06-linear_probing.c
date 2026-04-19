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

struct HashTable {
	int dim;
	Masina* vector;
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
	if (masina.id == -1) {
		printf("Masina invalida sau negasita.\n");
		return;
	}
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

HashTable* initializareHashTable(int dimensiune) {
	HashTable* hash = (HashTable*)malloc(sizeof(HashTable));
	hash->dim = dimensiune;
	hash->vector = (Masina*)malloc(sizeof(Masina) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		hash->vector[i].id = -1;
		hash->vector[i].model = NULL;
		hash->vector[i].numeSofer = NULL;
	}
	return hash;
}

int calculeazaHash(int id, int dimensiune) {
	return (id * 3) % dimensiune;
}

void deepCopy(Masina* dest, Masina src) {
	dest->id = src.id;
	dest->nrUsi = src.nrUsi;
	dest->pret = src.pret;
	dest->serie = src.serie;

	dest->model = (char*)malloc(strlen(src.model) + 1);
	strcpy(dest->model, src.model);

	dest->numeSofer = (char*)malloc(strlen(src.numeSofer) + 1);
	strcpy(dest->numeSofer, src.numeSofer);
}

void inserareMasinaInTabela(HashTable** hashPtr, Masina galerie) {
	HashTable* hash = *hashPtr;
	int hashCode = calculeazaHash(galerie.id, hash->dim);
	int inserat = 0;

	for (int i = 0; i < hash->dim; i++) {
		int index = (hashCode + i) % hash->dim;
		if (hash->vector[index].id == -1) {
			deepCopy(&(hash->vector[index]), galerie);
			inserat = 1;
			break;
		}
	}

	if (!inserat) {
		int dimensiuneNoua = hash->dim * 2;
		HashTable* hashNou = initializareHashTable(dimensiuneNoua);

		for (int i = 0; i < hash->dim; i++) {
			if (hash->vector[i].id != -1) {
				inserareMasinaInTabela(&hashNou, hash->vector[i]);
				free(hash->vector[i].model);
				free(hash->vector[i].numeSofer);
			}
		}

		inserareMasinaInTabela(&hashNou, galerie);

		free(hash->vector);
		free(hash);
		*hashPtr = hashNou;
	}
}

HashTable* citireMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) return NULL;

	HashTable* hash = initializareHashTable(4);
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		if (m.id != -1) {
			inserareMasinaInTabela(&hash, m);
			free(m.model);
			free(m.numeSofer);
		}
	}
	fclose(file);
	return hash;
}

void afisareTabelaDeMasini(HashTable* hash) {
	if (!hash) return;
	for (int i = 0; i < hash->dim; i++) {
		printf("Index nr. %d:\n", i);
		if (hash->vector[i].id != -1) {
			afisareMasina(hash->vector[i]);
		}
		printf("----------------\n");
	}
}

void dezalocareTabelaDeMasini(HashTable** hashPtr) {
	if (!hashPtr || !*hashPtr) return;
	HashTable* hash = *hashPtr;

	for (int i = 0; i < hash->dim; i++) {
		if (hash->vector[i].id != -1) {
			free(hash->vector[i].model);
			free(hash->vector[i].numeSofer);
		}
	}
	free(hash->vector);
	free(hash);
	*hashPtr = NULL;
}

float* calculeazaPreturiMediiPerClustere(HashTable* hash, int* outNrClustere) {
	if (hash == NULL || hash->dim == 0) {
		*outNrClustere = 0;
		return NULL;
	}
	int start_empty = -1;
	for (int i = 0; i < hash->dim; i++) {
		if (hash->vector[i].id == -1) {
			start_empty = i;
			break;
		}
	}
	if (start_empty == -1) {
		*outNrClustere = 1;
		float suma = 0;
		for (int i = 0; i < hash->dim; i++) {
			suma += hash->vector[i].pret;
		}
		float* medii = (float*)malloc(sizeof(float) * 1);
		medii[0] = suma / hash->dim;
		return medii;
	}

	float* medii_temp = (float*)malloc(sizeof(float) * ((hash->dim / 2) + 1));
	int numarClustere = 0;

	int current_size = 0;
	float current_suma = 0;

	for (int count = 0; count <= hash->dim; count++) {
		int index = (start_empty + 1 + count) % hash->dim;

		if (count == hash->dim) {
			if (current_size > 1) {
				medii_temp[numarClustere++] = current_suma / current_size;
			}
			break;
		}

		if (hash->vector[index].id != -1) {
			current_size++;
			current_suma += hash->vector[index].pret;
		}
		else {
			if (current_size > 1) {
				medii_temp[numarClustere++] = current_suma / current_size;
			}
			current_size = 0;
			current_suma = 0;
		}
	}

	*outNrClustere = numarClustere;

	if (numarClustere == 0) {
		free(medii_temp);
		return NULL;
	}

	float* medii_finale = (float*)malloc(sizeof(float) * numarClustere);
	for (int i = 0; i < numarClustere; i++) {
		medii_finale[i] = medii_temp[i];
	}
	free(medii_temp);

	return medii_finale;
}

Masina getMasinaDupaId(HashTable* hash, int id) {
	int hashCode = calculeazaHash(id, hash->dim);

	for (int i = 0; i < hash->dim; i++) {
		int index = (hashCode + i) % hash->dim;

		if (hash->vector[index].id == id) {
			Masina cautare;
			deepCopy(&cautare, hash->vector[index]);
			return cautare;
		}
		if (hash->vector[index].id == -1) {
			break;
		}
	}

	Masina m; m.id = -1;
	return m;
}

int main() {
	printf("Tabela de masini:\n\n");
	HashTable* hash = citireMasiniDinFisier("masini.txt");
	if (!hash) {
		printf("Eroare la citirea fisierului.\n");
		return 1;
	}
	afisareTabelaDeMasini(hash);
	printf("=========================================\n\n");

	int idCautat = 2;
	printf("Gasire masina cu idul %d:\n", idCautat);
	Masina gasita = getMasinaDupaId(hash, idCautat);
	afisareMasina(gasita);
	if (gasita.id != -1) {
		free(gasita.model);
		free(gasita.numeSofer);
	}
	printf("=========================================\n\n");

	printf("Adaugare masina in tabela:\n");
	Masina test = { 10, 4, 20000.0, (char*)"Dacia", (char*)"Ion", 'A' };
	inserareMasinaInTabela(&hash, test);
	afisareTabelaDeMasini(hash);
	printf("=========================================\n\n");

	printf("Calculul mediu de preturi pe cluster:\n\n");
	int numarClustere = 0;
	float* medii = calculeazaPreturiMediiPerClustere(hash, &numarClustere);

	if (medii != NULL) {
		for (int i = 0; i < numarClustere; i++) {
			printf("Cluster valid nr. %d -> Media: %.2f\n", i + 1, medii[i]);
		}
		free(medii);
	}
	else {
		printf("Nu exista clustere (coliziuni) cu mai mult de o masina.\n");
	}
	printf("-----------------------------------------\n");

	dezalocareTabelaDeMasini(&hash);
	printf("Tabela de masini dupa dezalocare:\n\n");
	afisareTabelaDeMasini(hash);
	printf("-----------------------------------------\n");
	printf("EOA - end of algo :D\n");

	return 0;
}
