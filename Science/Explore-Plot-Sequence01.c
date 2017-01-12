#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#include <plot.h>  /*  GNU libplot */

#define TRUE 1
#define FALSE 0

int maxiter;

void ColorMap();
int MSetLevel();

#define MAX_POINTS 20  /* number of points in sequence */

/*  set the max number of color indices */

#define MAX_COLORS 65535        /* 16 bit color */
/*#define MAX_COLORS 16777216*/     /* 24 bit color */
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

float ColorRed[MAX_COLORS];
float ColorGreen[MAX_COLORS];
float ColorBlue[MAX_COLORS];
int ColorHist[MAX_COLORS];

char FileBase[60];


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
    int Point;                 /* loop counter - points in sequence */
    int FrameNum;              /* number of current frame */
    int NumFrames;             /* number of frames between points */
    int NumPoints;             /* number of points in sequence */
    int MaxIterUsed=0;         /* max depth used - max color index used */
    int NumColorsUsed=0;       /* number of colors actually used */
    FILE *File;
    double xmin,xmax,ymin,ymax,cx,cy,lengthx,lengthy;
    double centerx[MAX_POINTS],centery[MAX_POINTS];
    double distx[MAX_POINTS],disty[MAX_POINTS];
    double incx,incy;
    double aspectx, aspecty;
    char FileName[75];
    char Resolution[8];

    /*    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474;*/

/****************************************************************************/
/**                                                                        **/
/**  Sequence 1:                                                           **/
/**    Sea horse tail in SHV - neat, at least 50000 dwell  (Ken Philip)    **/
/**    colormap:  1000 1000 8000 - 60000 50000 2000 (num 1 testing)        **/
/**    colormap:  40000 500 60000 - 60000 50000 2000                       **/
/**    colormap:  40000 500 60000 - 1000 1000 8000                         **/
/**                                                                        **/
/****************************************************************************/

    /*  points optimized for VGA aspect ratio */

    strcpy(FileBase,"Seq01-");

    centerx[0]= -0.7499225; centery[0]=0.0169325; distx[0]=1.5; 
    /* zoom/move to... */
    centerx[1]= -0.7499225; centery[1]=0.0169325; distx[1]=1.056e-5; 
    /* hold, and then move to... **/
    centerx[2]= -0.7499389; centery[2]=0.0169435; distx[2]=1.056e-5; 
    /*  hold, and then move to... */
    centerx[3]= -0.74996540; centery[3]=0.0169564; distx[3]=1.056e-5; 
    /*  hold, and then zoom in to ... */
    centerx[4]= -0.74996540; centery[4]=0.0169564; distx[4]=1.056e-6; 

    NumPoints=5;
    NumFrames=10;

#define DEBUG

    /***********************************/

#ifdef DEBUG
    strcat(FileBase,"575-"); nx=575; maxiter=65535;
#else
    strcat(FileBase,"6000-"); nx=6000; maxiter=65535;
#endif

/*  should be 16:10 HDTV at 1080 */
/*  CHANGE TO ASPECT16TO9       */

/*    nx=1920; ny=1080; maxiter=7000;*/


    /*  determine y values based on set aspect ratio */

#ifdef ASPECT35
    aspectx=3.0; aspecty=2.0;      /*  35mm  */
    printf("\n\nAspect Ratio:  35mm\n");
    strcat(FileBase,"35mm");
#endif
#ifdef ASPECTVGA
    aspectx=640.0; aspecty=480.0;  /*  VGA and iBook  */
    printf("\n\nAspect Ratio:  VGA\n");
    strcat(FileBase,"VGA");
#endif
#ifdef ASPECT16TO9
    aspectx=16.0; aspecty=9.0;     /*  16:9  */
    printf("\n\nAspect Ratio:  16:9\n");
    strcat(FileBase,"16x9");
#endif
#ifdef ASPECTSQUARE
    aspectx=1.0; aspecty=1.0;      /*  square  */
    printf("\n\nAspect Ratio:  Square\n");
    strcat(FileBase,"SQ");
