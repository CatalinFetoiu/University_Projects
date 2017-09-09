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


int main()
{
	char numeBMP[20];
	
	//se citesc numesc imaginii si culorile din input.txt
	int blue, green, red;
	FILE *input = fopen("input.txt", "r");
	if(input == NULL)
		return -1;
	fscanf(input, "%s%d%d%d", numeBMP, &blue, &green, &red);
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

	//se sare peste offset-ul imaginii
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
	int ok;
	if(ih.width % 4 != 0)
		ok = 1;
	int pad = 0;
	int padtotal = 0;

	//se citeste matricea de pixeli
	for(lin = 0; lin < ih.height; lin++)
	{
		//se calculeaza padding-ul tinand cont si de
		//padding-ul liniei anterioare
		pad = 4 - ((3 * (lin+1) * ih.width + padtotal) % 4);
		padtotal += pad;
		
		for(col = 0; col < ih.width; col++)
			fread(&im[lin][col], 1, sizeof(pixel), img);
			
		//se sare peste padding
		if(ok == 1)
			fseek(img, pad, 1);
	}
	fclose(img);
	
	//se schimba culoarea imaginii
	for(lin = 0; lin < ih.height; lin++)
		for(col = 0; col < ih.width; col++)
			
			//daca pixelul nu este alb acesta este colorat
			//conform input-ului
			if(im[lin][col].b + im[lin][col].g + im[lin][col].r != 765)
			{
				im[lin][col].b = blue;
				im[lin][col].g = green;
				im[lin][col].r = red;
			}

	//se elimina extensia .bmp si se adauga _task1.bmp
	int k = 0;
	while(numeBMP[k] != '.')
	{
		k ++;
	}
	numeBMP[k] = '\0';
	strcat(numeBMP, "_task1.bmp");

	//se deschide imaginea ce reprezinta output-ul
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
			fwrite(&im[lin][col], 1, sizeof(pixel), img);

		//octetii de padding sunt scrisi cu 0
		if(ok == 1)
			fwrite(&byte, 1, pad, img);
	}
	fclose(img);

	//se elibereaza memoria alocata
	for(lin = 0; lin < ih.height; lin++)
		free(im[lin]);
	free(im);

	return 0;
}
