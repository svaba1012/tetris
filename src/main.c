#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "highscore.h"

#ifdef _WIN32
    #include <conio.h>
    #define CLOCK_VAL 1000
    #define CLEAR_CMD "cls"
#endif
#ifdef unix
    #define CLOCK_VAL 1000000
    #include "conio_linux.h"
    #define CLEAR_CMD "clear"
#endif

#define X 12
#define Y 20
#define START 5
#define IVICA 6
#define POLAZNALINIJA START - 1

enum oblik{KVADRAT, STAP, GE, TE, GEK, LMUNJA, DMUNJA};

void crtaj(char *a, char *b, long long *rezultat);
void novaFigura(int pozX, int pozY, int *vrsta, int *fx, int *fy);
void napraviFiguru(char *a, int *fx, int *fy, int width);
void napraviKvadrat(int *x, int *y, int pozX, int pozY);
void napraviStap(int *x, int *y, int pozX, int pozY);
void napraviGe(int *x, int *y, int pozX, int pozY);
void napraviGek(int *x, int *y, int pozX, int pozY);
void napraviLmunja(int *x, int *y, int pozX, int pozY);
void napraviDmunja(int *x, int *y, int pozX, int pozY);
void napraviTe(int *x, int *y, int pozX, int pozY);
void ocisti(char *a, char *b, int width, int height);
void sacuvajKopiju(char *a, char *b);
void kontrolisi(char* tabla, int *x, int *y, char *c, int *vrsta);
bool isGameover(char *a);
void endMenu(long long *rezultat);
long long score(char *a);
void sledecaFigura(int *sledecaVrsta);
void postaviPozicijuNove(int *pozX, int *pozY, int *vrsta, int *sledecaVrsta);


int main(){
    char kontrola = 's';
    char tabla[Y][X], tablaKopija[Y][X], tablaSledecaF[IVICA][IVICA], tablaSledecaFKopija[IVICA][IVICA];
    int pozicijaDelovaFigureX[14], pozicijaDelovaFigureY[14], pozicijaDelovaSFX[14], pozicijaDelovaSFY[14];
    int cl;
    time_t t;
    int pozX, pozY, vrsta, sledecaVrsta;
    long long rezultat = 0;
    bool nova = true, gameover = false;
    for(char *p = tablaSledecaF[0], *q = tablaSledecaFKopija[0]; p < tablaSledecaF[0] + IVICA * IVICA; p++, q++){
        if((((p - tablaSledecaF[0]) % IVICA) == 0) || (((p - tablaSledecaF[0] + 1) % IVICA) == 0) || (p > tablaSledecaF[IVICA - 1]) || (p < tablaSledecaF[1]))
            *q = *p = '#';
        else
            *q = *p = ' ';
    }
    for(char *p = tabla[0], *q = tablaKopija[0]; p < tabla[0] + Y * X; p++, q++){
        if((((p - tabla[0]) % X) == 0) || (((p - tabla[0] + 1) % X) == 0) || (p > tabla[Y - 1]))
            *q = *p = '#';
        else
            *q = *p = ' ';
    }
    srand((unsigned)time(&t));
    sledecaVrsta = rand() % 7;
    while(1){
        ocisti(tabla[0], tablaKopija[0], X, Y);
        if(nova){
            nova = false;
            ocisti(tablaSledecaF[0], tablaSledecaFKopija[0], IVICA, IVICA);
            postaviPozicijuNove(&pozX, &pozY, &vrsta, &sledecaVrsta);
            novaFigura(pozX, pozY, &vrsta, pozicijaDelovaFigureX, pozicijaDelovaFigureY);
            novaFigura(3, 3, &sledecaVrsta, pozicijaDelovaSFX, pozicijaDelovaSFY);
            napraviFiguru(tablaSledecaF[0], pozicijaDelovaSFX, pozicijaDelovaSFY, IVICA);
        }
        napraviFiguru(tabla[0], pozicijaDelovaFigureX, pozicijaDelovaFigureY, X);
        for(int i = 4; i < 14; i++){
            for(int j = 0; j < 4; j++){
                if((tabla[pozicijaDelovaFigureY[i]][pozicijaDelovaFigureX[i]] != ' ') && (pozicijaDelovaFigureY[i] - 1 == pozicijaDelovaFigureY[j]) && (pozicijaDelovaFigureX[i] == pozicijaDelovaFigureX[j])){
                    nova = true;
                    sacuvajKopiju(tabla[0], tablaKopija[0]);
                    gameover = isGameover(tablaKopija[0]);
                    rezultat += score(tablaKopija[0]);
                    goto GIZDA;
                }
            }
        }
        if(gameover){
            break;
        }
        GIZDA:
        crtaj(tabla[START], tablaSledecaF[0], &rezultat);
        cl = clock();
        while(clock() - cl < CLOCK_VAL){
            // printf("Sekunda prosla\n");
            if(kbhit()){
                kontrola = getch();
                break;
            }
        }
        kontrolisi(tabla[0], pozicijaDelovaFigureX, pozicijaDelovaFigureY, &kontrola, &vrsta);
        system(CLEAR_CMD);
    }
    endMenu(&rezultat);
    return 0;
}