#endif

    ny=(int)( (double)nx*aspecty/aspectx );

    for (ix=0; ix<NumPoints; ix++)
      disty[ix]=distx[ix] * aspecty / aspectx;

    /********************************************/
    /* start outer loop on Points for animation */
    /********************************************/

    FrameNum=0;

    for (Point = 0; Point < NumPoints; Point++) {

      FrameNum++;

      xmin=centerx[Point] - distx[Point]; 
      xmax=centerx[Point] + distx[Point];

      if(xmax==xmin) 
	exit(-1);

      ymin=centery[Point] - disty[Point]; 
      ymax=centery[Point] + disty[Point];

      lengthx = (double)sqrt((xmin*xmin)+(xmax*xmax));
      lengthy = (double)sqrt((ymin*ymin)+(ymax*ymax));

      incx=(xmax-xmin)/((double)nx-1.0); 
      incy=(ymax-ymin)/((double)ny-1.0);

#ifdef DEBUG
      printf("Center (%e,%e)\n",centerx[Point],centery[Point]);
      printf("Distance (%e,%e)\n",distx[Point],disty[Point]);
      printf("(%e,%e) (%e,%e) %e ",xmin,xmax,ymin,ymax,(xmax-xmin));
      printf("Incs:  (%e,%e)\n",incx,incy);
#endif

      /* initialize the color map */

      ColorMap();

      /* zero the color histogram */

      for (ix=0; ix<MAX_COLORS ;ix++)
	ColorHist[ix]=0;

      /* initialize the GNU plotter for meta data output and size */

      OpenPlotter(nx, ny);

      /* do the fractal and plot the points */

      printf("\nFractaling %d x %d:\n  ",nx,ny);
      fflush((FILE *)stdout);

      for (iy=0;iy<ny-1;iy++) {

	if ( iy%100 == 0 ) { printf("%d ",iy); fflush((FILE *)stdout); }
	cy=ymin+iy*incy;

	for (ix=0;ix<nx-1;ix++) {

	  cx=xmin+ix*incx;

	  ColorIndex=(int)MSetLevel(cx,cy,maxiter);
	  if ( ColorIndex > MaxIterUsed ) MaxIterUsed = ColorIndex;

	  /* build the color histogram */

	  ColorHist[ColorIndex]++;

	  /* set color */

	  pl_pencolor((int)ColorRed[ColorIndex],(int)ColorGreen[ColorIndex],
		      (int)ColorBlue[ColorIndex]);
	  pl_fillcolor((int)ColorRed[ColorIndex],(int)ColorGreen[ColorIndex],
		       (int)ColorBlue[ColorIndex]);

	  /* plot point */

	  pl_point(ix,iy);

	} /* for ix */
      } /* for iy */

      printf("\n");

      pl_flushpl();  /* flush outstanding plot commands */
      pl_closepl();  /* close the plotter */

      printf("\nlength of x %f, length of y %f",lengthx,lengthy);
      printf("\ncenter point is (%f,%f)",centerx[Point],centery[Point]);
      printf("\ndist is (%f,%f)",distx[Point],disty[Point]);
      printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
      printf("\nnx %d, ny %d, dwell %d MaxIterUsed %d\n",
	     nx,ny,maxiter,MaxIterUsed);

      ClosePlotter();

      /* write the color histogram */

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


      printf("Done - %s.\n",FileBase);

    }  /* outer loop on Points */

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
    while ((iter<maxiter) & (x2+y2<250000.0)) {
	temp = x2 - y2 + cx; y = 2.0 * x * y + cy;
	x = temp; x2 = x * x; y2 = y * y; iter++;
    } /* while */
    if (iter==maxiter) {
      return(maxiter);
    } else {
      return(iter);
    }
}

