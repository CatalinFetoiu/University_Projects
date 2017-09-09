//314CB Fetoiu Catalin Emil

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//structura pentru lista simplu inlantuita generica
typedef struct celula
{
	void* info;
	struct celula* next;

} TCelula, *TLG, **ALG;

//structura pentru un nod din arborele de codificare
typedef struct nod
{
	//caracterul codificat (poate exista sau nu)
	char nume;
	
	//numarul variabil de fii este pastrat intr-o lista
	TLG children;

	int nrChildren;
	
	//codul din arbore
	char key;

	struct nod* parent;
	
} TArb;

//functia aloca o celula ce contine in campul info elementele unei
//structuri de tip arbore
TLG AlocareCelula(void* tree)
{
	TLG cell = malloc(sizeof(TCelula));
	if(cell == NULL)
		return NULL;

	cell->next = NULL;
	
	cell->info = malloc(sizeof(TArb));
	if(cell->info == NULL)
	{
		free(cell);
		return NULL;
	}

	//sunt copiate datele din adresa structurii la adresa celulei
	memcpy(cell->info, tree, sizeof(TArb));

	return cell;
}

//functia initializeaza o structura de tip arbore
TArb* InitArb()
{
	TArb* tree = malloc(sizeof(TArb));
	if(tree == NULL)
		return NULL;

	//codul si caracterul codificat sunt initializate cu '\0' pentru a putea
	//verifica daca intr-un anumit nod se gaseste un caracter codificat
	tree->nume = '\0';
	tree->key = '\0';
	
	tree->children = NULL;
	tree->nrChildren = 0;
	tree->parent = NULL;
	
	return tree;
}

//functia elibereaza recursiv un arbore
void EliberareArbore(void* arbore)
{
	TArb* tree = (TArb*)arbore;
	
	TLG child;
	TLG aux;
	
	//daca arborele nu are fii este eliberata memoria alocata pentru acesta
	if(tree->children == NULL)
	{
		free(tree);
	}
	//in caz contrar este eliberata mai intai memoria alocata pentru fii
	else
	{
		child = tree->children;
		
		//este eliberata fiecare celula din lista de fii
		while(child != NULL)
		{
			aux = child;
			child = child->next;
			EliberareArbore(aux->info);
			free(aux);
		}
		
		free(tree);
	}
}

//functia adauga in arbore o noua codificare prin separarea acesteia in
//caractere si plasarea lor recursiva in noduri
void add(void* arbore, char nume, char* codificare)
{	
	TArb* tree = (TArb*)arbore;
	
	TLG child = tree->children;
	TLG ant;
	
	//este parcursa lista de fii ai nodului
	while(child != NULL)
	{
		//daca primul caracter din codificare se afla intr-unul din fii
		if(((TArb*)child->info)->key == codificare[0])
		{
			//daca s-au epuizat caracterele din codificare este inserata
			//litera asociata acesteia
			if(codificare[1] == '\0')
			{
				((TArb*)child->info)->nume = nume;
			}
			//in caz contrar este apelata recursiv functia pentru caractere
			//ramase din codificare
			else
			{
				codificare ++;
				add(child->info, nume, codificare);
			}
		
			return;
		}
		else
		{
			ant = child;
			child = child->next;
		}
	}

	//daca inceputul codificarii nu a fost gasit in lista este inserat
	//un nou fiu ce contine caracterul respectiv
	if(child == NULL)
	{
		TArb* new = InitArb();
		if(new == NULL)
			return;

		//parintele noului nod este nodul curent
		new->parent = tree;
		new->key = codificare[0];
		
		//se aloca o noua celula ce contine primul caracter din codificare
		TLG cell = AlocareCelula(new);
		if(cell == NULL)
			return;
		
		free(new);

		//este inserata celula in lista de fii ai nodului
		if(tree->nrChildren == 0)
			tree->children = cell;
		else
			ant->next = cell;
		
		tree->nrChildren ++;

		//la fel ca in cazul inserarii intr-un unul din fii nodului se proce-
		//deaza in functie de numarul caracterelor ramase in codificare
		if(codificare[1] == '\0')
		{
			((TArb*)cell->info)->nume = nume;
		}
		else
		{
			codificare ++;
			add(cell->info, nume, codificare);
		}
	} 
}

