#include <stdio.h>

#include "/users2/staff/doug/fractal/wilson/MyLandscape.h"

void InitGauss()
{
  Nrand=4;
  Arand=1.0;
  GaussAdd=(float)sqrt(3.0*(double)Nrand);
  GaussFac=2.0*GaussAdd/((float)Nrand*Arand);
  srand48(time(0));
}

float Gauss()
{
  int i;
  float sum;

  sum=0.0;
  for (i=1;i<=Nrand;i++) sum=sum+(float)drand48();
  return(GaussFac*sum-GaussAdd);
}

float F3(delta,x0,x1,x2)
float delta,x0,x1,x2;
{
return((x0+x1+x2)/3.0+delta*Gauss());
}

float F4(delta,x0,x1,x2,x3)
float delta,x0,x1,x2,x3;
{
return((x0+x1+x2+x3)/4+delta*Gauss());
}

MidPointFM2D()
{
  int i,N,stage,x,y,y0,D,d;
  float delta;

  InitGauss();
  N=pow(2,maxlevel);
  delta=sigma;
  X[0][0]=delta*Gauss();
  X[0][N]=delta*Gauss();
  X[N][0]=delta*Gauss();
  X[N][N]=delta*Gauss();
  D=N;
  d=N/2;
  for (stage=1;stage<=maxlevel;stage++) {
    delta=delta*pow(0.5,0.5*H);
    for (x=d;x<=N-d;x+=D) {
      for (y=d;y<=N-d;y+=D) {
	X[x][y]=f4(delta,X[x+d][y+d],X[x+d][y-d],X[x-d][y+d],
		   X[x-d][y-d]);
      }  /* for y */
    }  /* for x */

    if (addition) {
      for (x=0;x<=N;x+=D) {
	for (y=0;y<=N;y+=D) {
	  X[x][y]=X[x][y]+delta*Gauss();
	}  /* for y */
      }  /* for x */
    }  /* if addition */

    delta=delta*pow(0.5,0.5*H);

    for (x=d;x<=N-d;x+=D) {
      X[x][0]=f3(delta,X[x+d][0],X[x-d][0],X[x][d]);
      X[x][N]=f3(delta,X[x+d][N],X[x-d][N],X[x][N-d]);
      X[0][x]=f3(delta,X[0][x+d],X[0][x-d],X[d][x]);
      X[N][x]=f3(delta,X[N][x+d],X[N][x-d],X[N-d][x]);
    }  /* for x */

    for (x=d;x<=N-d;x+=D) {
      for (y=D;y<=N-d;y+=D) {
	X[x][y]=f4(delta,X[x][y+d],X[x][y-d],X[x+d][y],X[x-d][y]);
      }  /* for x */
    }  /* for y */
    for (x=d;x<=N-d;x+=D) {
      for (y=d;y<=N-d;y+=D) {
	X[x][y]=f4(delta,X[x][y+d],X[x][y-d],X[x+d][y],X[x-d][y]);
      }  /* for x */
    }  /* for y */

    if (addition) {
      for (x=0;x<=N;x+=D) {
	for (y=0;y<=N;y+=D) {
	  X[x][y]=X[x][y]+delta*Gauss();
	}  /* for y */
      }  /* for x */
      for (x=d;x<=N-d;x+=D) {
	for (y=d;y<=N-d;y+=D) {
	  X[x][y]=X[x][y]+delta*Gauss();
	}  /* for y */
      }  /* for x */
    }  /* if addition */
    D=D/2;
    d=d/2;
  }  /* for stage */
}  /* MidPointFM2D */
