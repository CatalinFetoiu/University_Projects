//314CB Fetoiu Catalin Emil

#include "stiva.h"
#include "coada.h"
#include <math.h>

//functia returneaza diferenta id-urilor pentru doua switch-uri
int cmpId(void* s1, void* s2)
{
	int a = (*((TSwitch*)(s1))).id;
	int b = (*((TSwitch*)(s2))).id;
	return a - b;
}

//functia converteste o adresa ipv4 la un numar
unsigned long int conversie(char* ip)
{

	//se copiaza sirul primit ca parametru pentru a nu fi pierdut
	//dupa apelul functiei strtok
	char* aux = strdup(ip);
	if(aux == NULL)
		return 0;
	
	int nr = 24;
	unsigned long int result = 0;

	//sunt extrasi din adresa octetii (separati de punct), sunt convertiti
	//cu ajutorul functiei atoi si este incrementat rezultatul
	
	char* p = strtok(aux, ".");
	while(p != NULL)
	{
		result += atoi(p) * pow(2, nr);
		nr -= 8;
		p = strtok(NULL, ".");
	}
	free(aux);
	
	return result;
}

//functia returneaza ip-ul elementului de la inceputul cozii,
//acesta reprezentand ip-ul minim deoarece coada este ordonata
unsigned long int ipmin(TCoada* c)
{
	TSwitch sw = *((TSwitch*)(c->first->info));

	unsigned long int nr = conversie(sw.IP);
	return nr;
}

//functia adauga un switch in retea tinand cont de modul sau de functionare
void add(TStiva* s, TCoada* c, TSwitch sw, char* functionare)
{

	//daca switch-ul functioneaza in mod individual acesta este
	//inserat ordonat in functie de IP in coada
	if(strcmp(functionare, "SINGLE") == 0)
	{
		
		//in cazul in care coada este vida
		if(VidaC(c))
		{
			Inserare(c, sw);
			return;
		}

		TSwitch first;		
	
		TCoada* aux = InitC();
		if(aux == NULL)
			return;
		
		/*se extrag elemente din coada intr-o coada auxiliara pana cand
		  se intalneste primul switch fata de care switch-ul ce urmeaza
		  a fi inserat are IP-ul mai mic */

		while(!VidaC(c))
		{
			//primul element din coada
			first = *((TSwitch*)(c->first->info));
			if(conversie(sw.IP) < conversie(first.IP))
				break;
			
			Extragere(c, aux);
		}
		
		TCoada* aux1 = InitC();
		if(aux1 == NULL)
			return;
		
		//este inserat elementul in coada cu ajutorul unei cozi auxiliare
		Inserare(aux1, sw);
		ConcatQ(c, aux1);
		ConcatQ(aux1, aux);
		ConcatQ(aux, c);

		free(aux);
		free(aux1);
	}

	//daca switch-ul functioneaza in mod stiva
	else
	{
		//daca stiva este vida
		if(VidaS(s))
		{
			Push(s, sw);
			return;
		}

		//daca stiva are un singur element
		if(s->vf->next == NULL)
		{
			TStiva* aux = InitS();
			if(aux == NULL)
				return;
			
			//se insereaza noul element in interiorul stivei sau la baza
			if(strcmp(functionare, "BASE") == 0)
			{
				Pop(s, aux);
				Push(s, sw);
				Rastoarna(aux, s);
			}
			
			if(strcmp(functionare, "NON_BASE") == 0)
				Push(s, sw);
			
			free(aux);
			return;
		}  

		//daca elementul trebuie inserat in interiorul stivei
		if(strcmp(functionare, "NON_BASE") == 0)
		{
			TStiva* aux = InitS();
			if(aux == NULL)
				return;
			
			/*pe acelasi principiu ca si in cazul cozii, sunt extrase
			  elemente din stiva pana cand se poate insera noul switch */
			
			//stiva este parcursa pana la penultimul element
			while(s->vf->next != NULL)
			{
				if(cmpId(&sw, s->vf->info) <= 0)
					break;
				
				Pop(s, aux);
			}
		
			Push(s, sw);
			Rastoarna(aux, s);
			free(aux);
		}
	
		//daca elementul trebuie inserat la baza stivei
		if(strcmp(functionare, "BASE") == 0)
		{
			TStiva* aux = InitS();
			if(aux == NULL)
				return;
			TStiva* aux1 = InitS();
			if(aux1 == NULL)
			{
				free(aux1);
				return;
			}
			
			//se extrag toate elementele din stiva si se insereaza la baza
			//noul switch
			while(!VidaS(s))
				Pop(s, aux);
			
			Push(s, sw);
			
			//este retinut elementul ce se afla la baza stivei inainte
			//de inserare si este apoi inserat ordonat in noua stiva
			Pop(aux, aux1);
			
			Rastoarna(aux, s);
			
			TSwitch* ins = (TSwitch*)(aux1->vf->info);
			while(s->vf->next != NULL)
			{
				if(cmpId(ins, s->vf->info) <= 0)
					break;
				
				Pop(s, aux);
			}
	
			Rastoarna(aux1, s);
			Rastoarna(aux, s);
			free(aux1);
			free(aux);
		}
	}
}

