#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct Telefon 
{
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Telefon initializare(int id, int RAM, char* producator, float pret, char serie) 
{
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}

struct Telefon copiazaTelefon(struct Telefon t)
{
	struct Telefon telefon=initializare(t.id,t.RAM,t.producator,t.pret,t.serie);
	return telefon;
}

void afisare(struct Telefon t) 
{
	
	printf("%d %dGB RAM, %s, pret: %5.2f, serie: %c\n", t.id, t.RAM, t.producator, t.pret, t.serie);

}

void afisareVector(struct Telefon* vector, int nrElemente) 
{
	
	for (int i = 0;i < nrElemente;i++)
	{
		afisare(vector[i]);
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	
	if (nrElemente < nrElementeCopiate) nrElementeCopiate=nrElemente;

	struct Telefon* vectorNou = malloc(sizeof(struct Telefon) * nrElementeCopiate);	
	
	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = copiazaTelefon(vector[i]);
	}
	
	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) 
{

	for (int i = 0;i < *nrElemente;i++)
	{
		free((*vector)[i].producator);
		(*vector)[i].producator=NULL;
	}
	free(*vector);
	*vector =NULL;
	*nrElemente=0;
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pretMinim, struct Telefon** vectorNou, int* dimensiune) {
	*dimensiune=0;
	for (int i = 0;i < nrElemente;i++)
	{
		if (vector[i].pret>pretMinim)(*dimensiune)++;
	}

	*vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * (*dimensiune));
	int k=0;
	for (int i = 0;i < nrElemente;i++)
	{
		if (vector[i].pret > pretMinim)
		{
			(*vectorNou)[k]=copiazaTelefon(vector[i]);
			k++;
		}
	}

}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
    for (int i = 0;i < nrElemente;i++)
    {
        if (strcmp(vector[i].producator, conditie) == 0)
        {
            return vector[i];
        }
    }

	return initializare(-1,0,"Necunoscut",0.0,'-');

	
}



int main() {

	struct Telefon t;
	t = initializare(3, 4, "Samsung", 1500.00, 'A');
	int nrTelefoane = 3;
	int nrElementeCopiate = 2;
	struct Telefon* telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 2, "Xiaomi", 925.00, 'X');
	telefoane[1] = initializare(2, 6, "Apple", 2200.50, 'B');
	telefoane[2] = t;
	afisareVector(telefoane, nrTelefoane);

	struct Telefon* telefoaneCopiate = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrElementeCopiate);
	afisareVector(telefoaneCopiate, nrElementeCopiate);
	dezalocare(&telefoaneCopiate, &nrElementeCopiate);
	printf("\n");

	printf("Telefoane Scumpe:\n");
	struct Telefon* vectorNou;
	int dimensiune;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane,1000.00,&vectorNou,&dimensiune);
	afisareVector(vectorNou,dimensiune);
	printf("\n");

	printf("Primul telefon Xiaomi gasit:\n");
    afisare(getPrimulElementConditionat(telefoane, nrTelefoane, "Xiaomi"));
    return 0;
}
