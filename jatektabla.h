// jatektabla.h
#ifndef JATEKTABLA_H
#define JATEKTABLA_H

#include <string>

class JatekTabla {
private:
    char** alaptabla;
    int** poziciok;
    bool szomszedok[24][24]; //bool lett
    char* kovek;
    int malmok[16][3];
    int melyikmalomW[16]; //ÚJ
    int melyikmalomB[16]; //ÚJ

public:
    JatekTabla();
    ~JatekTabla();

    void tablaKirajzolas() const;
    void elhelyezKo(int poz, char szin);
    bool szomszedosPoz(int honnan, int hova) const;
    bool azonosSzin(int honnan, char c) const;
    bool pozicioSzabad(int poz) const;
    char* getKovek(); //ÚJ
    bool malomVan(char aktualisJatekos); //ÚJ
    const int* getMalom(int index) const {
        return malmok[index];
    } // Getter függvény a malmokhoz

    int getMelyikmalomW(int index) const { return melyikmalomW[index]; } //ÚJ
    int getMelyikmalomB(int index) const { return melyikmalomB[index]; } //ÚJ
    void setMelyikmalomW(int index, int ertek) { melyikmalomW[index] = ertek; } //ÚJ
    void setMelyikmalomB(int index, int ertek) { melyikmalomB[index] = ertek; } //ÚJ
};

#endif