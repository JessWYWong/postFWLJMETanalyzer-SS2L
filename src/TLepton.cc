#include "../interface/TLepton.h"

//Leptons
TLepton::TLepton(
    double pttemp, 
    double etatemp,
    double phitemp, 
    double energytemp, 
    int chargetemp, 
    float relIsotemp, 
    float miniIsotemp, 
    float susyisotemp
    ):
        pt(pttemp),
        eta(etatemp),
        phi(phitemp),
        energy(energytemp),
        charge(chargetemp),
        relIso(relIsotemp),
        miniIso(miniIsotemp),
        susyIso(susyisotemp)
{
  setLV();
}

