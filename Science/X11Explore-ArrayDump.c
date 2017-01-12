#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

/*#include <plot.h>*/  /*  GNU libplot */

#define TRUE 1
#define FALSE 0

int maxiter;

void ColorMap(int NumColorsUsed);
int MSetLevel();

/*  set the max number of color indices */

#define MAX_COLORS 65534        /* 16 bit color */
/*#define MAX_COLORS 16777216*/     /* 24 bit color */
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

float ColorRed[MAX_COLORS];
float ColorGreen[MAX_COLORS];
float ColorBlue[MAX_COLORS];
float RealColorRed[MAX_COLORS];
float RealColorGreen[MAX_COLORS];
float RealColorBlue[MAX_COLORS];
int ColorHist[MAX_COLORS];

char FileBase[50];

int Fractal[6000][6000];

/*

the idea here:

  generate the colorindex's into Fractal
  determine the number of colors used
  create the colormap customized by the number of colors used
  create the plot file with the colormap

*/

/*  GNU Plotter stuff */

/*
int Plotter;
FILE *PlotFile, *ErrorFile, *File;
int OpenPlotter();
void ClosePlotter();
*/

/*  Chose aspect ratio:

    ASPECT35 - 35mm film aspect ratio
    ASPECTVGA - VGA and most "normal" monitor aspect ratio
    ASPECT16TO9 - 16:9 aspect ratio (high definition)
    ASPECTSQUARE - Square aspect ratio

*/

#define ASPECTVGA

