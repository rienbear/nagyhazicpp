/**
 * @file feketejatekos.cpp
 * @brief Fekete játékos implementációja a malomjátékhoz
 * @details A fekete játékos mûveleteit valósítja meg (kõ elhelyezése, mozgatása, eltávolítása)
 */

 // A program figyelmen kívül hagyja a "biztonsági" CRT figyelmeztetéseket
#define _CRT_SECURE_NO_WARNINGS

#include "feketejatekos.h"

#include <iostream>
#include <cstring>
#include <fstream>



/**
 * @brief Fekete játékos konstruktor
 * @details A Jatekos osztály konstruktorát hívja meg
 */
FeketeJatekos::FeketeJatekos() : Jatekos() {}

/**
 * @brief Követ helyez el a táblára
 * @param tabla A játéktábla referenciája
 * @param poz A pozíció, ahova a követ helyezzük (1-24)
 * @return true, ha sikerült elhelyezni, false egyébként
 * @details Ellenõrzi, hogy van-e még köve a játékosnak, és hogy szabad-e a pozíció
 */
bool FeketeJatekos::elhelyezKo(JatekTabla& tabla, int poz) {
    if (kovekSzama > 0 && tabla.pozicioSzabad(poz)) {
        tabla.elhelyezKo(poz, 'B');
        kovekSzama--;
        tabla.tablaKirajzolas();
        std::cout << "Fekete jatekos elhelyezett egy kovet a(z) " << poz << ". poziciora." << std::endl;
        return true;
    }
    else {
        tabla.tablaKirajzolas();
        if (poz > 0 && poz < 25) {
            if (kovekSzama <= 0) {
                std::cout << "Nincs tobb elhelyezheto fekete ko!" << std::endl;
            }
            else {
                std::cout << "A(z) " << poz << ". pozicio mar foglalt!" << std::endl;
            }
        }
        else
        {
            if (poz != -1) {
                std::cout << "Nem ervenyes pozicio!" << std::endl;
            }
            else {
                std::cout << "Mentve" << std::endl;
            }
            
        }

        return false;
    }
}

/**
 * @brief Követ mozgat a táblán
 * @param tabla A játéktábla referenciája
 * @param honnan A pozíció, ahonnan mozgatjuk a követ
 * @param hova A pozíció, ahova mozgatjuk a követ
 * @param eltavolitva Az eltávolított kövek száma (6 esetén ugrás is lehet)
 * @return true, ha sikerült mozgatni, false egyébként
 * @details Külön kezeli az ugrásos és a normál lépéseket
 */
bool FeketeJatekos::mozgatKo(JatekTabla& tabla, int honnan, int hova, int eltavolitva) {
    if (eltavolitva == 6)
    {
        if (tabla.pozicioSzabad(hova) && tabla.azonosSzin(honnan, 'B')) {
            tabla.elhelyezKo(honnan, 'O');
            tabla.elhelyezKo(hova, 'B');
            tabla.tablaKirajzolas();
            std::cout << "Fekete jatekos ugratta a kovet " << honnan << " -> " << hova << std::endl;
            return true;
        }
        else {
            tabla.tablaKirajzolas();
            if ((honnan > 0 && honnan < 25) || (hova > 0 && hova < 25)) {
                if (!tabla.pozicioSzabad(hova)) {
                    std::cout << "A(z) " << hova << ". pozicio mar foglalt!" << std::endl;
                }
                else if (!tabla.azonosSzin(honnan, 'B')) {
                    std::cout << "A(z) " << honnan << ". pozicion nem fekete ko van!" << std::endl;
                }
            }
            else
            {
                if (honnan != -1 || hova != -1) {
                    std::cout << "Nem ervenyes pozicio!" << std::endl;
                }
                else {
                    std::cout << "Mentve" << std::endl;
                }
            }

            return false;
        }
    }

    if (tabla.pozicioSzabad(hova) && tabla.szomszedosPoz(honnan, hova) && tabla.azonosSzin(honnan, 'B')) {
        tabla.elhelyezKo(honnan, 'O');
        tabla.elhelyezKo(hova, 'B');
        tabla.tablaKirajzolas();
        std::cout << "Fekete jatekos mozgatta a kovet " << honnan << " -> " << hova << std::endl;
        return true;
    }
    else {
        tabla.tablaKirajzolas();
        if ((honnan > 0 && honnan < 25) || (hova > 0 && hova < 25)) {
            if (!tabla.pozicioSzabad(hova)) {
                std::cout << "A(z) " << hova << ". pozicio mar foglalt!" << std::endl;
            }
            else if (!tabla.szomszedosPoz(honnan, hova)) {
                std::cout << "A(z) " << honnan << " es " << hova << " poziciok nem szomszedosak!" << std::endl;
            }
            else if (!tabla.azonosSzin(honnan, 'B')) {
                std::cout << "A(z) " << honnan << ". pozicion nem fekete ko van!" << std::endl;
            }
        }
        else
        {
            if (honnan != -1 || hova != -1) {
                std::cout << "Nem ervenyes pozicio!" << std::endl;
            }
            else {
                std::cout << "Mentve" << std::endl;
            }
        }

        return false;
    }
}

