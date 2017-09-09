//Fetoiu Catalin Emil
//314CB

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//structura pentru elementele listei dublu inlantuite                         
//circulare generice
typedef struct celula
{
	void* info;
	struct celula* next;
	struct celula* prev;

} TCelula, *TLDI, **ALG;

//structura pentru un element de tip site
typedef struct
{
	char* key;
	char* value;
	int frequency;

} TSite;

//functia hash returneaza suma caracterelor ASCII ale sirului key
int fhash(char* key)
{
	int sum = 0;
	int i;
	for(i = 0; i < strlen(key); i++)
		sum += (int)(key[i]);
	
	return sum;
}

//definirea functiei de tip hash
typedef int (*TFHash)(char*);

//structura pentru tabela hash, structura ce contine un camp auxiliar
//max ce retine numarul total de elemente din tabela
typedef struct
{
	int M;
	TFHash fh;
	TLDI* v;
	int max;

} TH;

//functia returneaza lungimea unei liste
int length(TLDI L)
{
	if(L == NULL)
		return 0;

	TLDI inceput = L;
	//daca lista contine un singur element
	if(L->next == L)
		return 1;
	
	L = L->next;
	
	int length = 1;
	while(L != inceput)
	{
		length++;
		L = L->next;
	}
	return length;
}


//functia initializeaza o lista circulara cu un singur element ce
//contine in campul info elementele unei structuri de tip TSite
TLDI AlocareCelula(void* site)
{
	TLDI cell = malloc(sizeof(TCelula));
	if(cell == NULL)
		return NULL;
	
	cell->next = cell;
	cell->prev = cell;
	cell->info = malloc(sizeof(TSite));
	if(cell->info == NULL)
	{
		free(cell);
		return NULL;
	}
	//sunt copiate datele din adresa structurii la adresa celulei
	memcpy(cell->info, site, sizeof(TSite));
	
	return cell;
}

//functia compara campurile key pentru doua celule, fiind folosita
//in functia de inserare ordonata si in functia de stergere
int cmpKey(void* s1, void* s2)
{
	TSite* site1 = (TSite*)s1;
	TSite* site2 = (TSite*)s2;
	return strcmp(site1->key, site2->key);
}

//functia compara campurile frequency pentru doua celule, fiind de asemenea
//folosita in functia de inserare ordonata
int cmpFrequency(void* s1, void* s2)
{
	TSite* site1 = (TSite*)s1;
	TSite* site2 = (TSite*)s2;
	return site1->frequency - site2->frequency;
}

//functia insereaza o celula la inceputul unei liste circulare, modifi-
//cand adresa de inceput a listei
void InserareInceput(ALG aL, TLDI site)
{
	TLDI inceput = *aL;
	TLDI L = *aL;

	//in cazul in care lista este vida
	if(L == NULL)
	{
		*aL = site;
		return;
	}
	
	//in caz contrar elementul este inserat
	TLDI aux = inceput->prev;
	site->prev = aux;
	inceput->prev = site;
	site->next = inceput;
	aux->next = site;
		
	//se modifica inceputul listei
	*aL = site;
}

//functia insereaza o celula la sfarsitul unei liste, folosindu-se
//de functia InserareInceput
void InserareSfarsit(ALG aL, TLDI site)
{
	TLDI L = *aL;
	
	//daca lista este vida
	if(L == NULL)
	{
		*aL = site;
		return;
	}
	
	//in caz contrar se insereaza la inceput si adresa de inceput
	//puncteaza apoi la al doilea element din lista
	InserareInceput(&L, site);
	*aL = L->next;
}


//functia insereaza un element in lista tinand cont de ordonarea
//acesteia (descrescator dupa frecventa si in caz de egalitate 
//crescator alfabetic) si returneaza 1 daca inserarea reuseste
//si 0 in caz contrar
int Inserare(ALG aL, TLDI site)
{
	TLDI L = *aL;
	
	//inserare in lista vida
	if(L == NULL)
	{
		*aL = site;
		return 1;
	}

	TLDI inceput = L;
	
	//daca elementul este gasit in lista functia returneaza 0
	int i;
	for(i = 0; i < length(L); i++)
	{
		if(cmpKey(site->info, L->info) == 0)
			return 0;
		L = L->next;
	}
	
	L = inceput;
	i = 0;
	while(i < length(L))
	{		
		//daca frecventa este acceasi
		if(cmpFrequency(site->info, L->info) == 0)
		{
			//daca numele sunt ordonate alfabetic se insereaza in interiorul
			//listei sau la inceput
			if(cmpKey(site->info, L->info) < 0)
			{
				if(L == inceput)
				{
					InserareInceput(aL, site);
					return 1;
				}
				
				TLDI aux = L->prev;
				L->prev = site;
				site->next = L;
				site->prev = aux;
				aux->next = site;
				return 1;
			}
		}

		//daca frecventa este mai mare se insereaza in interior sau
		//la inceput
		if(cmpFrequency(site->info, L->info) > 0)
		{
			if(L == inceput)
			{
				InserareInceput(aL, site);
				return 1;
			}
			
			TLDI aux = L->prev;
			L->prev = site;
			site->next = L;
			site->prev = aux;
			aux->next = site;
			return 1;
		}
			
		i++;
		L = L->next;
	}

	//daca elementul nu a fost inserat sau gasit in lista atunci el
	//trebuie inserat la finalul listei
	InserareSfarsit(aL, site);
	return 1;
}

