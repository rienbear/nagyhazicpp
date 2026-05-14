#ifndef FEHERJATEKOS_H
#define FEHERJATEKOS_H

#include "jatekos.h"
#include "jatektabla.h"
#include <string>

class FeherJatekos : public Jatekos {
public:
    FeherJatekos();

    bool elhelyezKo(JatekTabla& tabla, int poz) override;
    bool mozgatKo(JatekTabla& tabla, int honnan, int hova, int eltavolitva) override;
    bool eltavolitKo(JatekTabla& tabla, int poz) override;

};

#endif