void crtaj(char *a, char *b, long long *rezultat){
    printf("Rezultat: %lld\n\n", *rezultat);
    for(char *p = b; p < b + IVICA * IVICA; p++){
        putchar(*p);
        if(p == b - 1 + IVICA * (IVICA / 2))
            printf("Sledeca figura");
        if((p - b + 1) % IVICA == 0)
            putchar('\n');
    }
    for(int i = 0; i < X; i++)
        putchar('#');
    putchar('\n');
    for(char *p = a; p < a + X * (Y - START); p++){
        putchar(*p);
        if((p - a + 1) % X == 0)
            putchar('\n');
    }
}

void postaviPozicijuNove(int *pozX, int *pozY, int *vrsta, int *sledecaVrsta){
    *vrsta = *sledecaVrsta;
    *sledecaVrsta = rand() % 7;
    *pozX = rand() % (X - 5) + 3;
    *pozY = POLAZNALINIJA;
}

void novaFigura(int pozX, int pozY, int *vrsta, int *fx, int *fy){
    switch(*vrsta){
        case KVADRAT:   napraviKvadrat(fx, fy, pozX, pozY);
                        break;
        case STAP:      napraviStap(fx, fy, pozX, pozY);
                        break;
        case GE:        napraviGe(fx, fy, pozX, pozY);
                        break;
        case TE:        napraviTe(fx, fy, pozX, pozY);
                        break;
        case GEK:       napraviGek(fx, fy, pozX, pozY);
                        break;
        case LMUNJA:    napraviLmunja(fx, fy, pozX, pozY);
                        break;
        case DMUNJA:    napraviDmunja(fx, fy, pozX, pozY);
                        break;
    }
}

void napraviFiguru(char *a, int *fx, int *fy, int width){
    for(int i = 0; i < 4; i++){
        a[fx[i] + width * fy[i]] = 'O';
    }
}

void napraviKvadrat(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX + 1;
    *(y + 1) = pozY;
    *(x + 2) = pozX;
    *(y + 2) = pozY - 1;
    *(x + 3) = pozX + 1;
    *(y + 3) = pozY - 1;
    *(x + 4) = pozX - 1;
    *(y + 4) = pozY - 1;
    *(x + 5) = pozX - 1;
    *(y + 5) = pozY;
    *(x + 6) = pozX;
    *(y + 6) = pozY - 2;
    *(x + 7) = pozX + 1;
    *(y + 7) = pozY - 2;
    *(x + 8) = pozX + 2;
    *(y + 8) = pozY - 1;
    *(x + 9) = pozX + 2;
    *(y + 9) = pozY;
    *(x + 10) = pozX + 1;
    *(y + 10) = pozY + 1;
    *(x + 11) = pozX;
    *(y + 11) = pozY + 1;
    *(x + 12) = pozX;
    *(y + 12) = pozY + 1;
    *(x + 13) = pozX;
    *(y + 13) = pozY + 1;
}

