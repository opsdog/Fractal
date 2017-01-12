#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/* fractal stuff */

int Fractal[6000][6000];
int NumColorsUsed;
char FileBase[256];


/*  set the max number of color indices */

/*#define MAX_COLORS 65535*/        /* 16 bit color */
#define MAX_COLORS 16777216     /* 24 bit color */
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

int ColorHist[MAX_COLORS];

void fractal();
double mdist();

#define Min(x,y) ((x)<(y) ? (x) : (y))
#define Max(x,y) ((x)>(y) ? (x) : (y))

double XMax, XMin, YMax, YMin, Pval, Qval, a, b, c, d, e, f, g, h,
  TXMax, TXMin, TYMax, TYMin, start_x, start_y;
int max_iterations;

int maxcol;
int maxrow;
double max_size = 500.0*1000.0;
int Index, old_Index;
double x, y, p, q, old_x, old_y;
double deltaP, deltaQ, threshold;
int i, j, row, col;

main()
{
  double centerx, centery, distx, disty;
  double aspectx, aspecty;
  int ix,iy;

  FILE *ArrayValues;

  XMax=.3644; XMin=.3524; YMax=.6500; YMin=.6380;
  XMax=1.5; XMin= -1.5; YMax=1.5; YMin= -1.5;


#define ASPECTVGA


  /*  centerx= 0.0; centery= 0.0; distx= 2.0; strcpy(FileBase,"Overview-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.000002; strcpy(FileBase,"Seq01-Mid-");*/
  centerx=0.36720359945; centery=0.5922317790; distx=0.001; strcpy(FileBase,"Seq02-Mid-");
  /*  centerx= -0.675; centery=0.025; distx=1.75; strcpy(FileBase,"Seq00-00-");*/
  /*  centerx= -1.250; centery=0.000; distx=0.250; strcpy(FileBase,"Seq06-06-");*/




/*  strcat(FileBase,"204-"); nx=204; ny=431; maxiter=48*1024;*/
/*  strcat(FileBase,"512-"); nx=512; ny=381; maxiter=64*1024;*/
/*  strcat(FileBase,"575-"); nx=575; ny=431; maxiter=64*1024;*/
/*  strcat(FileBase,"575-"); nx=575; ny=431; maxiter=64*1024;*/
    strcat(FileBase,"640-"); maxcol=640; max_iterations=64*1024;
  /* NTSC */
/*  strcat(FileBase,"720-"); nx=720; ny=480; maxiter=64*1024;*/
/*    strcat(FileBase,"1024-"); nx=1024; ny=768; maxiter=84*1024;*/
/*    strcat(FileBase,"2048-"); nx=2048; ny=768; maxiter=96*1024;*/
/*    strcat(FileBase,"4096-"); nx=4096; ny=768; maxiter=100*1024;*/
/*    strcat(FileBase,"6000-"); nx=6000; ny=4500; maxiter=128*1024;*/

/*  should be 16:9 HDTV at 1080 */
/*  CHANGE TO ASPECT16TO9       */

/*    strcat(FileBase,"1920-"); nx=1920; ny=1080; maxiter=96*2024;*/


#ifdef ASPECT35
  aspectx=3.0; aspecty=2.0;      /*  35mm  */
  printf("\n\nAspect Ratio:  35mm\n");
#endif
#ifdef ASPECTVGA
  aspectx=640.0; aspecty=480.0;  /*  VGA and iBook  */
  printf("\n\nAspect Ratio:  VGA\n");
#endif
#ifdef ASPECT16TO9
  aspectx=16.0; aspecty=9.0;     /*  16:9  */
  printf("\n\nAspect Ratio:  16:9\n");
#endif
#ifdef ASPECTNTSC
  aspectx=4.0; aspecty=3.0;     /*  4:3 for normal US TV */
  printf("\n\nAspect Ratio:  4:3\n");
#endif
#ifdef ASPECTSQUARE
  aspectx=1.0; aspecty=1.0;      /*  square  */
  printf("\n\nAspect Ratio:  Square\n");
#endif

  /* add aspect ratio to FileBase */

#ifdef ASPECT35
  strcat(FileBase,"35mm.values");
#endif
#ifdef ASPECTVGA
  strcat(FileBase,"VGA.values");
#endif
#ifdef ASPECT16TO9
  strcat(FileBase,"16x9.values");
#endif
#ifdef ASPECTSQUARE
  strcat(FileBase,"SQ.values");
#endif

  maxrow=(int)( (double)maxcol*aspecty/aspectx);
  disty=distx*aspecty/aspectx;

  XMin=centerx-distx; XMax=centerx+distx;
  YMin=centery-disty; YMax=centery+disty;

  printf("Center: (%f, %f)\n",centerx, centery);
  printf("Distance: (%f, %f)\n",distx, disty);
  printf("Corners:  (%f, %f), (%f, %f)\n",XMin,YMin,XMax,YMax);

  /* zero the color histogram */

  for (row=0; row<MAX_COLORS ;row++)
    ColorHist[row]=0;

  for (col=0; col<maxcol; col++)
    for (row=0; row<maxrow; row++)
      Fractal[col][row]=0;


  fractal();

  /* generate the color histogram */

  for (col=0; col<maxcol; col++)
    for (row=0; row<maxrow; row++)
      ColorHist[Fractal[col][row]]++;

  /*  determine number of colors used */

  NumColorsUsed=0;
  for (col=0; col<MAX_COLORS; col++)
    if ( ColorHist[col] )
      NumColorsUsed++;

  printf("\nNumber of colors used:  %d\n",NumColorsUsed);

  /* write out the values */

  ArrayValues=fopen((const char *)FileBase,"w");

  fprintf(ArrayValues,"%d %d\n",maxcol, maxrow);

  for (col=0; col<maxcol; col++)
    for (row=0; row<maxrow; row++) {

      fprintf(ArrayValues,"%d\n",Fractal[col][row]);

    }

  fclose(ArrayValues);

  printf("\n\nDone - %s\n",FileBase);

}

