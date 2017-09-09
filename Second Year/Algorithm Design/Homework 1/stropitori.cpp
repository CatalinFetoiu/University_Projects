/* Copyright 2017
 * Fetoiu Catalin-Emil 324CB
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

int N;
ll S;

/* Vectorii ce contin coordonatele stropitorilor, respectiv puterile acestora */
ll coordinate[1000010];
ll power[1000010];

/* Matricea folosita pentru a salva rezultatele calculate. Pentru fiecare
   stropitoare este retinut numarul maxim de stropitori ce pot fi alese incepand
   de la ea, pentru toate cele 3 pozitii in care aceasta se poate afla. 0
   reprezinta starea blocata, 1 reprezinta faptul ca stropeste la stanga si 2
   faptul ca stropeste la dreapta                                            */
int dp[1000010][4];

/* Functia determina numarul maxim de stropitori "active" ce pot fi alese,
   folosind o varianta iterativa de programare dinamica                      */
void iterativeSolve() {
	for (int index = N-1; index >= 0; index--) {
		/* Pentru ultima stropitoare se verifica daca pozitionarea ei la
		   dreapta nu depaseste marginea stadionului si daca pozitionarea ei
		   la stanga nu depaseste penultima stropitoare                      */
		if (index == N-1) {
			dp[index][0] = 0;
			if (coordinate[index] + power[index] > S) {
				dp[index][2] = 0;
			} else {
				dp[index][2] = 1;
			}
			if (coordinate[index] - power[index] > coordinate[index-1]) {
				dp[index][1] = 1;
			} else {
				dp[index][1] = 0;
			}
		} else {
			/* Cazul in care stropitoarea initiala este pozitionata la stanga
			   este tratat separat deoarece necesita verificarea faptului ca
			   aceasta nu stropeste in afara stadionului. In rest algoritmul
			   este acelasi ca si pentru stropitorile din "interior"        */
			if (index == 0) {
				if (coordinate[index] - power[index] >= 0) {
					dp[index][1] = 1 + max(dp[index+1][0], dp[index+1][2]);
					if (coordinate[index+1] - power[index+1] >
						coordinate[index]) {
						dp[index][1] = max(dp[index][1], 1 + dp[index+1][1]);
					}
				} else {
					dp[index][1] = 0;
				}
			}

			/* Daca stropitoarea curenta este alesa blocata, inseamna ca
			   stropitoarea urmatoare poate fi alesa blocata sau pozitionata la
			   dreapta. Se verifica daca stropitoarea urmatoare poate fi
			   pozitionata la stanga. Rezultatul este dat de maximul dintre
			   aceste optiuni pentru stropitoarea urmatoare                  */
			dp[index][0] = max(dp[index+1][0], dp[index+1][2]);
			if (coordinate[index] < coordinate[index+1] - power[index+1]) {
				dp[index][0] = max(dp[index][0], dp[index+1][1]);
			}

			/* Se verifica daca pozitionarea stropitorii curente la stanga nu
			   trece peste stropitoarea precedenta, in caz afirmativ rezultatul
			   fiind calculat la fel in cazul in care aceasta este blocata   */
			if (index != 0) {
				if (coordinate[index-1] >= coordinate[index] - power[index]) {
					dp[index][1] = 0;
				} else {
					dp[index][1] = 1 + max(dp[index+1][0], dp[index+1][2]);
					if (coordinate[index] <
					   coordinate[index+1] - power[index+1]) {
						dp[index][1] = 1 + max(dp[index][0], dp[index+1][1]);
					}
				}
			}

			/* Se verifica daca prin pozitionarea stropitorii curente la dreap-
			   ta, aceasta nu depaseste urmatoarea stropitoare sau daca nu se
			   intersecteaza cu stropitoarea urmatoare, daca aceea este orienta-
			   ta spre stanga                                                */
			dp[index][2] = 0;
			if (coordinate[index] + power[index] <
			   coordinate[index+1] - power[index+1]) {
				dp[index][2] = max(dp[index][2], 1 + dp[index+1][1]);
			}
			if (coordinate[index] + power[index] < coordinate[index+1]) {
				dp[index][2] = max(dp[index][2], 1 + dp[index+1][0]);
				dp[index][2] = max(dp[index][2], 1 + dp[index+1][2]);
			}
		}
	}
}


int main() {
	FILE* in = freopen("stropitori.in", "r", stdin);
	FILE* out = freopen("stropitori.out", "w", stdout);

	string name;
	getline(cin, name);

	scanf("%d%lld", &N, &S);
	for (int i = 0; i < N; i++) {
		scanf("%lld", &coordinate[i]);
	}
	for (int i = 0; i < N; i++) {
		scanf("%lld", &power[i]);
	}

	/* Pentru o singura stropitoare, se verifica daca aceasta poate stropi in
	   stanga sau in dreapta fara a depasi marginile stadiounului            */
	if (N == 1) {
		if (coordinate[0] - power[0] >= 0 || (coordinate[0] + power[0] <= S)) {
			printf("1\n");
		} else {
			printf("0\n");
		}
	/* Daca sunt prezente mai multe stropitori, rezultatul este determinat
	   folosind o varianta iterativa de programare dinamica                 */
	} else {
		iterativeSolve();
		ll result = max(dp[0][0], max(dp[0][1], dp[0][2]));

		printf("%lld\n", result);
	}

	fclose(in);
	fclose(out);

	return 0;
}