main(argc,argv)
int argc;
char *argv[];
{
  int nx,ny,ix,iy;
  int ColorIndex;
  int MaxIterUsed=0;  /* max depth used - max color index used */
  int NumColorsUsed=0; /* number of colors actually used */
  FILE *File, *ArrayValues;
  double xmin,xmax,ymin,ymax,cx,cy,lengthx,lengthy,centerx,centery;
  double distx,disty,incx,incy;
  double aspectx, aspecty;
  double MandelScale;
  char FileName[75];
  char Resolution[8];

  /****************************************************************************/
  /**                                                                        **/
  /**  starting point                                                        **/
  /**                                                                        **/
  /****************************************************************************/

  centerx= -0.675; centery=0.025; distx=1.425; strcpy(FileBase,"MandelStart-");

  /****************************************************************************/
  /**                                                                        **/
  /**            test for figuring out sequenceXX frames                     **/
  /**                                                                        **/
  /****************************************************************************/


  /*  strcpy(FileBase,"MandelTest--");*/
  /*  centerx= -1.290625; centery= -0.064; distx=0.1;*/
  /*  centerx= -1.3306;   centery= -0.084; distx=0.1;*/


  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 7                                                            **/
  /**                                                                        **/
  /****************************************************************************/


/*    centerx= -0.675; centery=0.025; distx=1.425; strcpy(FileBase,"Seq07-00-");*/
/* find an interesting path to... */
  /*reference*/    centerx= -0.7499406; centery= 0.009840; distx=0.000015; strcpy(FileBase,"Seq07-14-");
/*    centerx= -0.7499300; centery= 0.009840; distx=0.000010; strcpy(FileBase,"Seq07-14-");*/


  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 6                                                            **/
  /**                                                                        **/
  /**    colormap 2, 6, or 11                                                **/
  /**                                                                        **/
  /****************************************************************************/

/*    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474; strcpy(FileBase,"Seq06-00-");*/
/*    centerx= -0.995; centery= -0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-01-");*/
/*  starting around the mandel - might need more steps */
/*    centerx= -0.995; centery= -0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-02-");*/
/*    centerx= -1.05875; centery= -0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-03-");*/
/*    centerx= -1.1225; centery= -0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-04-");*/
/*    centerx= -1.18625; centery= -0.108; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-05-");*/

/*  "center" of move around mandel */
/*    centerx= -1.250; centery= -0.000; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-06-");*/

/*    centerx= -1.18625; centery= 0.108; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-07-");*/
/*    centerx= -1.1225; centery= 0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-08-");*/
/*    centerx= -1.05875; centery= 0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-09-");*/

/*    centerx= -0.995; centery= 0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-10-");*/
/* finished around the mandel */
/*  step 11 should be in the center of the trough, before diving in */
/*    centerx= -0.780; centery= 0.180; distx=0.250; disty=0.250;strcpy(FileBase,"Seq06-11-");*/
/*  slight dive to */
/*    centerx= -0.780; centery= 0.180; distx=0.075; disty=0.250;strcpy(FileBase,"Seq06-12-");*/
/*  dive and move to point */
/*    centerx= -0.7500623; centery= 0.010; distx=0.000125; disty=0.250;strcpy(FileBase,"Seq06-13-");*/
/*  move/zoom to show detail here */
/*    centerx= -0.7499600; centery= 0.009846; distx=0.000040; disty=0.250;strcpy(FileBase,"Seq06-14-");*/
/*  move to show the mini-mandelbrot - self replication */
/*    centerx= -0.7499575; centery= 0.0098430; distx=0.000001; disty=0.250;strcpy(FileBase,"Seq06-15-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 08                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx=0.3584; centery=0.644; distx=0.006; strcpy(FileBase,"Seq08-01-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 11                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -1.4578; centery=0.0; distx=0.04; strcpy(FileBase,"Seq11-01-");*/
  /*  centerx= -1.4578; centery=0.0; distx=0.006; strcpy(FileBase,"Seq11-02-");*/
  /*  centerx= -1.4578; centery=0.0; distx=0.0006; strcpy(FileBase,"Seq11-03-");*/
  /*  centerx= -1.4578; centery=0.0; distx=0.0002; strcpy(FileBase,"Seq11-04-");*/
  /*  centerx= -1.4578; centery=0.0; distx=0.0001; strcpy(FileBase,"Seq11-05-");*/
  /*  centerx= -1.4578; centery=0.0; distx=0.00004; strcpy(FileBase,"Seq11-06-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 09                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -1.48; centery=0.0; distx=0.001; strcpy(FileBase,"Seq09-01-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 5                                                            **/
  /**    Colormap:  12, 9                                                    **/
  /**                                                                        **/
  /****************************************************************************/

  /* this is a NEAT sequence */
  /*  centerx= -0.675; centery=0.025; distx=1.425; strcpy(FileBase,"Seq05-01-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=1.5; strcpy(FileBase,"Seq05-02-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.002; strcpy(FileBase,"Seq05-03-");*/
  /*  centerx= -0.7779347; centery=0.135057500998; distx=0.00002; strcpy(FileBase,"Seq05-04-");*/
  /*  centerx= -0.7779324; centery=0.135055515998; distx=0.000002; strcpy(FileBase,"Seq05-05-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.0000002; strcpy(FileBase,"Seq05-06-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.000000000002; strcpy(FileBase,"Seq05-07-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.00000000000026; strcpy(FileBase,"Seq05-08-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.00000000000005; strcpy(FileBase,"Seq05-09-");*/
  /*  centerx= -0.7779324; centery=0.135055500998; distx=0.00000000000000655; strcpy(FileBase,"Seq05-10-");*/

  /* zoom into this point when i get more precision */
  /*  centerx= -0.777932399999978; centery=0.135055500998072; distx=0.00000000000005; strcpy(FileBase,"Seq05-11-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  testing comparison to X11Explore.f                                    **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -0.777932399999978; centery=0.135055500998072; distx=0.000000000000005; strcpy(FileBase,"Blocky-Cdouble-");*/


  /****************************************************************************/
  /**                                                                        **/
  /**  Good still here - colormaps 1, 4, 10, 11, 13                          **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -0.777932529; centery=0.13505553; distx=0.000000165; strcpy(FileBase,"MandelStill-01-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Good still near here                                                  **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -0.7499365; centery= 0.009840; distx=0.000010; strcpy(FileBase,"MandelStill-02-");*/
  /*  centerx= -0.7499365; centery= 0.009838; distx=0.000010; strcpy(FileBase,"MandelStill-03-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2914061419; centery=0.0149788696; distx=0.00095; strcpy(FileBase,"MandelStill-04-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2917761419; centery=0.0148588696; distx=0.00055; strcpy(FileBase,"MandelStill-05-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2916361419; centery=0.0149588696; distx=0.00040; strcpy(FileBase,"MandelStill-06-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2909561419; centery=0.0155088696; distx=0.00055; strcpy(FileBase,"MandelStill-07-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2908061419; centery=0.0155088696; distx=0.00055; strcpy(FileBase,"MandelStill-08-");*/

  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2909061419; centery=0.0155688696; distx=0.00020; strcpy(FileBase,"MandelStill-09-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2908961419; centery=0.0155818696; distx=0.00014; strcpy(FileBase,"MandelStill-10-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2906255419; centery=0.0151058696; distx=0.00010; strcpy(FileBase,"MandelStill-11-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2906247419; centery=0.0151058696; distx=0.00008; strcpy(FileBase,"MandelStill-12-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2906224419; centery=0.0151058696; distx=0.00003; strcpy(FileBase,"MandelStill-13-");*/


  /****************************************************************************/
  /**  Good still near here                                                  **/
  /****************************************************************************/

  /*  centerx=0.2916855419; centery=0.0145658696; distx=0.00110; strcpy(FileBase,"MandelStill-14-");*/


  /****************************************************************************/
  /**  Good still near here - SQUARE                                         **/
  /****************************************************************************/

  /*  centerx=0.2916105419; centery=0.0146158696; distx=0.00010; strcpy(FileBase,"MandelStill-15-");*/

  /****************************************************************************/
  /**  Good still near here - SQUARE                                         **/
  /****************************************************************************/

  /*  centerx=0.2916105419; centery=0.0146158696; distx=0.00015; strcpy(FileBase,"MandelStill-16-");*/


  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 10                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx=0.36720359945; centery=0.5922317790; distx=1.5; strcpy(FileBase,"Seq10-01-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.75; strcpy(FileBase,"Seq10-02-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.25; strcpy(FileBase,"Seq10-03-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.10; strcpy(FileBase,"Seq10-04-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.001; strcpy(FileBase,"Seq10-05-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.0005; strcpy(FileBase,"Seq10-06-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.000001; strcpy(FileBase,"Seq10-07-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.0000001; strcpy(FileBase,"Seq10-08-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.00000005; strcpy(FileBase,"Seq10-09-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.00000001; strcpy(FileBase,"Seq10-10-");*/
  /*  centerx=0.36720359945; centery=0.5922317790; distx=0.0000000080; strcpy(FileBase,"Seq10-11-");*/
  /*  centerx=0.36720359945; centery=0.5922317794; distx=0.0000000002; strcpy(FileBase,"Seq10-12-");*/
  /*  centerx=0.36720359945; centery=0.5922317794; distx=0.00000000002; strcpy(FileBase,"Seq10-13-"); */
  /*  centerx=0.36720359945; centery=0.5922317794; distx=0.000000000002; strcpy(FileBase,"Seq10-14-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 12                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -1.8323762603103880; centery=0.0014286451041698; distx=2.0; strcpy(FileBase,"Seq12-01-");*/
  /*  centerx= -1.8323762603103880; centery=0.0014286451041698; distx=0.00002; strcpy(FileBase,"Seq12-02-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 13                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=2.0; strcpy(FileBase,"Seq13-01-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.5; strcpy(FileBase,"Seq13-02-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.02; strcpy(FileBase,"Seq13-03-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.002; strcpy(FileBase,"Seq13-04-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.0002; strcpy(FileBase,"Seq13-05-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00002; strcpy(FileBase,"Seq13-06-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.000002; strcpy(FileBase,"Seq13-07-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.0000002; strcpy(FileBase,"Seq13-08-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000008; strcpy(FileBase,"Seq13-09-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.000000008; strcpy(FileBase,"Seq13-10-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.000000006; strcpy(FileBase,"Seq13-11-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000000008; strcpy(FileBase,"Seq13-12-");*/
  /*  centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000000004; strcpy(FileBase,"Seq13-13-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 4                                                            **/
  /**    colormaps:  2, 4, 1                                                 **/
  /**                                                                        **/
  /****************************************************************************/

  /*  centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=1.75; strcpy(FileBase,"Seq04-01-");*/
  /*  centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.1; disty=0.1; strcpy(FileBase,"Seq04-02-");*/
  /*  centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.0008; disty=0.0008; strcpy(FileBase,"Seq04-03-");*/
  /*  centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.00002; disty=0.00002;strcpy(FileBase,"Seq04-04-");*/
  /*  centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.000002; disty=0.000002; strcpy(FileBase,"Seq04-05-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 15                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  'John Dewey Jones midget' on the spike  (Ken Philip) */
  /*  centerx= -1.99638; centery=0.0; distx=1.5; strcpy(FileBase,"Seq15-01-");*/
  /*  centerx= -1.99638; centery=0.0; distx=0.05; strcpy(FileBase,"Seq15-02-");*/
  /*  centerx= -1.99638; centery=0.0; distx=1.43e-5; strcpy(FileBase,"Seq15-03-");*/

  /****************************************************************************/
  /**                                                                        **/
  /** Sequence 2                                                             **/
  /**   COLORMAP05                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  Quad spiral in East Valley - NEAT!! (Ken Philip) */
  /*  centerx=0.2913061419; centery=0.0151788696; distx=1.5; strcpy(FileBase,"Seq02-01-");*/
  /*  centerx=0.2913061419; centery=0.0151788696; distx=1.23e-8; strcpy(FileBase,"Seq02-02-");*/
  /*  centerx=0.2913061419; centery=0.0151788696; distx=0.000000005; strcpy(FileBase,"Seq02-03-");*/
  /*  centerx=0.2913061419; centery=0.0151788696; distx=0.0000000008; strcpy(FileBase,"Seq02-04-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 1:                                                           **/
  /**    Sea horse tail in SHV - neat, at least 50000 dwell  (Ken Philip)    **/
  /**    colormap:  1000 1000 8000 - 60000 50000 2000 (num 1 testing)        **/
  /**    colormap:  40000 500 60000 - 60000 50000 2000                       **/
  /**    colormap:  40000 500 60000 - 1000 1000 8000                         **/
  /**                                                                        **/
  /**  Colormaps:  2, 3?, 10, 11, 4                                          **/
  /**                                                                        **/
  /****************************************************************************/

  /*  points optimized for VGA aspect ratio */

  /*  centerx= -0.675; centery=0.025; distx=1.425; strcpy(FileBase,"Seq01-01-");*/
  /*  centerx= -0.7499225; centery=0.0169325; distx=1.5; strcpy(FileBase,"Seq01-02-");*/
  /* zoom/move to... */
  /*  centerx= -0.7499225; centery=0.0169325; distx=1.056e-5; strcpy(FileBase,"Seq01-03-");*/
  /*  centerx= -0.7499225; centery=0.0169325; distx=1.056e-5; strcpy(FileBase,"Seq01-04-");*/
  /* hold, and then move to... **/
  /*    centerx= -0.7499389; centery=0.0169435; distx=1.056e-5; strcpy(FileBase,"Seq01-05-");*/
  /*  centerx= -0.7499389; centery=0.0169435; distx=1.056e-5; strcpy(FileBase,"Seq01-06-");*/
  /*  hold, and then move to... */
  /*  centerx= -0.74996540; centery=0.0169564; distx=1.056e-5; strcpy(FileBase,"Seq01-07-");*/
  /*  centerx= -0.74996540; centery=0.0169564; distx=1.056e-5; strcpy(FileBase,"Seq01-08-");*/
  /* next should look like the last */
  /*  centerx= -0.74996897; centery=0.0169620; distx=1.056e-6; strcpy(FileBase,"Seq01-09-");*/
  /*  centerx= -0.74996897; centery=0.0169620; distx=1.056e-6; strcpy(FileBase,"Seq01-10-");*/
  /*  one more sequence to look like the last 2 */
  /*  centerx= -0.749969344; centery=0.01696235; distx=0.0000003056; strcpy(FileBase,"Seq01-11-");*/
  /*  centerx= -0.749969344; centery=0.01696235; distx=0.0000003056; strcpy(FileBase,"Seq01-12-");*/
  /*  move centered over the blue */
  /*  centerx= -0.7499693494686478; centery=0.0169623462865075; distx=1.056e-7; strcpy(FileBase,"Seq01-13-");*/
  /*  centerx= -0.7499693494686478; centery=0.0169623462865075; distx=1.056e-7; strcpy(FileBase,"Seq01-14-");*/
  /*  dive into the blue */
  /*  centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.0000000000000048; strcpy(FileBase,"Seq01-15-");*/
  /*  centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.000000000000008; strcpy(FileBase,"Seq01-16-");*/
  /*  end point for 575 VGA - 84k dwell */
  /*  centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.00000000000000371995; strcpy(FileBase,"Seq01-17-");*/
  /*  centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.000000000000001; strcpy(FileBase,"Seq01-18-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 14                                                           **/
  /**                                                                        **/
  /**  Rising sun sorta thing - 32k at least                                 **/
  /**                                                                        **/
  /****************************************************************************/


  /*  centerx= -0.74996934950; centery=0.0169623455; distx=0.000000001056; strcpy(FileBase,"Seq14-07-");*/


  /****************************************************************************/
  /**                                                                        **/
  /**  Sequence 16                                                           **/
  /**                                                                        **/
  /****************************************************************************/

  /*  Rabbit Valley, between north atom and its head  (Ken Philip) */
  /*  not impressed */
  /*  centerx= -0.117824; centery=0.838990; distx=1.5; strcpy(FileBase,"Seq16-01-");*/
  /*  centerx= -0.117824; centery=0.838990; distx=0.00152671; strcpy(FileBase,"Seq16-02-");*/

  /****************************************************************************/
  /**                                                                        **/
  /**   Sequence 3                                                           **/
  /**    <400 Colors                                                         **/
  /**                                                                        **/
  /****************************************************************************/

  /*  near small midget on the spike  (Ken Philip) */
  /*  centerx= -1.711638721; centery=0.000449787; distx=1.5; strcpy(FileBase,"Seq03-01-");*/
  /*  centerx= -1.7116387198; centery=0.0004497826; distx=1.4124e-8; strcpy(FileBase,"Seq03-02-");*/
  /*  centerx= -1.7116387198; centery=0.0004497826; distx=1.4124e-9; strcpy(FileBase,"Seq03-03-");*/
  /*  hold and zoom to */
  /*  centerx= -1.711638719870; centery=0.000449782569; distx=1.4124e-10; strcpy(FileBase,"Seq03-04-");*/
  /*  centerx= -1.711638719870; centery=0.000449782569; distx=0.00000000014124; strcpy(FileBase,"Seq03-05-");*/



  /*  strcat(FileBase,"204-"); nx=204; ny=431; maxiter=48*1024;*/
  /*  strcat(FileBase,"512-"); nx=512; ny=381; maxiter=64*1024;*/
  /*  strcat(FileBase,"575-"); nx=575; ny=431; maxiter=64*1024;*/
  /*  strcat(FileBase,"575-"); nx=575; ny=431; maxiter=64*1024;*/
  strcat(FileBase,"640-"); nx=640; ny=480; maxiter=64*1024;
  /* NTSC */
  /*  strcat(FileBase,"720-"); nx=720; ny=480; maxiter=64*1024;*/
  /*    strcat(FileBase,"1024-"); nx=1024; ny=768; maxiter=84*1024;*/
  /*    strcat(FileBase,"2048-"); nx=2048; ny=768; maxiter=96*1024;*/
  /*    strcat(FileBase,"4096-"); nx=4096; ny=768; maxiter=100*1024;*/
  /*    strcat(FileBase,"6000-"); nx=6000; ny=4500; maxiter=128*1024;*/

  /*  should be 16:9 HDTV at 1080 */
  /*  CHANGE TO ASPECT16TO9       */

  /*    strcat(FileBase,"1920-"); nx=1920; ny=1080; maxiter=96*2024;*/

#ifdef HANDSPECIFY
    printf("nx?"); scanf("%d",&nx);
    printf("ny?"); scanf("%d",&ny);
    printf("maxiter?"); scanf("%d",&maxiter);
    printf("center x?"); scanf("%f",&centerx);
    printf("center y?"); scanf("%f",&centery);
    printf("distance x?"); scanf("%f",&distx);
    printf("distance y?"); scanf("%f",&disty);
#endif

    /*  determine y values based on set aspect ratio */

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

    ny=(int)( (double)nx*aspecty/aspectx );
    disty=distx*aspecty/aspectx;


    xmin=centerx-distx; xmax=centerx+distx;
    ymin=centery-disty; ymax=centery+disty;
    lengthx = (double)sqrt((xmin*xmin)+(xmax*xmax));
    lengthy = (double)sqrt((ymin*ymin)+(ymax*ymax));
    incx=(xmax-xmin)/((double)nx-1.0); incy=(ymax-ymin)/((double)ny-1.0);
    if(xmax==xmin) {exit(-1);}

#ifdef DEBUG
    printf("Center (%e,%e)\n",centerx,centery);
    printf("Distance (%e,%e)\n",distx,disty);
    printf("(%e,%e) (%e,%e) %e \n",xmin,xmax,ymin,ymax,(xmax-xmin));
    printf("Incs:  (%e,%e)\n",incx,incy);
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
    strcat(FileBase,"Square.values");
#endif

    /* zero the color histogram */

    for (ix=0; ix<MAX_COLORS ;ix++)
      ColorHist[ix]=0;

    for (ix=0; ix<nx; ix++)
      for (iy=0; iy<ny; iy++)
	Fractal[ix][iy]=0;

    /* do the fractal and plot the points */

    printf("\nFractaling %s at %d x %d:\n  ",FileBase,nx,ny);
    fflush((FILE *)stdout);

    for (iy=0;iy<ny-1;iy++) {
      if ( iy%100 == 0 ) { printf("%d ",iy); fflush((FILE *)stdout); }
      cy=ymin+iy*incy;
      for (ix=0;ix<nx-1;ix++) {
	cx=xmin+ix*incx;

	/*printf("%34.30f %34.30f\n",cx, cy);*/
	/*printf("%64.60f\n",cx);*/
	ColorIndex=(int)MSetLevel(cx,cy,maxiter);
	Fractal[ix][iy]=ColorIndex;

	if ( ColorIndex > MaxIterUsed ) MaxIterUsed = ColorIndex;

	/* build the color histogram */

	ColorHist[ColorIndex]++;

      } /* for ix */
    } /* for iy */

    printf("\n");

    printf("\nlength of x %f, length of y %f",lengthx,lengthy);
    printf("\ncent (%34.30f,%34.30f)",centerx,centery);
    printf("\ndist (%34.30f,%34.30f)",distx,disty);
    printf("\nfrom (%34.30f,%34.30f)\n  to (%34.30f,%34.30f)",xmin,ymin,xmax,ymax);
    printf("\nincs (%34.30f,%34.30f)",incx, incy);
    printf("\nnx %d, ny %d, dwell %d MaxIterUsed %d\n",
	   nx,ny,maxiter,MaxIterUsed);

    /*  determine number of colors used */

    NumColorsUsed=0;
    for (ix=0; ix<MAX_COLORS; ix++)
      if ( ColorHist[ix] )
	NumColorsUsed++;

    printf("\nNumber of colors used:  %d",NumColorsUsed);

    /* write out the values */

    ArrayValues=fopen((const char *)FileBase,"w");

    fprintf(ArrayValues,"%d %d\n",nx, ny);
    for (ix=0; ix<nx; ix++)
      for (iy=0; iy<ny; iy++)
	fprintf(ArrayValues,"%d\n",Fractal[ix][iy]);

    fclose(ArrayValues);

    /*  try scaling to PNGFromValues maxcolors  */
    /*  makes no difference  */

    /*
    strcat(FileBase,"-2");

    MandelScale=60000000.0/(double)MaxIterUsed;

    printf("\n\nMandelScale:  %f\n",MandelScale);

    ArrayValues=fopen((const char *)FileBase,"w");

    fprintf(ArrayValues,"%d %d\n",nx, ny);
    for (ix=0; ix<nx; ix++)
      for (iy=0; iy<ny; iy++)
	fprintf(ArrayValues,"%d\n",(int)((double)Fractal[ix][iy]*MandelScale));

    fclose(ArrayValues);
    */


    printf("\n\nDone - %s\n",FileBase);

}

int MSetLevel(cx,cy,maxiter)
double cx,cy;
int maxiter;
{
    int iter;
    double x,y,x2,y2,temp;
    /*dougee*/

    x = 0; y = 0; x2 = 0; y2 = 0; iter = 0;
    /*    while ( (iter<maxiter) & ((x2+y2)<2500000.0) ) {*/
    while ( (iter<maxiter) & ((x2+y2)<60000000.0) ) {
      temp = x2 - y2 + cx; y = 2.0 * x * y + cy;
      x = temp; x2 = x * x; y2 = y * y; iter++;
    } /* while */

    return(iter);

    /*
    if (iter==maxiter) {
      return(maxiter);
    } else {
      return(iter);
    }
    */
}

