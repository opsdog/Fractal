#include <stdio.h>

#include "Complex.h"

int Fractal1[6000][6000];
int Fractal2[6000][6000];

int main(int argc, char *argv[])
{

  double rz, rrz, iz, iiz;
  double ur, ui;
  double zr, zi;
  double rmin, rmax, imin, imax;
  double test, testup2;
  double distr, disti;
  double aspectx, aspecty, centerr, centeri;
  double ir;
  double maxinternaltest;
  int i, colour, maxcolors, nwidth, nheight;
  int nr, ni, dwell, maxdwell;
  FILE *Fractal1Out, *Fractal2Out;
  char FileBase[60], FileOut1[60], FileOut2[60];

  /* set aspect ratio */
  
  aspectx=640.0; aspecty=480.0;

  /* get fractal parameters */

  /* Sequence 1 */
  /*  test=1024.0*1024.0*1024.0; dwell=1024;*/

  centerr=0.0; centeri=0.0; distr=1.5;
  /* zoom to here  centerr=0.0; centeri=0.0; distr=0.75;*/
  /* move to here  centerr=0.434; centeri= -0.542; distr=0.075;*/
  /* move to here  centerr=0.4665; centeri= -0.5540; distr=0.008;*/
  /* move to here  centerr=0.46630; centeri= -0.55926; distr=0.0015;*/
  /* move to here  centerr=0.466215; centeri= -0.559032; distr=0.00008;*/
  /* move to blue center  centerr=0.466204445; centeri= -0.559042325; distr=0.000025;*/
  /* zoom to white  centerr=0.466204445; centeri= -0.559042325; distr=0.0000000000278;*/

  /* there's a neat still close to this - move up and to the left - VGA */
  /*    centerr=0.42; centeri= -0.53; distr=0.75;*/

  /* another neat still close to here - VGA */
  /*  centerr=0.42; centeri= -0.53; distr=0.2;*/

  /* another neat still close to here - VGA */
  /*  centerr=0.469; centeri= -0.557; distr=0.050;*/

  /* still here - */
  /* colormap 4 */
  centerr=0.4662045217; centeri= -0.5590422732; distr=0.0000001; strcpy(FileBase,"DustyStill-02");

  /* neat still here - VGA */
  /*  centerr=0.466305; centeri= -0.559245; distr=0.0015; strcpy(FileBase,"DustyStill-01");*/




  nwidth=640; test=256.0*1024.0; dwell=256*1024;
  nwidth=640; test=1024.0*1024.0*1024.0; dwell=1024;
  nwidth=6000; test=1024.0*1024.0*1024.0; dwell=4*1024;
  /*  nwidth=720; test=1024.0*1024.0**1024.0; dwell=4*1024;*/


  /*  end of parameters */

  testup2 = test*test;

  /* set height and distance by aspect ratio */

  disti=distr*aspecty/aspectx;
  nheight=nwidth*aspecty/aspectx;

  /* determine the calculation bounding box */

  rmin=centerr-distr;
  rmax=centerr+distr;
  imin=centeri-disti;
  imax=centeri+disti;

  /* zero the fractal arrays */

  printf("Zeroing Fractal arrays...");

  for (nr=0; nr<nwidth; nr++)
    for (ni=0; ni<nheight; ni++) {
      Fractal1[nr][ni]=0;
      Fractal2[nr][ni]=0;
    }

  printf("Done\n");

  /* zero the statistical counters */

  maxdwell=0; maxcolors=dwell;

  ur= -0.74; ui= 0.25;

  printf("Fractalling %d by %d...\n", nwidth, nheight);

  for (nr=0; nr<nwidth; nr++) {
    if ( nr%100 == 0 ) { printf("%d ",nr); fflush((FILE *)stdout); }
    rrz=rmin+nr*(rmax-rmin)/((double)nwidth-1.0);
    for (ni=0; ni<nheight; ni++) {
      iiz=imin+ni*(imax-imin)/(nheight-1);
      zr=rrz; zi=iiz;
      i=0;
      ComplexSquarePlusU(zr, zi, ur, ui);

      while ( ( i<=dwell ) & 
	      ( (ResultReal*ResultReal+ResultImag*ResultImag)<testup2 ) ) {
	i++;
	zr=ResultReal; zi=ResultImag;
	ComplexSquarePlusU(zr, zi, ur, ui);
      }  /* while dwell loop */

      colour=i;

      if ( maxdwell < i) maxdwell=i;

      /* convergence test 1 */

      if ( (ResultReal*ResultReal + ResultImag*ResultImag) > testup2 )
	Fractal1[nr][ni]=colour;

      /* convergence test 2 */

      if ( (abs(ResultReal) < test) & (abs(ResultImag) < test) )
	Fractal2[nr][ni]=colour;

    }  /* for ni to height */
  }  /* for nr to width */
  printf("\n");

  printf("Max Dwell:  %d\n",maxdwell);

  /* write out the fractal arrays */

  if ( strcmp(FileBase,"") == 0 ) {

    /* default filenames */

    Fractal1Out=fopen("DustyCurls-array1","w");
    Fractal2Out=fopen("DustyCurls-array2","w");

    printf("\n\nWriting DustyCurls-array1 and 2.\n");

  }  else {

    /* custom filenames - we're generating a still */

    switch (nwidth) {

      case 640 : 
	strcat(FileBase,"-640"); break;
      case 720 : 
	strcat(FileBase,"-720"); break;
      case 6000 :
	strcat(FileBase,"-6000"); break;
      default :
	strcat(FileBase,"-Res"); break;

    }

    strcat(FileBase,"-VGA");

#ifdef DEBUG
    printf("FileBase: %s\n",FileBase);
#endif

    strcpy(FileOut1,FileBase); strcat(FileOut1,"-array1");
    strcpy(FileOut2,FileBase); strcat(FileOut2,"-array2");

#ifdef DEBUG
    printf("FileOut1:  %s\nFileOut2:  %s\n",FileOut1, FileOut2);
#endif

    Fractal1Out=fopen(FileOut1,"w");
    Fractal2Out=fopen(FileOut2,"w");

    printf("\n\nWriting %s and 2.\n",FileOut1);


  }

  fprintf(Fractal1Out,"%d %d\n",nwidth, nheight);
  fprintf(Fractal2Out,"%d %d\n",nwidth, nheight);

  for (nr=0; nr<nwidth; nr++)
    for (ni=0; ni<nheight; ni++) {
      fprintf(Fractal1Out,"%d\n",Fractal1[nr][ni]);
      fprintf(Fractal2Out,"%d\n",Fractal2[nr][ni]);
    }

  fclose(Fractal1Out);
  fclose(Fractal2Out);



}

