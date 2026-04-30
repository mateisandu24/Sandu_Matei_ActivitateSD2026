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

struct NodArbore {
	Masina masina;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(NodArbore** radacina, Masina masinaNoua) {
	if (*radacina != NULL) {
		if ((*radacina)->masina.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->stanga,masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
	}
	else {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->masina = masinaNoua;
		nou->dreapta= NULL;
		nou->stanga= NULL;
		(*radacina)=nou;
	}
	
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier,"r");
	NodArbore* radacina =NULL;
	if (file) {
		while (!feof(file)) {
			Masina masina = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&radacina,masina);
		}
	}
	fclose(file);
	return radacina;
}

void afisareMasiniDinArbore(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->masina);
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasiniDinArbore(radacina->dreapta);
	}
}

void afisareInOrdine(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareInOrdine(radacina->stanga);
		afisareMasina(radacina->masina);
		afisareInOrdine(radacina->dreapta);
	}
}

void afisarePostOrdine(NodArbore* radacina) {
	if (radacina != NULL) {
	afisarePostOrdine(radacina->stanga);
	afisareInOrdine(radacina->dreapta);
	afisareMasina(radacina->masina);
	}
	
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina != NULL) {
		
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);

		free((*radacina)->masina.numeSofer);
		free((*radacina)->masina.model);
		free(*radacina);

		*radacina = NULL;
	}
	
}

Masina getMasinaByID(NodArbore *radacina, int id) {
	Masina m;
	m.id=-1;
	if (radacina == NULL) {
		return m;
	}
	else if (radacina->masina.id > id) {
			return getMasinaByID(radacina->stanga,id);
	}
	else if (radacina->masina.id < id){
		return getMasinaByID(radacina->dreapta,id);
	}
	else {
		m = radacina->masina;
		m.model = (char*)malloc(strlen(radacina->masina.model)+1);
		strcpy(m.model, radacina->masina.model);
		m.numeSofer = (char*)malloc(strlen(radacina->masina.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->masina.numeSofer);
		return m;
	}
}

int determinaNumarNoduri(NodArbore* radacina) {
	if (radacina != NULL) {
		return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
	}
	else {
		return 0;
	}
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina) {
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(NodArbore* radacina) {
	if (radacina) {
		return radacina->masina.pret + calculeazaPretTotal(radacina->dreapta) + calculeazaPretTotal(radacina->stanga);
	}
	else {
		return 0;
	}
	
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	if (radacina) {
		if (strcmp(radacina->masina.numeSofer, numeSofer) == 0) {
			return radacina->masina.pret + calculeazaPretulMasinilorUnuiSofer(radacina->stanga,numeSofer)+ calculeazaPretulMasinilorUnuiSofer(radacina->dreapta,numeSofer);
		}
		else {
			return calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
		}
	}
	else {
		return 0;
	}
}

int main() {
	NodArbore* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	printf("Afisare in ordine:\n");
	afisareInOrdine(arbore);
	printf("----------------\n");

	printf("Afisare in preordine:\n");
	afisareMasiniDinArbore(arbore);
	printf("----------------\n");

	printf("Afisare in postordine:\n");
	afisarePostOrdine(arbore);
	printf("----------------\n");

	printf("Afisare masina cu id 5:\n");
	afisareMasina(getMasinaByID(arbore,5));
	printf("----------------\n");


	printf("Sunt %d noduri in arbore.\n",determinaNumarNoduri(arbore));
	printf("----------------\n");

	printf("Arborele are inaltimea %d.\n",calculeazaInaltimeArbore(arbore));
	printf("----------------\n");

	printf("Toate masinile valoareaza %2.f um.\n",calculeazaPretTotal(arbore));
	printf("----------------\n");


	printf("Masinile lui %s valoreaza %2.f um.\n","Ionescu",calculeazaPretulMasinilorUnuiSofer(arbore,"Ionescu"));
}
