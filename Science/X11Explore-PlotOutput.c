#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#include <plot.h>  /*  GNU libplot */

#define TRUE 1
#define FALSE 0

int maxiter;

void ColorMap(int NumColorsUsed);
int MSetLevel();

/*  set the max number of color indices */

/*#define MAX_COLORS 65535*/        /* 16 bit color */
#define MAX_COLORS 16777216     /* 24 bit color */
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

int Plotter;
FILE *PlotFile, *ErrorFile, *File;
int OpenPlotter();
void ClosePlotter();

/*  Chose aspect ratio:

    ASPECT35 - 35mm film aspect ratio
    ASPECTVGA - VGA and most "normal" monitor aspect ratio
    ASPECT16TO9 - 16:0 aspect ratio (high definition)
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
    FILE *File;
    double xmin,xmax,ymin,ymax,cx,cy,lengthx,lengthy,centerx,centery;
    double distx,disty,incx,incy;
    double aspectx, aspecty;
    char FileName[75];
    char Resolution[8];

    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474; strcpy(FileBase,"Seq00-00-");

/****************************************************************************/
/**                                                                        **/
/**  Sequence 7                                                            **/
/**                                                                        **/
/****************************************************************************/


/*    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474; strcpy(FileBase,"Seq07-00-");*/
/* find an interesting path to... */
/*reference    centerx= -0.7499406; centery= 0.009840; distx=0.000015; disty=0.250;strcpy(FileBase,"Seq07-14-");*/
/*    centerx= -0.7499300; centery= 0.009840; distx=0.000010; disty=0.250;strcpy(FileBase,"Seq07-14-");*/


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
    centerx= -0.7499575; centery= 0.0098430; distx=0.000001; disty=0.250;strcpy(FileBase,"Seq06-15-");

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/* centerx=0.3584; centery=0.644;distx=0.006;disty=0.006;*/

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/* centerx= -1.4578; centery=0.0; distx=0.04; disty=0.08;*/
/* centerx= -1.4578; centery=0.0; distx=0.006; disty=0.006;*/
/* centerx= -1.4578; centery=0.0; distx=0.0006; disty=0.0006;*/
/* centerx= -1.4578; centery=0.0; distx=0.0002; disty=0.0002;*/
/* centerx= -1.4578; centery=0.0; distx=0.0001; disty=0.0001;*/
/* centerx= -1.4578; centery=0.0; distx=0.00004; disty=0.00004;*/

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/* centerx= -1.48; centery=0.0; distx=0.001; disty=0.001;*/

/****************************************************************************/
/**                                                                        **/
/**  Sequence 5                                                            **/
/**    Colormap:  9                                                        **/
/**                                                                        **/
/****************************************************************************/

/* this is a NEAT sequence */
/* centerx= -0.7779324; centery=0.135055500998; distx=1.5; disty=1.5; strcpy(FileBase,"Seq05-00-");*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.002; disty=0.002; strcpy(FileBase,"Seq05-01-");*/
/** centerx= -0.7779324; centery=0.135055500998; distx=0.00002; disty=0.00002; strcpy(FileBase,"Seq05-02-");*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.000002; disty=0.000002; strcpy(FileBase,"Seq05-03-");*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.0000002; disty=0.0000002; strcpy(FileBase,"Seq05-04-");*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.000000000002; disty=0.000000000002; strcpy(FileBase,"Seq05-05-");*/
    centerx= -0.7779324; centery=0.135055500998; distx=0.00000000000026; disty=0.0000000000015; strcpy(FileBase,"Seq05-06-");
/* centerx= -0.7779324; centery=0.135055500998; distx=0.00000000000005; disty=0.00000000000005; strcpy(FileBase,"Seq05-07-");*/

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/* centerx=0.36720359945; centery=0.5922317790; distx=1.5; disty=1.5;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.75; disty=0.75;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.25; disty=0.25;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.10; disty=0.10;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.001; disty=0.001;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.0005; disty=0.0005;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.000001; disty=0.000001;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.0000001; disty=0.0000001;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.00000005; disty=0.00000005;*/
/* centerx=0.36720359945; centery=0.5922317790; distx=0.00000001; disty=0.00000001;*/
/** centerx=0.36720359945; centery=0.5922317790; distx=0.0000000080; disty=0.0000000080;*/
/* centerx=0.36720359945; centery=0.5922317794; distx=0.0000000002; disty=0.0000000002;*/
/** centerx=0.36720359945; centery=0.5922317794; distx=0.00000000002; disty=0.00000000002;*/
/* centerx=0.36720359945; centery=0.5922317794; distx=0.000000000002; disty=0.000000000002;*/

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/* centerx= -1.8323762603103880; centery=0.0014286451041698; distx=2.0; disty=2.0;*/
/* centerx= -1.8323762603103880; centery=0.0014286451041698; distx=0.00002; disty=0.00002;*/

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=2.0; disty=2.0;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.5; disty=0.5;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.02; disty=0.02;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.002; disty=0.002;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.0002; disty=0.0002;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00002; disty=0.00002;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.000002; disty=0.000002;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.0000002; disty=0.0000002;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000008; disty=0.00000008;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.000000008; disty=0.000000008;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.000000006; disty=0.000000006;*/
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000000008; disty=0.00000000008;*/
/**** centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000000004; disty=0.00000000004;*/

