#ifndef FEKETEJATEKOS_H
#define FEKETEJATEKOS_H

#include "jatekos.h"
#include "jatektabla.h"
#include <string>

class FeketeJatekos : public Jatekos {
public:
    FeketeJatekos();

    bool elhelyezKo(JatekTabla& tabla, int poz) override;
    bool mozgatKo(JatekTabla& tabla, int honnan, int hova, int eltavolitva) override;
    bool eltavolitKo(JatekTabla& tabla, int poz) override;


};

#endif