void napraviStap(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX;
    *(y + 1) = pozY - 1;
    *(x + 2) = pozX;
    *(y + 2) = pozY - 2;
    *(x + 3) = pozX;
    *(y + 3) = pozY + 1;
    *(x + 4) = pozX - 1;
    *(y + 4) = pozY - 1;
    *(x + 5) = pozX - 1;
    *(y + 5) = pozY;
    *(x + 6) = pozX - 1;
    *(y + 6) = pozY - 2;
    *(x + 7) = pozX - 1;
    *(y + 7) = pozY + 1;
    *(x + 8) = pozX;
    *(y + 8) = pozY - 3;
    *(x + 9) = pozX;
    *(y + 9) = pozY + 2;
    *(x + 10) = pozX + 1;
    *(y + 10) = pozY + 1;
    *(x + 11) = pozX + 1;
    *(y + 11) = pozY;
    *(x + 12) = pozX + 1;
    *(y + 12) = pozY - 2;
    *(x + 13) = pozX + 1;
    *(y + 13) = pozY - 1;

}
void napraviGe(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX;
    *(y + 1) = pozY + 1;
    *(x + 2) = pozX;
    *(y + 2) = pozY - 1;
    *(x + 3) = pozX + 1;
    *(y + 3) = pozY - 1;
    *(x + 4) = pozX - 1;
    *(y + 4) = pozY - 1;
    *(x + 5) = pozX - 1;
    *(y + 5) = pozY;
    *(x + 6) = pozX - 1;
    *(y + 6) = pozY - 1;
    *(x + 7) = pozX - 1;
    *(y + 7) = pozY + 1;
    *(x + 8) = pozX;
    *(y + 8) = pozY - 2;
    *(x + 9) = pozX;
    *(y + 9) = pozY + 2;
    *(x + 10) = pozX + 2;
    *(y + 10) = pozY - 1;
    *(x + 11) = pozX + 1;
    *(y + 11) = pozY;
    *(x + 12) = pozX + 1;
    *(y + 12) = pozY - 2;
    *(x + 13) = pozX + 1;
    *(y + 13) = pozY + 1;
}
void napraviTe(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX + 1;
    *(y + 1) = pozY;
    *(x + 2) = pozX - 1;
    *(y + 2) = pozY;
    *(x + 3) = pozX;
    *(y + 3) = pozY - 1;
    *(x + 4) = pozX + 1;
    *(y + 4) = pozY + 1;
    *(x + 5) = pozX;
    *(y + 5) = pozY + 1;
    *(x + 6) = pozX - 1;
    *(y + 6) = pozY + 1;
    *(x + 7) = pozX - 2;
    *(y + 7) = pozY;
    *(x + 8) = pozX + 2;
    *(y + 8) = pozY;
    *(x + 9) = pozX - 1;
    *(y + 9) = pozY - 1;
    *(x + 10) = pozX + 1;
    *(y + 10) = pozY + 1;
    *(x + 11) = pozX;
    *(y + 11) = pozY - 2;
    *(x + 12) = pozX + 1;
    *(y + 12) = pozY - 1;
    *(x + 13) = pozX + 1;
    *(y + 13) = pozY - 1;
}

