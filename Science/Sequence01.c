#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#include <plot.h>  /*  GNU libplot */

#define TRUE 1
#define FALSE 0

int maxiter;

void ColorMap(int NumColorsUsed, int Frame);
int MSetLevel();

/*  set the max number of color indices */

/*#define MAX_COLORS 65535*/        /* 16 bit color */
#define MAX_COLORS 16777216     /* 24 bit color */
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

#define MAXFRAMES 10000

float ColorRed[MAX_COLORS];
float ColorGreen[MAX_COLORS];
float ColorBlue[MAX_COLORS];
float RealColorRed[MAX_COLORS];
float RealColorGreen[MAX_COLORS];
float RealColorBlue[MAX_COLORS];
int ColorHist[MAX_COLORS];

char FileBase[MAXFRAMES][60];

int Fractal[6000][6000];

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
  int i,nx,ny,ix,iy,Frame,NumFrames,NumAnchors;
  int ColorIndex;
  int MaxIterUsed=0;  /* max depth used - max color index used */
  int NumColorsUsed=0; /* number of colors actually used */
  FILE *File;
  FILE *FrameStats;
  double xmin,xmax,ymin,ymax,cx,cy,lengthx,lengthy, lengthdist,
    centerx[MAXFRAMES],
    centery[MAXFRAMES];
  double distx[MAXFRAMES],
    disty[MAXFRAMES],
    incx,
    incy,
    incdist;
  double Acenterx[MAXFRAMES],
    Acentery[MAXFRAMES],
    Adistx[MAXFRAMES];
  int Asteps[MAXFRAMES];
  double aspectx, aspecty;
  char FileName[MAXFRAMES][75];
  char Resolution[10];
  char FrameNumber[8];

#ifdef ASPECT35
  aspectx=3.0; aspecty=2.0;      /*  35mm  */
#endif
#ifdef ASPECTVGA
  aspectx=640.0; aspecty=480.0;  /*  VGA and iBook  */
#endif
#ifdef ASPECT16TO9
  aspectx=16.0; aspecty=9.0;     /*  16:9  */
#endif
#ifdef ASPECTSQUARE
  aspectx=1.0; aspecty=1.0;      /*  square  */
#endif

  /*
    "normal" maxiters:
      204 - 32k
      575 - 64k
      720 - 70k
      1024 - 96k
      6000 - 128k
      1920 - 128k
  */

  /*  strcpy(Resolution,"204-"); nx=204; ny=153; maxiter=32*1024;*/
  strcpy(Resolution,"575-"); nx=575; ny=431; maxiter=64*1024;
  /* NTSC */
  /* strcpy(Resolution,"720-"); nx=720; ny=480; maxiter=70*1024;
  /*  strcpy(Resolution,"1024-"); nx=1024; ny=768; maxiter=96*1024;*/
  /*    nx=2048; ny=1536; maxiter=65535;*/
  /*  strcpy(Resolution,"6000-"); nx=6000; ny=4500; maxiter=128*1024;*/

  /*  should be 16:9 HDTV at 1080 */
  /*  CHANGE TO ASPECT16TO9       */

  /*  strcpy(Resolution,"1920-"); nx=1920; ny=1080; maxiter=128*1024;*/


  /*************************************************************************/
  /**                                                                     **/
  /**  Sequence 1:                                                        **/
  /**    Sea horse tail in SHV - neat, at least 50000 dwell  (Ken Philip) **/
  /**    colormap:  1000 1000 8000 - 60000 50000 2000 (num 1 testing)     **/
  /**    colormap:  40000 500 60000 - 60000 50000 2000                    **/
  /**    colormap:  40000 500 60000 - 1000 1000 8000                      **/
  /**                                                                     **/
  /**  Colormaps:  2, 3?, 10, 11, 4                                       **/
  /**                                                                     **/
  /**  Points optimized for VGA                                           **/
  /**                                                                     **/
  /*************************************************************************/