/****************************************************************************/
/**                                                                        **/
/**  Sequence 4                                                            **/
/**    colormaps:  2, 4, 1                                                 **/
/**                                                                        **/
/****************************************************************************/

/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=1.75; disty=1.75; strcpy(FileBase,"Seq04-00-");*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.1; disty=0.1; strcpy(FileBase,"Seq04-01-");*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.0008; disty=0.0008; strcpy(FileBase,"Seq04-02-");*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.00002; disty=0.00002;strcpy(FileBase,"Seq04-03-");*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.000002; disty=0.000002; strcpy(FileBase,"Seq04-04-");*/

/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/*  'John Dewey Jones midget' on the spike  (Ken Philip) */
/* centerx= -1.99638; centery=0.0; distx=1.5; disty=1.5;*/
/* centerx= -1.99638; centery=0.0; distx=0.05; disty=0.05;*/
/* centerx= -1.99638; centery=0.0; distx=1.43e-5; disty=1.43e-5;*/

/****************************************************************************/
/**                                                                        **/
/** Sequence 2                                                             **/
/**   COLORMAP05                                                           **/
/**                                                                        **/
/****************************************************************************/

/*  Quad spiral in East Valley - NEAT!! (Ken Philip) */
/* centerx=0.2913061419; centery=0.0151788696; distx=1.5; disty=1.5; strcpy(FileBase,"Seq02-00-");*/
/* centerx=0.2913061419; centery=0.0151788696; distx=1.23e-8; disty=1.23e-8; strcpy(FileBase,"Seq02-01-");*/
/*** centerx=0.2913061419; centery=0.0151788696; distx=0.000000005; disty=0.000000005; strcpy(FileBase,"Seq02-02-");*/
/* centerx=0.2913061419; centery=0.0151788696; distx=0.0000000008; disty=0.0000000008; strcpy(FileBase,"Seq02-03-");*/

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

    /*    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474; strcpy(FileBase,"Seq01-00-");*/
    /*centerx= -0.7499225; centery=0.0169325; distx=1.5; disty=1.5; strcpy(FileBase,"Seq01-01-");*/
    /* zoom/move to... */
 /*    centerx= -0.7499225; centery=0.0169325; distx=1.056e-5; disty=1.056e-5; strcpy(FileBase,"Seq01-02-");*/
 /*    centerx= -0.7499225; centery=0.0169325; distx=1.056e-5; disty=1.056e-5; strcpy(FileBase,"Seq01-02-");*/
    /* hold, and then move to... **/
    /* centerx= -0.7499389; centery=0.0169435; distx=1.056e-5; disty=1.056e-5; strcpy(FileBase,"Seq01-03-");*/
    /* centerx= -0.7499389; centery=0.0169435; distx=1.056e-5; disty=1.056e-5; strcpy(FileBase,"Seq01-03-");*/
    /*  hold, and then move to... */
    /*    centerx= -0.74996540; centery=0.0169564; distx=1.056e-5; disty=1.056e-5; strcpy(FileBase,"Seq01-04-");*/
    /*    centerx= -0.74996540; centery=0.0169564; distx=1.056e-5; disty=1.056e-5; strcpy(FileBase,"Seq01-04-");*/
    /* next should look like the last */
    /*    centerx= -0.74996897; centery=0.0169620; distx=1.056e-6; disty=1.056e-5; strcpy(FileBase,"Seq01-05-");*/
    /*    centerx= -0.74996897; centery=0.0169620; distx=1.056e-6; disty=1.056e-5; strcpy(FileBase,"Seq01-05-");*/
    /*  one more sequence to look like the last 2 */
    /*    centerx= -0.749969344; centery=0.01696235; distx=0.0000003056; disty=1.056e-5; strcpy(FileBase,"Seq01-06-");*/
    /*    centerx= -0.749969344; centery=0.01696235; distx=0.0000003056; disty=1.056e-5; strcpy(FileBase,"Seq01-06-");*/
    /*  move centered over the blue */
    /*    centerx= -0.7499693494686478; centery=0.0169623462865075; distx=1.056e-7; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/
    /*    centerx= -0.7499693494686478; centery=0.0169623462865075; distx=1.056e-7; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/
    /*  dive into the blue */
 /*    centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.0000000000000048; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/
 /*    centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.000000000000008; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/
    /*  end point for 575 VGA - 84k dwell centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.00000000000000371995; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/
    /*    centerx= -0.7499693494686478; centery=0.0169623462865075; distx=0.000000000000001; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/




/****************************************************************************/
/**                                                                        **/
/**   Rising sun sorta thing - 32k at least                                **/
/**                                                                        **/
/****************************************************************************/


      /*      centerx= -0.74996934950; centery=0.0169623455; distx=0.000000001056; disty=1.056e-5; strcpy(FileBase,"Seq01-07-");*/



