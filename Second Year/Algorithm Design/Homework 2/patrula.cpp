/* Copyright 2017
 * Fetoiu Catalin-Emil 324CB
 */

#include <limits.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

#define traverse(list, it) \
    for (vector<int>::iterator it = list.begin(); it != list.end(); it++)

typedef pair<int, int> ii;
typedef long long ll;

/* Graful este retinut folosit liste de adiacenta. Vectorul countEdges retine
   numarul de muchii directe intre doua noduri                               */
int N, M;
vector<vector<int> > AdjList;
ll countEdges[101][101];

/* Matricile folosite de catre algoritmul Floyd-Warsahll. Pria retine numarul
   de cai minime intre oricare doua noduri din graf, iar a doua retine distanta
   intre oricare pereche de noduri                                           */
ll countPaths[101][101];
int distances[101][101];

/* Calculeaza, pentru doua noduri i, j si un nod k, cate muchii aflate pe
   toate drumurile minime intre i si j au unul din capete k. In cazul in care
   k este egal cu i sau j, rezultatul este numarul total de drumuri minime
   intre i si j. In caz contrar rezultatul este de doua ori produsul dintre 
   numarul de cai minime de la i la k si numarul de cai minime de la k la j */
ll countRoads(int i, int j, int k) {
    if (k == i || k == j) {
        return countPaths[i][j];
    }

    return 2 * (countPaths[i][k] * countPaths[k][j]);
}

/* Este folosit algoritmul Floyd-Warshall pentru a determina distantele minime
   intre oricare doua noduri din graf, precum si numarul de cai minime intre
   oricare pereche de noduri                                                 */
void floydWarshall() {
    memset(countPaths, 0, sizeof(countPaths));
    memset(distances, INT_MAX - 1000, sizeof(distances));

    /* Distantele intre noduri vecine sunt initializate cu 1, iar numarul de
       cai minime intre acestea cu numarul de muchii directe                 */
    for (int node = 0; node < N; node++) {
        distances[node][node] = 0;
        traverse(AdjList[node], it) {
            int next = *it;

            distances[node][next] = 1;
            countPaths[node][next] = countEdges[node][next];
        }
    }

    /* Modificarea adusa variantei clasice a algoritmului este urmatoarea:
       Atunci cand este intalnit un nod k prin care trece un drum mai scurt
       intre nodurile i si j decat cel curent, numarul de cai minime intre i
       si j este resetat ca fiind egal cu produsul dintre numarul de cai intre
       i si k, respectiv numarul de cai intre k si j. Daca este intalnit un nod
       k prin care trece un drum de lungime egala cu drumul minim curent,
       numarul de cai de la i la j este incrementat cu produsul de la cazul
       anterior                                                              */
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (distances[i][k] + distances[k][j] < distances[i][j]) {
                    distances[i][j] = distances[i][k] + distances[k][j];
                    countPaths[i][j] = countPaths[i][k] * countPaths[k][j];

                } else if (distances[i][k] + distances[k][j]
                           == distances[i][j] && k != i && k != j) {
                    countPaths[i][j] += countPaths[i][k] * countPaths[k][j];
                }
            }
        }
    }
}

/* Determina si afiseaza numarul total de cai minime intre nodurile 1 si N,
   precum si media maxima a muchiilor de pe aceste cai, acoperite de un
   anumit nod                                                               */
void computeResults() {
    ll numberOfPaths = countPaths[0][N-1];

    ll maxCoverage = 0;

    /* Pentru toate nodurile ce se afla pe un drum minim de la 1 la N, este
       calculat numarul de muchii acoperite si este determinat maximul dintre
       acestea                                                               */
    for (int k = 0; k < N; k++) {
        if (distances[0][k] + distances[k][N-1] == distances[0][N-1]) {
            ll currentCoverage = countRoads(0, N-1, k);
            maxCoverage = max(maxCoverage, currentCoverage);
        }
    }

    cout << numberOfPaths << endl;

    double average = (double)maxCoverage / (double)numberOfPaths;
    printf("%.3lf\n", average);
}


int main() {
    FILE* in = freopen("patrula.in", "r", stdin);
    FILE* out = freopen("patrula.out", "w", stdout);

    cin >> N >> M;

    AdjList.clear();
    AdjList.resize(N);
    memset(countEdges, 0, sizeof(countEdges));

    /* Pentru ficare muchie citita, aceasta este adaugata in graf doar daca
       numarul sau de aparitii este 0, altfel numarul de aparitii este
       incrementat                                                          */
    int start, end;
    for (int i = 0; i < M; i++) {
        cin >> start >> end;
        start--;
        end--;

        if (countEdges[start][end] == 0) {
            AdjList[start].push_back(end);
            AdjList[end].push_back(start);
        }

        countEdges[start][end]++;
        countEdges[end][start]++;
    }

    /* Este aplicat algoritmul Floyd-Warshall si sunt afisate rezultatele
       cerute                                                             */

    floydWarshall();

    computeResults();

    fclose(in);
    fclose(out);

    return 0;
}
