#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>


double MSetPot(double cx, double cy, int maxiter);

/***********************************************************/
/**                                                       **/
/**            program specific defines                   **/
/**                                                       **/
/***********************************************************/

/*  set the max number of color indices */

/*#define MAX_COLORS 65535*/        /* 16 bit color */
#define MAX_COLORS 200000
/*#define MAX_COLORS 16777216*/     /* 24 bit color */
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

/*  Chose aspect ratio:

    ASPECT35 - 35mm film aspect ratio
    ASPECTVGA - VGA and most "normal" monitor aspect ratio
    ASPECT16TO9 - 16:9 aspect ratio (high definition)
    ASPECTSQUARE - Square aspect ratio

*/

#define ASPECTVGA
/*#define ASPECT16TO9*/

double MSet[6000][6000];
int Colors[6000][6000];

int main(int argc, char *argv[])
{

  int nx,ny,maxiter,ix,iy;
  int ColorIndex, MaxIterUsed, NumColorsUsed;
  int ColorHist[MAX_COLORS];

  double cx,cy,centerx,centery,lengthx,lengthy;
  double distx,disty,xmin,xmax,ymin,ymax;
  double aspectx, aspecty;

  double maxPot = 0.0, 
    minPot = (double)MAX_COLORS;

  double scaleColor;

  char FileBase[1024];

  FILE *ArrayValues;


  /***********************************************************/
  /**                                                       **/
  /**  Baseline image                                       **/
  /**                                                       **/
  /***********************************************************/

  /*  centerx= -1.0; centery=0.0; distx=2.0; strcpy(FileBase,"XPot-Baseline-");*/
  centerx= -0.675; centery=0.025; distx=1.425; strcpy(FileBase,"XPot-Baseline-");


  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 7                                                            **/
  /**                                                                        **/
  /****************************************************************************/


  /*  centerx= -0.675; centery=0.025; distx=1.425; strcpy(FileBase,"XPot-Seq07-00-");*/
/* find an interesting path to... */
  /*reference*/ /*   centerx= -0.7499406; centery= 0.009840; distx=0.000015; disty=0.250;strcpy(FileBase,"XPot-Seq07-14-");*/
/*    centerx= -0.7499300; centery= 0.009840; distx=0.000010; disty=0.250;strcpy(FileBase,"XPot-Seq07-15-");*/

  /***********************************************************/
  /**                                                       **/
  /**  Sequence 01                                          **/
  /**                                                       **/
  /**    colormap 11                                        **/
  /**                                                       **/
  /***********************************************************/

  centerx=.3584; centery=.644; distx=.6; strcpy(FileBase,"XPot-Seq01-01-");
  /*  centerx=.3584; centery=.644; distx=.06; strcpy(FileBase,"XPot-Seq01-02-");*/
  /*  centerx=.3584; centery=.644; distx=.006; strcpy(FileBase,"XPot-Seq01-03-");*/

  /***********************************************************/
  /**                                                       **/
  /**  Sequence 02                                          **/
  /**                                                       **/
  /***********************************************************/

  /*  centerx= -1.250; centery= -0.000; distx=0.250; disty=0.250;strcpy(FileBase,"XPot-Seq02-01-");*/


#ifdef HANDSPECIFY
  printf("nx?"); scanf("%d",&nx);
  printf("ny?"); scanf("%d",&ny);
  printf("maxiter?"); scanf("%d",&maxiter);
#endif

  /*  printf("center x?"); scanf("%f",&centerx);*/
  /*  printf("center y?"); scanf("%f",&centery);*/
  /*  printf("distance x?"); scanf("%f",&distx);*/
  /*  printf("distance y?"); scanf("%f",&disty);*/

  /*  strcat(FileBase,"204-"); nx=204; maxiter=1024;*/
  /*  strcat(FileBase,"512-"); nx=512; maxiter=32*1024;*/
  strcat(FileBase,"640-"); nx=640; maxiter=64*1024;
  /*  NTSC  */
  /*  strcat(FileBase,"720-"); nx=720; maxiter=64*1024;*/
  /*  strcat(FileBase,"1024-"); nx=1024; maxiter=84*1024;*/
  /*  strcat(FileBase,"2048-"); nx=2048; maxiter=96*1024;*/
  /*  strcat(FileBase,"4096-"); nx=4096; maxiter=100*1024;*/
  /*  strcat(FileBase,"6000-"); nx=6000; maxiter=128*1024;*/

  /*  should be 16:9 HDTV at 1080 */
  /*  CHANGE TO ASPECT16TO9       */
  /*  strcat(FileBase,"1920-"); nx=1920; maxiter=96*1024;*/

  /*  determine y values based on set aspect ratio */

#ifdef ASPECT35
  aspectx=3.0; aspecty=2.0;      /*  35mm  */
  printf("\nAspect Ratio:  35mm\n");
  strcat(FileBase,"35mm.values");
#endif
#ifdef ASPECTVGA
  aspectx=640.0; aspecty=480.0;  /*  VGA and iBook  */
  printf("\nAspect Ratio:  VGA\n");
  strcat(FileBase,"VGA.values");
#endif
#ifdef ASPECT16TO9
  aspectx=16.0; aspecty=9.0;     /*  16:9  */
  printf("\nAspect Ratio:  16:9\n");
  strcat(FileBase,"16x9.values");
#endif
#ifdef ASPECTNTSC
  aspectx=4.0; aspecty=3.0;     /*  4:3 for normal US TV */
  printf("\nAspect Ratio:  4:3\n");
  strcat(FileBase,"NTSC.values");
#endif
#ifdef ASPECTSQUARE
  aspectx=1.0; aspecty=1.0;      /*  square  */
  printf("\nAspect Ratio:  Square\n");
  strcat(FileBase,"1x1.values");
#endif

  ny=(int)( (double)nx*aspecty/aspectx );
  disty=distx*aspecty/aspectx;


  xmin = centerx - distx; xmax = centerx + distx;
  ymin = centery - disty; ymax = centery + disty;

  /*
  lengthx = (double)sqrt((xmin*xmin)+(xmax*xmax));
  lengthy = (double)sqrt((ymin*ymin)+(ymax*ymax));
  */

  lengthx = sqrt((xmin*xmin)+(xmax*xmax));
  lengthy = sqrt((ymin*ymin)+(ymax*ymax));

  /* zero the color histogram */

  for (ix=0; ix<MAX_COLORS ;ix++)
    ColorHist[ix]=0;

  for (ix=0; ix<nx; ix++)
    for (iy=0; iy<ny; iy++)
      MSet[ix][iy]=0.0;

  printf("\nFractaling %s at %d x %d:\n  ",FileBase,nx,ny);

  for (iy=0;iy<ny;iy++) {
    if ( iy%100 == 0 ) { printf("%d ",iy); fflush((FILE *)stdout); }
        cy=ymin+(double)iy*(ymax-ymin)/((double)ny-1.0);
    for (ix=0;ix<nx;ix++) {
      cx=xmin+(double)ix*(xmax-xmin)/((double)nx-1.0);
      MSet[ix][iy] = MSetPot(cx, cy, maxiter);

      if ( MSet[ix][iy] > maxPot) maxPot = MSet[ix][iy];
      if ( MSet[ix][iy] < minPot) minPot = MSet[ix][iy];

    }
  }

  printf("\n\n");

  printf("length of x %f, length of y %f",lengthx,lengthy);
  printf("\ncenter point is (%f,%f)",centerx,centery);
  printf("\ndist is (%f,%f)",distx,disty);
  printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
  printf("\nnx %d, ny %d, dwell %d\n",nx,ny,maxiter);
  printf("potential from %e to %e\n",minPot, maxPot);

  /*  determine number of colors used */

  scaleColor = (double)MAX_COLORS / maxPot;

  printf("scaleColor %e --> %d\n",scaleColor, ( (int)(scaleColor * maxPot) ));

  for (ix=0 ; ix<nx; ix++)
    for (iy=0 ; iy<ny; iy++) {
      Colors[ix][iy]=(int)( MSet[ix][iy] * scaleColor );
      if ( Colors[ix][iy] > MaxIterUsed ) MaxIterUsed = Colors[ix][iy];
      /* build the color histogram */
      ColorHist[Colors[ix][iy]]++;
    }

  NumColorsUsed=0;
  for (ix=0; ix<MAX_COLORS; ix++)
    if ( ColorHist[ix] )
      NumColorsUsed++;

  printf("\nNumber of colors used:  %d\n",NumColorsUsed);

  /* write out the values */

  printf("FileBase:  %s\n",FileBase);

  ArrayValues=fopen((const char *)FileBase,"w");

  fprintf(ArrayValues,"%d %d\n",nx, ny);
  for (ix=0; ix<nx; ix++)
    for (iy=0; iy<ny; iy++)
      /*      fprintf(ArrayValues,"%e\n",MSet[ix][iy]);*/
      fprintf(ArrayValues,"%d\n",Colors[ix][iy]);

  fclose(ArrayValues);



  printf("\n\nDone - %s\n",FileBase);



  /*  plot4(nx,ny,argc,argv);*/

}

double MSetPot(double cx, double cy, int maxiter)
{
  int iter;
  double x,y,x2,y2,temp,potential;

  x = cx; x2 = x * x;
  y = cy; y2 = y * y;
  iter = 0;

  /*  while ((iter<maxiter) & ((x2+y2)<100000.0)) {*/
  while ((iter<maxiter) & ((x2+y2)<1000000.0)) {
    temp = x2 - y2 + cx;
    y = 2.0 * x * y + cy;
    x = temp;
    x2 = x * x;
    y2 = y * y;
    iter++;
  }

  if (iter<maxiter) 
    potential=0.5*log(x2+y2)/pow(2.0,(double)iter);
  else
    potential=0.0;

  /*
#ifdef DEBUG
  printf("\npotential is %e",potential);
#endif
  */

  return(potential);
}

