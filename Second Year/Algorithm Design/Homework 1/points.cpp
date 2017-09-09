/* Copyright 2017
 * Fetoiu Catalin-Emil 324CB
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/* Structura folosita pentru a retine intervale */
struct Interval {
	int leftEnd;
	int rightEnd;

	Interval(int leftEnd, int rightEnd) {
		this->leftEnd = leftEnd;
		this->rightEnd = rightEnd;
	}
};

/* Vectorii ce contin intervalele si punctele */
int N, M;
vector<Interval> intervals;
vector<int> points;

/* Verifica daca un punct apartine unui interval */
bool containsPoint(Interval i, int point) {
	return i.leftEnd <= point && point <= i.rightEnd;
}

/* Functia de comparatie folosita pentru sortarea in ordine crescatoare a 
   intervalelor. Sunt comparate capetele din stanga si, in caz de egalitate 
   capetele din dreapta                                                     */
bool cmpIntervals(Interval i1, Interval i2) {
	if (i1.leftEnd < i2.leftEnd) {
		return true;
	} else if (i1.leftEnd == i2.leftEnd) {
		return i1.rightEnd <= i2.rightEnd;
	}

	return false;
}

/* Returneaza, folosind cautare binara, indicele primului punct aflat in
   afara intervalului dat ca parametru. In cazul in care toate punctele sunt
   continute de interval, este returnat N (numarul de puncte)                */
int getNextPoint(Interval i) {
	vector<int>::iterator it = upper_bound(points.begin(), points.end(),
											i.rightEnd);
	if (it == points.end()) {
		return N;
	} else {
		return it - points.begin();
	}
}

/* Returneaza numarul minim de intervale necesare acoperirii punctelor */
int getMinIntervals() {
	int count = 0;
	int index = 0;

	/* Indicele primului punct ramas neacoperit */
	int uncovered = 0;

	/* Indicele intervalului care acopera primul punct ramas neacoperit si
	   are capatul din dreapta cel mai mare                                */
	int maxReachingInterval = -1;

	/* Sunt parcurse intervalele sortate */
	while (index < M) {
		/* Cat timp capatul din stanga al intervalului este mai mic decat
		   coordonata primului punct neacoperit                           */
		if (intervals[index].leftEnd <= points[uncovered]) {
			/* Pentru intervalele ce contin punctul curent, este updatat
			   intervalul cu capatul dreapta mai mare                     */
			if (containsPoint(intervals[index], points[uncovered])) {
				if (maxReachingInterval == -1) {
					maxReachingInterval = index;
				} else if (intervals[index].rightEnd >
						intervals[maxReachingInterval].rightEnd) {
					maxReachingInterval = index;
				}
			}
			if (index == M-1) {
				count++;
			}
			index++;
		/* Daca au fost epuizate intervalele ce contin punctul curent, este
		   incrementat contorul si este determinat urmatorul punct ramas
		   neacoperit                                                      */
		} else {
			count++;
			uncovered = getNextPoint(intervals[maxReachingInterval]);
			maxReachingInterval = -1;
		}

		/* Daca toate punctele au fost acoperite se iese din bucla */
		if (uncovered >= N) {
			break;
		}
	}

	return count;
}


int main() {
	FILE* in = freopen("points.in", "r", stdin);
	FILE* out = freopen("points.out", "w", stdout);

	scanf("%d%d", &N, &M);

	points.clear();
	points.resize(N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &points[i]);
	}

	intervals.clear();
	int leftEnd, rightEnd;
	for (int i = 0; i < M; i++) {
		scanf("%d%d", &leftEnd, &rightEnd);
		intervals.push_back(Interval(leftEnd, rightEnd));
	}

	/* Intervalele sunt sortate in ordine crescatoare dupa capatul din stanga,
	   respectiv cel din dreapta                                             */
	sort(intervals.begin(), intervals.end(), cmpIntervals);

	/* Este determinat numarul minim de intervale necesare pentru acoperire */
	int result = getMinIntervals();
	printf("%d\n", result);

	fclose(in);
	fclose(out);

	return 0;
}