//functia elibereaza memoria alocata pentru o celula
void FreeCelula(TLDI celula)
{
	TSite* site = (TSite*)(celula->info);
	free(site->key);
	free(site->value);
	free(site);
	free(celula);
}

//functia elimina un element dintr-o lista in cazul in care acesta este
//gasit si returneaza 0 sau 1 daca stergerea a fost efectuata cu succes
int Stergere(ALG aL, TLDI site)
{
	TLDI L = *aL;
	TLDI inceput = L;
	
	if(L == NULL)
		return 0;
	
	//stergere in cazul listei cu un singur element
	if(length(L) == 1)
	{
		if(cmpKey(L->info, site->info) == 0)
		{
			*aL = NULL;
			
			return 1;
		}
		return 0;
	}
	
	//se sterge celula daca este gasita in lista
	int i = 0;
	while(i < length(L))
	{
		if(cmpKey(L->info, site->info) == 0)
		{

			TLDI aux1 = L->prev;
			TLDI aux2 = L->next;
			aux2->prev = aux1;
			aux1->next = aux2;
		   	
			//se modifica adresa de inceput a listei
			if(L == inceput)
				*aL = L->next;

			return 1;
		}
		i++;
		L = L->next;
	}
	return 0;
}

//functia initializeaza o tabela hash cu M liste dublu inlantuite circulare
TH* IniTH(int M, TFHash fh)
{
	TH* a = calloc(sizeof(TH), 1);
	if(a == NULL) 
		return NULL;
	
	//se initializeaza intrarile din vectorul de liste cu NULL
	a->v = calloc(M, sizeof(TCelula));  
	if(a->v == NULL)
	{
		free(a);
		return NULL;
	}
	
	a->M = M;
	a->fh = fh;
	//valoarea ce retine numarul de elemente din tabela hash este
	//initializata cu 0
	a->max = 0;
	
	return a;
}

//functia scrie in fisierul g campurile value din celulele unei liste L
void print_list(TLDI L, FILE* g)
{
	if(L == NULL)
		fprintf(g, "VIDA");
	else
	{
		int i;
		//este parcursa lista si este scris fiecare element in fisier
		for(i = 0; i < length(L); i++)
		{
			fprintf(g, "(%s)", ((TSite*)(L->info))->value);
			if(i != (length(L) - 1))
				fprintf(g, " ");
			L = L->next;
		}
	}
}

//functia scrie intr-un fisier g campurile value din listele nevide
//ale unei tabele hash, impreuna cu indicii corespunzatori
void print(TH* a, FILE* g)
{
	int i;
	for(i = 0; i < a->M; i++)
	{
		if(a->v[i] != NULL)
		{
			fprintf(g, "%d: ", i);
			print_list(a->v[i], g);
			fprintf(g, "\n");
		}
	}
}

//functia aloca o noua tabela hash cu dublul dimensiunii tabelei initiale
//si insereaza fiecare element din vechea tabela in cea noua, totodata
//elementele din prima tabela fiind sterse
TH* Realoca(TH* a)
{
	//se initializeaza noua tabela hash
	TH* b = IniTH(2 * (a->M), a->fh);
	b->max = a->max;
	
	int i, index;
	for(i = 0; i < (a->M); i++)
	{
		//sunt copiate in noua tabela elementele fiecarei liste nevide
		if((a->v[i]) != NULL)
		{
			TLDI L = a->v[i];
			while(L != NULL)
			{	
				TLDI aux = AlocareCelula(L->info);
				
				//se insereaza pe pozitia din tabela data de functia
				//hash fiecare element
				index = b->fh(((TSite*)(L->info))->key);
				index = index % (b->M);
				Inserare(&(b->v[index]), aux);
				
				//sunt sterse elementele din vechea tabela
				Stergere(&(a->v[i]), L);
				L = a->v[i];
			}
		}
	}
	return b;
}

//functia insereaza in tabela hash un element cu frecventa 0, numele key si
//valorarea value in cazul in care acesta nu este gasit
void set(TH** a, char* key, char* value)
{
	//daca numarul de elemente depaseste dublul lungimii vectorului de liste
	//tabela este realocata
	if(((*a)->max) > 2 * ((*a)->M))
		*a = Realoca(*a);
	
	//indicele listei unde urmeaza a fi inserat elementul
	int i = (*a)->fh(key);
	i = i%((*a)->M);
	
	//se aloca o noua celula cu informatiile key si value
	TSite s;
	s.key = key;
	s.value = value;
	s.frequency = 0;
	TLDI aux = AlocareCelula(&s);
	
	TLDI L = (*a)->v[i];
	int ok;
	
	//daca lungimea listei este mai mare decat a->M este sters ultimul
	//element si este modificat numarul total de elemente al tabelei
	if(length((*a)->v[i]) == ((*a)->M))
	{
		TLDI elim = L->prev;
		ok = Stergere(&((*a)->v[i]), elim);
		if(ok == 1)
			(*a)->max -= 1;
	}
	
	//daca inserarea s-a realizat cu succes atunci numarul total de elemente
	//al tabelei creste
	ok = Inserare(&((*a)->v[i]), aux);
	if(ok == 1)
		(*a)->max += 1;
}