/****************************************************************************/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/

/*  Rabbit Valley, between north atom and its head  (Ken Philip) */
/*  not impressed */
/* centerx= -0.117824; centery=0.838990; distx=1.5; disty=1.5;*/
/* centerx= -0.117824; centery=0.838990; distx=0.00152671; disty=0.00152671;*/

/****************************************************************************/
/**                                                                        **/
/**   Sequence 3                                                           **/
/**    <400 Colors                                                         **/
/**                                                                        **/
/****************************************************************************/

/*  near small midget on the spike  (Ken Philip) */
/* centerx= -1.711638721; centery=0.000449787; distx=1.5; disty=1.5; strcpy(FileBase,"Seq03-00");*/
/* centerx= -1.7116387198; centery=0.0004497826; distx=1.4124e-8; strcpy(FileBase,"Seq03-01-");*/
/* centerx= -1.7116387198; centery=0.0004497826; distx=1.4124e-9; strcpy(FileBase,"Seq03-02-");*/
    /*  hold and zoom to */
/*** centerx= -1.711638719870; centery=0.000449782569; distx=1.4124e-10; strcpy(FileBase,"Seq03-03");*/
/*** centerx= -1.711638719870; centery=0.000449782569; distx=0.00000000014124; strcpy(FileBase,"Seq03-04");*/

/*    nx=204; ny=153; maxiter=2000;*/
/*  strcat(FileBase,"575-"); nx=575; ny=431; maxiter=84*1024;*/
  strcat(FileBase,"575-"); nx=575; ny=431; maxiter=32*1024;
  /* NTSC */
/*  strcat(FileBase,"720-"); nx=720; ny=480; maxiter=64*1024;*/
/*  strcat(FileBase,"1024-"); nx=1024; ny=768; maxiter=42*1024;*/
/*    nx=2048; ny=1536; maxiter=65535;*/
/*    strcat(FileBase,"6000-"); nx=6000; ny=4500; maxiter=128*1024;*/

/*  should be 16:9 HDTV at 1080 */
/*  CHANGE TO ASPECT16TO9       */

/*    strcat(FileBase,"1920-"); nx=1920; ny=1080; maxiter=84*2024;*/

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
    printf("(%e,%e) (%e,%e) %e ",xmin,xmax,ymin,ymax,(xmax-xmin));
    printf("Incs:  (%e,%e)\n",incx,incy);
#endif

    /* add aspect ratio to FileBase */

#ifdef ASPECT35
    strcat(FileBase,"35mm-");
#endif
#ifdef ASPECTVGA
    strcat(FileBase,"VGA-");
#endif
#ifdef ASPECT16TO9
    strcat(FileBase,"16x9-");
#endif
#ifdef ASPECTSQUARE
    strcat(FileBase,"SQ-");
