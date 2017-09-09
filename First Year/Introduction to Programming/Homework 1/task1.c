// Fetoiu Catalin Emil 314CB

#include <stdio.h>
#include <math.h>

// functia returneaza suma divizorilor unui numar n,
// divizori diferiti de n
int sumadiv (int n) 
{
  int i;
  int s = 1;
  // pentru a eficientiza calculul sumei divizorilor parcurgem
  // numerele pana la radical din n
  for (i = 2; i <= sqrt (n); i++)  
  {
      if (n % i == 0)
	  {
	      s = s + i + (n / i);
	  }
  }
  return s;
}

int main ()
{
  int i;
  // citim numarul K
  int K;
  scanf ("%d", &K);
  // verificam pentru numerele mai mici decat K conditia ceruta
  // si afisam perechile ce respecta aceasta conditie 
  for (i = 1; i < K; i++) 
  {
      if (sumadiv (sumadiv (i)) == i && i < sumadiv (i))
	  {
	      printf ("(%d, %d)\n", i, sumadiv (i));
	  }
  }
  return 0;
}


