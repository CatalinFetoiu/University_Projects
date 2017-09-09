/* Copyright 2017
 * Fetoiu Catalin-Emil 324CB
 */

#include <limits.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef pair<int, int> ii;

#define traverse(c, it) \
    for (vector<int>::iterator it = c.begin(); it != c.end(); it++)

int N, M, P, K;

/* Graful este retinut folosit liste de adiacenta. Vectorul countEdges retine
   numarul de muchii directe intre doua noduri                               */
vector<vector<int> > AdjList;
int countEdges[101][101];

/* Contine pentru fiecare doua noduri u si v din graf, fiecare muchie directa 
   intre acestea si fiecare pokemon, timpul necesar parcurgerii drumului u-v */
int times[101][101][1001][22];

/* Matricea folosita in cadrul algoritmului de programare dinamica. Retine
   pentru fiecare triplet de tip oras curent, pokemon curent si numar de
   schimbari posibile, timpul minim in care se poate ajunge in orasul N   */
int dp[101][22][22];

/* Vectorul folosit in cadrul algoritmului Dijkstra pentru a retine distantele
   fata de nodul sursa                                                       */
int distances[101];

/* Determina costul minim al parcurgerii unei muchii atunci cand pokemonul nu
   mai poate fi schimbat (k = 0), prin verificarea tuturor muchiilor directe
   intre cele doua noduri. Este de asemenea actualizat si timpul total in
   care se poate ajunge in orasul N                                          */
int getCostWithoutChange(int start, int end, int pokemon) {
    int minCost = INT_MAX;

    int localBest = INT_MAX;

    for (int index = 0; index < countEdges[start][end]; index++) {
        localBest = min(dp[end][pokemon][0] +
            times[start][end][index][pokemon], localBest);
        minCost = min(minCost, times[start][end][index][pokemon]);
    }

    dp[start][pokemon][0] = min(dp[start][pokemon][0], localBest);

    return minCost;
}

/* Determina costul minim al parcurgerii unei muchii atunci cand pokemonul
   curent poate fi schimbat. Este aleasa cea mai optima varianta intre
   optiunile de a continua drumul cu acelasi pokemon sau de a utiliza un
   alt pokemon pentru parcurgerea muchiei. Este de asemenea actualizat si
   timpul total in care se poate in ajunge la destinatie                  */
int getCostWithChange(int start, int end, int pokemon, int k) {
    int minCost = INT_MAX;

    int localBest = INT_MAX;

    for (int index = 0; index < countEdges[start][end]; index++) {
        localBest = min(dp[end][pokemon][k] +
            times[start][end][index][pokemon], localBest);
        minCost = min(minCost, times[start][end][index][pokemon]);
    }

    for (int index = 0; index < countEdges[start][end]; index++) {
        for (int nextPokemon = 0; nextPokemon < P; nextPokemon++) {
            if (nextPokemon != pokemon) {
                localBest = min(dp[end][nextPokemon][k-1] +
                    times[start][end][index][nextPokemon], localBest);
                minCost = min(minCost, times[start][end][index][nextPokemon]);
            }
        }
    }

    dp[start][pokemon][k] = min(dp[start][pokemon][k], localBest);

    return minCost;
}

/* Este folosit algoritmul Dijkstra avand ca sursa orasul N pentru a determina,
   pentru fiecare oras, timpul in care se poate ajunge la destinatie incepand cu
   un anumit pokemon si un anumit numar de schimbari permise (aceste numere
   fiind fixate). Alegerea ca sursa a nodului N poate fi inlocuita cu alegerea
   nodului 1, graful fiind bidirectional. Parametrul type indica daca pot fi
   efectuate schimbari, dupa caz fiind folosite functiile de cost de mai sus */
void dijkstra(int type, int pokemon, int k) {
    priority_queue<ii, vector<ii>, greater<ii> > pq;

    int destination = N-1;

    for (int i = 0; i < 101; i++) {
        distances[i] = INT_MAX;
    }

    distances[destination] = 0;
    pq.push(make_pair(0, destination));

    while (!pq.empty()) {
        ii front = pq.top();
        pq.pop();
        int node = front.second;
        int d = front.first;

        if (d == distances[node]) {
            traverse(AdjList[node], it) {
                int next = *it;
                int edgeCost = -1;

                if (type == 0) {
                    edgeCost = getCostWithoutChange(next, node, pokemon);
                }
                if (type == 1) {
                    edgeCost = getCostWithChange(next, node, pokemon, k);
                }

                if (distances[node] + edgeCost < distances[next]) {
                    distances[next] = distances[node] + edgeCost;
                    pq.push(make_pair(distances[next], next));
                }
            }
        }
    }
}



int main() {
    FILE* in = freopen("pokemoni.in", "r", stdin);
    FILE* out = freopen("pokemoni.out", "w", stdout);

    cin >> N >> M >> P >> K;

    AdjList.clear();
    AdjList.resize(N);

    memset(countEdges, 0, sizeof(countEdges));

    int start, end;
    for (int i = 0; i < M; i++) {
        cin >> start >> end;
        start--;
        end--;

        if (countEdges[start][end] == 0) {
            AdjList[start].push_back(end);
            AdjList[end].push_back(start);
        }

        int index = countEdges[start][end];
        for (int pokemon = 0; pokemon < P; pokemon++) {
            cin >> times[start][end][index][pokemon];
            times[end][start][index][pokemon] =
                times[start][end][index][pokemon];
        }

        countEdges[start][end]++;
        countEdges[end][start]++;
    }

    for (int i = 0; i < 101; i++) {
        for (int j = 0; j < 22; j++) {
            for (int k = 0; k < 22; k++) {
                dp[i][j][k] = INT_MAX;
            }
        }
    }

    /* Pentru fiecare pokemon si fiecare numar de schimbari, timpul pentru
       nodul destinatie este initializat cu 0                              */
    int destination = N-1;
    for (int k = 0; k <= K; k++) {
        for (int pokemon = 0; pokemon < P; pokemon++) {
            dp[destination][pokemon][k] = 0;
        }
    }

    /* Sunt determinati timpii de parcurgere folosind fiecare pokemon, atunci
       cand nu mai pot fi efectuate schimbari                                */
    for (int pokemon = 0; pokemon < P; pokemon++) {
        dijkstra(0, pokemon, -1);
    }

    /* Pentru fiecare numar de schimbari nenul si fiecare pokemon sunt deter-
       minati timpii de parcurgere pentru fiecare oras                       */
    for (int k = 1; k <= K; k++) {
        for (int pokemon = 0; pokemon < P; pokemon++) {
            dijkstra(1, pokemon, k);
        }
    }

    /* Rezultatul este determinat ca fiind minimul intre timpii de parcurgere
       obtinuti pornind din orasul initial cu fiecare pokemon                */
    int bestTime = INT_MAX;
    for (int i = 0; i < P; i++) {
        bestTime = min(bestTime, dp[0][i][K]);
    }

    cout << bestTime << endl;

    fclose(in);
    fclose(out);

    return 0;
}
