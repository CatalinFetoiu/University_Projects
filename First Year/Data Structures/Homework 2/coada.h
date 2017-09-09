//Functiile pentru structura de tip coada

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "switch.h"

#ifndef coada
#define coada

//structura pentru coada
typedef struct coada
{
	TLG first;
	TLG last;

} TCoada;

//functia verifica daca o stiva este vida
int VidaC(TCoada* c)
{
	if(c->first == NULL)
		return 1;
	else
		return 0;
}

//functia initializeaza o coada
TCoada* InitC()
{
	TCoada* c = malloc(sizeof(TCoada));
	if(c == NULL)
		return NULL;
	
	//inceputul si sfarsitul puncteaza la NULL pentru a putea
	//fi verificata conditia de coada vida
	c->first = NULL;
	c->last = NULL;
	
	return c;
}

//functia insereaza un element la sfarsitul unei cozi
void Inserare(TCoada* c, TSwitch elem)
{
	TLG cell = AlocareCelula(elem);
	if(cell == NULL)
		return;

	if(c->first == NULL)
	{
		c->first = cell;
		c->last = cell;
		return;
	}

	c->last->next = cell;
	c->last = cell;
}

//functia extrage elementul de la inceputul unei cozi intr-o coada destinatie
void Extragere(TCoada* c, TCoada* destinatie)
{
	if(c->first == NULL)
		return;

	//inceputul cozii
	TLG first = c->first;
	TLG ant = first;

	//coada sursa are un singur element
	if(c->first == c->last)
	{
		c->first = NULL;
		c->last = NULL;
	}
	else
		c->first = first->next;
	
	ant->next = NULL;

	//coada destinatie este vida
	if(destinatie->first == NULL)
	{
		destinatie->first = ant;
		destinatie->last = ant;
		return;
	}
	
	destinatie->last->next = ant;
	destinatie->last = ant;
}

//functia elibereaza memoria alocata pentru o coada
void DistrugereCoada(TCoada* c)
{
	if(c->first == NULL)
	{
		free(c);
		return;
	}

	TCoada* aux = InitC();
	if(aux == NULL)
		return;

	while(!VidaC(c))
	{
		Extragere(c, aux);
		EliberareCelula(aux->first);
		
		aux->first = NULL;
		aux->last = NULL;
	}
	free(aux);

	free(c);
}

//functia concateneaza doua cozi prin extrageri succesive
void ConcatQ(TCoada* c1, TCoada* c2)
{
	while(!VidaC(c1))
		Extragere(c1, c2);      
}

//functia afiseaza elementele dintr-o coada
void AfisareC(TCoada* c, FILE* g)
{
	TLG a = c->first;
	if(a == NULL)
	{
		printf("VIDA\n");
		return;
	}

	TCoada* aux = InitC();
	if(aux == NULL)
		return;

	//se extrag elementele din coada intr-o coada auxiliara, sunt afisate
	//si apoi inserate in coada initiala
	while(!VidaC(c))
	{
		Extragere(c, aux);
		AfisareSwitchC(aux->last->info, g);
	}

	ConcatQ(aux, c);
	
	free(aux);
}

#endif



		
	