void napraviGek(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX;
    *(y + 1) = pozY + 1;
    *(x + 2) = pozX;
    *(y + 2) = pozY - 1;
    *(x + 3) = pozX - 1;
    *(y + 3) = pozY - 1;
    *(x + 4) = pozX + 1;
    *(y + 4) = pozY - 1;
    *(x + 5) = pozX - 1;
    *(y + 5) = pozY;
    *(x + 6) = pozX - 2;
    *(y + 6) = pozY - 1;
    *(x + 7) = pozX - 1;
    *(y + 7) = pozY + 1;
    *(x + 8) = pozX;
    *(y + 8) = pozY - 2;
    *(x + 9) = pozX;
    *(y + 9) = pozY + 2;
    *(x + 10) = pozX + 1;
    *(y + 10) = pozY - 1;
    *(x + 11) = pozX + 1;
    *(y + 11) = pozY;
    *(x + 12) = pozX - 1;
    *(y + 12) = pozY - 2;
    *(x + 13) = pozX + 1;
    *(y + 13) = pozY + 1;
}
void napraviLmunja(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX;
    *(y + 1) = pozY + 1;
    *(x + 2) = pozX + 1;
    *(y + 2) = pozY;
    *(x + 3) = pozX + 1;
    *(y + 3) = pozY - 1;
    *(x + 4) = pozX;
    *(y + 4) = pozY - 1;
    *(x + 5) = pozX - 1;
    *(y + 5) = pozY;
    *(x + 6) = pozX + 1;
    *(y + 6) = pozY - 2;
    *(x + 7) = pozX - 1;
    *(y + 7) = pozY + 1;
    *(x + 8) = pozX;
    *(y + 8) = pozY + 2;
    *(x + 9) = pozX + 1;
    *(y + 9) = pozY + 1;
    *(x + 10) = pozX + 2;
    *(y + 10) = pozY;
    *(x + 11) = pozX + 2;
    *(y + 11) = pozY - 1;
    *(x + 12) = pozX + 2;
    *(y + 12) = pozY - 1;
    *(x + 13) = pozX + 1;
    *(y + 13) = pozY + 1;
}
void napraviDmunja(int *x, int *y, int pozX, int pozY){
    *x = pozX;
    *y = pozY;
    *(x + 1) = pozX;
    *(y + 1) = pozY + 1;
    *(x + 2) = pozX - 1;
    *(y + 2) = pozY;
    *(x + 3) = pozX - 1;
    *(y + 3) = pozY - 1;
    *(x + 4) = pozX;
    *(y + 4) = pozY - 1;
    *(x + 5) = pozX + 1;
    *(y + 5) = pozY;
    *(x + 6) = pozX + 1;
    *(y + 6) = pozY + 1;
    *(x + 7) = pozX - 1;
    *(y + 7) = pozY - 2;
    *(x + 8) = pozX;
    *(y + 8) = pozY + 2;
    *(x + 9) = pozX - 1;
    *(y + 9) = pozY + 1;
    *(x + 10) = pozX - 2;
    *(y + 10) = pozY;
    *(x + 11) = pozX - 2;
    *(y + 11) = pozY - 1;
    *(x + 12) = pozX - 2;
    *(y + 12) = pozY - 1;
    *(x + 13) = pozX + 1;
    *(y + 13) = pozY + 1;
}

void ocisti(char *a, char *b, int width, int height){
   for(char *p = a, *q = b; p < a + height * width; p++, q++){
        *p = *q;
    }
}

void sacuvajKopiju(char *a, char *b){
   for(char *p = a, *q = b; p < a + Y * X; p++, q++){
        *q = *p;
    }
}