#endif

    /* zero the color histogram */

    for (ix=0; ix<MAX_COLORS ;ix++)
      ColorHist[ix]=0;

    for (ix=0; ix<nx; ix++)
      for (iy=0; iy<ny; iy++)
	Fractal[ix][iy]=0;

    /* do the fractal and plot the points */

    printf("\nFractaling %d x %d:\n  ",nx,ny);
    fflush((FILE *)stdout);

    for (iy=0;iy<ny-1;iy++) {
      if ( iy%100 == 0 ) { printf("%d ",iy); fflush((FILE *)stdout); }
      cy=ymin+iy*incy;
      for (ix=0;ix<nx-1;ix++) {
	cx=xmin+ix*incx;

	ColorIndex=(int)MSetLevel(cx,cy,maxiter);
	Fractal[ix][iy]=ColorIndex;

	if ( ColorIndex > MaxIterUsed ) MaxIterUsed = ColorIndex;

	/* build the color histogram */

	ColorHist[ColorIndex]++;

      } /* for ix */
    } /* for iy */

    printf("\n");

    printf("\nlength of x %f, length of y %f",lengthx,lengthy);
    printf("\ncenter point is (%f,%f)",centerx,centery);
    printf("\ndist is (%f,%f)",distx,disty);
    printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
    printf("\nnx %d, ny %d, dwell %d MaxIterUsed %d\n",
	   nx,ny,maxiter,MaxIterUsed);

    /*  determine number of colors used */

    NumColorsUsed=0;
    for (ix=0; ix<MAX_COLORS; ix++)
      if ( ColorHist[ix] )
	NumColorsUsed++;

    printf("\nNumber of colors used:  %d\n",NumColorsUsed);

    /* initialize the color map */

    ColorMap(NumColorsUsed);

    /*
    Map the colors we just defined to the actual colors used...
    */

    int RealIndex;
    RealIndex=0;
    for (ix=0; ix<MAX_COLORS; ix++)
      if ( ColorHist[ix] ) {
	RealColorRed[ix]=ColorRed[RealIndex];
	RealColorGreen[ix]=ColorGreen[RealIndex];
	RealColorBlue[ix]=ColorBlue[RealIndex];
	/*	printf("Mapping %d to %d\n",ix,RealIndex);*/
	RealIndex++;
      }

    printf("RealIndex out of loop is %d\n",RealIndex);

    int MinUsed, MaxUsed;
    MinUsed=MAX_COLORS; MaxUsed=0;
    for (ix=0; ix<nx; ix++)
      for (iy=0; iy<ny; iy++) {
	if ( Fractal[ix][iy] > MaxUsed) MaxUsed=Fractal[ix][iy];
	if ( Fractal[ix][iy] < MinUsed) MinUsed=Fractal[ix][iy];
      }

    printf("\nMin ColorIndex Actual:  %d\n",MinUsed);
    printf("Max ColorIndex Actual:  %d\n",MaxUsed);
    printf("RealColor[EndIndex]:  (%f, %f, %f) - %d\n", RealColorRed[MaxUsed],
	   RealColorGreen[MaxUsed], RealColorBlue[MaxUsed], MaxUsed);
    printf("Color[NumColorsUsed]:  (%f, %f, %f) - %d\n", ColorRed[RealIndex-1],
	   ColorGreen[RealIndex-1], ColorBlue[RealIndex-1], (RealIndex-1));


    /* initialize the GNU plotter for meta data output and size */

    OpenPlotter(nx, ny);
    for (iy=0; iy<ny-1; iy++)
      for (ix=0; ix<nx-1; ix++) {

	/* set color */

	pl_pencolor((int)RealColorRed[Fractal[ix][iy]],
		    (int)RealColorGreen[Fractal[ix][iy]],
		    (int)RealColorBlue[Fractal[ix][iy]]);
	pl_fillcolor((int)RealColorRed[Fractal[ix][iy]],
		     (int)RealColorGreen[Fractal[ix][iy]],
		     (int)RealColorBlue[Fractal[ix][iy]]);

	/* plot point */

	pl_point(ix,iy);

      }  /*  plot file loop */

    pl_flushpl();  /* flush outstanding plot commands */
    pl_closepl();  /* close the plotter */

    ClosePlotter();



    /* write the color histogram */

#ifdef WRITEHISTOGRAMS
    strcpy(FileName,"Hist-Full-");
    strcat(FileName,FileBase);
    strcat(FileName,".txt");

    File=fopen(FileName,"w");
    for (nx=0;nx<=maxiter;nx++)
      fprintf(File,"%d %d\n",nx,ColorHist[nx]);
    fclose(File);

    strcpy(FileName,"Hist-NZ-");
    strcat(FileName,FileBase);
    strcat(FileName,".txt");

    File=fopen(FileName,"w");
    for (nx=0;nx<=maxiter;nx++) 
      if ( ColorHist[nx] )
	fprintf(File,"%d, %d\n",nx,ColorHist[nx]);
    fclose(File);
#endif

    printf("\n\nDone - %s.\n",FileBase);

}

