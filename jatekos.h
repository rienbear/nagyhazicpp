// jatekos.h
#ifndef JATEKOS_H
#define JATEKOS_H

#include "jatektabla.h"
#include <string>

class Jatekos {
protected:
    int kovekSzama;
    int eltavolitott;

public:
    Jatekos();
    virtual ~Jatekos() {}

    virtual bool elhelyezKo(JatekTabla& tabla, int poz) = 0;
    virtual bool mozgatKo(JatekTabla& tabla, int honnan, int hova, int eltavolitva) = 0;
    virtual bool eltavolitKo(JatekTabla& tabla, int poz) = 0;

    int getKovekSzama() const { return kovekSzama; }
    int getEltavolitott() const { return eltavolitott; }
    void setKovekSzama(int szam) { kovekSzama = szam; }
    void setEltavolitott(int szam) { eltavolitott = szam; }
};

#endif