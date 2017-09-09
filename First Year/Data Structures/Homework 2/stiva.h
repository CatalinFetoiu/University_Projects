//Functiile pentru structura de tip stiva

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "switch.h"

#ifndef stiva
#define stiva

//structura pentru stiva
typedef struct stiva
{
	TLG vf;

} TStiva;

//functia verifica daca o stiva este vida
int VidaS(TStiva* s)
{
	if(s->vf == NULL)
		return 1;
	else
		return 0;
}

//functia initializeaza o stiva
TStiva* InitS()
{
	TStiva* s = malloc(sizeof(TStiva));
	if(s == NULL)
		return NULL;
	
	//varful stivei este initializat cu NULL pentru a putea fi
	//verificata conditia de stiva vida
	s->vf = NULL;

	return s;
}

//functia aloca o celula avand informatia un switch, celula fiind apoi
//inserata in stiva primita ca parametru
void Push(TStiva* s, TSwitch elem)
{
	TLG aux = s->vf;
	
	TLG cell = AlocareCelula(elem);
	if(cell == NULL)
		return;
	
	if(VidaS(s))
	{
		s->vf = cell;
		return;
	}
	
	cell->next = aux;
	s->vf = cell;
}

//functia extrage primul element din stiva sursa in stiva destinatie
void Pop(TStiva* s, TStiva* destinatie)
{
	if(s->vf == NULL)
		return;
	
	TLG ant = s->vf;

	//se avanseaza in stiva sursa
	s->vf = s->vf->next;
	
	//se insereaza celula in stiva destinatie
	ant->next = destinatie->vf;
	destinatie->vf = ant;
}

//functia elibereaza memoria alocata pentru o stiva
void DistrugereStiva(TStiva* s)
{
	if(s->vf == NULL)
	{
		free(s);
		return;
	}
	
	TStiva* aux = InitS();
	if(aux == NULL)
		return;
	
	while(!VidaS(s))
	{
		Pop(s, aux);
		EliberareCelula(aux->vf);
		
		aux->vf = NULL;
	}
	free(aux);	
	
	free(s);
}

//functia rastoarna stiva s1 in stiva s2 prin extrageri succesive
void Rastoarna(TStiva* s1, TStiva* s2)
{
	while(!VidaS(s1))
		Pop(s1, s2);
}

//functia suprapune doua stive s1 si s2
void Suprapune(TStiva* s1, TStiva* s2)
{
	TStiva* aux = InitS();
	if(aux == NULL)
		return;

	Rastoarna(s1, aux);
	Rastoarna(aux, s2);

	DistrugereStiva(aux);
}

	
//functia afiseaza elementele unei stive
void AfisareS(TStiva* s, FILE* g)
{
	TLG a = s->vf;
	if(a == NULL)
	{
		printf("VIDA\n");
		return;
	}

	TStiva* aux = InitS();
	if(aux == NULL)
		return;
	
	//elementele sunt extrase intr-o stiva auxiliara, sunt afisate, fiind
	//inserate apoi in stiva intitiala
	while(!VidaS(s))
	{
		Pop(s, aux);
		AfisareSwitchS(aux->vf->info, g);
	}

	Rastoarna(aux, s);
	free(aux);	
}

#endif


	