void kontrolisi(char* tabla, int *x, int *y, char *c, int *vrsta){
    int ox, oy, h1 = 0, h2 = 0, adresa;
    bool deoFigure = false;
    switch(*c){
        case 'a':   for(int i = 4; i < 14; i++){
                        for(int j = 0; j < 4; j++){
                            if((tabla[y[i] * X + x[i]] != ' ') && (y[i] == y[j]) && (x[i] + 1 == x[j])){
                                goto IVICA1;
                            }
                        }
                    }
                    for(int i = 0; i < 14; i++){
                        x[i]--;
                    }
                    IVICA1:
                        break;
        case 's':   for(int i = 0; i < 14; i++){
                        y[i]++;
                    }
                    break;
        case 'd':   for(int i = 4; i < 14; i++){
                        for(int j = 0; j < 4; j++){
                            if((tabla[y[i] * X + x[i]] != ' ') && (y[i] == y[j]) && (x[i] - 1 == x[j])){
                                goto IVICA2;
                            }
                        }
                    }
                    for(int i = 0; i < 14; i++){
                        x[i]++;
                    }
                    IVICA2:
                        break;
        case 'w':   for(int i = 1; i < 4; i++){
                        if(y[0] - y[i] > h1){
                            h1 = y[0] - y[i];
                        }
                        if(y[i] - y[0] > h2){
                            h2 = y[i] - y[0];
                        }
                    }
                    for(int i = 1; i <= h1; i++){
                        adresa = y[0] * X + x[0] + i;
                        for(int j = 1; j < 4; j++){
                            if(adresa == y[j] * X + x[j])
                                deoFigure = true;
                        }
                        if(((tabla[adresa]) != ' ') && !deoFigure)
                            goto IVICA3;
                    }
                    deoFigure = false;
                    for(int i = 1; i <= h2; i++){
                        adresa = y[0] * X + x[0] - i;
                        for(int j = 1; j < 4; j++){
                            if(adresa == y[j] * X + x[j])
                                deoFigure = true;
                        }
                        if(((tabla[adresa]) != ' ') && !deoFigure)
                            goto IVICA3;
                    }
                    if(*vrsta == KVADRAT)
                        break;
                    for(int i = 1; i < 14; i++){
                        ox = x[i] - x[0];
                        oy = y[0] - y[i];
                        x[i] = x[0] + oy;
                        y[i] = y[0] + ox;
                    }
                    IVICA3:
                        break;
        default:    break;
    }
    *c = 's';
}

bool isGameover(char *a){
    for(char *p = a; p < a + X * START; p++){
        if(*p == 'O')
            return true;
    }
    return false;
}

void endMenu(long long *rezultat){
    char c;
    system(CLEAR_CMD);
    printf("Rezultat: %lld\n", *rezultat);
    FILE* fajlSaRezultatima;
    fajlSaRezultatima = fopen("./highscore.txt", "r+");
    if(fajlSaRezultatima == NULL){
        fajlSaRezultatima = fopen("./highscore.txt", "w");
        char userScore[8] = "user\n0\n";
        for(int i = 0; i < 10; i++){
            fwrite(userScore, 1, 7, fajlSaRezultatima);
        }
        fclose(fajlSaRezultatima);
        fajlSaRezultatima = fopen("./highscore.txt", "r+");
    }
    newHighScore(*rezultat, fajlSaRezultatima);
    printf("Pritisnite 0 za izlaz ili 1 za novu igru\n");
    while((c != '0') && (c != '1')){
        if(kbhit())
            c = getch();
    }
    system(CLEAR_CMD);
    if(c == '1')
        main();
}

long long score(char *a){
    int brojKolona = 0;
    int indeksiKolona[4];
    bool punRed;
    for(int i = Y - 2; i > START; i--){
        if(brojKolona > 3)
            break;
        punRed = true;
        for(int j = 1; j < X - 1; j++){
            if(a[i * X + j] == ' '){
                punRed = false;
                break;
            }
        }
        if(punRed){
            indeksiKolona[brojKolona++] = i;
        }
    }

    for(int i = brojKolona - 1; i >= 0; i--){
        for(int j = indeksiKolona[i]; j > START; j--){
            for(int s = 1; s < X - 1; s++){
                a[j * X + s] = a[(j - 1) * X + s];
            }
        }
    }
    if(brojKolona == 0)
        return 0;
    brojKolona--;
    if(brojKolona < 2)
        return (80 * pow(5, brojKolona));
    return (80 * pow(5, brojKolona)) + 1;
}

void sledecaFigura(int *sledecaVrsta){

}
