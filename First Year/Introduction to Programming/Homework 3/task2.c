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

//functia returneaza 1 daca este gasita o cifra 1 ce incepe
//pe coloana col si 0 in caz contrar
int Recunoastere1(int **cif, int col, int height)
{
	int i, j;
	int lin;
	int cifra = 0;
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
			//pixel colorat la stanga si la dreapta cifra devine 1
			if(s == 5 && s1 == 0 && s2 == 0)
			{
				cifra = 1;

				//se suprascrie matricea unde s-a gasit cifra 1
				for(i = lin; i <= lin+4; i++)
				{
					for(j = col; j <= col+4; j++)
						cif[i][j] = 2;
				}
			}
		}
	}
		
	return cifra;
}


int main()
{
	//se citeste numele imaginii
	char numeBMP[20];
	FILE *input = fopen("input.txt", "r");
	if(input == NULL)
		return -1;
	fscanf(input, "%s", numeBMP);
	fclose(input); 

	//headere-le imaginii
	fileheader fh;
	infoheader ih; 

	// se deschide imaginea in format readonly
	FILE *img = fopen(numeBMP, "rb");

	fread(&fh, 1, sizeof(fileheader), img);
	fread(&ih, 1, sizeof(infoheader), img);

	//se sare peste offset
	fseek(img, fh.imageDataOffset, 0);
	
	//matricea de pixeli
	int i;
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

	//se elimina extenisa.bmp si se adauga.txt
	int k = 0;
	while(numeBMP[k] != '.')
	{
		k ++;
	}
	numeBMP[k] = '\0';
	strcat(numeBMP, "_task2.txt");

	//pozitia unde incepe o cifra diferita de 1
	int pos;

	//se recunosc cifrele din imagine
	FILE *output = fopen(numeBMP, "w");
	if(output == NULL)
		return -1;
	
	for(col = 0; col < (ih.width-4); col++)
	{	
		//se verifica daca pe coloana incepe o cifra diferita de 1
		for(pos = 0; pos < ih.height; pos++)
		{
			if(cif[pos][col] == 1)
				break;
		}
	
		//daca pe coloana incepe o matrice ce contine cifra 1 este
		//scris 1 in output
		if(Recunoastere1(cif, col, ih.height) == 1)
			fprintf(output, "1");
		
		//daca s-a gasit un colt al unei cifre se detecteaza cifra
		//si este scrisa in output
		if(pos != ih.height)
		{
			fprintf(output, "%d", RecunoastereCifra(cif, pos, col));
		}
	} 
	fclose(output);

	//se elibereaza memoria alocata
	for(lin = 0; lin < ih.height; lin++)
		free(im[lin]);
	free(im);
	for(lin = 0; lin < ih.height; lin++)
		free(cif[lin]);
	free(cif);
	
	return 0;
}












		
		
	







