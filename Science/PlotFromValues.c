/*

  program to read a values file and produce a plot file

*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <plot.h>  /*  GNU libplot */

/*  GNU Plotter stuff */

int Plotter;
char PlotFileName[256];         /*  name of the plot file  */
char ErrorFileName[256];        /*  name of the error file  */
FILE *PlotFile, *ErrorFile, *File;
int OpenPlotter(int nx, int ny);
void ClosePlotter();


#undef DEBUG
#define DEBUG

#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

/**********************************************************/
/*  program specific defines  */
/**********************************************************/

/*#define MAX_COLORS 65535*/        /* 16 bit color */
#define MAX_COLORS 16777216     /* 24 bit color */
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

#define MAXRESOLUTIONX 6001
#define MAXRESOLUTIONY 6001

/**********************************************************/
/*  global variables  */
/**********************************************************/

int Fractal[MAXRESOLUTIONX][MAXRESOLUTIONY];
int ColorHist[MAX_COLORS];

float ColorRed[MAX_COLORS];
float ColorGreen[MAX_COLORS];
float ColorBlue[MAX_COLORS];
float RealColorRed[MAX_COLORS];
float RealColorGreen[MAX_COLORS];
float RealColorBlue[MAX_COLORS];

void ColorMap(int NumColorsUsed);