#define COLORMAP04
#define ANCHORSONLY

  NumAnchors=15;
  Acenterx[0]= -0.675; Acentery[0]=0.025; Adistx[0]=1.425; Asteps[0]=90;
  Acenterx[1]= -0.7499225; Acentery[1]=0.0169325; Adistx[1]=1.5;  Asteps[1]=90;
    /* zoom/move to... */
  Acenterx[2]= -0.7499225; Acentery[2]=0.0169325; Adistx[2]=1.056e-5;  Asteps[2]=90;
  Acenterx[3]= -0.7499225; Acentery[3]=0.0169325; Adistx[3]=1.056e-5;  Asteps[3]=180;
    /* hold, and then move to... **/
  Acenterx[4]= -0.7499389; Acentery[4]=0.0169435; Adistx[4]=1.056e-5;  Asteps[4]=90;
  Acenterx[5]= -0.7499389; Acentery[5]=0.0169435; Adistx[5]=1.056e-5; Asteps[5]=180;
    /*  hold, and then move to... */
  Acenterx[6]= -0.74996540; Acentery[6]=0.0169564; Adistx[6]=1.056e-5; Asteps[6]=90;
  Acenterx[7]= -0.74996540; Acentery[7]=0.0169564; Adistx[7]=1.056e-5; Asteps[7]=180;
    /* next should look like the last */
  Acenterx[8]= -0.74996897; Acentery[8]=0.0169620; Adistx[8]=1.056e-6; Asteps[8]=90;
  Acenterx[9]= -0.74996897; Acentery[9]=0.0169620; Adistx[9]=1.056e-6; Asteps[9]=180;
    /*  one more sequence to look like the last 2 */
  Acenterx[10]= -0.749969344; Acentery[10]=0.01696235; Adistx[10]=0.0000003056; Asteps[10]=90;
  Acenterx[11]= -0.749969344; Acentery[11]=0.01696235; Adistx[11]=0.0000003056; Asteps[11]=180;
    /*  move centered over the blue */
  Acenterx[12]= -0.7499693494686478; Acentery[12]=0.0169623462865075; Adistx[12]=1.056e-7; Asteps[12]=90;
  Acenterx[13]= -0.7499693494686478; Acentery[13]=0.0169623462865075; Adistx[13]=1.056e-7; Asteps[13]=300;
    /*  dive into the blue */
  Acenterx[14]= -0.7499693494686478; Acentery[14]=0.0169623462865075; Adistx[14]=0.000000000000005; Asteps[14]=1;


  /*************************************************************************/
  /**                                                                     **/
  /**  anchor frames defined - generate individual frame info             **/
  /**                                                                     **/
  /*************************************************************************/

  NumFrames=0;

  printf("Generating individual frame data...\n");

  /* 2 second hold on first frame */

#ifndef ANCHORSONLY
  for (i=0; i<(2*30); i++) {
    centerx[NumFrames]=Acenterx[0];
    centery[NumFrames]=Acentery[0];
    distx[NumFrames]=Adistx[0];
    sprintf(FrameNumber,"%4d",NumFrames);
    for (ix=0; ix<4; ix++)
      if (FrameNumber[ix] == ' ') FrameNumber[ix]='0';
    strcpy(FileBase[NumFrames],"Seq01-");
    strcat(FileBase[NumFrames],FrameNumber);
    strcat(FileBase[NumFrames],"-");
    NumFrames++;
  }  /* end of 2 second hold generation */
#endif