//functia sterge elementul cu numele key dintr-o tabela hash
void delete(TH* a, char* key)
{
	//indicele listei in care este cautat elementul
	int i = a->fh(key);
	i = i%(a->M);

	TSite s;
	s.key = key;
	TLDI aux = AlocareCelula(&s);
	
	//daca elementul a fost sters numarul de elemente din tabela scade
	int ok = Stergere(&(a->v[i]), aux);
	if(ok == 1)
		a->max -= 1;
}

//functia returneaza IP-ul (value) unui site dintr-o lista daca acesta
//este gasit in lista si returneaza NULL in caz contrar
char* reordonare(ALG aL, TLDI site)
{
	if(*aL == NULL)
		return NULL;

	TLDI L = *aL;

	//este cautat elementul in lista
	int i = 0;
	while(i < length(L))  
	{
		if(cmpKey(L->info, site->info) == 0)
			break;
		i++;
		L = L->next;
	}

	//daca elementul a fost gasit
	if(i != length(L))
	{
		//se aloca o celula cu aceeasi informatie
		TLDI aux = AlocareCelula(L->info);
		
		TLDI elim = L;
		//elementul este sters din lista
		Stergere(aL, elim);
	
		//este incrementata frecventa si este inserata celula alocata in lista
		((TSite*)(aux->info))->frequency++;

		Inserare(aL, aux);
		return ((TSite*)(L->info))->value;
	}
	else
		return NULL;
}

//functia incrementeaza frecventa elementului cu numele key din tabela hash
//si reordoneaza lista
void get(TH* a, char* key, FILE* g)
{
	TSite s;
	s.key = key;
	TLDI aux = AlocareCelula(&s);
	
	int i = a->fh(key);
	i = i % (a->M);

	char* sir = reordonare(&(a->v[i]), aux);
	
	//este scris IP-ul site-ului in fisierul g
	if(sir == NULL)
		fprintf(g, "NULL\n");
	else
		fprintf(g, "%s\n", sir);
}

//functia elibereaza memoria alocata pentru o tabela hash si pentru
//celulele din listele acesteia
void DistrugereHash(TH* a)
{
	int i, j;
	for(i = 0; i < a->M; i++)
	{
		if(a->v[i] != NULL)
		{
			TLDI L = a->v[i];
			
			for(j = 0; j < length(L); j++)
			{
				//se retine pozitia urmatoare din lista pentru a nu fi pierdute
				//legaturile odata cu apelarea functiei FreeCelula
				TLDI aux = L;
				L = L->next;
				FreeCelula(aux);
			}
		}
	}
	
	//se elibereaza memoria alocata pentru vectorul de liste
	free(a->v[i]);
}
	


int main(int argc, char *argv[])
{	
	int M = atoi(argv[1]);
	
	//se aloca o tabela cu dimensiunea data ca argument programului
	TH* a = IniTH(M, fhash);
	
	char buf[1000];
	char* key;
	char* value;
	int index;
	
	//sunt deschise pentru citire, respectiv scriere fisierul de input si cel
	//de output
	FILE* f = fopen(argv[2], "r");
	FILE* g = fopen(argv[3], "w");
	
	//se citesc datele din fisierul de input si se aplica operatia ceruta
	//asupra tabelei hash
	while(fscanf(f, "%s", buf) == 1)
	{
		if(strcmp(buf, "set") == 0)
		{
			fscanf(f, "%s", buf);
			key = strdup(buf);
			fscanf(f, "%s", buf);
			value = strdup(buf);
			set(&a, key, value);
		}
	
		if(strcmp(buf, "get") == 0)
		{
			fscanf(f, "%s", buf);
			key = strdup(buf);
			get(a, key, g);
		}

		if(strcmp(buf, "remove") == 0)
		{
			fscanf(f, "%s", buf);
			key = strdup(buf);
			delete(a, key);
		}

		if(strcmp(buf, "print") == 0)
			print(a, g);

		if(strcmp(buf, "print_list") == 0)
		{
			fscanf(f, "%d", &index);
			//se verifica daca indicele exista in vectorul de liste al tabelei
			if(index >= 0 && index < a->M)
			{
				fprintf(g, "%d: ", index);
				print_list(a->v[index], g);
				fprintf(g, "\n");
			}
		}
	}
	
	fclose(f);
	fclose(g); 

	//se elibereaza memoria alocata pentru tabela hash
	DistrugereHash(a);
	
	return 0;
}









	
