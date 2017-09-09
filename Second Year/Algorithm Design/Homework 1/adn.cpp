/* Copyright 2017
 * Fetoiu Catalin-Emil 324CB
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/* Sirurile ce sunt intercalate, impreuna cu sirul target */
int N;
string first, second, third, fourth, target;

/* Matricile folosite pentru memoizarea rezultatelor obtinute pentru toate
   starile posibile. Acestea sunt retinute ca unsigned char pentru a optimiza
   consumul de memorie, rezultatele posibile fiind doar 0 si 1               */
unsigned char dp2[102][102];
unsigned char dp3[102][102][102];
unsigned char dp4[102][102][102][102];

/* Functia rezolva folosind programare dinamica cazul N = 2. Parametrii index1
   si index2 retin pozitiile curente in cele doua siruri. Rezultatul este 1 in
   cazul in care subsirul din sirul target incepand de la indicele index1 +
   index2 poate fi format prin inteclasarea subsirurilor celor doua siruri
   "mici", ce incep de la indicii index1 si index2. In caz contrare este retur-
   nat 0. Rezultatele sunt salvate in matricea dp2                           */
unsigned char solve2(int index1, int index2) {
	/* Daca starea curenta a fost deja evaluata, este intors rezultatul
	   salvat                                                                */
	if (dp2[index1][index2] != 255) {
		return dp2[index1][index2];
	}

	/* Daca sirurile initiale au fost parcurse in intregime, rezultatul intors
	   este 1 deoarece se poate ajunge in aceasta starea doar daca toate carac-
	   terele intalnite pana atunci au facut match cu un caracter din sirul
	   final                                                                 */
	if (index1 == first.size() && index2 == second.size()) {
		return dp2[index1][index2] = 1;
	}

	/* Indicele curent in sirul final este calculat ca suma indicilor din
	   cele doua siruri initiale                                            */
	int totalIndex = index1 + index2;
	unsigned char option1 = 0, option2 = 0;

	/* In cazul in care exista match intre caracterul curent din primul sir si
	   cel din sirul target, indicele din primul sir este avansat            */
	if (index1 < first.size() && first[index1] == target[totalIndex]) {
		option1 = solve2(index1 + 1, index2);
	}
	/* In cazul in care exista match intre caracterul curent din al doilea sir 
	   si cel din sirul target, indicele din al doilea sir este avansat      */
	if (index2 < second.size() && second[index2] == target[totalIndex]) {
		option2 = solve2(index1, index2 + 1);
	}

	return dp2[index1][index2] = max(option1, option2);
}

/* Functia pentru cazul N = 3 foloseste acelasi algoritm ca pentru cazul N = 2,
   fiind retinuti indicii curenti in cele 3 siruri "mici", iar la fiecare pas
   incercandu-se cresterea pe rand a acestora, in cazul in care se exista
   match intre caracterul curent din acestea si cel din sirul target         */
unsigned char solve3(int index1, int index2, int index3) {
	if (dp3[index1][index2][index3] != 255) {
		return dp3[index1][index2][index3];
	}

	if (index1 == first.size() && index2 == second.size()
		&& index3 == third.size()) {
		return dp3[index1][index2][index3] = 1;
	}

	int totalIndex = index1 + index2 + index3;
	unsigned char option1 = 0, option2 = 0, option3 = 0;

	if (index1 < first.size() && first[index1] == target[totalIndex]) {
		option1 = solve3(index1 + 1, index2, index3);
		if (option1 == 1) {
			return dp3[index1][index2][index3] = 1;
		}
	}
	if (index2 < second.size() && second[index2] == target[totalIndex]) {
		option2 = solve3(index1, index2 + 1, index3);
		if (option2 == 1) {
			return dp3[index1][index2][index3] = 1;
		}
	}
	if (index3 < third.size() && third[index3] == target[totalIndex]) {
		option3 = solve3(index1, index2, index3 + 1);
		if (option3 == 1) {
			return dp3[index1][index2][index3] = 1;
		}
	}

	return dp3[index1][index2][index3] = max(max(option1, option2), option3);
}

