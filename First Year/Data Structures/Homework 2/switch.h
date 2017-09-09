//Functiile pentru structura switch si pentru liste simple 
//inlantuite generice

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef switch
#define switch

//structura pentru un switch din retea
typedef struct
{
	int id;
	char* nume;
	char* IP;
	char* functionare;

} TSwitch;

//structura pentru lista simplu inlantuita generica
typedef struct cell
{
	struct cell* next;
	void* info;

} TCelula, *TLG, **ALG;

//functia afiseaza id-ul unui switch din coada
void AfisareSwitchC(void* sw, FILE* g)
{
	TSwitch s = *((TSwitch*)(sw));
	fprintf(g, "%d ", s.id);
}

//functia afiseaza un switch din stiva conform cerintei
void AfisareSwitchS(void* sw, FILE* g)
{
	TSwitch s = *((TSwitch*)(sw));
	fprintf(g, "%d %s %s\n", s.id, s.IP, s.nume);
}

//functia returneaza un switch avand campurile id, nume, IP si
//functionare primite ca parametrii
TSwitch AlocareSwitch(int id, char* nume, char* IP, char* functionare)
{
	TSwitch s;
	s.id = id;
	s.nume = strdup(nume);

	s.IP = strdup(IP);
	if(s.IP == NULL)
		free(s.nume);

	s.functionare = strdup(functionare);
	if(s.functionare == NULL)
	{
		free(s.nume);
		free(s.IP);
	}

	return s;
}

//functia elibereaza memoria alocata pentru un switch
void EliberareSwitch(TSwitch s)
{
	free(s.nume);
	free(s.IP);
	free(s.functionare);
}

//functia aloca o celula cu avand informatia o structura de tip switch
TLG AlocareCelula(TSwitch a)
{
	TLG cell = malloc(sizeof(TCelula));
	if(cell == NULL)
		return NULL;
	
	cell->info = malloc(sizeof(TSwitch));
	if(cell->info == NULL)
	{
		free(cell);
		return NULL;
	}
	
	//sunt copiate datele din adresa structurii la adresa celulei
	memcpy(cell->info, &a, sizeof(TSwitch));

	cell->next = NULL;
	
	return cell;
}

//functia elibereaza memoria alocata pentru o celula din lista
void EliberareCelula(TLG cell)
{
	TSwitch *s = (TSwitch*)(cell->info);

	TSwitch s1 = *s;
	EliberareSwitch(s1);
	
	free(cell->info);
	free(cell);
}

#endif