void ColorMap()
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

  printf("\n\nCOLORMAP CHOICES:");
  printf("\n\n  0)  Default Colormap");
  printf("\n  1)  Greyscale, Black Background");
  printf("\n  2)  Greyscale, White Background");
  printf("\n  3)  Specify\n");
  printf("\nChoose a Colormap:  ");
  /*  scanf("%d",&ColorChoice);*/

  ColorChoice = 4;

  if (ColorChoice) {

    /******************************************************************/
    /*  black to white                                                */
    /******************************************************************/

    if (ColorChoice==1) {

      redinc=65535.0/(float)maxiter;
      greeninc=65535.0/(float)maxiter;
      blueinc=65535.0/(float)maxiter;

      printf("redinc %f, greeninc %f, blueinc %f\n",redinc,greeninc,blueinc);

      for (i=0,redstart=0.0,greenstart=0.0,bluestart=0.0;
	   i<=maxiter;
	   i++) {
	ColorRed[i]=redstart+(float)i*redinc;
	ColorGreen[i]=greenstart+(float)i*greeninc;
	ColorBlue[i]=bluestart+(float)i*blueinc;
      }
    }  /* ColorChoice 1 */

    /******************************************************************/
    /*  white to black                                                */
    /******************************************************************/

    if (ColorChoice==2) {

      redinc=65535.0/(float)maxiter;
      greeninc=65535.0/(float)maxiter;
      blueinc=65535.0/(float)maxiter;

      printf("redinc %f, greeninc %f, blueinc %f\n",redinc,greeninc,blueinc);

      for (i=0,redstart=1.0,greenstart=1.0,bluestart=1.0;
	   i<=maxiter;
	   i++) {
	ColorRed[i]=redstart-(float)i*redinc;
	ColorGreen[i]=greenstart-(float)i*greeninc;
	ColorBlue[i]=bluestart-(float)i*blueinc;
      }
    }  /* ColorChoice 2 */
    /*****************************************************************************/

    if (ColorChoice==3) {

      printf("\nStarting point:  Red Green Blue\n");
      scanf("%f %f %f",&RedStart,&GreenStart,&BlueStart);
      printf("\nEnding point:  Red Green Blue\n");
      scanf("%f %f %f",&RedEnd,&GreenEnd,&BlueEnd);

      ColorRedInc=(RedEnd-RedStart)/(float)maxiter;
      ColorGreenInc=(GreenEnd-GreenStart)/(float)maxiter;
      ColorBlueInc=(BlueEnd-BlueStart)/(float)maxiter;

      printf("\ninc's:  %f, %f, %f\n",ColorRedInc,ColorGreenInc,ColorBlueInc);

      for (i=0,RedValue=RedStart,GreenValue=GreenStart,BlueValue=BlueStart;
	   i<=maxiter;
	   i++,
	   RedValue+=ColorRedInc,GreenValue+=ColorGreenInc,
	   BlueValue+=ColorBlueInc) {
	   ColorRed[i]=RedValue;
	   ColorGreen[i]=GreenValue;
	   ColorBlue[i]=BlueValue;
      }
    }

    /* ColorChoice 3 */

    if (ColorChoice == 4) {

      /*  4 way colormap */

#define COLORMAP02

#ifdef COLORMAP01   /* primaries ending in purple */
      strcat(FileBase,"CMap01-");
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
      strcat(FileBase,"CMap02-");
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
      strcat(FileBase,"CMap03-");
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
      strcat(FileBase,"CMap04-");
      RedStart1=65535.0; GreenStart1=65535.0; BlueStart1=65535.0;
      RedEnd1=65535.0; GreenEnd1=0.0; BlueEnd1=0.0;

      RedStart2=65535.0; GreenStart2=0.0; BlueStart2=0.0;
      RedEnd2=0.0; GreenEnd2=65535.0; BlueEnd2=0.0;

      RedStart3=0.0; GreenStart3=65535.0; BlueStart3=0.0;
      RedEnd3=0.0; GreenEnd2=0.0; BlueEnd3=65535.0;

      RedStart4=0.0; GreenStart4=0.0; BlueStart4=65535.0;
      RedEnd4=65535.0; GreenEnd4=65535.0; BlueEnd4=65535.0;
#endif

#ifdef COLORMAP05  /* primaries ending in black */
      strcat(FileBase,"CMap05-");
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
      strcat(FileBase,"CMap06-");
      RedStart1=28013.0; GreenStart1=3084.0; BlueStart1=27242.0;
      RedEnd1=21845.0; GreenEnd1=3084.0; BlueEnd1=27242.0;

      RedStart2=21845.0; GreenStart2=3084.0; BlueStart2=27242.0;
      RedEnd2=13621.0; GreenEnd2=3084.0; BlueEnd2=27242.0;

      RedStart3=13621.0; GreenStart3=3084.0; BlueStart3=27242.0;
      RedEnd3=3084.0; GreenEnd3=3084.0; BlueEnd3=27242.0;

      RedStart4=3084.0; GreenStart4=3084.0; BlueStart4=27242.0;
      RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

      ColorRedInc1 = (RedEnd1-RedStart1)/((float)maxiter/4.0+1.0);
      ColorGreenInc1 = (GreenEnd1-GreenStart1)/((float)maxiter/4.0+1.0);
      ColorBlueInc1 = (BlueEnd1-BlueStart1)/((float)maxiter/4.0+1.0);

      ColorRedInc2 = (RedEnd2-RedStart2)/((float)maxiter/4.0+1.0);
      ColorGreenInc2 = (GreenEnd2-GreenStart2)/((float)maxiter/4.0+1.0);
      ColorBlueInc2 = (BlueEnd2-BlueStart2)/((float)maxiter/4.0+1.0);

      ColorRedInc3 = (RedEnd3-RedStart3)/((float)maxiter/4.0+1.0);
      ColorGreenInc3 = (GreenEnd3-GreenStart3)/((float)maxiter/4.0+1.0);
      ColorBlueInc3 = (BlueEnd3-BlueStart3)/((float)maxiter/4.0+1.0);

      ColorRedInc4 = (RedEnd4-RedStart4)/((float)maxiter/4.0+1.0);
      ColorGreenInc4 = (GreenEnd4-GreenStart4)/((float)maxiter/4.0+1.0);
      ColorBlueInc4 = (BlueEnd4-BlueStart4)/((float)maxiter/4.0+1.0);

      printf("\n\n    Colormaps:\n");
      printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	     BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
      printf("    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	     BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);
      printf("    %f %f %f --> %f  %f %f\n",RedStart3, GreenStart3,
	     BlueStart3, RedEnd3, GreenEnd3, BlueEnd3);
      printf("    %f %f %f --> %f  %f %f\n",RedStart4, GreenStart4,
	     BlueStart4, RedEnd4, GreenEnd4, BlueEnd4);

      for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
	   i<=(int)((float)maxiter/4.0);
	   i++,
	     RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	     BlueValue+=ColorBlueInc1) {

	ColorRed[i]=RedValue;
	ColorGreen[i]=GreenValue;
	ColorBlue[i]=BlueValue;

      }  /* assign first quarter colors */

      printf("    Out of first fourth:  (%d) %f, %f, %f\n",
	     i, RedValue, GreenValue, BlueValue);

      for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
	   i<=(int)((float)maxiter*2.0/4.0);
	   i++,
	     RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	     BlueValue+=ColorBlueInc2) {

	ColorRed[i]=RedValue;
	ColorGreen[i]=GreenValue;
	ColorBlue[i]=BlueValue;

      }  /* assign second quarter colors */

      printf("    Out of second fourth:  (%d) %f, %f, %f\n",
	     i, RedValue, GreenValue, BlueValue);

      for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
	   i<=(int)((float)maxiter*3.0/4.0);
	   i++,
	     RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	     BlueValue+=ColorBlueInc3) {

	ColorRed[i]=RedValue;
	ColorGreen[i]=GreenValue;
	ColorBlue[i]=BlueValue;

      }  /* assign third quarter colors */

      printf("    Out of third fourth:  (%d) %f, %f, %f\n",
	     i, RedValue, GreenValue, BlueValue);

      for (RedValue=RedStart4,GreenValue=GreenStart4,BlueValue=BlueStart4;
	   i<=maxiter;
	   i++,
	     RedValue+=ColorRedInc4,GreenValue+=ColorGreenInc4,
	     BlueValue+=ColorBlueInc4) {

	ColorRed[i]=RedValue;
	ColorGreen[i]=GreenValue;
	ColorBlue[i]=BlueValue;

      }  /* assign fourth quarter colors */

      printf("    Out of fourth fourth:  (%d) %f, %f, %f\n",
	     i, RedValue, GreenValue, BlueValue);


    }

    /* ColorChoice 4 */

  }  /*if ColorChoice*/

#ifdef DEBUG
  File=fopen("DefinedColors.txt","w");
  for (i=0;i<=maxiter;i++)
    fprintf(File,
	    "Color %d:  <%f, %f, %f>\n",
	    i,ColorRed[i],ColorGreen[i],ColorBlue[i]);
  fclose(File);
#endif

#undef DEBUG

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
