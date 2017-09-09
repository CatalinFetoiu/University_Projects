// Fetoiu Catalin Emil 314CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// variabila MAX retine numarul maxim de caractere ale unei linii din text
#define MAX 12000

// functia construieste lista de cuvinte distincte list, vectorul
// de aparitii v si returneaza numarul de cuvinte distincte c
int cuvinte(char **p, int n, char **list, int c, int *v)
{
	int i, j;
	char *separator = ". !':;?\"\n,";
	int ok;
	c = 0;	
	char *t;
	for(i = 0; i < n; i ++)
	{
		// fiecare linie a textului este impartita cu ajutorul functiei
		// strtok in cuvinte
		t = strtok(p[i], separator);
		while(t != NULL)
		{
			ok = 1;

			// pentru fiecare cuvant de pe fiecare linie se verifica daca acesta
			// apare in lista de cuvinte, iar in caz afirmativ elementul 
			// corespunzator din vectorul de aparitii este incrementat
	    	for(j = 0; j < c; j++)
			{
				if(strcmp(t, list[j]) == 0)
				{
					ok = 0;
					v[j] ++;
				}
			}

			// in cazul in care cuvantul nu a fost gasit in lista acesta este
			// adaugat pe pozitia urmatoare si totodata este modificat si
			// vectorul de aparitii
			if(ok == 1)
			{
				strcpy(list[c], t);
				v[c] ++;
				c ++;
			}
			t = strtok(NULL, separator);
		}
	}
	return c;
}

// functia returneaza minimul dintre numerele a si b
int min(int a, int b)
{
	if(a < b)
		return a;
	else
		return b;
}

// functia returneaza distanta dintre doua siruri de caractere
// c1 si c2
int dist(char *c1, char *c2)
{
	int c = 0;
	int i;
	int minim = min(strlen(c1), strlen(c2));
	
	// pentru primele min(strlen(c1), strlen(c2)) adunam la variabila
	// modulul diferentei codurilor ASCII ale caracterlor d pe
	// aceeasi pozitie
	for(i = 0; i < minim; i++)
	{
		c += abs(c1[i] - c2[i]);
	}
	
	// se completeaza cu '\0' sirul cu lungimea mai mica si se
	// repeta procedeul de mai sus pentru elementele ramase in
	// sirul cu lungimea mai mare
	if(strlen(c1) < strlen(c2))
	{
		for(i = minim; i < strlen(c2); i++)
		{
			c += abs('\0' - c2[i]);
		}
	}
	else
	{
		for(i = minim; i < strlen(c1); i++)
		{
			c += abs('\0' - c1[i]);
		}
	}
	return c;
}

// functia sorteaza lista de cuvinte diferite descrescator in functie
// de numarul de aparitii si apoi le sorteaza crescator alfabetic
void sortare(char **list, int NR, int *v)
{
	int i, j;
	// se aloca memorie pentru sirul aux folosit la interschimbare
	char *aux;
	aux = malloc(MAX * sizeof(char));
	// numarul aux1 este folosit pentru interschimbarea valorilor
	// din vectorul de aparitii
	int aux1;
	
	// se sorteaza descrescator cuvintele in functie de numarul
	// de aparitii
	for(i = 0; i < (NR-1); i++)
	{
		for(j = (i+1); j <= (NR-1); j++)
		{
			// in cazul in care doua cuvinte nu sunt sortate acestea sunt
			// interschimbate, in acelasi timp fiind interschimbate si
			// elementele corespunzatoare din vectorul de aparitii
			if(v[i] < v[j])
			{
				strcpy(aux, list[i]);
				strcpy(list[i], list[j]);
				strcpy(list[j], aux);
				aux1 = v[i];
				v[i] = v[j];
				v[j] = aux1;
			}
		}
	}
	
	// se ordoneaza crescator alfabetic cuvintele cu acelasi numar
	// de aparitii in text
	for(i = 0; i < (NR-1); i++)
	{
		for(j = (i+1); j <= (NR-1); j++)
		{
			if((v[i] == v[j]) && (strcmp(list[i], list[j]) > 0))
			{
				strcpy(aux, list[i]);
				strcpy(list[i], list[j]);
				strcpy(list[j], aux);
				aux1 = v[i];
				v[i] = v[j];
				v[j] = aux1;
			}
		}
	}
}
				
int main()
{
	// se  citeste numarul de linii ale primului text
	int n;
	int i, j;
	scanf("%d", &n);
	getchar();
	
	// se aloca memorie si se citeste primul text
	char **text1;
	text1 = malloc(n * sizeof(char *));
	if(text1 == NULL)
		return -1;
	for(i = 0; i < n; i++)
	{
		text1[i] = malloc(MAX * sizeof(char));
		fgets(text1[i], MAX, stdin);
	}
	
	// se citeste numarul de linii ale celui de-al doilea
	// text
	int m;
	scanf("%d", &m);
	getchar();

	// se aloca memorie si se citeste cel de-al doilea text
	char **text2;
	text2 = malloc(m * sizeof(char *));
	if(text2 == NULL)
		return -1;
	for(i = 0; i < m; i++)
	{
		text2[i] = malloc(MAX * sizeof(char));
		fgets(text2[i], MAX, stdin);
	}

	// se aloca memorie pentru vectorii ce contin
	// numarul de aparitii
	int *v1;
	v1 = calloc(MAX, sizeof(int));
	int *v2;
	v2 = calloc(MAX, sizeof(int));
	
	// numarul de cuvinte distincte din text1
	int NR1;
	// numarul de cuvinte distincte din text2
	int NR2;
	
	// se aloca memorie pentru lista de cuvinte distincte
	// din primul text
	char **list1;
	list1 = malloc(MAX * sizeof(char *));
	if(list1 == NULL)
		return -1; 
	for(i = 0;i < MAX; i++)    
	{
		list1[i] = calloc(MAX, sizeof(char));
	} 

	// se aloca memorie pentru lista de cuvinte distincte 
	// din cel de-al doilea text
	char **list2;
	list2 = malloc(MAX * sizeof(char *));
	if(list2 == NULL)
		return -1; 
	for(i = 0;i < MAX; i++)    
	{
		list2[i] = calloc(MAX, sizeof(char));
	}
	
	// se determina cuvintele distinctele si numarul de aparitii ale
	// acestora din text1 si text2
	NR1 = cuvinte(text1, n, list1, NR1, v1);
	NR2 = cuvinte(text2, m, list2, NR2, v2);

	// se sorteaza listele de cuvinte si vectorii de aparitii
	sortare(list1, NR1, v1);
	sortare(list2, NR2, v2);
	
	int NR = min(NR1, NR2);

	// variabila sim reprezinta gradul de similitudine dintre
	// cele 2 texte
	int sim = 0;
	sim = 100 * abs(NR1 - NR2);
	
	// se determina gradul de similitudine dupa formula din enunt
	for(i = 0; i < NR; i++)
	{
		sim += (v1[i] + v2[i]) * dist(list1[i],list2[i]);
	}
	
	// se afiseaza gradul de similitudine dintre cele 2 texte
	printf("%d\n",sim);
	
	// se elibereaza memoria alocata in interiorul programului
	free(v1);
	free(v2);
	for(i = 0; i < n; i++)
		free(text1[i]);
	free(text1);
	for(i = 0; i < m; i++)
		free(text2[i]);
	free(text2); 
	for(i = 0;i < MAX; i++)    
	{
		free(list1[i]);
	} 
	free(list1);
	for(i = 0;i < MAX; i++)    
	{
		free(list2[i]);
	} 
	free(list2);
	return 0;
}








