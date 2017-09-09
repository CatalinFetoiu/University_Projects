// Fetoiu Catalin Emil 314CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// variabila MAX retine numarul maxim de caractere ale
// unei linii din text
#define MAX 12000

// functia returneaza numarul de aparitii ale lui
// cuv in sirul de caractere p
int operatie1(char *p, char *cuv)
{
	char *t;
	int c = 0;

	// folosind functia strtok impartim sirul in cuvinte separate
	// de delimitatorii din enunt
	t = strtok(p, " \",.'?!\n;");
	while(t != NULL)
	{
		// daca token-ul si cuv sunt egale contorul c creste
		if(strcmp(t, cuv) == 0)
		{
			c++;
		}
		t = strtok(NULL, " \",.'?!\n;");
	}
	return c;
}

// functia permuta un cuvant cu k caractere la dreapta
char *permutare(char *cuv, int k)
{
	int i;
	// sirul aux se construieste ca fiind sirul initial permutat
	char *aux;
	aux = calloc((strlen(cuv) + 1), sizeof(char));
	if(aux == NULL)
		return NULL;

	// permutarea sa face modulo strlen(cuv) deoarece permutarea cu strlen(cuv)
	// litere la dreapta nu schimba cuvantul
	k = k % strlen(cuv);

	// se construiese sirul aux si este returnat de catre functie
	for(i = 0; i < strlen(cuv); i ++)
	{
		aux[(i+k) % (strlen(cuv))] = cuv[i];
	}
	return aux;
}

// functie care sterge primele n caractere ale unui sir s
void strdel(char *s, int n)
{
  	int i;
	char *p = s;
	// fiecare din primele n caractere este sters, sirul fiind mutat la
	// fiecare pas cu o pozitie la stanga
  	for (i = 0; i < n; i++)
  	{
		s = p;
		while(*s != '\0')
		{
			*s = *(s + 1);
			s++;
		}
	}
}

// functia insereaza sirul s2 la inceputul sirului s1
void strins(char *s1, char *s2)
{
 	char *p = s1;
 	char *q = s2;
 	int i;
	// sirul s1 este mutat cu strlen(s2) caractere la dreapta
 	for (i = strlen(s1) - 1; i >= 0; i--)
 	{
	     s1[i + strlen (s2)] = s1[i];
 	}
	// sirul s2 este inserat la inceputul lui s1 (pe pozitiile
	// eliberate de mutarea caracterelor lui s1 la dreapta)
 	for (i = 1; i <= strlen (s2); i++)
 	{
  	   *p = *q;
  	   p++;
  	   q++;
 	}
}

//functia inlocuieste fiecare aparitie a lui cuv1 in sirul p cu cuv2
char *operatie2(char *p, char *cuv1, char *cuv2)
{
	char *copie;
	copie = calloc((strlen(p) + 1), sizeof(char));
	if(copie == NULL)
		return NULL;
	
	// sirul p este copiat pentru a nu fi pierdut odata cu folosirea
	// functiei strtok
	strcpy(copie, p);
	char *aux2 = "\" ,\n.'?!;";
	
	// contorul k creste la fiecare aparitie a lui cuv1 in sir
	int k = 0;
	char *t;
	
	// sirul copie este impartit de functia strtok in cuvinte separate de
	// delimitatorii din sirul aux2
	t = strtok(copie, aux2);
	while(t != NULL)
	{
		// se verifica daca token-ul t si cuv1 sunt egale
		if(t != NULL && strcmp(t, cuv1) == 0)
		{
			// se sterg strlen(cuv1) caractere din sirul p de pe pozitia unde
			// s-a gasit cuv1 (k se inmulteste cu diferenta lungimilor cuvintelor deoarece
			// la fiecare inserare pozitiile unde se afla cuv1 se muta la dreapta)
			strdel(p + (t-copie) + k*(strlen(cuv2) - strlen(cuv1)), strlen(cuv1));
			
			// se insereaza sirul cuv2 pe aceeasi pozitie
			strins(p + (t-copie) + k*(strlen(cuv2) - strlen(cuv1)), cuv2);
			k ++;
		}
		t = strtok(NULL, aux2);
	}
	// se elibereaza memoria alocata pentru copie
	free(copie);
	return p;
}

