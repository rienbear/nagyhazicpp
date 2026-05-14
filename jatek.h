#ifndef JATEK_H
#define JATEK_H

#include "jatekos.h"
#include "jatektabla.h"
#include "feherjatekos.h" 
#include "feketejatekos.h"
#include <string>

enum class JatekFazis {
    BABU_LERAKAS = 1,
    BABU_MOZGATAS = 2,
    JATEK_VEGE = 3
};



class Jatek {
private:
    Jatekos* feherJatekos;
    Jatekos* feketeJatekos;
    JatekTabla tabla;
    JatekFazis jatekFazis;
    char aktualisJatekos; //ÚJ!!


public:
    Jatek();
    ~Jatek();

    void jatekInditas();
    void allapotMentes(const std::string& fajlnev);
    void allapotBetoltes(const std::string& fajlnev);
    void jatekosLepes(Jatekos* jatekos, char jatekosSzin, JatekFazis fazis); //ÚJ!!!
    void malomEsetenEltavolitas(Jatekos* jatekos, char jatekosSzin, char ellenfelSzin); //ÚJ!!
    bool ervenyesLepes(int poz);
};

#endif