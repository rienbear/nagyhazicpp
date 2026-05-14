/**
 * @file jatektabla.cpp
 * @brief A malomjáték táblájának implementációja
 * @details Kezeli a játéktábla megjelenítését, a malmok ellenõrzését és a játékállapot nyilvántartását
 */

 // A program figyelmen kívül hagyja a "biztonsági" CRT figyelmeztetéseket
#define _CRT_SECURE_NO_WARNINGS

#include "jatektabla.h"
#include <iostream>
#include <cstring>
#include <fstream>


// Windows-specifikus könyvtár beolvasása a clear képernyõhöz
#ifdef _WIN32
#include <cstdlib>
#endif

using namespace std;

/**
 * @brief Játéktábla konstruktor
 * @details Inicializálja a tábla állapotát:
 * - Létrehozza a tábla grafikus reprezentációját
 * - Beállítja a pozíciók koordinátáit
 * - Definiálja a malmok kombinációit
 * - Beállítja a szomszédsági mátrixot
 * - Inicializálja a kövek tömbjét
 */
JatekTabla::JatekTabla() {
    // A táblarajz inicializálása: karaktertömb soronkénti másolása
    alaptabla = new char* [13];
    const char* ideiglenesTabla[] = {
        "O-----O-----O",
        "|     |     |",
        "| O---O---O |",
        "| |   |   | |",
        "| | O-O-O | |",
        "| | |   | | |",
        "O-O-O   O-O-O",
        "| | |   | | |",
        "| | O-O-O | |",
        "| |   |   | |",
        "| O---O---O |",
        "|     |     |",
        "O-----O-----O"
    };
    for (int i = 0; i < 13; ++i) {
        alaptabla[i] = new char[14];
        strcpy(alaptabla[i], ideiglenesTabla[i]);
    }

    // Pozíciók (24 mezõ, mindegyikhez 2 koordináta)
    poziciok = new int* [24];
    int tmp[24][2] = {
        {0,0},{0,6},{0,12},{2,2},{2,6},{2,10},
        {4,4},{4,6},{4,8},{6,0},{6,2},{6,4},
        {6,8},{6,10},{6,12},{8,4},{8,6},{8,8},
        {10,2},{10,6},{10,10},{12,0},{12,6},{12,12}
    };
    for (int i = 0; i < 24; ++i) {
        poziciok[i] = new int[2];
        poziciok[i][0] = tmp[i][0];
        poziciok[i][1] = tmp[i][1];
    }

    // Malmok definiálása (16 malom, mindegyik 3 pozícióból áll)
    int tmp_malmok[16][3] = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12},
        {13, 14, 15}, {16, 17, 18}, {19, 20, 21}, {22, 23, 24},
        {1, 10, 22}, {2, 5, 8}, {3, 15, 24}, {4, 11, 19},
        {6, 14, 21}, {7, 12, 16}, {9, 13, 18}, {17, 20, 23}
    };
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 3; ++j) {
            malmok[i][j] = tmp_malmok[i][j];
        }
    }

    // Szomszédsági mátrix inicializálása
    int szomszedsagok[][2] = {
        // Külsõ gyûrû 
        {0,1}, {1,2}, {2,11}, {11,23}, {23,22}, {22,21}, {21, 9}, {9,0},

        // Középsõ gyûrû 
        {3,4}, {4,5}, {5,13}, {13,20}, {20,19}, {19,18}, {18,10}, {10,3},

        // Belsõ gyûrû (négyzet, 12-15)
        {6,7}, {7,8}, {8,12}, {12,17}, {17,16}, {16,15}, {15,11}, {11,6},

        // Radiális kapcsolatok (külsõ-középsõ-belsõ összekötések)
        {9,10}, {10,11},   // Vízszintes bal oldal
        {1,4}, {4,7},   // Függõleges felsõ
        {12,13}, {13,14}, // Vízszintes jobb oldal
        {16,19}, {19,22}  // Függõleges alsó
    };

    // Minden érték false-ra inicializálása
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 24; ++j) {
            szomszedok[i][j] = false;
        }
    }

    // Szomszédos pozíciók beállítása
    for (auto& par : szomszedsagok) {
        int i = par[0];
        int j = par[1];
        szomszedok[i][j] = true;
        szomszedok[j][i] = true; // Szimmetrikus, mert ha A szomszédos B-vel, akkor B is szomszédos A-val
    }

    // Kövek inicializálása (24 mezõ + 1 dummy elem a könnyebb indexeléshez)
    kovek = new char[25];
    for (int i = 1; i <= 24; ++i) kovek[i] = 'O'; // 'O' = üres mezõ
    for (int i = 0; i < 16; ++i) {
        melyikmalomW[i] = 0;
    }
    for (int i = 0; i < 16; ++i) {
        melyikmalomB[i] = 0;
    }
}

/**
 * @brief Játéktábla destruktor
 * @details Felszabadítja a dinamikusan lefoglalt memóriaterületeket:
 * - Az alaptábla tömbjét
 * - A pozíciók tömbjét
 * - A kövek tömbjét
 */