//functia elimina codificarea asociata unei litere din arbore
void del(void* arbore, char nume)
{
	TArb* tree = (TArb*)arbore;
	
	TArb* aux;
	TLG elib;
	TLG ant;
	TLG aux1;
	TLG child;
	
	//daca nodul curent contine litera cautata
	if(tree->nume == nume)
	{
		//daca nodul nu este frunza se elimina doar decodificarea
		if(tree->children != NULL)
		{
			tree->nume = '\0';
			return;
		}
		
		//daca nodul este frunza este parcurs arborele in sus cu ajutorul
		//campului parinte
		while(tree->parent != NULL)
		{
			//auxiliarul retine nodul curent si tree retine nodul parinte
			aux = tree;
			tree = tree->parent;
			
			//parcurgerea se opreste atunci cand este intalnit primul nod ce
			//contine o litera, are cel putin 2 fii sau este nodul radacina
			if(tree->nume != '\0' || tree->parent == NULL || tree->nrChildren > 1)
			{
				child = tree->children;
				ant = NULL;
				
				//este cautat nodul ce trebuie eliminat in lista de fii ai
				//parintelui sau
				while(child != NULL)
				{
					if(aux->key == ((TArb*)child->info)->key)
					{
						break;
					}
					else
					{
						ant = child;
						child = child->next;
					}
				}
					
				//daca celula ce trebuie eliminata se afla la inceputul listei
				if(ant == NULL)
				{
					elib = tree->children;
					tree->children = tree->children->next;
					tree->nrChildren --;
				}
				//daca celula se afla in interiorul sau la finalul listei
				else
				{		
					elib = ant->next;
					ant->next = child->next;
					tree->nrChildren--;
				}
				
				//este eliberata memoria alocata pentru subarbore si pentru
				//celula in care acesta era retinut
				EliberareArbore(elib->info);
				free(elib);
				break;
			}
		}
	}		
	
	//daca nu s-a gasit litera aceasta este cautata recursiv in arbore
	else
	{
		child = tree->children;
		if(child == NULL)
			return;

		//daca nodul curent are cel putin un fiu
		else
		{
			while(child != NULL)
			{
				aux1 = child;
				child = child->next;
				del(aux1->info, nume);
			}
		}
	}
}
			
//functia returneaza adancimea maxima a unui arbore
int maxDepth(void* arbore)
{
	TArb* tree = (TArb*)arbore;
	//daca nodul este frunza
	if(tree->children == NULL)
		return 1;

	int max = 0;
	TLG child = tree->children;
	
	//in cazul in care nodul nu este frunza este determinata adancimea
	//maxima a fiilor acestuia
	while(child != NULL)
	{
		if(maxDepth(child->info) > max)
			max = maxDepth(child->info);

		child = child->next;
	}
	
	//adancimea returnata este cu o unitate mai mare decat adancimea
	//maxima a subarborilor
	return 1 + max;
}

//functia afiseaza nodurile de pe un anumit nivel al unui arbore
void showNivel(void* arbore, int crtlevel, int level, FILE* g)
{
	TArb* tree = (TArb*)arbore;
	
	//daca nivelul curent este nivelul cautat sunt afisate
	//informatiile din nod
	if(crtlevel == level)
	{
		//daca nodul este radacina arborelui
		if(level == 1)
		{
			fprintf(g, "(:)");
		}
		else
		{
			if(tree->nume == '\0')
				fprintf(g, "(%c:)", tree->key);
			else
				fprintf(g, "(%c:%c)", tree->key, tree->nume);
		}
	}
	//in caz contrar este apelata recursiv functia pentru totii fiii
	//nodului, nivelul curent fiind incrementat
	else
	{
		TLG child = tree->children;
		
		if(child == NULL)
			return;
		
		while(child != NULL)
		{
			showNivel(child->info, crtlevel+1, level, g);
			child = child->next;
		}
	}
}

//functia afiseaza toate nodurile unui arbore
void show(void* arbore, FILE* g)
{
	TArb* tree = (TArb*)arbore;

	//este determinata adancimea maxima a arborelui
	int nr = maxDepth(tree);

	//este afisat fiecare nivel cu ajutorul functiei precedente
	int i;
	for(i = 1; i <= nr; i++)
	{
		showNivel(tree, 1, i, g);
		fprintf(g, "\n");
	}
}

