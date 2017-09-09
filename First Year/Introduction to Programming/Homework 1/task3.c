// Fetoiu Catalin Emil 314CB

#include <stdio.h>
#include <math.h>

// functia aplica o mutare asupra unui numar N si returneaza
// noua valoare obtinuta
unsigned long long mutare (unsigned long long N, int n)
{
  int i, j, k;
  // variabilele ce vor stoca linia si coloana pozitiei initiale
  // (l1,c1) si pozitiei finale (l2,c2)
  int l1, c1, l2, c2;
  unsigned long long aux1, aux2;
  for (j = 1; j <= 4; j++)
  {
      k = 0;
      // determinam pozitia initiala si pozitia finala
      // corespunzatoare unei mutari prin impartiri
      // succesive la 2
      for (i = 1; i <= 3; i++)
	  {
	      if (n % 2 == 1)
	          k = k + pow (2, i - 1);
	      n = n / 2;
	  }
      // asociem valorile obtinute variabilelor pentru 
      // linie si coloana
      if (j == 1)
	      c1 = k;
      if (j == 2)
	      l1 = k;
      if (j == 3)
	      c2 = k;
      if (j == 4)
	      l2 = k;
  }
  // variabile ce au in reprezentarea pe biti 1 pe pozitia
  // initiala, respectiv finala, in rest 0
  aux1 = pow (2, 64 - (8 * l1 + c1 + 1));
  aux2 = pow (2, 64 - (8 * l2 + c2 + 1));
  // verificam daca pe pozitia actuala se afla sau nu
  // un soldat
  if ((N / aux1) % 2 == 1)
  {
      // verificam daca pozitia initiala este diferita de
      // pozitia finala si efectuam schimbarea lui n
      if (aux1 != aux2)
	  {
	      N = N ^ aux1;
	      N = N ^ aux2;
	  }
  }
  return N;
}

int main ()
{
  unsigned long long N;
  int m;
  int i, nr;
  // citim numarul N si numarul de mutari efectuate
  scanf ("%llu", &N);
  scanf ("%d", &nr);
  // pentru fiecare mutare citita de la tastatura modificam
  // N conform enuntului
  for (i = 1; i <= nr; i++)
  {
      scanf ("%d", &m);
      N = mutare (N, m);
  }
  // afisam numarul N obtinut dupa efectuarea tuturor mutarilor
  printf ("%llu", N);
  return 0;
}
