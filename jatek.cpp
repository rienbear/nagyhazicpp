/**
 * @file jatek.cpp
 * @brief A malomjáték fõ vezérlõ osztályának implementációja
 * @details Kezeli a játékmenetet, a játékosok váltását, állapot mentést és betöltést
 */

 // A program figyelmen kívül hagyja a "biztonsági" CRT figyelmeztetéseket
#define _CRT_SECURE_NO_WARNINGS

#include "jatek.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <limits> // std::numeric_limits


/**
 * @brief Pozíció bekérése a felhasználótól
 * @param jatekosSzin Az aktuális játékos színe ('W' vagy 'B')
 * @param szoveg A megjelenítendõ segédüzenet
 * @return A bekért pozíció (1-24)
 * @details Hibakezeléssel ellátott bemenet olvasás a standard inputról
 */
int bekeresPozicio(char jatekosSzin, const std::string& szoveg) {
    int poz;
    std::cout << (jatekosSzin == 'W' ? "Feher" : "Fekete") << " jatekos kovetkezik " << szoveg << "(1-24): ";
    while (!(std::cin >> poz)) {
        std::cout << "Hibas bemenet. Kerem, szamot adjon meg 1 es 24 kozott: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return poz;
}

/**
 * @brief Érvényes lépés ellenõrzése
 * @param poz A vizsgálandó pozíció
 * @return true, ha a lépés érvényes, false ha nem (pl. mentés kérés)
 */
bool Jatek::ervenyesLepes(int poz) {
    if (poz == -1) {
        tabla.tablaKirajzolas();
        allapotMentes("mentes.txt");
        return false;
    }
    return true;
}

/**
 * @brief Játék osztály konstruktor
 * @details Inicializálja a játékot, létrehozza a játékosokat és beállítja a kezdõállapotot
 */
Jatek::Jatek()
    : tabla(), jatekFazis(JatekFazis::BABU_LERAKAS), aktualisJatekos('W')
{
    feherJatekos = new FeherJatekos();
    feketeJatekos = new FeketeJatekos();
}

/**
 * @brief Játék osztály destruktor
 * @details Felszabadítja a dinamikusan lefoglalt játékos objektumokat
 */
Jatek::~Jatek() {
    delete feherJatekos;
    delete feketeJatekos;
}

/**
 * @brief Játékos lépésének kezelése
 * @param jatekos A lépést végzõ játékos objektum
 * @param jatekosSzin A játékos színe ('W' vagy 'B')
 * @param fazis A játék aktuális fázisa (BABU_LERAKAS vagy BABU_MOZGATAS)
 */
void Jatek::jatekosLepes(Jatekos* jatekos, char jatekosSzin, JatekFazis fazis) {
    if (fazis == JatekFazis::BABU_LERAKAS) {
        int poz = bekeresPozicio(jatekosSzin, "");
        ervenyesLepes(poz);
        while (!jatekos->elhelyezKo(tabla, poz)) {
            poz = bekeresPozicio(jatekosSzin, "");
            ervenyesLepes(poz);
        }
    }
    else if (fazis == JatekFazis::BABU_MOZGATAS) {
        int honnan = bekeresPozicio(jatekosSzin, "honnan");
        ervenyesLepes(honnan);
        int hova = bekeresPozicio(jatekosSzin, "hova");
        ervenyesLepes(hova);
        while (!jatekos->mozgatKo(tabla, honnan, hova, (jatekosSzin == 'W' ? feketeJatekos->getEltavolitott() : feherJatekos->getEltavolitott()))) {
            honnan = bekeresPozicio(jatekosSzin, "honnan");
            ervenyesLepes(honnan);
            hova = bekeresPozicio(jatekosSzin, "hova");
            ervenyesLepes(hova);
        }
    }
}

/**
 * @brief Malom esetén követ eltávolít
 * @param jatekos Az eltávolítást végzõ játékos
 * @param jatekosSzin A játékos színe
 * @param ellenfelSzin Az ellenfél színe
 * @details Ha malom keletkezett, lehetõséget ad egy ellenfél kõ eltávolítására
 */
void Jatek::malomEsetenEltavolitas(Jatekos* jatekos, char jatekosSzin, char ellenfelSzin) {
    if (tabla.malomVan(jatekosSzin)) {
        int honnan;
        std::cout << (jatekosSzin == 'W' ? "A feher" : "A fekete") << " jatekosnak malomja lett! Eltavolithatja a "
            << (ellenfelSzin == 'W' ? "feher" : "fekete") << " egy kovet ebbol a poziciobol: " << std::endl;
        while (!(std::cin >> honnan)) {
            std::cout << "Hibas bemenet. Kerem, szamot adjon meg: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        while (!jatekos->eltavolitKo(tabla, honnan)) {
            std::cout << "Ervenytelen pozicio. Kerem, adjon meg egy ervenyes poziciot az eltavolitashoz: ";
            while (!(std::cin >> honnan)) {
                std::cout << "Hibas bemenet. Kerem, szamot adjon meg: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        tabla.malomVan(ellenfelSzin);
    }
}

/**
 * @brief Játék indítása és fõ ciklusa
 * @details Vezérli a játék fázisait (kõ lerakás, mozgatás) és a játékosok váltását
 */
void Jatek::jatekInditas() {
    tabla.tablaKirajzolas();

    while (jatekFazis != JatekFazis::JATEK_VEGE) {
        char aktualisSz = aktualisJatekos;
        Jatekos* aktualisJatekosObj = (aktualisSz == 'W' ? feherJatekos : feketeJatekos);
        char ellenfelSz = (aktualisSz == 'W' ? 'B' : 'W');

        if (jatekFazis == JatekFazis::BABU_LERAKAS) {
            jatekosLepes(aktualisJatekosObj, aktualisSz, jatekFazis);
            malomEsetenEltavolitas(aktualisJatekosObj, aktualisSz, ellenfelSz);
            if (feherJatekos->getKovekSzama() == 0 && feketeJatekos->getKovekSzama() == 0) {
                tabla.tablaKirajzolas();
                jatekFazis = JatekFazis::BABU_MOZGATAS;
            }
        }
        else if (jatekFazis == JatekFazis::BABU_MOZGATAS) {
            jatekosLepes(aktualisJatekosObj, aktualisSz, jatekFazis);
            malomEsetenEltavolitas(aktualisJatekosObj, aktualisSz, ellenfelSz);
            if (feherJatekos->getEltavolitott() == 7 || feketeJatekos->getEltavolitott() == 7)
            {
                std::cout << "Vege a jateknak! A nyertes a " << (feketeJatekos->getEltavolitott() == 7 ? "fekete " : "feher ") << "jatekos! Gratulalok!" << std::endl;
                jatekFazis = JatekFazis::JATEK_VEGE;
            }
        }
        aktualisJatekos = ellenfelSz;
    }
}

/**
 * @brief Játékállapot mentése fájlba
 * @param fajlnev A célfájl neve
 * @details Elmenti a játék aktuális állapotát a megadott fájlba
 */
void Jatek::allapotMentes(const std::string& fajlnev) {
    std::ofstream fajl(fajlnev);
    if (!fajl) {
        std::cerr << "Hiba a fajl megnyitasakor menteshez: " << fajlnev << std::endl;
        return;
    }

    fajl << static_cast<int>(jatekFazis) << std::endl;

    // Játékosok adatainak mentése
    fajl << feherJatekos->getKovekSzama() << " " << feherJatekos->getEltavolitott() << std::endl;
    fajl << feketeJatekos->getKovekSzama() << " " << feketeJatekos->getEltavolitott() << std::endl;

    // Malom adatok mentése
    for (int i = 0; i < 16; ++i) {
        fajl << tabla.getMelyikmalomW(i) << " ";
    }
    fajl << std::endl;
    for (int i = 0; i < 16; ++i) {
        fajl << tabla.getMelyikmalomB(i) << " ";
    }
    fajl << std::endl;

    const char* kovek = tabla.getKovek();
    for (int i = 1; i <= 24; ++i) {
        fajl << kovek[i];
    }
    fajl << std::endl << aktualisJatekos << std::endl;

    fajl.close();
    std::cout << "Allapot mentve a(z) " << fajlnev << " fajlba.\n";
}

/**
 * @brief Játékállapot betöltése fájlból
 * @param fajlnev A forrásfájl neve
 * @details Betölti a játék állapotát a megadott fájlból
 */
void Jatek::allapotBetoltes(const std::string& fajlnev) {
    std::ifstream fajl(fajlnev);
    if (!fajl) {
        std::cerr << "Hiba a fajl betoltesekor!" << std::endl;
        return;
    }

    int fazis;
    fajl >> fazis;
    jatekFazis = static_cast<JatekFazis>(fazis);
    if (fazis < static_cast<int>(JatekFazis::BABU_LERAKAS) || fazis > static_cast<int>(JatekFazis::JATEK_VEGE)) {
        std::cerr << "Hiba: Ervenytelen JatekFazis ertek: " << fazis << std::endl;
    }

    // Játékosok adatainak betöltése
    int feherKovekSzama, feherEltavolitott, feketeKovekSzama, feketeEltavolitott;
    fajl >> feherKovekSzama >> feherEltavolitott;
    fajl >> feketeKovekSzama >> feketeEltavolitott;

    feherJatekos->setKovekSzama(feherKovekSzama);
    feherJatekos->setEltavolitott(feherEltavolitott);
    feketeJatekos->setKovekSzama(feketeKovekSzama);
    feketeJatekos->setEltavolitott(feketeEltavolitott);

    // Malom adatok betöltése
    for (int i = 0; i < 16; ++i) {
        int temp;
        fajl >> temp;
        tabla.setMelyikmalomW(i, temp);
    }
    for (int i = 0; i < 16; ++i) {
        int temp;
        fajl >> temp;
        tabla.setMelyikmalomB(i, temp);
    }

    for (int i = 1; i <= 24; ++i) {
        char c;
        fajl >> c;
        tabla.elhelyezKo(i, c);
    }

    fajl >> aktualisJatekos;
    fajl.close();
    std::cout << "Allapot betoltve a(z) " << fajlnev << " fajlbol.\n";
}