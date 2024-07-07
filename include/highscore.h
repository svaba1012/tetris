#ifndef HIGH_SCORE
#define HIGH_SCORE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #define CLEAR_CMD "cls"
#endif
#ifdef unix
    #define CLEAR_CMD "clear"
#endif


#define NAMELENGHT 20
#define SCORELENGHT 20
#define NUMOFHIGHSCORES 10

void newHighScore(long long trenutniRezultat, FILE* fajlSaRezultatima);
long long* binaryInsert(long long* niz, long long newValue, int size);
int insertInOrder(long long* niz, long long newValue, int size);
void insertAtIndex(char** niz, int index, char* newValue);

#endif // HIGH_SCORE
