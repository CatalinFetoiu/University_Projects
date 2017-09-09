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

#define traverse(list, it) \
    for (vector<int>::iterator it = list.begin(); it != list.end(); it++)

#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2

#define alphabetSize 26

/* Vectorul in care sunt retinute cuvintele, impreuna cu un vector ce retine
   literele adaugate in graf                                                 */
int N;
vector<string> words;
bool chosen[26];

/* Graful (orientat) de litere este retinut folosind liste de adiacenta */
vector<vector<int> > AdjList;
vector<int> topoSort;

/* Vectorul visited este folosit pentru a eticheta nodurile in cadrul
   parcurgerii dfs a grafului de litere. Cele doua flag-uri indica gasirea 
   unui ciclu in graf sau prezenta unei ordonari lexicografice gresite,
   determinata inaintea parcurgerii                                         */
int visited[26];
bool foundCycle;
bool wrongOrder;


/* Functii folosite pentru conversia intre o litera si un intreg */
int getIntValue(char c) {
    return c - 'a';
}

char getCharValue(int value) {
    return 'a' + value;
}

/* Determina pentru doua cuvinte consecutive, constrangerile necesare pentru ca
   ordinea sa fie lexicografica. Cele doua cuvinte sunt parcurse pana la
   minimul intre lungimile acestora, pana cand este intalnita o pereche de
   litere diferite. Pentru acestea este adaugata in graf o muchie de la litera
   din primul cuvant catre litera din al doilea cuvant. Daca nu este gasita
   nicio astfel de pereche si lungimea primului cuvant este mai mare decat
   a celui de-al doilea, inseamna ca ordinea nu este lexicografica           */
void linkWords(int index) {
    string firstWord = words[index];
    string secondWord = words[index+1];

    int minLength = min(firstWord.size(), secondWord.size());
    int count = 0;

    for (int i = 0; i < minLength; i++) {
        int first = getIntValue(words[index][i]);
        int second = getIntValue(words[index+1][i]);

        if (first != second) {
            AdjList[first].push_back(second);
            chosen[first] = true;
            chosen[second] = true;
            count++;

            break;
        }
    }

    if (count == 0 && firstWord.size() > secondWord.size()) {
        cout << "Imposibil" << endl;
        wrongOrder = true;
    }
}

/* Creeaza graful corespunzator ordonarii literelor in alfabet, prin apelarea
   functiei linkWords pentru toate perechile de cuvinte consecutive          */
void createGraph() {
    AdjList.clear();
    AdjList.resize(alphabetSize);

    memset(chosen, false, sizeof(chosen));
    wrongOrder = false;

    for (int i = 0; i < N-1; i++) {
        linkWords(i);
        if (wrongOrder) {
            return;
        }
    }
}

/* Realizeaza o parcurgere in adancime a grafului de litere, nodurile fiind
   etichetate ca WHITE (nevizitat), GRAY (in curs de vizitare) sau BLACK 
   (vizitat). Daca pentru un nod, unul din vecinii sai este in curs de vizitare,
   graful contine un ciclu si este afisat mesajul "Imposibil"               */
void dfs(int node) {
    if (foundCycle) {
        return;
    }

    visited[node] = DFS_GRAY;

    traverse(AdjList[node], next) {
        if (visited[*next] == DFS_WHITE) {
            dfs(*next);
        } else if (visited[*next] == DFS_GRAY && *next != node) {
            foundCycle = true;
            return;
        }
    }

    topoSort.push_back(node);
    visited[node] = DFS_BLACK;
}

/* Realizeaza sortarea topologica a grafului de litere */
void topologicalSort() {
    memset(visited, false, sizeof(visited));
    topoSort.clear();
    foundCycle = false;

    for (int node = 0; node < alphabetSize; node++) {
        if (chosen[node]) {
            if (visited[node] == DFS_WHITE) {
                dfs(node);
                if (foundCycle) {
                    return;
                }
            }
        }
    }

    reverse(topoSort.begin(), topoSort.end());
}

/* Afiseaza alfabetul corespunzator ordinii obtinute in urma sortarii topo-
   logice. Sunt afisate mai intai literele din graf, apoi literele ramase,
   ordinea acestora din urma in alfabet nefiind importanta                  */
void printTopologicalOrder() {
    if (foundCycle) {
        cout << "Imposibil" << endl;
    } else {
        for (unsigned int i = 0; i < topoSort.size(); i++) {
            cout << getCharValue(topoSort[i]);
        }
        for (int i = 0; i < alphabetSize; i++) {
            if (!chosen[i]) {
                cout << getCharValue(i);
            }
        }
        cout << endl;
    }
}



int main() {
    FILE* in = freopen("permutari.in", "r", stdin);
    FILE* out = freopen("permutari.out", "w", stdout);

    cin >> N;

    words.clear();
    words.resize(N);

    for (int i = 0; i < N; i++) {
        cin >> words[i];
    }

    /* Este creat graful si este realizata sortarea topologica a acestuia, fiind
       afisat mesajul "Imposibil" in cazul in care ordinea nu este corecta sau
       graful contine cicluri                                               */

    createGraph();

    if (wrongOrder) {
        return 0;
    }

    topologicalSort();

    printTopologicalOrder();

    fclose(in);
    fclose(out);

    return 0;
}
