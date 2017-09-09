//314CB Fetoiu Catalin Emil

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp_header.h"

//structura ce contine combinatia de culori dintr-un pixel
typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} pixel;

//functia returneaza cifra reprezentata in matricea 5x5
//ce incepe pe linia lin si coloana col
int RecunoastereCifra(int **cif, int lin, int col)
{
	int i, j;
	//numarul de pixeli din matricea 5x5
	int s = 0;
	int cifra;
	
	//se numara pixelii colorati din matricea 5x5
	for(i = lin; i <= lin+4; i++)
	{
		for(j = col; j <= col+4; j++)
		{
			if(cif[i][j] == 1)
				s ++;
		}
	}
	
	//este determinata cifra in functie de numarul de pixeli
	//si de anumite pozitii din matrice
	if(s == 17 && cif[lin+1][col] == 1)
		cifra = 5;
	if(s == 17 && cif[lin+3][col] == 1)
		cifra = 2;
	if(s == 17 && cif[lin+1][col+4] == 1 && cif[lin+3][col+4] == 1)
		cifra = 3;
	if(s == 16)
		cifra = 0;
	if(s == 19)
		cifra = 8;
	if(s == 18 && cif[lin+1][col+4] == 1)
		cifra = 9;
	if(s == 18 && cif[lin+3][col] == 1)
		cifra = 6;
	if(s == 9)
		cifra = 7;
	if(s == 11)
		cifra = 4;

	//se suprascrie matricea 5x5 pentru a nu fi detectati
	//pixelii colorati la alte verificari
	for(i = lin; i <= lin+4; i++)
	{
		for(j = col; j <= col+4; j++)
			cif[i][j] = 2;
	}
	
	return cifra;
}

//functia verifica daca pe o coloana incepe cifra 1 si returneaza linia
//unde aceasta incepe (iar in cazul care nu este gasita returneaza -1)
int Recunoastere1(int **cif, int col, int height)
{
	int i;
	int lin;
	int s, s1, s2;

	for(lin = 0; lin < (height-4); lin++)
	{
		//daca se gaseste un pixel colorat pe coloana
		if(cif[lin][col+4] == 1)
		{
			s = 0;
			for(i = lin; i <= lin+4; i++)
			{
				if(cif[i][col+4] == 1)
					s ++;
			}

			s1 = 0;
			for(i = lin; i <= lin+4; i++)
			{
				if(cif[i][col+3] == 1)
					s1 ++;
			}

			s2 = 0;
			for(i = lin; i <= lin+4; i++)
			{
				if(cif[i][col+5] == 1)
					s2 ++;
			}

			//daca sunt 5 pixeli colorati incepand cu cel detectat si niciun
			//pixel colorat la stanga si la dreapta este returnata linia
			if(s == 5 && s1 == 0 && s2 == 0)
			{
				return lin;
			}
		}
	}
		
	//daca nu s-a gasit niciun 1
	return -1;
}

//functia coloreaza in alb o cifra din imagine
void suprascrie(pixel **im, int lin, int col)
{
	int i, j;
	for(i = 0; i <= 4; i++)
	{
		for(j = 0; j <= 4; j++)
		{
			im[lin+i][col+j].b = 255;
			im[lin+i][col+j].g = 255;
			im[lin+i][col+j].r = 255;
		}
	}
}

//functia muta o cifra in locul alteia si o coloreaza in alb pe cea mutata
void mutare(pixel **im, int lin1, int col1, int lin2, int col2)
{
	int i, j;
	
	for(i = 0; i <= 4; i++)
	{
		for(j = 0; j <= 4; j++)
			im[lin1+i][col1+j] = im[lin2+i][col2+j];
	}
	
	for(i = 0; i <= 4; i++)
	{
		for(j = 0; j <= 4; j++)
		{
			im[lin2+i][col2+j].b = 255;
			im[lin2+i][col2+j].g = 255;
			im[lin2+i][col2+j].r = 255;
		}
	}
}