int MSetLevel(cx,cy,maxiter)
double cx,cy;
int maxiter;
{
    int iter;
    double x,y,x2,y2,temp;
    /*dougee*/

    x = 0; y = 0; x2 = 0; y2 = 0; iter = 0;
    /*    while ((iter<maxiter) & (x2+y2<250000.0)) {*/
    /*    while ((iter<maxiter) & (x2+y2<250000.0)) {*/
    while ((iter<maxiter) & (x2+y2<2500000.0)) {
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

void ColorMap(NumColorsUsed)
int NumColorsUsed;
{
  int i,j,ColorChoice;
  float colorinc,redstart,greenstart,bluestart;
  float redinc,greeninc,blueinc;
  float RedValue, GreenValue, BlueValue;
  float RedStart, RedEnd, GreenStart, GreenEnd, BlueStart, BlueEnd;
  float RedStart1, RedStart2, RedStart3, RedEnd1, RedEnd2, RedEnd3;
  float RedStart4, RedEnd4;
  float GreenStart1, GreenStart2, GreenStart3, GreenEnd1, GreenEnd2, GreenEnd3;
  float GreenStart4, GreenEnd4;
  float BlueStart1, BlueStart2, BlueStart3, BlueEnd1, BlueEnd2, BlueEnd3;
  float BlueStart4, BlueEnd4;
  float ColorRedInc, ColorGreenInc, ColorBlueInc;
  float ColorRedInc1, ColorRedInc2, ColorRedInc3, ColorRedInc4;
  float ColorGreenInc1, ColorGreenInc2, ColorGreenInc3, ColorGreenInc4;
  float ColorBlueInc1, ColorBlueInc2, ColorBlueInc3, ColorBlueInc4;
  FILE *File;

  /*  4 way colormap */

#define COLORMAP11
#define PRINTCOLORMAPS

#ifdef COLORMAP01   /* primaries ending in purple */
#define QUADCOLOR
  strcat(FileBase,"CMap01");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65535.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=65535; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=0.0; GreenEnd2=65535.0; BlueEnd2=0.0;

  RedStart3=0.0; GreenStart3=65535.0; BlueStart3=0.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=65535.0;

  RedStart4=0.0; GreenStart4=0.0; BlueStart4=65535.0;
  RedEnd4=40000.0; GreenEnd4=500.0; BlueEnd4=60000.0;
#endif

#ifdef COLORMAP02  /* red to orange to black */
#define QUADCOLOR
  strcat(FileBase,"CMap02");
  RedStart1=65535.0; GreenStart1=18504.0; BlueStart1=0.0;
  RedEnd1=65535.0; GreenEnd1=33924.0; BlueEnd1=0.0;

  RedStart2=65535.0; GreenStart2=33924.0; BlueStart2=0.0;
  RedEnd2=65535; GreenEnd2=52428.0; BlueEnd2=0.0;

  RedStart3=65535.0; GreenStart3=52428.0; BlueStart3=0.0;
  RedEnd3=65535.0; GreenEnd3=61680.0; BlueEnd3=0.0;

  RedStart4=65535.0; GreenStart4=61680.0; BlueStart4=0.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP03  /* red to purple to black */
#define QUADCOLOR
  strcat(FileBase,"CMap03");
  RedStart1=33667.0; GreenStart1=19018.0; BlueStart1=20303.0;
  RedEnd1=65535.0; GreenEnd1=0.0; BlueEnd1=6168.0;

  RedStart2=65535.0; GreenStart2=0.0; BlueStart2=6168.0;
  RedEnd2=65535.0; GreenEnd2=0.0; BlueEnd2=24672.0;

  RedStart3=65535.0; GreenStart3=0.0; BlueStart3=24672.0;
  RedEnd3=45489.0; GreenEnd3=16191.0; BlueEnd3=39064.0;

  RedStart4=45489.0; GreenStart4=16191.0; BlueStart4=39064.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP04  /* primaries ending in white */
#define QUADCOLOR
  strcat(FileBase,"CMap04");
  RedStart1=65535.0; GreenStart1=65535.0; BlueStart1=65535.0;
  RedEnd1=65535.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=65535.0; GreenStart2=0.0; BlueStart2=0.0;
  /*  RedEnd2=0.0; GreenEnd2=65535.0; BlueEnd2=0.0;*/
  RedEnd2=24415.0; GreenEnd2=7967.0; BlueEnd2=1542.0;

  RedStart3=24415.0; GreenStart3=7967.0; BlueStart3=1542.0;
  /*  RedStart3=0.0; GreenStart3=65535.0; BlueStart3=0.0;*/
  /*  RedEnd3=0.0; GreenEnd2=0.0; BlueEnd3=65535.0;*/
  RedEnd3=20945.0; GreenEnd3=6810.0; BlueEnd3=11051.0;

  RedStart4=20945.0; GreenStart4=6810.0; BlueStart4=11051.0;
  /*  RedStart4=0.0; GreenStart4=0.0; BlueStart4=65535.0;*/
  /*  RedEnd4=65536.0; GreenEnd4=65535.0; BlueEnd4=65535.0;*/
  RedEnd4=17476.0; GreenEnd4=5654.0; BlueEnd4=20560.0;
#endif

#ifdef COLORMAP05  /* primaries ending in black */
#define QUADCOLOR
  strcat(FileBase,"CMap05");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65535.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=65535.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=0.0; GreenEnd2=65535.0; BlueEnd2=0.0;

  RedStart3=0.0; GreenStart3=65535.0; BlueStart3=0.0;
  RedEnd3=0.0; GreenEnd2=0.0; BlueEnd3=65535.0;

  RedStart4=0.0; GreenStart4=0.0; BlueStart4=65535.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP06  /* purple to blue to black */
#define QUADCOLOR
  strcat(FileBase,"CMap06");
  RedStart1=28013.0; GreenStart1=3084.0; BlueStart1=27242.0;
  RedEnd1=21845.0; GreenEnd1=3084.0; BlueEnd1=27242.0;

  RedStart2=21845.0; GreenStart2=3084.0; BlueStart2=27242.0;
  RedEnd2=13621.0; GreenEnd2=3084.0; BlueEnd2=27242.0;

  RedStart3=13621.0; GreenStart3=3084.0; BlueStart3=27242.0;
  RedEnd3=3084.0; GreenEnd3=3084.0; BlueEnd3=27242.0;

  RedStart4=3084.0; GreenStart4=3084.0; BlueStart4=27242.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP07  /* purple to red to black */
#define QUADCOLOR
  strcat(FileBase,"CMap07");
  RedStart1=28013.0; GreenStart1=3084.0; BlueStart1=27242.0;
  RedEnd1=21845.0; GreenEnd1=3084.0; BlueEnd1=27242.0;

  RedStart2=21845.0; GreenStart2=3084.0; BlueStart2=27242.0;
  RedEnd2=13621.0; GreenEnd2=3084.0; BlueEnd2=27242.0;

  RedStart3=13621.0; GreenStart3=3084.0; BlueStart3=27242.0;
  RedEnd3=65535.0; GreenEnd3=3084.0; BlueEnd3=27242.0;

  RedStart4=65535.0; GreenStart4=3084.0; BlueStart4=27242.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP08  /* purple to bright blue to black */
#define QUADCOLOR
  strcat(FileBase,"CMap08");
  RedStart1=28013.0; GreenStart1=3084.0; BlueStart1=27242.0;
  RedEnd1=21845.0; GreenEnd1=3084.0; BlueEnd1=27242.0;

  RedStart2=21845.0; GreenStart2=3084.0; BlueStart2=27242.0;
  RedEnd2=13621.0; GreenEnd2=3084.0; BlueEnd2=27242.0;

  RedStart3=13621.0; GreenStart3=3084.0; BlueStart3=27242.0;
  RedEnd3=3084.0; GreenEnd3=3084.0; BlueEnd3=65535.0;

  RedStart4=3084.0; GreenStart4=3084.0; BlueStart4=65535.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP09  /* purple to red to blue to black */
#define QUADCOLOR
  strcat(FileBase,"CMap09");
  RedStart1=17476.0; GreenStart1=5654.0; BlueStart1=20560.0;
  RedEnd1=20560.0; GreenEnd1=5654.0; BlueEnd1=5654.0;

  RedStart2=20560.0; GreenStart2=5654.0; BlueStart2=5654.0;
  RedEnd2=65535.0; GreenEnd2=3084.0; BlueEnd2=7242.0;

  RedStart3=65535.0; GreenStart3=3084.0; BlueStart3=7242.0;
  RedEnd3=3084.0; GreenEnd3=3084.0; BlueEnd3=65535.0;

  RedStart4=3084.0; GreenStart4=3084.0; BlueStart4=65535.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP10  /* purple to red to blue to black */
#define TRICOLOR
  strcat(FileBase,"CMap10");
  RedStart1=17476.0; GreenStart1=5654.0; BlueStart1=20560.0;
  RedEnd1=65535.0; GreenEnd1=3084.0; BlueEnd1=7242.0;

  RedStart2=65535.0; GreenStart2=3084.0; BlueStart2=7242.0;
  RedEnd2=3084.0; GreenEnd2=3084.0; BlueEnd2=65535.0;

  RedStart3=3084.0; GreenStart3=3084.0; BlueStart3=65535.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=0.0;
#endif

#ifdef COLORMAP11  /* fire attempt 1 */
#define TRICOLOR

  /* burnt red - 24415, 7967, 1542 */
  /* orange - 64764, 29555, 15677 */
  /* red - 65535, 3084, 7242 */

  strcat(FileBase,"CMap11");
  RedStart1=24415.0; GreenStart1=7967.0; BlueStart1=1542.0;
  RedEnd1=64764.0; GreenEnd1=29555.0; BlueEnd1=15677.0;

  RedStart2=64764.0; GreenStart2=29555.0; BlueStart2=15677.0;
  RedEnd2=65535.0; GreenEnd2=3084.0; BlueEnd2=7242.0;

  RedStart3=65535.0; GreenStart3=3084.0; BlueStart3=7242.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=0.0;
#endif

#ifdef QUADCOLOR

  ColorRedInc1 = (RedEnd1-RedStart1)/(float)(NumColorsUsed/4-1);
  ColorGreenInc1 = (GreenEnd1-GreenStart1)/(float)(NumColorsUsed/4-1);
  ColorBlueInc1 = (BlueEnd1-BlueStart1)/(float)(NumColorsUsed/4-1);

  ColorRedInc2 = (RedEnd2-RedStart2)/(float)(NumColorsUsed/4);
  ColorGreenInc2 = (GreenEnd2-GreenStart2)/(float)(NumColorsUsed/4);
  ColorBlueInc2 = (BlueEnd2-BlueStart2)/(float)(NumColorsUsed/4);

  ColorRedInc3 = (RedEnd3-RedStart3)/(float)(NumColorsUsed/4);
  ColorGreenInc3 = (GreenEnd3-GreenStart3)/(float)(NumColorsUsed/4);
  ColorBlueInc3 = (BlueEnd3-BlueStart3)/(float)(NumColorsUsed/4);

  ColorRedInc4 = (RedEnd4-RedStart4)/(float)(NumColorsUsed/4);
  ColorGreenInc4 = (GreenEnd4-GreenStart4)/(float)(NumColorsUsed/4);
  ColorBlueInc4 = (BlueEnd4-BlueStart4)/(float)(NumColorsUsed/4);

#ifdef PRINTCOLORMAPS
  printf("\n\n    Colormaps:\n");
  printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  printf("    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	 BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);
  printf("    %f %f %f --> %f  %f %f\n",RedStart3, GreenStart3,
	 BlueStart3, RedEnd3, GreenEnd3, BlueEnd3);
  printf("    %f %f %f --> %f  %f %f\n",RedStart4, GreenStart4,
	 BlueStart4, RedEnd4, GreenEnd4, BlueEnd4);

  printf("    Inc1's: %f, %f, %f\n", 
	 ColorRedInc1, ColorGreenInc1, ColorBlueInc1);
  printf("    Inc2's: %f, %f, %f\n", 
	 ColorRedInc2, ColorGreenInc2, ColorBlueInc2);
  printf("    Inc3's: %f, %f, %f\n", 
	 ColorRedInc3, ColorGreenInc3, ColorBlueInc3);
  printf("    Inc4's: %f, %f, %f\n", 
	 ColorRedInc4, ColorGreenInc4, ColorBlueInc4);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<(NumColorsUsed/4);
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

#ifdef DEBUG
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign first quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of first fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<(NumColorsUsed/2);
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

#ifdef DEBUG
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign second quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of second fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
       i<(NumColorsUsed*3/4);
       i++,
	 RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	 BlueValue+=ColorBlueInc3) {
    
    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

#ifdef DEBUG
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign third quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of third fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart4,GreenValue=GreenStart4,BlueValue=BlueStart4;
       i<NumColorsUsed;
       i++,
	 RedValue+=ColorRedInc4,GreenValue+=ColorGreenInc4,
	 BlueValue+=ColorBlueInc4) {

    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

#ifdef DEBUG
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign fourth quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of fourth fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

#endif

#ifdef TRICOLOR

  ColorRedInc1 = (RedEnd1-RedStart1)/(float)(NumColorsUsed/3-1);
  ColorGreenInc1 = (GreenEnd1-GreenStart1)/(float)(NumColorsUsed/3-1);
  ColorBlueInc1 = (BlueEnd1-BlueStart1)/(float)(NumColorsUsed/3-1);

  ColorRedInc2 = (RedEnd2-RedStart2)/(float)(NumColorsUsed/3-1);
  ColorGreenInc2 = (GreenEnd2-GreenStart2)/(float)(NumColorsUsed/3-1);
  ColorBlueInc2 = (BlueEnd2-BlueStart2)/(float)(NumColorsUsed/3-1);

  ColorRedInc3 = (RedEnd3-RedStart3)/(float)(NumColorsUsed/3);
  ColorGreenInc3 = (GreenEnd3-GreenStart3)/(float)(NumColorsUsed/3);
  ColorBlueInc3 = (BlueEnd3-BlueStart3)/(float)(NumColorsUsed/3);

#ifdef PRINTCOLORMAPS
  printf("\n\n    Colormaps:\n");
  printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  printf("    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	 BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);
  printf("    %f %f %f --> %f  %f %f\n",RedStart3, GreenStart3,
	 BlueStart3, RedEnd3, GreenEnd3, BlueEnd3);

  printf("    Inc1's: %f, %f, %f\n", 
	 ColorRedInc1, ColorGreenInc1, ColorBlueInc1);
  printf("    Inc2's: %f, %f, %f\n", 
	 ColorRedInc2, ColorGreenInc2, ColorBlueInc2);
  printf("    Inc3's: %f, %f, %f\n", 
	 ColorRedInc3, ColorGreenInc3, ColorBlueInc3);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<(NumColorsUsed/3);
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

#ifdef DEBUG
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign first quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of first third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<(NumColorsUsed*2/3);
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

  }  /* assign second quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of second third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
       i<NumColorsUsed;
       i++,
	 RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	 BlueValue+=ColorBlueInc3) {
    
    ColorRed[i]=(float)((int)RedValue);
    ColorGreen[i]=(float)((int)GreenValue);
    ColorBlue[i]=(float)((int)BlueValue);

  }  /* assign third quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of third third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

#endif


#ifdef DEBUG
  File=fopen("DefinedColors.txt","w");
  for (i=0;i<=NumColorsUsed;i++)
    fprintf(File,
	    "Color %d:  <%f, %f, %f>\n",
	    i,ColorRed[i],ColorGreen[i],ColorBlue[i]);
  fclose(File);
#endif

}

int OpenPlotter(int nx, int ny)
{

  char bitmapsize[20];
  char FileName[75];

  strcpy(FileName,FileBase);
  strcat(FileName,".plot");

  if ( (int)(PlotFile=fopen(FileName,"w")) == -1 ) { 
    printf("Could not open PlotterFile.txt - exiting.\n");
    exit(-1);
  }

  strcpy(FileName,FileBase);
  strcat(FileName,"-Errors.plot");

  if ( (int)(ErrorFile=fopen(FileName,"w")) == -1 ) {
    printf("Could not open PlotErrors.txt - exiting.\n");
    fclose(PlotFile);
    exit(-1);
  }

  sprintf(bitmapsize,"%dx%d\0",nx,ny);

  pl_parampl("BG_COLOR", "white");
  pl_parampl("META_PORTABLE", "no");
  pl_parampl("BITMAPSIZE", bitmapsize);
  
  if ( (Plotter=pl_newpl("meta", NULL, PlotFile, ErrorFile)) < 0 ) {
    printf("Could not open the plotter - exiting.\n");
    fclose(PlotFile);
    fclose(ErrorFile);
    exit(Plotter);
  }

  if ( (pl_selectpl(Plotter)) < 0 ) {
    printf("Plotter %d could not be selected - exiting\n",
	    Plotter);
    fclose(PlotFile);
    fclose(ErrorFile);
    pl_deletepl(Plotter);
  }

  if ( (pl_openpl()) < 0 ) {
    printf("Plotter %d could not be opened - exiting\n",
	    Plotter);
    fclose(PlotFile);
    fclose(ErrorFile);
    pl_deletepl(Plotter);
  }

  /*    pl_fscale((double)nx, (double)ny);*/
  pl_space(0, (ny-1), (nx-1), 0);
  pl_filltype(1);

  return(0);
}

void ClosePlotter()
{

  fclose(PlotFile);
  fclose(ErrorFile);
  pl_selectpl(0);
  pl_deletepl(Plotter);

}