JatekTabla::~JatekTabla() {
    for (int i = 0; i < 13; ++i) {
        delete[] alaptabla[i];
    }
    delete[] alaptabla;

    for (int i = 0; i < 24; ++i) {
        delete[] poziciok[i];
    }
    delete[] poziciok;

    delete[] kovek;
}

/**
 * @brief Tábla kirajzolása a képernyõre
 * @details Frissíti a képernyõt és kirajzolja a tábla aktuális állapotát,
 *          figyelembe véve a lehelyezett köveket.
 *          Platformfüggõ képernyõtörlést használ (Windows/Linux).
 */
void JatekTabla::tablaKirajzolas() const {
#ifndef CPORTA
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
#endif // !CPORTA



    // Ideiglenes tábla létrehozása és feltöltése az alaptáblából
    char ideiglenesTabla[13][14];
    for (int i = 0; i < 13; ++i) {
        strcpy(ideiglenesTabla[i], alaptabla[i]);
    }

    // A kövek aktuális állásának berajzolása az ideiglenes táblára
    for (int i = 1; i <= 24; ++i) {
        int x = poziciok[i - 1][0];
        int y = poziciok[i - 1][1];
        if (kovek[i] == 'W') {
            ideiglenesTabla[x][y] = 'W';
        }
        else if (kovek[i] == 'B') {
            ideiglenesTabla[x][y] = 'B';
        }
    }

    // A tábla soronkénti kiíratása
    for (int i = 0; i < 13; ++i) {
        cout << ideiglenesTabla[i] << endl;
    }
}

/**
 * @brief Kövek tömbjének lekérdezése
 * @return A kövek tömbjére mutató pointer
 */
char* JatekTabla::getKovek() {
    return kovek;
}

/**
 * @brief Követ helyez el a megadott pozícióra
 * @param poz A pozíció (1-24)
 * @param szin A kõ színe ('W' vagy 'B')
 */
void JatekTabla::elhelyezKo(int poz, char szin) {
    if (poz >= 1 && poz <= 24) kovek[poz] = szin;
}

/**
 * @brief Ellenõrzi, hogy egy pozíció szabad-e
 * @param poz Az ellenõrizendõ pozíció (1-24)
 * @return true, ha a pozíció szabad ('O'), false egyébként
 */
bool JatekTabla::pozicioSzabad(int poz) const {
    return kovek[poz] == 'O';
}

/**
 * @brief Szomszédság ellenõrzése
 * @param honnan Az elsõ pozíció
 * @param hova A második pozíció
 * @return true, ha a két pozíció szomszédos, false egyébként
 */
bool JatekTabla::szomszedosPoz(int honnan, int hova) const {
    return szomszedok[honnan - 1][hova - 1];
}

/**
 * @brief Szín egyezés ellenõrzése
 * @param honnan Az ellenõrizendõ pozíció
 * @param c Az elvárt szín ('W' vagy 'B')
 * @return true, ha a pozícióban lévõ kõ színe megegyezik a paraméterrel, false egyébként
 */
bool JatekTabla::azonosSzin(int honnan, char c) const {
    return (kovek[honnan] == c);
}

/**
 * @brief Malom ellenõrzése
 * @param aktualisJatekos Az aktuális játékos színe ('W' vagy 'B')
 * @return true, ha az aktuális játékosnak új malma keletkezett, false egyébként
 * @details Ellenõrzi, hogy az aktuális játékosnak van-e malma a táblán,
 *          és hogy ez új malom-e (az elõzõ állapothoz képest)
 */
bool JatekTabla::malomVan(char aktualisJatekos) {
    bool vanmalom = false;
    bool ujmalome = false;
    int mostanimalmok[16];
    for (int i = 0; i < 16; ++i) {
        if (kovek[malmok[i][0]] != 'O' &&
            kovek[malmok[i][0]] == kovek[malmok[i][1]] &&
            kovek[malmok[i][1]] == kovek[malmok[i][2]] &&
            kovek[malmok[i][0]] == aktualisJatekos) { // Csak az aktuális játékos malma számít
            mostanimalmok[i] = 1;
            vanmalom = true;
        }
        else
        {
            mostanimalmok[i] = 0;
        }
    }

    if (aktualisJatekos == 'W')
    {
        for (int i = 0; i < 16; ++i) {
            if (mostanimalmok[i] != melyikmalomW[i] && mostanimalmok[i] == 1 && vanmalom)
            {
                ujmalome = true;
            }
        }
        for (int i = 0; i < 16; i++)
        {
            melyikmalomW[i] = mostanimalmok[i];
        }
    }
    else
    {
        for (int i = 0; i < 16; ++i) {
            if (mostanimalmok[i] != melyikmalomB[i] && mostanimalmok[i] == 1 && vanmalom)
            {
                ujmalome = true;
            }
        }
        for (int i = 0; i < 16; i++)
        {
            melyikmalomB[i] = mostanimalmok[i];
        }
    }

    return ujmalome;
}