float delta;

void fractal()
{
  double distance, distance2, deltaD, temp;
  int k, end, color;

#ifdef DEBUG
  printf("Entering fractal\n");
#endif

  deltaP=(XMax-XMin)/((double)maxcol+1.0);
  deltaQ=(YMax-YMin)/((double)maxrow+1.0);
  delta=(deltaP+deltaQ)/4.0;
  delta=deltaQ/2.0;

  printf("\nFractaling %d x %d:\n  ",maxcol,maxrow);
  fflush((FILE *)stdout);

  for (col=0;col<=maxcol;col++) {
    p=XMin+(double)col*deltaP;
    if ( col%100 == 0 ) { printf("%d ",col); fflush((FILE *)stdout); }
    for (row=0;row<=maxrow;row++) {
      q=YMax-(double)row*deltaQ;
      distance=mdist();
      if (distance < deltaQ) {
	Fractal[col][row]=0;
      } else {
	end=Max(1,Min(20,(int)(distance/(2.0*deltaQ))));
	q=YMax-((double)row+(double)end)*deltaQ;
	distance2=mdist();
	deltaD=(distance-distance2)/(double)end;
	for (k=0;k<=end;k++) {
	  Fractal[col][row]=(int)(distance/deltaQ);
	  if (Fractal[col][row]>max_iterations) {
	    Fractal[col][row]=max_iterations;
	  }
	  else
	    if (Fractal[col][row]<0) {
	      Fractal[col][row]=0;
	    }

	  row++;

	  if (row>maxrow)
	    break;
	  distance-=deltaD;
	}

	row--;
      }
    }
  }

  printf("\n");

#ifdef DEBUG
  printf("Exiting fractal\n");
#endif

}


double mdist()
{
  int i, flag, row_dist;
  static int old_Index;
  double dist=0.0, temp, xsq, ysq, xprime,
    yprime, orbit_x[100000], orbit_y[100000], denom, old_x, old_y;
  double sum;

#ifdef DEBUG
  printf("Entering mdist (%d)\n",MAX_COLORS);
#endif

  i=0; flag=0; xsq=0.0; ysq=0.0; x=0.0; y=0.0; xprime=0.0; yprime=0.0; Index=0;
  orbit_x[0]=0.0; 
  orbit_y[0]=0.0; 
  old_x=0.0; old_y=0.0;

  while ((Index<max_iterations) && ( xsq + ysq < max_size)) {
    Index++;
    y=2.0*x*y+q;
    orbit_y[Index]=y;
    x=xsq-ysq+p;
    orbit_x[Index]=x;
    xsq=x*x; ysq=y*y;
    if (old_Index == max_iterations) {
      if ((Index & 15) == 0) {
	old_x=x;
	old_y=y;
      } else 
	if ((fabs(old_x-x) + fabs(old_y-x)) < delta)
	  Index=max_iterations-1;
    }  /* if old_Index */
  }

  sum=xsq+ysq;
  if (sum>max_size) {
    while ((i<Index) && (!flag)) {
      temp=2.0*(orbit_x[i] * xprime - orbit_y[i] * yprime) + 1.0;
      yprime=2.0*(orbit_y[i] * xprime + orbit_x[i] * yprime);
      xprime=temp;
      if (Max(fabs(xprime),fabs(yprime))>1e14)
	flag=1;
      i++;
    }
    if (!flag) {
      dist=log(xsq+ysq)*sqrt((xsq+ysq)/(xprime*xprime+yprime*yprime));
    }
  }

#ifdef DEBUG
  printf("Exiting mdist\n");
#endif

  return(dist);
}