/**
 * @brief Követ távolít el a tábláról
 * @param tabla A játéktábla referenciája
 * @param poz A pozíció, ahonnan eltávolítjuk a követ
 * @return true, ha sikerült eltávolítani, false egyébként
 * @details Ellenõrzi, hogy a kõ malomban van-e, és csak akkor távolítja el, ha nincs más lehetõség
 */
bool FeketeJatekos::eltavolitKo(JatekTabla& tabla, int poz) {
    // Érvénytelen pozíció ellenõrzése
    if (poz < 1 || poz > 24) {
        std::cerr << "Hiba: Ervenytelen pozicio: " << poz << std::endl;
        return false;
    }

    // Ellenõrizzük, hogy a pozíció foglalt-e, és a megfelelõ színû kõ van-e ott
    if (!(tabla.pozicioSzabad(poz)) && tabla.azonosSzin(poz, 'W')) {

        // Ellenõrizzük, hogy a kõ malomban van-e
        bool malombanVan = false;
        for (int i = 0; i < 16; ++i) {
            const int* malom = tabla.getMalom(i);  // Az i-edik malom lekérése
            if ((malom[0] == poz || malom[1] == poz || malom[2] == poz) &&
                tabla.getKovek()[malom[0]] == 'W' &&
                tabla.getKovek()[malom[1]] == 'W' &&
                tabla.getKovek()[malom[2]] == 'W') {
                malombanVan = true;
                break;
            }
        }

        // Ha a kõ malomban van, további ellenõrzés szükséges
        if (malombanVan) {
            // Ellenõrizzük, hogy van-e másik eltávolítható kõ
            bool vanMasik = false;
            for (int i = 1; i <= 24; ++i) {
                if (tabla.getKovek()[i] == 'W') {

                    bool ezIsMalom = false;
                    for (int j = 0; j < 16; ++j) {
                        const int* aktMalom = tabla.getMalom(j);
                        if ((aktMalom[0] == i || aktMalom[1] == i || aktMalom[2] == i) &&
                            tabla.getKovek()[aktMalom[0]] == 'W' &&
                            tabla.getKovek()[aktMalom[1]] == 'W' &&
                            tabla.getKovek()[aktMalom[2]] == 'W') {
                            ezIsMalom = true;
                            break;
                        }
                    }
                    if (!ezIsMalom) {
                        vanMasik = true;
                        break;
                    }
                }
            }
            if (vanMasik) {
                tabla.tablaKirajzolas();
                std::cout << "Ez a ko malomban van. Valassz masik kovet!" << std::endl;
                return false;
            }
            // Ha nincs másik, eltávolíthatjuk
            else {
                tabla.elhelyezKo(poz, 'O');
                eltavolitott++;
                tabla.tablaKirajzolas();
                std::cout << "Fekete jatekos eltavolitott egy feher kovet a(z) " << poz << ". poziciobol." << std::endl;
                return true;
            }
        }

        // Ha a kõ nincs malomban, eltávolítjuk
        tabla.elhelyezKo(poz, 'O');
        eltavolitott++;
        tabla.tablaKirajzolas();
        std::cout << "Fekete jatekos eltavolitott egy feher kovet a(z) " << poz << ". poziciobol." << std::endl;
        return true;

    }
    else {
        // Hibaüzenetek a különbözõ esetekre
        tabla.tablaKirajzolas();
        if (tabla.pozicioSzabad(poz)) {
            std::cout << "A pozicio ures: " << poz << std::endl;
        }
        else {
            std::cout << "Nem a megfelelo szinu ko van a pozicion: " << poz << std::endl;
        }
        return false;
    }
}