#ifdef ANCHORSONLY
  for (ix=0; ix<NumAnchors; ix++) {
#else
  for (ix=0; ix<NumAnchors-1; ix++) {
#endif

#ifdef ANCHORSONLY
    centerx[NumFrames]=Acenterx[ix];
    centery[NumFrames]=Acentery[ix];
    distx[NumFrames]=Adistx[ix];
    sprintf(FrameNumber,"%4d",NumFrames);
    for (iy=0; iy<4; iy++)
      if (FrameNumber[iy] == ' ') FrameNumber[iy]='0';
    strcpy(FileBase[NumFrames],"Seq01-");
    strcat(FileBase[NumFrames],FrameNumber);
    strcat(FileBase[NumFrames],"-");
    NumFrames++;
#else
    /*
    lengthx=(double)sqrt( (Acenterx[ix]*Acenterx[ix]) + (Acenterx[ix+1]*Acenterx[ix+1]) );
    lengthy=(double)sqrt( (Acentery[ix]*Acentery[ix]) + (Acentery[ix+1]*Acentery[ix+1]) );
    lengthdist=(double)sqrt( (Adistx[ix]*Adistx[ix]) + (Adistx[ix+1]*Adistx[ix+1]) );
    */
    incx=(Acenterx[ix+1]-Acenterx[ix])/((double)Asteps[ix]);
    incy=(Acentery[ix+1]-Acentery[ix])/((double)Asteps[ix]);
    incdist=(Adistx[ix+1]-Adistx[ix])/((double)Asteps[ix]);
    for (iy=0; iy<Asteps[ix]; iy++) {
      centerx[NumFrames]=Acenterx[ix]+((double)iy*incx);
      centery[NumFrames]=Acentery[ix]+((double)iy*incy);
      distx[NumFrames]=Adistx[ix]+((double)iy*incdist);
      sprintf(FrameNumber,"%4d",NumFrames);
      for (i=0; i<4; i++)
	if (FrameNumber[i] == ' ') FrameNumber[i]='0';
      strcpy(FileBase[NumFrames],"Seq01-");
      strcat(FileBase[NumFrames],FrameNumber);
      strcat(FileBase[NumFrames],"-");
      NumFrames++;
    }  /* for each step */
#endif

  }  /*  for each anchor frame */

  printf("Going to generate %d frames...\n",NumFrames);


  /*************************************************************************/
  /**                                                                     **/
  /**  frame control loop:                                                **/
  /**                                                                     **/
  /**    add resolution to filebase                                       **/
  /**    determine disty based on aspect ratio                            **/
  /**    add aspect ration to filebase                                    **/
  /**    do the fractal                                                   **/
  /**    do colormap                                                      **/
  /**    write plot file                                                  **/
  /**                                                                     **/
  /**                                                                     **/
  /*************************************************************************/

  FrameStats=fopen("FrameStats-Sequence01","w");

  for (Frame=0; Frame<NumFrames; Frame++) {

    /*  add frame resolution to FileBase */

    strcat(FileBase[Frame],Resolution);

    /*  determine y values based on set aspect ratio */

    ny=(int)( (double)nx*aspecty/aspectx );
    disty[Frame]=distx[Frame]*aspecty/aspectx;

    xmin=centerx[Frame]-distx[Frame]; xmax=centerx[Frame]+distx[Frame];
    ymin=centery[Frame]-disty[Frame]; ymax=centery[Frame]+disty[Frame];
    lengthx = (double)sqrt((xmin*xmin)+(xmax*xmax));
    lengthy = (double)sqrt((ymin*ymin)+(ymax*ymax));
    incx=(xmax-xmin)/((double)nx-1.0); incy=(ymax-ymin)/((double)ny-1.0);
    if(xmax==xmin) {exit(-1);}

#ifdef DEBUG
    printf("Center (%e,%e)\n",centerx[Frame],centery[Frame]);
    printf("Distance (%e,%e)\n",distx[Frame],disty[Frame]);
    printf("(%e,%e) (%e,%e) %e ",xmin,xmax,ymin,ymax,(xmax-xmin));
    printf("Incs:  (%e,%e)\n",incx,incy);
#endif

    /* add aspect ratio to FileBase */

#ifdef ASPECT35
    strcat(FileBase[Frame],"35mm-");
#endif
#ifdef ASPECTVGA
    strcat(FileBase[Frame],"VGA-");
#endif
#ifdef ASPECT16TO9
    strcat(FileBase[Frame],"16x9-");
#endif
#ifdef ASPECTSQUARE
    strcat(FileBase[Frame],"SQ-");
#endif

    /********************************************************************/
    /**                                                                **/
    /**  Begin the generation loop                                     **/
    /**                                                                **/
    /********************************************************************/

    /* zero the color histogram and fractal */

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

    /*  write out the stats for the frame */

    printf("\n");
    printf("\nlength of x %f, length of y %f",lengthx,lengthy);
    printf("\ncenter point is (%f,%f)",centerx[Frame],centery[Frame]);
    printf("\ndist is (%f,%f)",distx[Frame],disty[Frame]);
    printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
    printf("\nnx %d, ny %d, dwell %d MaxIterUsed %d\n",
	   nx,ny,maxiter,MaxIterUsed);

    fprintf(FrameStats,"\n");
    fprintf(FrameStats,"%s",FileBase[Frame]);
    fprintf(FrameStats,"\nlength of x %f, length of y %f",lengthx,lengthy);
    fprintf(FrameStats,"\ncenter point is (%f,%f)",centerx[Frame],
	    centery[Frame]);
    fprintf(FrameStats,"\ndist is (%f,%f)",distx[Frame],disty[Frame]);
    fprintf(FrameStats,"\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
    fprintf(FrameStats,"\nnx %d, ny %d, dwell %d MaxIterUsed %d\n",
	   nx,ny,maxiter,MaxIterUsed);

    /*  determine number of colors used */

    NumColorsUsed=0;
    for (ix=0; ix<MAX_COLORS; ix++)
      if ( ColorHist[ix] )
	NumColorsUsed++;
    
    printf("Number of colors used:  %d\n",NumColorsUsed);
    fprintf(FrameStats,"Number of colors used:  %d\n",NumColorsUsed);

    /* initialize the color map */

    ColorMap(NumColorsUsed,Frame);

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

    /*    printf("RealIndex out of loop is %d\n",RealIndex);*/

    int MinUsed, MaxUsed;
    MinUsed=MAX_COLORS; MaxUsed=0;
    for (ix=0; ix<nx; ix++)
      for (iy=0; iy<ny; iy++) {
	if ( Fractal[ix][iy] > MaxUsed) MaxUsed=Fractal[ix][iy];
	if ( Fractal[ix][iy] < MinUsed) MinUsed=Fractal[ix][iy];
	}
    /*
      printf("\nMin ColorIndex Actual:  %d\n",MinUsed);
      printf("Max ColorIndex Actual:  %d\n",MaxUsed);
    */

    /* initialize the GNU plotter for meta data output and size */
    
    OpenPlotter(nx, ny, Frame);
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

    printf("\n\nDone - %s.\n",FileBase[Frame]);

  } /*  end of frame generation loop */

  fclose(FrameStats);

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

void ColorMap(NumColorsUsed, Frame)
int NumColorsUsed;
int Frame;
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

#ifdef COLORMAP01   /* primaries ending in purple */
#define QUADCOLOR
  strcat(FileBase[Frame],"CMap01");
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
  strcat(FileBase[Frame],"CMap02");
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
  strcat(FileBase[Frame],"CMap03");
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
  strcat(FileBase[Frame],"CMap04");
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
  strcat(FileBase[Frame],"CMap05");
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
  strcat(FileBase[Frame],"CMap06");
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
  strcat(FileBase[Frame],"CMap07");
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
  strcat(FileBase[Frame],"CMap08");
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
  strcat(FileBase[Frame],"CMap09");
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
  strcat(FileBase[Frame],"CMap10");
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

  strcat(FileBase[Frame],"CMap11");
  RedStart1=24415.0; GreenStart1=7967.0; BlueStart1=1542.0;
  RedEnd1=64764.0; GreenEnd1=29555.0; BlueEnd1=15677.0;

  RedStart2=64764.0; GreenStart2=29555.0; BlueStart2=15677.0;
  RedEnd2=65535.0; GreenEnd2=3084.0; BlueEnd2=7242.0;

  RedStart3=65535.0; GreenStart3=3084.0; BlueStart3=7242.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=0.0;
#endif

#ifdef QUADCOLOR

  ColorRedInc1 = (RedEnd1-RedStart1)/((float)NumColorsUsed/4.0);
  ColorGreenInc1 = (GreenEnd1-GreenStart1)/((float)NumColorsUsed/4.0);
  ColorBlueInc1 = (BlueEnd1-BlueStart1)/((float)NumColorsUsed/4.0);

  ColorRedInc2 = (RedEnd2-RedStart2)/((float)NumColorsUsed/4.0);
  ColorGreenInc2 = (GreenEnd2-GreenStart2)/((float)NumColorsUsed/4.0);
  ColorBlueInc2 = (BlueEnd2-BlueStart2)/((float)NumColorsUsed/4.0);

  ColorRedInc3 = (RedEnd3-RedStart3)/((float)NumColorsUsed/4.0);
  ColorGreenInc3 = (GreenEnd3-GreenStart3)/((float)NumColorsUsed/4.0);
  ColorBlueInc3 = (BlueEnd3-BlueStart3)/((float)NumColorsUsed/4.0);

  ColorRedInc4 = (RedEnd4-RedStart4)/((float)NumColorsUsed/4.0);
  ColorGreenInc4 = (GreenEnd4-GreenStart4)/((float)NumColorsUsed/4.0);
  ColorBlueInc4 = (BlueEnd4-BlueStart4)/((float)NumColorsUsed/4.0);

#ifdef COLORMAPDETAILS
  printf("\n\n    Colormaps:\n");
  printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  printf("    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	 BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);
  printf("    %f %f %f --> %f  %f %f\n",RedStart3, GreenStart3,
	 BlueStart3, RedEnd3, GreenEnd3, BlueEnd3);
  printf("    %f %f %f --> %f  %f %f\n",RedStart4, GreenStart4,
	 BlueStart4, RedEnd4, GreenEnd4, BlueEnd4);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<=(int)((float)NumColorsUsed/4.0);
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign first quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of first fourth:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<=(int)((float)NumColorsUsed*2.0/4.0);
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign second quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of second fourth:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
#endif

  for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
       i<=(int)((float)NumColorsUsed*3.0/4.0);
       i++,
	 RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	 BlueValue+=ColorBlueInc3) {
    
    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign third quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of third fourth:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
#endif

  for (RedValue=RedStart4,GreenValue=GreenStart4,BlueValue=BlueStart4;
       i<=NumColorsUsed;
       i++,
	 RedValue+=ColorRedInc4,GreenValue+=ColorGreenInc4,
	 BlueValue+=ColorBlueInc4) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign fourth quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of fourth fourth:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
#endif

#endif

#ifdef TRICOLOR

  ColorRedInc1 = (RedEnd1-RedStart1)/((float)NumColorsUsed/3.0);
  ColorGreenInc1 = (GreenEnd1-GreenStart1)/((float)NumColorsUsed/3.0);
  ColorBlueInc1 = (BlueEnd1-BlueStart1)/((float)NumColorsUsed/3.0);

  ColorRedInc2 = (RedEnd2-RedStart2)/((float)NumColorsUsed/3.0);
  ColorGreenInc2 = (GreenEnd2-GreenStart2)/((float)NumColorsUsed/3.0);
  ColorBlueInc2 = (BlueEnd2-BlueStart2)/((float)NumColorsUsed/3.0);

  ColorRedInc3 = (RedEnd3-RedStart3)/((float)NumColorsUsed/3.0);
  ColorGreenInc3 = (GreenEnd3-GreenStart3)/((float)NumColorsUsed/3.0);
  ColorBlueInc3 = (BlueEnd3-BlueStart3)/((float)NumColorsUsed/3.0);

#ifdef COLORMAPDETAILS
  printf("\n\n    Colormaps:\n");
  printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  printf("    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	 BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);
  printf("    %f %f %f --> %f  %f %f\n",RedStart3, GreenStart3,
	 BlueStart3, RedEnd3, GreenEnd3, BlueEnd3);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<=(int)((float)NumColorsUsed/3.0);
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign first quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of first third:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<=(int)((float)NumColorsUsed*2.0/3.0);
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign second quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of second third:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
#endif

  for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
       i<=(int)((float)NumColorsUsed);
       i++,
	 RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	 BlueValue+=ColorBlueInc3) {
    
    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign third quarter colors */

#ifdef COLORMAPDETAILS
  printf("    Out of third third:  (%d) %f, %f, %f\n",
	 i, RedValue, GreenValue, BlueValue);
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

int OpenPlotter(int nx, int ny, int Frame)
{

  char bitmapsize[20];
  char FileName[75];

  strcpy(FileName,FileBase[Frame]);
  strcat(FileName,".plot");

  if ( (int)(PlotFile=fopen(FileName,"w")) == -1 ) { 
    printf("Could not open PlotterFile.txt - exiting.\n");
    exit(-1);
  }

  strcpy(FileName,FileBase[Frame]);
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
