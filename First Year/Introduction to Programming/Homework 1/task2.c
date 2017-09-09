// Fetoiu Catalin Emil 314CB

#include <stdio.h>
#include <math.h>

// functia returneaza inversul lui n (numarul ce are cifrele inversate)
int invers (int n)  
{
  int k = 0;
  while (n != 0)
  {
      k = (k + (n % 10)) * 10;
      n = n / 10;
  }
  k = k / 10;
  return k;
}

// functia verifica daca un numar n se scrie ca si produs
// de 2 numere distincte de p cifre
int verificprodus (int n, int p) 
{
  int ok = 0;
  int j;
  for (j = pow (10, p - 1); j < pow (10, p); j++)
  {
      if (n % j == 0 && j != (n / j) && pow (10, p - 1) <= (n / j)
	  && (n / j) < pow (10, p))
	  {
	      ok = 1;
	  }
      if (ok == 1)
	      break;
  }
  if (ok == 1)
      return 1;
  return 0;
}

int
main ()
{
  int p, x;
  int i;
  // initializam cu 0 numarul ce va contoriza cate numere respecta 
  // conditia din enunt
  int contor = 0;  
  // citim numarul de cifre p si numarul x
  scanf ("%d %d", &p, &x);
  // verificam cate numere mai mici ca x sunt palindroame
  // si se pot scrie ca produs de 2 numere distincte de p cifre
  int t = pow(10, 2*p);
  // verific daca x este mai mare sau egal cu 10 la puterea 2p
  if(t <= x)
  {
      x = t;
  }
  for (i=1; i < x; i++)  
  {
      if (invers (i) == i && verificprodus (i, p) == 1)  
	  {
	      contor++;
	  }
  }
  if (p == 1)
  {
      // in cazul p = 1 afisam contor + 1 deoarece il includem
      // si pe 0 ca palindrom
      printf ("%d", contor + 1); 
  }
  else
  {
      printf ("%d", contor);
  }
  return 0;
}


