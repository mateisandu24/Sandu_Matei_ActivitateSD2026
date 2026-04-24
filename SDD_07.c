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
	Heap heap;
	
  heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.masini = (Masina*)malloc(lungime * sizeof(Masina));
	
  return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int left = 2 * pozitieNod + 1;
	int right = 2 * pozitieNod + 2;
	int pozitieMax = pozitieNod;

	if (pozitieMax<heap.nrElemente && heap.masini[left].pret>heap.masini[pozitieMax].pret) {
		pozitieMax = left;
	}

	if (pozitieMax<heap.nrElemente && heap.masini[right].pret>heap.masini[pozitieMax].pret) {
		pozitieMax = right;
	}

	if (pozitieMax != pozitieNod) {

		Masina aux = heap.masini[pozitieMax];
		heap.masini[pozitieMax] = heap.masini[pozitieNod];
		heap.masini[pozitieNod] = aux;
	
		filtreazaHeap(heap, pozitieMax);
	}


}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	
  if(!file) {
		printf("Eroare la deschiderea fisierului!");
		return;
	}
	
  else {
		Heap heap = initializareHeap(10);
	
    while (!feof(file)) {
			heap.masini[heap.nrElemente++] = citireMasinaDinFisier(file);
		}
		
    fclose(file);
		
    for (int i = (heap.nrElemente - 2) / 2 ; i >= 0; i--) {
			filtreazaHeap(heap, i);
		}
		
    return heap;
	}
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	Masina aux;
	
  if (heap->nrElemente <= 0) {
		printf("Heap-ul este gol!");
		
    aux.id = -1;
		
    return aux;
	}
	
  else {
		heap->nrElemente--;
		
    aux = heap->masini[0];
		heap->masini[0] = heap->masini[heap->nrElemente];
		heap->masini[heap->nrElemente] = aux;
		
    filtreazaHeap(*heap, 0);
		
    return aux;
	}
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0;i < heap->lungime;i++) {
		free(heap->masini[i].model);
		free(heap->masini[i].numeSofer); 
	}
	
  free(heap->masini);
  heap->masini = NULL;
	heap->lungime=0;
	heap->nrElemente = 0;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
  
	afisareHeap(heap);
	
  printf("Extrageri");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	
  printf("Heap Ascuns\n");
	afiseazaHeapAscuns(heap);
	
  dezalocareHeap(&heap);
	
  afisareHeap(heap);
	
  return 0;

	return 0;
}
