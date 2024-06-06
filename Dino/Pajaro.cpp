#include "Pajaro.h"

Pajaro::Pajaro(int posicionInicial) {  
  z = posicionInicial;  
}

void Pajaro::mover() {   
  if(z > 0) {
    z = z - 1;
  }
  else {
    z = 16;
  }  
}

int Pajaro::obtenerPosicion() {
  return z;
}