/* Pentru cazul N = 4 am folosit programare dinamica iterativa, intrucat metoda
   recursiva necesita apeluri de memset ce costau prea mult timp, fiind folosite
   pentru matrici de dimensiune 100^4. Formulele si cazurile utilizate sunt
   aceleasi ca si in cazul recursiv */
void iterativeDP() {
	for (int index1 = first.size(); index1 >= 0; index1--) {
		for (int index2 = second.size(); index2 >= 0; index2--) {
			for (int index3 = third.size(); index3 >= 0; index3--) {
				for (int index4 = fourth.size(); index4 >= 0; index4--) {
					int totalIndex = index1 + index2 + index3 + index4;
					unsigned char option1 = 0, option2 = 0,
								  option3 = 0, option4 = 0;

					if (index1 == first.size() && index2 == second.size() &&
					   index3 == third.size() && index4 == fourth.size()) {
						dp4[index1][index2][index3][index4] = 1;
						continue;
					}
					if (index1 < first.size() &&
					   first[index1] == target[totalIndex]) {
						option1 = dp4[index1 + 1][index2][index3][index4];
						if (option1 == 1) {
							dp4[index1][index2][index3][index4] = 1;
							continue;
						}
					}
					if (index2 < second.size() &&
					   second[index2] == target[totalIndex]) {
						option2 = dp4[index1][index2 + 1][index3][index4];
						if (option2 == 1) {
							dp4[index1][index2][index3][index4] = 1;
							continue;
						}
					}
					if(index3 < third.size() &&
					   third[index3] == target[totalIndex]) {
						option3 = dp4[index1][index2][index3 + 1][index4];
						if (option3 == 1) {
							dp4[index1][index2][index3][index4] = 1;
							continue;
						}
					}
					if (index4 < fourth.size() &&
					   fourth[index4] == target[totalIndex]) {
						option4 = dp4[index1][index2][index3][index4 + 1];
						if (option4 == 1) {
							dp4[index1][index2][index3][index4] = 1;
							continue;
						}
					}

					int result = max(max(option1, option2),
						             max(option3, option4));
					dp4[index1][index2][index3][index4] = result;
				}
			}
		}
	}
}



int main() {
	FILE* in = freopen("adn.in", "r", stdin);
	FILE* out = freopen("adn.out", "w", stdout);

	int T;
	cin >> T;

	while (T--) {
		cin >> N;

		/* Pentru un singur sir, acesta este comparat cu sirul target */
		if (N == 1) {
			cin >> first >> target;
			if (first == target) {
				cout << 1 << endl;
			} else {
				cout << 0 << endl;
			}
		/* Pentru doua siruri, se verifica daca suma lungimilor lor este aceeasi
		   cu cea a sirului final, fiind apoi apelata functia solve2         */
		} else if (N == 2) {
			cin >> first >> second >> target;
			memset(dp2, 255, sizeof(dp2));

			if (first.size() + second.size() == target.size()) {
				int result = solve2(0, 0);
				cout << result << endl;
			} else {
				cout << 0 << endl;
			}
		/* Pentru trei siruri, se verifica daca suma lungimilor lor este aceeasi
		   cu cea a sirului final, fiind apoi apelata functia solve3         */
		} else if (N == 3) {
			cin >> first >> second >> third >> target;
			memset(dp3, 255, sizeof(dp3));

			if (first.size() + second.size() + third.size() == target.size()) {
				int result = solve3(0, 0, 0);
				cout << result << endl;
			} else {
				cout << 0 << endl;
			}
		/* Pentru patru siruri, se verifica daca suma lungimilor lor este 
		   aceeasi cu cea a sirului final, fiind apoi apelata metoda de progra-
		   mare dinamica iterativa                                          */
		} else if (N == 4) {
			cin >> first >> second >> third >> fourth >> target;

			if (first.size() + second.size() + third.size() +
			   fourth.size() == target.size()) {
				iterativeDP();
				int result = dp4[0][0][0][0];
				cout << result << endl;
			} else {
				cout << 0 << endl;
			}
		}
	}

	fclose(in);
	fclose(out);

	return 0;
}