//functia elimina din retea switch-ul cu un id dat
void del(int n, TStiva** stive, TCoada* c, int id)
{
	int ok = 0;
	
	//se cauta elementul in coada
	if(!VidaC(c))
	{
		TCoada* aux1 = InitC();
		if(aux1 == NULL)
			return;
		TCoada* aux2 = InitC();
		if(aux2 == NULL)
		{
			free(aux1);
			return;
		}

		//se extrag elemente din coada pana cand este gasit cel cu
		//id-ul cautat
		while(!VidaC(c))
		{
			if((*((TSwitch*)(c->first->info))).id == id)
				break;
		
			Extragere(c, aux1);
		}

		//daca s-a gasit elementul in coada acesta este extras
		if(!VidaC(c))
		{
			Extragere(c, aux2);
			DistrugereCoada(aux2);
			ok = 1;
		}
		else
			free(aux2);
		
		//este refacuta coada initiala
		ConcatQ(c, aux1);
		ConcatQ(aux1, c);
		free(aux1);
		
		//daca elementul a fost gasit atunci nu mai este cautat in stive
		if(ok == 1)
			return;
	}
	
	//este cautat elementul in cele n stive
	int i;
	for (i = 0; i < n; i++)
	{
		TStiva* s = stive[i];
		
		if(!VidaS(s))
		{
			TStiva* s1 = InitS();
			if(s1 == NULL)
				return;
			TStiva* s2 = InitS();
			if(s2 == NULL)
			{
				free(s1);
				return;
			}

			//se extrag elemente din stiva pana cand se gaseste id-ul
			//cautat
			while(!VidaS(s))
			{
				if((*((TSwitch*)(s->vf->info))).id == id)
					break;
			
				Pop(s, s1);
			}

			//daca switch-ul a fost gasit, este eliminat din retea
			if(!VidaS(s))
			{
				Pop(s, s2);
				DistrugereStiva(s2);
				ok = 1;
			}
			else
				free(s2);
		
			Rastoarna(s1, s);
			free(s1);
			if(ok == 1)
				break;
		}  
	}
}

//functia schimba modul de functionare al unui switch cu un id dat
void set(int n, TStiva** stive, int dest, TCoada* c, int id, char* functionare)
{
	int ok = 0;
	
	//se cauta elementul in coada la fel ca in cazul functiei del
	if(!VidaC(c))
	{
		TCoada* aux = InitC();
		if(aux == NULL)
			return;
		
		while(!VidaC(c))
		{
			if((*((TSwitch*)(c->first->info))).id == id)
				break;
		
			Extragere(c, aux);
		}

		//daca nu s-a gasit elementul in coada, aceasta este refacuta
		if(VidaC(c))
		{
			ConcatQ(c, aux);
			ConcatQ(aux, c);
		}
		
		//daca s-a gasit elementul in coada
		else
		{
			//daca noul mod de functionare nu trebuie schimbat coada este
			//refacuta
			if(strcmp(functionare, "SINGLE") == 0)
			{
				ConcatQ(c, aux);
				ConcatQ(aux, c);
			}

			/*daca modul de functionare trebuie schimbat, switch-ul este
			  eliminat din coada si inserat din nou in retea cu ajutorul
			  functiei add */
			else
			{
				TSwitch sw = *((TSwitch*)(c->first->info));
				TSwitch sw1 = AlocareSwitch(sw.id, sw.nume, sw.IP, sw.functionare);

				TCoada* aux2 = InitC();
				if(aux2 == NULL)
					return;
				Extragere(c, aux2);
			
				DistrugereCoada(aux2);
				
				ConcatQ(c, aux);
				ConcatQ(aux, c);

				add(stive[dest], c, sw1, functionare);
			}

			ok = 1;
		}
		free(aux);
	}

	//daca elementul a fost gasit in coada nu mai este cautat in stive
	if(ok == 1)
		return;

	//este cautat switch-ul in cele n stive
	int i;
	for(i = 0; i < n; i++)
	{
		TStiva* s = stive[i];
	
		if(!VidaS(s))
		{
			TStiva* st = InitS();
			if(st == NULL)
				return;
			
			//este cautat switch-ul la fel ca in functia del
			while(!VidaS(s))
			{
				if((*((TSwitch*)(s->vf->info))).id == id)
					break;
			
				Pop(s, st);
			}

			if(VidaS(s))
				Rastoarna(st, s);
			
			//daca s-a gasit elementul in stiva este eliminat si apoi inserat
			//in retea cu noua functionalitate cu ajutorul functiei add
			else
			{
				TSwitch sw = *((TSwitch*)(s->vf->info));
				TSwitch sw1 = AlocareSwitch(sw.id, sw.nume, sw.IP, sw.functionare);
				
				TStiva* st2 = InitS();
				if(st2 == NULL)
					return;
		
				Pop(s, st2);
				DistrugereStiva(st2);

				Rastoarna(st, s);
			
				add(stive[dest], c, sw1, functionare);
				ok = 1;
			}
			free(st);
		}
		
		//daca switch-ul a fost gasit nu mai este cautat in celelalte stive
		if(ok == 1)
			break;
	}
}

