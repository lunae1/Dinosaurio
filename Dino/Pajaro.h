#ifndef Pajaro_h
#define Pajaro_h
#include "Arduino.h"

class Pajaro {
  private:    
    int z;         

  public:
    Pajaro(int posicionInicial);
    void mover();
    int obtenerPosicion();    
};

#endif
 