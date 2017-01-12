#include <stdio.h>

#include "Complex.h"

int main(int argc, char *argv[])
{

  double ZR, ZI, UR, UI;

  ZR = 0.1; 
  ZI = 0.2;
  UR = 0.3;
  UI = 0.4;

  ComplexMultiply(ZR, ZI, UR, UI);
  printf("Mult:  %f, %f\n", ResultReal, ResultImag);

  ComplexAdd(ZR, ZI, UR, UI);
  printf("Add:  %f, %f\n", ResultReal, ResultImag);

  ComplexSquarePlusU(ZR, ZI, UR, UI);
  printf("^2+:  %f, %f\n", ResultReal, ResultImag);

  ComplexMultiply(ZR, ZI, ZR, ZI);
  ZR = ResultReal; ZI = ResultImag;
  ComplexAdd(ZR, ZI, UR, UI);
  printf("^2+:  %f, %f\n", ResultReal, ResultImag);

  

}