//functia afiseaza informatiile din switch-urile din retea
void show(int n, TStiva** stive, TCoada* c, FILE* g)
{

	//sunt afisate elementele cozii
	if(c->first == NULL)
		fprintf(g, "{}\n");
	else
	{
		fprintf(g, "{");
		AfisareC(c, g);
		fprintf(g, "}\n");
	}
	
	//sunt afisate elementele din fiecare stiva
	int i;
	for (i = 0; i < n; i++)
	{
		TStiva* s = stive[i];

		if(s->vf == NULL)
			fprintf(g, "%d:\n", i);
		else
		{
			fprintf(g, "%d:\n", i);
			AfisareS(s, g);
		}
	}
}
	
	
int main(int argc, char* argv[])
{
	//sunt deschise pentru citire, respectiv scriere, fisierele primite
	//ca parametrii in linia de comanda
	FILE* f = fopen(argv[1], "r");
	FILE* g = fopen(argv[2], "w");

	//este citit numarul de stive din retea
	int N;
	fscanf(f, "%d", &N);

	TStiva** stive = malloc(N * sizeof(TStiva*));
	if(stive == NULL)
		return 0;

	//se aloca memorie pentru stivele din retea
	int i, j;
	for (i = 0; i < N; i++)
	{
		stive[i] = InitS();
		if(stive[i] == NULL)
		{
			for(j = 0; j < i; j++)
				free(stive[j]);
			free(stive);
		}
	}

	//se aloca memorie pentru coada
	TCoada* c = InitC();
	if(c == NULL)
		return 0;

	char* buf = malloc(10 * sizeof(char));
	char* nume = malloc(40 * sizeof(char));
	char* principal = malloc(10 * sizeof(char));
	char* IP = malloc(40 * sizeof(char));
	char* functionare = malloc(10 * sizeof(char));
	
	int numar;
	int id;
	TSwitch sw;
	
	//se citesc operatiile din fisierul de input si se aplica asupra
	//switch-urilor din retea
	while(fscanf(f, "%s", buf) == 1)
	{
		if(strcmp(buf, "add") == 0)
		{
			fscanf(f, "%d", &id);
			fscanf(f, "%s", nume);
			fscanf(f, "%s", IP);
			fscanf(f, "%s", functionare);

			sw = AlocareSwitch(id, nume, IP, functionare);
			
			if(strcmp(functionare, "SINGLE") == 0)
				add(stive[0], c, sw, "SINGLE");

			if(strcmp(functionare, "STACK") == 0)
			{
				fscanf(f, "%d", &numar);
				fscanf(f, "%s", principal);
				add(stive[numar], c, sw, principal);
			}
		}

		if(strcmp(buf, "del") == 0)
		{
			fscanf(f, "%d", &id);
			del(N, stive, c, id);
		}

		if(strcmp(buf, "set") == 0)
		{
			fscanf(f, "%d", &id);
			fscanf(f, "%s", functionare);
			
			if(strcmp(functionare, "SINGLE") == 0)
				set(N, stive, 0, c, id, "SINGLE");
			
			if(strcmp(functionare, "STACK") == 0)
			{
				fscanf(f, "%d", &numar);
				fscanf(f, "%s", functionare);
				set(N, stive, numar, c, id, functionare);
			}
		}

		if(strcmp(buf, "ipmin") == 0)
		{
			if(VidaC(c))
				fprintf(g, "ipmin=0\n");
			else
				fprintf(g, "ipmin=%lu\n", ipmin(c));
		}
		
		if(strcmp(buf, "show") == 0)
		{
			show(N, stive, c, g);
			fprintf(g, "\n");
		}  
	}

	free(buf);
	free(nume);
	free(IP);
	free(principal);
	free(functionare);

	//se inchid fisierele din input si output
	fclose(f);
	fclose(g);

	//se elibereaza memoria alocata pentru stivele din retea
	for (i = 0; i < N; i++)
		DistrugereStiva(stive[i]);
	
	free(stive);

	//se elibereaza memoria alocata pentru coada
	DistrugereCoada(c);
	
	return 0;
}