//functia verifica daca un numar apare intr-un vector
int cautare(int N, int *v, int m)
{
	int ok = 0;
	int i;
	for(i = 0; i < N; i++)
	{
		if(m == v[i])
		{
			ok = 1;
			break;
		}
	}
	return ok;
}

int main()
{
	char numeBMP[20];
	int c1, c2, c3;

	//se citeste numele imaginii
	FILE *input = fopen("input.txt", "r");
	if(input == NULL)
		return -1;
	fscanf(input, "%s%d%d%d", numeBMP, &c1, &c2, &c3); 

	//se citesc numerele ce trebuie eliminate din imagine
	int sterg[50];
	//numarul de cifre ce trebuie eliminate
	int x = 0;
	while(fscanf(input, "%d", &sterg[x]) == 1)
	{
		x ++;
	}
	
	fclose(input); 

	//headere-le imaginii
	fileheader fh;
	infoheader ih; 

	//se deschide imaginea in format readonly
	FILE *img = fopen(numeBMP, "rb");
	if(img == NULL)
		return -1;
	
	fread(&fh, 1, sizeof(fileheader), img);
	fread(&ih, 1, sizeof(infoheader), img);

	//se sare peste offset
	fseek(img, fh.imageDataOffset, 0);
	
	//matricea de pixeli
	int i, j;
	int lin, col;

	pixel **im;
	im = malloc(ih.height * sizeof(pixel *));
	if(im == NULL)
		return -1;
	for(lin = 0; lin < ih.height; lin++)
	{
		im[lin] = calloc(ih.width, sizeof(pixel));
		if(im[lin] == NULL)
		{
			for(i = 0; i < lin; i++)
				free(im[i]);
			free(im);
		}
	}
	
	//ok verifica daca este nevoie de padding
	int ok = 0;
	if(ih.width % 4 != 0)
		ok = 1;
	int pad = 0;
	int padtotal = 0;
	
	//se citeste matricea de pixeli
	for(lin = 0; lin < ih.height; lin++)
	{
		//se calculeaza padding-ul tinand cont si de padding-ul
		//liniei anterioare
		pad = 4 - ((3 * (lin+1) * ih.width + padtotal) % 4);
		padtotal += pad;
		
		for(col = 0; col < ih.width; col++)
			//imaginea este rasturnata
			fread(&im[ih.height - lin - 1][col], 1, sizeof(pixel), img);
	
		//se sare peste padding
		if(ok == 1)
			fseek(img, pad, 1);
	}
	fclose(img);

	//matricea cu 1 in loc de pixel colorat si 0 in loc de pixel alb
	int **cif;
	cif = malloc(ih.height * sizeof(int *));
	if(cif == NULL)
		return -1;
	for(lin = 0; lin < ih.height; lin++)
	{
		cif[lin] = calloc(ih.width, sizeof(int));
		if(cif[lin] == NULL)
		{
			for(i = 0; i < lin; i++)
				free(cif[i]);
			free(cif);
		}
	}

	for(lin = 0; lin < ih.height; lin++)
	{
		for(col = 0; col < ih.width; col++)
		{
			if(im[lin][col].b + im[lin][col].g + im[lin][col].r == 765)
				cif[lin][col] = 0;
			else
				cif[lin][col] = 1;
		}
	}

	//pozitia unde incepe o cifra diferita de 1
	int pos;
	
	//se elimina extenisa.bmp si se adauga.txt
	int k = 0;
	while(numeBMP[k] != '.')
	{
		k ++;
	}
	numeBMP[k] = '\0';
	strcat(numeBMP, "_task3.bmp");

	//vectorul cu cifrele ce apar in imagine
	int *v;
	v = calloc(100, sizeof(int));
	for(lin = 0; lin < 100; lin ++)
		v[lin] = 11;
	//pozitia in vector
	int c = 0;

	//vectorii retin pozitiile unde incep cifrele
	int *poz_l;
	poz_l = calloc(100, sizeof(int));
	int *poz_c;
	poz_c = calloc(100, sizeof(int));

	//se recunosc cifrele din imagine
	for(col = 0; col < (ih.width-4); col++)
	{	
		//se verifica daca pe coloana incepe o cifra diferita de 1
		for(pos = 0; pos < ih.height; pos++)
		{
			if(cif[pos][col] == 1)
				break;
		}
	
		//daca pe coloana incepe o matrice ce contine cifra 1 este
		//scris 1 in vector si coordonatele matricei 5x5 sunt scrise
		//in vectorii corespunzatori
		if(Recunoastere1(cif, col, ih.height) != -1)
		{
			v[c] = 1;
			poz_l[c] = Recunoastere1(cif, col, ih.height);
			poz_c[c] = col;
			lin = poz_l[c];
			c ++;

			//este suprascrisa matricea 5x5 unde apare 1
			for(i = lin; i <= lin+4; i++)
			{
				for(j = col; j <= col+4; j++)
					cif[i][j] = 2;
			}
		}
		
		//daca s-a gasit un colt al unei cifre se detecteaza cifra
		//si este scrisa in vector, iar coordonatele matricei 5x5 sunt
		//scrise in vectorii corespunzatori
		if(pos != ih.height)
		{
			v[c] = RecunoastereCifra(cif, pos, col);
			poz_l[c] = pos;
			poz_c[c] = col;
			c ++;
		}
	} 

	//se elimina cifrele date din matricea de pixeli
	i = 0;
	while(i < (c-1))
	{
		ok = 0;
		//cat timp cifra de pe o pozitia i trebuie eliminata
		while(ok == 0)
		{
			//daca cifra trebuie eliminata sunt mutate cifrele la stanga incepand
			//cu pozitia i si in acelasi timp sunt mutate la stanga elementele
			//din vectorul v
			if(cautare(x, sterg, v[i]) == 1) //&& v[i+1] != 11)
			{
				
				for(j = i; j < (c-1); j++)
				{
					mutare(im, poz_l[j], poz_c[j], poz_l[j+1], poz_c[j+1]);
				}
				for(j = i; j < (c-1); j++)
				{	
						v[j] = v[j+1];
				}
				v[c-1] = 11;
			}
			else
				ok = 1;
		}
		i++;
	}
	if(cautare(x, sterg, v[c-1]) == 1)
		suprascrie(im, poz_l[c-1], poz_c[c-1]); 


	//se creeaza noua imagine
	img = fopen(numeBMP, "wb");
	if(img == NULL)
		return -1;
	
	//se scriu headere-le imaginii
	fwrite(&fh, 1, sizeof(fileheader), img);
	fwrite(&ih, 1, sizeof(infoheader), img);

	//se sare peste offset-ul imaginii
	fseek(img, fh.imageDataOffset, 0);
	
	//ok verifica daca este nevoie de padding
	ok = 0;
	if(ih.width % 4 != 0)
		ok = 1;
	pad = 0;
	padtotal = 0;
	
	int byte = 0;
	//este scrisa matricea de pixeli
	for(lin = 0; lin < ih.height ; lin++)
	{
		//se calculeaza padding-ul
		pad = 4 - ((3 * (lin+1) * ih.width + padtotal) % 4);
		padtotal += pad;
		
		for(col = 0; col < ih.width; col++)
			fwrite(&im[ih.height-lin-1][col], 1, sizeof(pixel), img);

		//octetii de padding sunt scrisi cu 0
		if(ok == 1)
			fwrite(&byte, 1, pad, img);
	}
	fclose(img);

	//se elibereaza memoria alocata pentru vectori
	free(v);
	free(poz_l);
	free(poz_c);

	//se elibereaza memoria alocata pentru matrice
	for(lin = 0; lin < ih.height; lin++)
		free(cif[lin]);
	free(cif);
	for(lin = 0; lin < ih.height; lin++)
		free(im[lin]);
	free(im);
	
	return 0;
}