// functia insereaza in locul fiecarui cuvant din sir cuvantul permutat
// la dreapta
char *operatie3(char *sir, int k)
{
	// sirul este copiat pentru a nu fi pierdut odata cu folosirea functiei strtok
	char *copie;
	copie = calloc(strlen(sir) + 2, sizeof(char));
	strcpy(copie, sir);

    char *aux2 = "\" \n,.'?!;";
	char *t;
	// sirul este impartit in cuvinte de functia strtok
	t = strtok(copie, aux2);
	while(t != NULL)
	{
		if(t != NULL)
		{
			// fiecare cuvant este inlocuit in sir de catre cuvantul permutat
			strdel(sir + (t-copie), strlen(t));
			strins(sir + (t-copie), permutare(t, k));
		}
		t = strtok(NULL, aux2);
	}
	// se elibereaza memoria alocata pentru copie
	free(copie);
	return sir;
}

int main()
{
	int N;
	int i, j;
	// se citeste numarul de linii ale textului
	scanf("%d", &N);
	getchar();
	
	// se aloca memorie pentru textul p (variabila de tip **)
	char **p;
	p = malloc(N * sizeof(char *));
	if(p == NULL)
		return -1;
	
	// se aloca memorie pentru fiecare linie a textului si se verifica 
	// daca alocarea a reusit, in cazul in care nu a reusit alocarea memoria
	// este eliberata
	for(i = 0;i < N; i++)
	{
		p[i] = calloc(MAX , sizeof(char));
		if(p[i] == NULL)
		{
			for(j = 0; j < i; j++)
				free(p[j]);
			free(p);
		}
	}
	// se citesc cele N linii ale textului
	for(i = 0; i < N; i++)
	{
		fgets(p[i], MAX, stdin);
	}
	
	// se aloca memorie pentru cuvintele ce urmeaza a fi citite in cadrul operatiilor
	char *cuv;
	cuv = calloc(MAX, sizeof(char));
	char *cuv1;
	cuv1 = calloc(MAX, sizeof(char));
	char *cuv2;
	cuv2 = calloc(MAX, sizeof(char));
	char *aux;
	aux = calloc(MAX, sizeof(char));

	// variabila c va retine numarul de aparitii ale unui cuvant in text
	int c = 0;

	// se citeste numarul de operatii ce urmeaza sa fie efectuate
	int m;
	scanf("%d", &m);
	getchar();
	
	for(i = 0; i < m; i++)
	{
		// la fiecare pas se citeste de la tastatura o operatie
		int operatie;
		scanf("%d", &operatie);
		getchar();
		// operatia 1
		if(operatie == 1)
		{
			// se citeste cuvantul ce va fi cautat in text
			scanf("%s", cuv);

			// pentru fiecare linie, c creste cu numarul de aparitii ale cuvantului
			// in acea linie
			for(j = 0; j < N; j++)
			{
				strcpy(aux, p[j]);
				c += operatie1(aux, cuv);
			}
			printf("%d\n", c);
			c = 0;
		}
		// operatia 2
		if(operatie == 2)
		{
			// se citesc cuvintele ce urmeaza sa fie inlocuite in text
			scanf("%s%s", cuv1, cuv2);

			// se realizeaza inlocuirea pentru fiecare
			for(j = 0; j < N; j++)
			{
				// daca lungimea lui cuv2 este mai mare ca cea a lui cuv1 
				// se aloca mai multa memorie
                if(cuv1 < cuv2)
                {
                    p[j] = realloc(p[j], 15*(MAX + strlen(cuv2) - strlen(cuv1)) * sizeof(char));
                }

				// se afiseaza textul modificat de operatia 2
				printf("%s", operatie2(p[j], cuv1, cuv2));
			}
		}	
		// operatia 3
		if(operatie == 3)
		{
			// se citeste numarul k (cu cat vor fi permutate cuvintele)
			int k;
			scanf("%d", &k);

			// cuvintele fiecarei linii sunt permutate si apoi afisate
			for(j = 0;j < N; j++)
			{
				printf("%s",operatie3(p[j],k));
			}
		}
	}
	// se elibereaza memoria alocata
	free(cuv);
	free(cuv1);
	for(i=0;i<N;i++)
	{
		free(p[i]);
	}  
	free(p);
	return 0;
}
