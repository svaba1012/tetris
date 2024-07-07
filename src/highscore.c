#include "highscore.h"



void newHighScore(long long trenutniRezultat, FILE* fajlSaRezultatima){
    char* allHighScores[NUMOFHIGHSCORES];
    char strScore[SCORELENGHT];
    long long llscore[NUMOFHIGHSCORES];
    char novoIme[NAMELENGHT];
    int index;
    for(int j = 0; j < NUMOFHIGHSCORES; j++){
        allHighScores[j] = malloc(NAMELENGHT);
        fgets(allHighScores[j], NAMELENGHT, fajlSaRezultatima);
        fgets(strScore, SCORELENGHT, fajlSaRezultatima);
        llscore[j] = strtoll(strScore, NULL, 10);
    }
    if(trenutniRezultat == -1)
        ;
    else if(trenutniRezultat < llscore[0]){
        fclose(fajlSaRezultatima);
        return;
    }else{
        puts("Oborili ste rekord, da li zelite da ga upisete?(1 - DA)(ostalo - NE)");
        char c[4];
        gets(c);
        if((strncmp(c, "1", 1)) != 0){
            fclose(fajlSaRezultatima);
            return;
        }
        puts("Upisite ime:");
        fgets(novoIme, NAMELENGHT, stdin);
        index = insertInOrder(llscore, trenutniRezultat, NUMOFHIGHSCORES);
        system(CLEAR_CMD);
        printf("\nSvaka cast %d. ste na tabeli najboljih rezultata!\n", NUMOFHIGHSCORES - index);
        insertAtIndex(allHighScores, index, novoIme);
        rewind(fajlSaRezultatima);
        for(int i = 0; i < NUMOFHIGHSCORES; i++){
            fputs(allHighScores[i], fajlSaRezultatima);
            sprintf(strScore, "%lld\n", llscore[i]);
            fputs(strScore, fajlSaRezultatima);
        }
    }
    char* nameAndScore;
    char redniBroj[4];
    puts("");
    puts("\t\tHIGH SCORES");
    for(int i = NUMOFHIGHSCORES - 1; i >= 0; i--){
        nameAndScore = malloc(NAMELENGHT * 2 + 10);
        sprintf(redniBroj, "%d) ", NUMOFHIGHSCORES - i);
        strcpy(nameAndScore, redniBroj);
        *(allHighScores[i] + strlen(allHighScores[i]) - 1) = '\0';
        strcat(nameAndScore, allHighScores[i]);
        strcat(nameAndScore, " - ");
        sprintf(strScore, "%lld", llscore[i]);
        strcat(nameAndScore, strScore);
        puts(nameAndScore);
        free(nameAndScore);
    }
    puts("\n");
    fclose(fajlSaRezultatima);
}

long long* binaryInsert(long long* niz, long long newValue, int size){
    if(size > 1){
        if(newValue >= niz[size / 2])
            binaryInsert(niz + size / 2 + 1, newValue, size / 2 - 1);
        else
            binaryInsert(niz, newValue, size / 2);
    }else
        if(newValue >= niz[0])
            return niz + 1;
        else
            return niz;
}

int insertInOrder(long long* niz, long long newValue, int size){
    long long *p;
    int index;
    p = binaryInsert(niz, newValue, size);
    index = p - niz;
    if(index == 0)
        return -1;
    index--;
    memcpy(niz, niz + 1, sizeof(long long) * index);
    niz[index] = newValue;
    return index;
}

void insertAtIndex(char** niz, int index, char* newValue){
    memcpy(niz, niz + 1, sizeof(char*) * index);
    niz[index] = newValue;
}