main(int argc, char *argv[])
{

  int nx, ny;                     /*  counters mostly  */
  int resX, resY;                 /*  resolution of the fractal  */
  int minZ = MAX_COLORS,          /*  min/max value in fractal  */
    maxZ = 0;
  int medianZ;                    /*  most often occuring Z value  */
  int NumColorsUsed;              /*  number of distinct Z values  */

  int Num;                        /*  general use integer  */
  int WaterLine;                  /*  Z value of the waterline  */
  int RetVal;                     /*  return value  */

  FILE *InputFile;                /*  values file to read  */
  /*  FILE *PlotFile;*/                 /*  GNU plot file written  */


  /*  basic sanity checking  */

  if ( argc < 2 ) {
    printf("Give me a values file, bonehead\n");
    exit(EXIT_FAILURE);
  }

  /*  create the plot file name  */

  nx=0;
  while ( argv[1][nx] != '.' ) {
    PlotFileName[nx] = argv[1][nx];
    ErrorFileName[nx] = argv[1][nx];
    nx++;
  }
  PlotFileName[nx] = '\0';

  strcat(PlotFileName,".plot");
  strcat(ErrorFileName,".error");

#ifdef DEBUG
  printf("PlotFileName:  %s\n",PlotFileName);
  printf("ErrorFileName:  %s\n",ErrorFileName);
#endif

  /*  open and read the values file  */

  printf("Opening file %s...\n",argv[1]);

  InputFile=fopen(argv[1],"r");
#ifdef DEBUG
  printf("  -->File opened.\n");
#endif
  fscanf(InputFile,"%d %d", &resX, &resY);
  printf("About to read %d by %d values...\n",resX, resY);
  for (nx=0; nx<resX; nx++)
    for (ny=0; ny<resY; ny++)
      fscanf(InputFile,"%d", &Fractal[nx][ny]);
  fclose(InputFile);

  /**********************************************************/
  /*  poke around the fractal values a bit  */
  /**********************************************************/

  /* build the color histogram */

  printf("Building Z value histogram...\n");

  for (nx=0; nx<MAX_COLORS; nx++)
    ColorHist[nx]=0;

  for (nx=0; nx<resX; nx++)
    for (ny=0; ny<resY; ny++) {
      ColorHist[Fractal[nx][ny]]++;
      if ( Fractal[nx][ny] > maxZ )  maxZ = Fractal[nx][ny];
      if ( Fractal[nx][ny] < minZ )  minZ = Fractal[nx][ny];
    }

  NumColorsUsed=0;
  for (nx=0; nx<MAX_COLORS; nx++)
    if ( ColorHist[nx] ) NumColorsUsed++;

  printf("  Number of distinct Z's:  %d\n",NumColorsUsed);
  printf("  Min Z:                   %d",minZ);
  nx = minZ + 1;
  while ( nx < MAX_COLORS ) {
    if ( ColorHist[nx] != 0 ) {
      printf(" jump to %d\n",nx);
      nx = MAX_COLORS + 1;
    }  /*  found first non-zero Z value  */
    nx++;
  }  /*  while looking for next used Z value  */
  printf("  Max Z:                   %d\n",maxZ);

  /*  median Z value is potential default waterline  */

  Num = -1;
  for ( nx = 0 ; nx < MAX_COLORS ; nx++ )
    if ( ColorHist[nx] > Num )  Num = nx;
  medianZ = Num;
  WaterLine = medianZ;

  printf("  Median Z:                %d with %d values\n",
	 medianZ, ColorHist[medianZ]);

  /**********************************************************/
  /*                                                        */
  /*  set the colormap  */
  /*                                                        */
  /**********************************************************/

  ColorMap(NumColorsUsed);

  /*  Map the colors we just defined to the actual colors used...  */

  int RealIndex;
  int ix,iy;
  RealIndex=0;
  for (ix=0; ix<MAX_COLORS; ix++)
    if ( ColorHist[ix] ) {
      RealColorRed[ix]=ColorRed[RealIndex];
      RealColorGreen[ix]=ColorGreen[RealIndex];
      RealColorBlue[ix]=ColorBlue[RealIndex];
      /*      printf("Mapping %d to %d\n",ix,RealIndex);*/
      RealIndex++;
    }

#ifdef CMDEBUG
  printf("RealIndex out of loop is %d\n",RealIndex);
#endif

  int MinUsed, MaxUsed;
  MinUsed=MAX_COLORS; MaxUsed=0;
  for (ix=0; ix<resX; ix++)
    for (iy=0; iy<resY; iy++) {
      if ( Fractal[ix][iy] > MaxUsed) MaxUsed=Fractal[ix][iy];
      if ( Fractal[ix][iy] < MinUsed) MinUsed=Fractal[ix][iy];
    }

  printf("\nMin ColorIndex Actual:  %d\n",MinUsed);
  printf("Max ColorIndex Actual:  %d\n",MaxUsed);
  printf("RealColor[EndIndex]:  (%f, %f, %f) - %d\n", RealColorRed[MaxUsed],
	 RealColorGreen[MaxUsed], RealColorBlue[MaxUsed], MaxUsed);
  printf("Color[NumColorsUsed]:  (%f, %f, %f) - %d\n", ColorRed[RealIndex-1],
	 ColorGreen[RealIndex-1], ColorBlue[RealIndex-1], (RealIndex-1));


  /**********************************************************/
  /*                                                        */
  /*  start the plot input file  */
  /*                                                        */
  /**********************************************************/

  printf("Starting plot file (%s)...\n", PlotFileName);

  /*
  RetVal=(int)(PlotFile=fopen(PlotFileName,"w"));
  if (RetVal == -1 ) {
    printf("Couldn't open plot file - exiting.\n");
    exit(RetVal);
  }
  */

  /*  dougee */

  OpenPlotter(resX, resY);

  for (iy=0; iy<resY; iy++)
    for (ix=0; ix<resX; ix++) {

      /*      printf("  %d %d --> %d\n",ix, iy, Fractal[ix][iy]);*/

      /*
      printf("  (%d,%d) --> %d %d %d\n",ix,iy,
	     (int)RealColorRed[Fractal[ix][iy]],
	     (int)RealColorGreen[Fractal[ix][iy]],
	     (int)RealColorBlue[Fractal[ix][iy]]);
      */

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

#define COLORMAP06
#define PRINTCOLORMAPS

#ifdef COLORMAP01   /* primaries ending in purple */
#define QUADCOLOR
  /*  strcat(FileBase,"CMap01");*/
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
  /*  strcat(FileBase,"CMap02");*/
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
  /*  strcat(FileBase,"CMap03");*/
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
  /*  strcat(FileBase,"CMap04");*/
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
  /*  strcat(FileBase,"CMap05");*/
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
  /*  strcat(FileBase,"CMap06");*/
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
  /*  strcat(FileBase,"CMap07");*/
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
  /*  strcat(FileBase,"CMap08");*/
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
  /*  strcat(FileBase,"CMap09");*/
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
  /*  strcat(FileBase,"CMap10");*/
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

  /*  strcat(FileBase,"CMap11");*/
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
  printf("\n\n  Colormaps:\n");
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

#ifdef CMDEBUG
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

#ifdef CMDEBUG
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

#ifdef CMDEBUG
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

#ifdef CMDEBUG
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
  printf("\n\n  Colormaps:\n");
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

#ifdef CMDEBUG
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


#ifdef CMDEBUG
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
  char FileBase[75];


  if ( (int)(PlotFile=fopen(PlotFileName,"w")) == -1 ) { 
    printf("Could not open %s - exiting.\n",PlotFileName);
    exit(-1);
  }

  if ( (int)(ErrorFile=fopen(ErrorFileName,"w")) == -1 ) {
    printf("Could not open %s - exiting.\n",ErrorFileName);
    fclose(PlotFile);
    exit(-1);
  }

  sprintf(bitmapsize,"%dx%d\0",nx,ny);

  pl_parampl("BG_COLOR", "white");
  pl_parampl("META_PORTABLE", "yes");
  pl_parampl("BITMAPSIZE", bitmapsize);

  printf("  bitmap:  %s\n",bitmapsize);
  
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
