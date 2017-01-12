double ResultReal, ResultImag;

void ComplexMultiply(double zr, double zi, double ur, double ui)
{

  ResultReal = zr*ur - zi*ui;
  ResultImag = zr*ui + zi*ur;

}

void ComplexAdd(double zr, double zi, double ur, double ui)
{

  ResultReal = zr+ur;
  ResultImag = zi+ui;

}

void ComplexSquarePlusU(double zr, double zi, double ur, double ui)
{
  double SquareReal, SquareImag;

  SquareReal = zr*zr - zi*zi;
  SquareImag = zr*zi + zi*zr;
  ResultReal = SquareReal + ur;
  ResultImag = SquareImag + ui;
  

}
