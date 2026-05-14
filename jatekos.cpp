/**
 * @file jatekos.cpp
 * @brief A Jatekos osztály alapvetõ implementációja
 * @details A malomjáték játékosainak alaposztályát valósítja meg
 */

 // A program figyelmen kívül hagyja a "biztonsági" CRT figyelmeztetéseket
#define _CRT_SECURE_NO_WARNINGS

#include "jatekos.h"
#include <iostream>
#include <cstring>
#include <fstream>


/**
 * @brief Játékos osztály konstruktora
 * @details Inicializálja a játékos kezdõértékeit: 9 darab kövvel és 0 eltávolított kövvel
 */
Jatekos::Jatekos() {
    kovekSzama = 9;
    eltavolitott = 0;
}