//functia returneaza litera asociata unei codificari din arbore	
char decodificare(void* arbore, char* codificare)
{
	TArb* tree = (TArb*)arbore;
	
	//daca s-au epuizat caracterele din codificare este returnata litera
	if(codificare[0] == '\0')
		return tree->nume;

	//se cauta subarborele ce contine primul caracter din codificare
	TLG child = tree->children;
	while(child != NULL)
	{
		if(((TArb*)child->info)->key == codificare[0])
			break;
		else
			child = child->next;
	}

	//este cautata litera pe baza caracterelor ramase
	codificare ++;
	return decodificare(child->info, codificare);
}

//functia decodifica o propozitie ale carei cuvinte sunt separate de
//un delimitator dat
void propozitie(void* arbore, char* codificare, char* delimitator, FILE* g)
{
	TArb* tree = (TArb*)arbore;
	char* p = strtok(codificare, delimitator);
	
	//este separata propozitia in cuvinte cu ajutorul functiei strtok, fiecare
	//caracter al unui cuvant fiind decodificat cu ajutorul functiei precedente
	while(p != NULL)
	{
		//se verifica daca intre cuvinte exista spatii
		if(p[0] == ' ')
		{
			fprintf(g, " ");
			p ++;
			if(p != NULL)
			{
				fprintf(g, "%c", decodificare(tree, p));
			}
		}
		else
		{
			fprintf(g, "%c", decodificare(tree, p));
		}
		
		p = strtok(NULL, delimitator);
	}
}
	

int main(int argc, char* argv[])
{
	//este initializat arborele folosit pentru decodificare
	TArb* t1 = InitArb();
	
	//este deschis fisierul in care va fi afisat arborele dupa
	//fiecare operatie
	FILE* arbore = fopen(argv[3], "w");
	//este deschis fisierul in care va fi scrisa decodificarea textului
	FILE* text = fopen(argv[4], "w");

	FILE* f = fopen(argv[1], "r");
	if(f == NULL)
		return 0;
	
	int N, M;
	
	//este citit numarul de codificari initiale
	fscanf(f, "%d", &N);

	//variabilele in care se citesc datele de intrare
	char* litera = malloc(2 * sizeof(char));
	if(litera == NULL)
		return 0;
	char* buf = malloc(10 * sizeof(char));
	if(buf == NULL)
		return 0;
	char* cod = malloc(200 * sizeof(char));
	if(cod == NULL)
		return 0;
	char* separator = malloc(2 * sizeof(char));
	if(separator == NULL)
		return 0;

	//sunt adaugate in arborele codificarile initiale
	int i;
	for(i = 0; i < N; i++)
	{
		fscanf(f, "%s", litera);
		fscanf(f, "%s", cod);
		add(t1, litera[0], cod);
	}
	//este afisat arborele initial
	show(t1, arbore);

	//este citit separatorul textului codificat si numarul de operatii
	fscanf(f, "%s", separator);
	fscanf(f, "%d", &M);

	//se efectueaza fiecare operatie asupra arborelui, acesta fiind
	//afisat la fiecare pas
	for(i = 0; i < M; i++)
	{
		fscanf(f, "%s", buf);
		if(strcmp(buf, "add") == 0)
		{
			fscanf(f, "%s", litera);
			fscanf(f, "%s", cod);
			add(t1, litera[0], cod);
		}
		if(strcmp(buf, "delete") == 0)
		{
			fscanf(f, "%s", litera);
			del(t1, litera[0]);
		}
	
		show(t1, arbore);
	}
	fclose(f);
	
	//este citit textul codificat linie cu linie cu ajutorul functiei fgets
	f = fopen(argv[2], "r");
	char* line = malloc(7000 * sizeof(char));
	if(line == NULL)
		return 0;

	while(fgets(line, 7000, f) != NULL)
	{
		line[strlen(line) - 1] = '\0';
		
		//daca linia citita este linie noua
		if(line[0] == '\0')
		{
			fprintf(text, "\n");
		}
		//in caz contrar este scrisa in fisier propozitia decodificata
		else
		{
			propozitie(t1, line, separator, text);
			fprintf(text, "\n");
		}
	}
	fclose(f);
	
	//sunt inchise fisierele de output
	fclose(arbore);
	fclose(text);
	
	//este eliberata memoria alocata
	free(line);
	free(litera);
	free(buf);
	free(cod);
	free(separator);

	EliberareArbore(t1);
	
	return 0;
}

	

	
	
	
	
