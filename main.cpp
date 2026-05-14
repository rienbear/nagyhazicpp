/**
 * @file main.cpp
 * @brief A malomjáték fõ programfájlja és tesztesetei
 * @details Ez a fájl tartalmazza a program belépési pontját és az egységteszteket
 */

 // A program figyelmen kívül hagyja a "biztonsági" CRT figyelmeztetéseket
#define _CRT_SECURE_NO_WARNINGS

//#define CPORTA // Tesztelesi mod bekapcsolasa

#include "jatek.h"
#include "gtest_lite.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifdef CPORTA

/**
 * @brief Tesztesetek futtatása
 * @details A program teszt módban végrehajtja az alábbi teszteseteket:
 * - Játékos osztály alap mûködésének tesztelése
 * - Kövek mozgatásának tesztelése
 * - Ugratás funkció tesztelése
 * - Malomképzés és kõ eltávolítás tesztelése
 * - Játéktábla alap funkcióinak tesztelése
 * - Mentés és betöltés funkció tesztelése
 */
void tesztFuttatas() {
    TEST(JatekosTeszt, AlapKovek) {
        JatekTabla tabla;
        FeherJatekos feher;
        FeketeJatekos fekete;

        // Alap körelhelyezés tesztelése
        EXPECT_TRUE(feher.elhelyezKo(tabla, 1));
        EXPECT_EQ(8, feher.getKovekSzama());
        EXPECT_TRUE(tabla.azonosSzin(1, 'W'));

        EXPECT_TRUE(fekete.elhelyezKo(tabla, 4));
        EXPECT_EQ(8, fekete.getKovekSzama());
        EXPECT_TRUE(tabla.azonosSzin(4, 'B'));

        // Érvénytelen körelhelyezés tesztelése
        EXPECT_FALSE(feher.elhelyezKo(tabla, 1)); // Foglalt pozíció
        EXPECT_FALSE(feher.elhelyezKo(tabla, 25)); // Érvénytelen pozíció
        feher.setKovekSzama(0);
        EXPECT_FALSE(feher.elhelyezKo(tabla, 2)); // Nincs több kõ
    } END

        TEST(JatekosTeszt, Mozgatas) {
        JatekTabla tabla;
        FeherJatekos feher;
        FeketeJatekos fekete;

        // Kezdeti kövek elhelyezése
        feher.elhelyezKo(tabla, 1);
        fekete.elhelyezKo(tabla, 11);
        feher.elhelyezKo(tabla, 2);

        // Érvényes mozgatás tesztelése
        EXPECT_TRUE(feher.mozgatKo(tabla, 1, 10, 0));
        EXPECT_FALSE(tabla.azonosSzin(1, 'W'));
        EXPECT_TRUE(tabla.azonosSzin(10, 'W'));
        // Érvénytelen mozgatások
        EXPECT_FALSE(feher.mozgatKo(tabla, 10, 5, 0)); // Nem szomszédos
        EXPECT_FALSE(feher.mozgatKo(tabla, 10, 11, 0)); // Foglalt cél
        EXPECT_FALSE(fekete.mozgatKo(tabla, 1, 2, 0)); // Nincs ott a kõ
    } END

        TEST(JatekosTeszt, Ugratas) {
        JatekTabla tabla;
        FeherJatekos feher;
        FeketeJatekos fekete;

        // Kezdeti állapot
        feher.elhelyezKo(tabla, 1);
        fekete.elhelyezKo(tabla, 2);
        feher.elhelyezKo(tabla, 10);
        fekete.setEltavolitott(6); // Ugrás engedélyezése

        // Ugrás tesztelése
        EXPECT_TRUE(feher.mozgatKo(tabla, 1, 3, 6));
        EXPECT_TRUE(tabla.azonosSzin(3, 'W'));
        EXPECT_FALSE(tabla.azonosSzin(1, 'W'));
    } END

        TEST(JatekosTeszt, MalomEsEltavolitas) {
        JatekTabla tabla;
        FeherJatekos feher;
        FeketeJatekos fekete;

        // Malom létrehozása (1-2-3)
        feher.elhelyezKo(tabla, 1);
        feher.elhelyezKo(tabla, 2);
        feher.elhelyezKo(tabla, 3);

        // Malom ellenõrzése
        EXPECT_TRUE(tabla.malomVan('W'));

        // Ellenfél kõ elhelyezése
        fekete.elhelyezKo(tabla, 4);

        // Kõ eltávolítás tesztelése
        EXPECT_TRUE(feher.eltavolitKo(tabla, 4));
        EXPECT_EQ(1, feher.getEltavolitott());

        // Nem eltávolítható kõ (mert az is malomban van)
        fekete.elhelyezKo(tabla, 10);
        fekete.elhelyezKo(tabla, 11);
        fekete.elhelyezKo(tabla, 12);
        EXPECT_FALSE(fekete.eltavolitKo(tabla, 10));
    } END

        TEST(JatekTablaTeszt, AlapFunkciok) {
        JatekTabla tabla;

        // Alap inicializálás tesztelése
        EXPECT_EQ('O', tabla.getKovek()[1]);
        EXPECT_EQ('O', tabla.getKovek()[24]);

        // Kõ elhelyezés tesztelése
        tabla.elhelyezKo(1, 'W');
        EXPECT_EQ('W', tabla.getKovek()[1]);

        // Szomszédosság tesztelése
        EXPECT_TRUE(tabla.szomszedosPoz(1, 2));
        EXPECT_TRUE(tabla.szomszedosPoz(1, 10));
        EXPECT_FALSE(tabla.szomszedosPoz(1, 5));

        // Szín azonosítás tesztelése
        EXPECT_TRUE(tabla.azonosSzin(1, 'W'));
        EXPECT_FALSE(tabla.azonosSzin(1, 'B'));
        EXPECT_FALSE(tabla.azonosSzin(2, 'W'));

        // Pozíció szabad tesztelése
        EXPECT_FALSE(tabla.pozicioSzabad(1));
        EXPECT_TRUE(tabla.pozicioSzabad(2));
    } END

        TEST(JatekTablaTeszt, MalomKepzes) {
        JatekTabla tabla;

        // Malom tesztelése
        tabla.elhelyezKo(1, 'W');
        tabla.elhelyezKo(2, 'W');
        tabla.elhelyezKo(3, 'W');
        EXPECT_TRUE(tabla.malomVan('W'));

        // Másik malom tesztelése
        tabla.elhelyezKo(10, 'B');
        tabla.elhelyezKo(11, 'B');
        tabla.elhelyezKo(12, 'B');
        EXPECT_TRUE(tabla.malomVan('B'));

        // Nem malom esete
        tabla.elhelyezKo(4, 'W');
        tabla.elhelyezKo(5, 'W');
        EXPECT_FALSE(tabla.malomVan('W'));
    } END

}

/**
 * @brief Fõprogram teszt módban
 * @return 0, ha minden teszt sikeresen lefutott
 */
int main() {
    tesztFuttatas();
    return 0;
}

#else // Ha CPORTA nincs definialva, akkor a jatek indul el

/**
 * @brief Fõprogram normál játék módban
 * @details A program indulásakor lehetõséget ad új játék kezdésére vagy
 *          korábbi mentés betöltésére. A játék közben a '-1' beírásával
 *          menthetõ a játék állapota.
 * @return 0, ha a program normálisan lefutott
 */
int main() {
    Jatek jatek;
    int valasztas = 0;
    std::string mentettFajlNeve = "mentes.txt";

    std::cout << "Udvozollek a Malom Jatekban!" << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Valassz az alabbi lehetosegek kozul:" << std::endl;
    std::cout << "1. Uj jatek kezdese" << std::endl;
    std::cout << "2. Korabbi mentes betoltese" << std::endl;
    std::cout << "Ha a jatek kozben menteni szeretned a jatekot akkor ird be a jatek menete kozben, hogy '-1'! " << std::endl;
    std::cout << "Valasztasod (1-2): ";

    while (!(std::cin >> valasztas) || (valasztas != 1 && valasztas != 2)) {
        std::cout << "Ervenytelen valasztas. Kerlek, adj meg 1-et vagy 2-t: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (valasztas == 2) {
        std::ifstream file(mentettFajlNeve);
        if (file) {
            file.close();
            jatek.allapotBetoltes(mentettFajlNeve);
            std::cout << "Jatek allapota sikeresen betoltve." << std::endl;
        }
        else {
            std::cerr << "Hiba: A(z) \"" << mentettFajlNeve << "\" fajl nem talalhato. Uj jatek kezdodik." << std::endl;
        }
    }

    jatek.jatekInditas();
}

#endif