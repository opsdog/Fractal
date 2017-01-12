/*

  program to read a values file and produce a plot file

*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <png.h>  /*  libpng stuff */


/*
#undef DEBUG
#define DEBUG
*/

/*
#undef CMDEBUG
#define CMDEBUG
*/

#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

/**********************************************************/
/*                                                        */
/*            program specific defines                    */
/*                                                        */
/**********************************************************/

/*#define MAX_COLORS 65534*/        /* 16 bit color */
/*#define MAX_COLORS 16777216*/     /* 24 bit color */
#define MAX_COLORS 60000001
/*#define MAX_COLORS 4294967296*/   /* 32 bit color */

/*
#define MAXRESOLUTIONX 6001
#define MAXRESOLUTIONY 6001
*/
#define MAXRESOLUTIONX 10001
#define MAXRESOLUTIONY 10001

/**********************************************************/
/*                                                        */
/*                 global variables                       */
/*                                                        */
/**********************************************************/

int Fractal[MAXRESOLUTIONX][MAXRESOLUTIONY];
int ColorHist[MAX_COLORS];

float ColorRed[MAX_COLORS];
float ColorGreen[MAX_COLORS];
float ColorBlue[MAX_COLORS];
float RealColorRed[MAX_COLORS];
float RealColorGreen[MAX_COLORS];
float RealColorBlue[MAX_COLORS];

FILE *StatsFile;                /*  stats for the conversion  */

char PNGFileName[256];

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
  FILE *PNGFile;                  /*  GNU plot file written  */

  char ErrorFileName[256];
  char StatsFileName[256];

  /*  libpng stuff  */

  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  size_t x, y;
  png_byte ** row_pointers = NULL;
  /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
  int status = -1;
  /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
  */
  int pixel_size = 6;
  int depth = 16;


  /*  basic sanity checking  */

  if ( argc < 2 ) {
    printf("Give me a values file, bonehead\n");
    exit(EXIT_FAILURE);
  }

  /*  create the png file name  */

  nx=0;
  while ( argv[1][nx] != '.' ) {
    PNGFileName[nx] = argv[1][nx];
    ErrorFileName[nx] = argv[1][nx];
    StatsFileName[nx] = argv[1][nx];
    nx++;
  }
  PNGFileName[nx] = '\0';
  ErrorFileName[nx] = '\0';
  StatsFileName[nx] = '\0';
  strcat(StatsFileName,".stats");

  /*  open the stats file for writing  */

  printf("Opening file %s...\n",StatsFileName);

  StatsFile=fopen(StatsFileName,"w");
#ifdef DEBUG
  printf("  -->File opened.\n");
#endif

  fprintf(StatsFile,"%s\n",argv[1]);

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
  /*                                                        */
  /*        poke around the fractal values a bit            */
  /*                                                        */
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
  fprintf(StatsFile,"  Number of distinct Z's:  %d\n",NumColorsUsed);
  printf("  Min Z:                   %d",minZ);
  fprintf(StatsFile,"  Min Z:                   %d",minZ);
  nx = minZ + 1;
  while ( nx < MAX_COLORS ) {
    if ( ColorHist[nx] != 0 ) {
      printf(" jump to %d\n",nx);
      fprintf(StatsFile," jump to %d\n",nx);
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
  fprintf(StatsFile,"  Median Z:                %d with %d values\n",
	 medianZ, ColorHist[medianZ]);

  /**********************************************************/
  /*                                                        */
  /*                   set the colormap                     */
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

  fprintf(StatsFile,"\nMin ColorIndex Actual:  %d\n",MinUsed);
  fprintf(StatsFile,"Max ColorIndex Actual:  %d\n",MaxUsed);
  fprintf(StatsFile,"RealColor[EndIndex]:  (%f, %f, %f) - %d\n", RealColorRed[MaxUsed],
	 RealColorGreen[MaxUsed], RealColorBlue[MaxUsed], MaxUsed);
  fprintf(StatsFile,"Color[NumColorsUsed]:  (%f, %f, %f) - %d\n", ColorRed[RealIndex-1],
	 ColorGreen[RealIndex-1], ColorBlue[RealIndex-1], (RealIndex-1));

  /**********************************************************/
  /*                                                        */
  /*               write the stats file                     */
  /*                                                        */
  /**********************************************************/

  fclose(StatsFile);

  /**********************************************************/
  /*                                                        */
  /*               start the png file                       */
  /*                                                        */
  /**********************************************************/

  strcat(PNGFileName,".png");
  strcat(ErrorFileName,".error");

#ifdef DEBUG
  printf("PNGFileName:  %s\n",PNGFileName);
  printf("ErrorFileName:  %s\n",ErrorFileName);
#endif

  printf("Starting PNG file (%s)...\n", PNGFileName);

  png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
    goto png_create_write_struct_failed;
  }
    
  info_ptr = png_create_info_struct (png_ptr);
  if (info_ptr == NULL) {
    goto png_create_info_struct_failed;
  }
    
  /* Set up error handling. */

  if (setjmp (png_jmpbuf (png_ptr))) {
    goto png_failure;
  }

  /* Set image attributes. */

  png_set_IHDR (png_ptr,
		info_ptr,
		resX,
		resY,
		depth,

		/*PNG_COLOR_TYPE_RGB,*/
		/*PNG_COLOR_MASK_COLOR,*/
		2,

		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

  png_color_8 sig_bit;
  sig_bit.red=depth;
  sig_bit.blue=depth;
  sig_bit.green=depth;

  png_set_sBIT(png_ptr, info_ptr, &sig_bit);

  if (depth > 8)
    png_set_swap(png_ptr);

  png_set_shift(png_ptr, &sig_bit);

  png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

  /* Initialize rows of PNG. */


  PNGFile=fopen(PNGFileName,"wb");
  if (!PNGFile) {
    printf("Couldn't open PNG file - exiting.\n");
    /*    exit((int)PNGFile);*/
    return(-1);
  }

  /*  dougee */

  short int nukeme;
  short int nukeme2;
  char nukeme3[6];
  char nukeme4[6];
  char rowtriple[7];
  
  row_pointers = png_malloc (png_ptr, resY * sizeof (png_bytep));

  for (iy=0; iy<resY; iy++) {
    /*    png_byte *row = png_malloc (png_ptr, depth * resX * pixel_size);*/
    png_byte *row = png_malloc (png_ptr, depth * resX);
    row_pointers[iy] = row;
    for (ix=0; ix<resX; ix++) {

      /*
      printf("  %d %d --> %d\n",ix, iy, Fractal[ix][iy]);
      printf("  (%d,%d) --> %d %d %d\n",ix,iy,
	     (int)RealColorRed[Fractal[ix][iy]],
	     (int)RealColorGreen[Fractal[ix][iy]],
	     (int)RealColorBlue[Fractal[ix][iy]]);
      */


      /*  works  
      nukeme=(int)RealColorRed[Fractal[ix][iy]];
      memcpy(&rowtriple[1], &nukeme, 1);
      memcpy(&rowtriple[0], &nukeme + 1, 1);
      nukeme=(int)RealColorGreen[Fractal[ix][iy]];
      memcpy(&rowtriple[3], &nukeme, 1);
      memcpy(&rowtriple[2], &nukeme + 1, 1);
      nukeme=(int)RealColorBlue[Fractal[ix][iy]];
      memcpy(&rowtriple[5], &nukeme, 1);
      memcpy(&rowtriple[4], &nukeme + 1, 1);
      rowtriple[6] = '\0';
      */

      nukeme=(int)RealColorRed[Fractal[ix][iy]];
      memcpy(&rowtriple[0], &nukeme, 1);
      nukeme2 = nukeme >> 8;
      memcpy(&rowtriple[1], &nukeme2, 1);
#ifdef CMASSIGN
      printf("  Red - (%d,%d) %d (%X) should = %ld\n", iy,ix,
	     (int)RealColorRed[Fractal[ix][iy]],
	     nukeme,
	     *row);
      printf("    %x %x %x %x %x %x\n",rowtriple[0], rowtriple[1], 
	     rowtriple[2], rowtriple[3], rowtriple[4], rowtriple[5]);
#endif
      nukeme=(int)RealColorGreen[Fractal[ix][iy]];
      memcpy(&rowtriple[2], &nukeme, 1);
      nukeme2 = nukeme >> 8;
      memcpy(&rowtriple[3], &nukeme2, 1);
#ifdef CMASSIGN
      printf("  Green - (%d,%d) %d (%X) should = %ld\n", iy,ix,
	     (int)RealColorGreen[Fractal[ix][iy]],
	     nukeme,
	     *row);
      printf("    %x %x %x %x %x %x\n",rowtriple[0], rowtriple[1], 
	     rowtriple[2], rowtriple[3], rowtriple[4], rowtriple[5]);
#endif
      nukeme=(int)RealColorBlue[Fractal[ix][iy]];
      memcpy(&rowtriple[4], &nukeme, 1);
      nukeme2 = nukeme >> 8;
      memcpy(&rowtriple[5], &nukeme2, 1);
      rowtriple[6] = '\0';
#ifdef CMASSIGN
      printf("  Blue - (%d,%d) %d (%X) should = %ld\n", iy,ix,
	     (int)RealColorBlue[Fractal[ix][iy]],
	     nukeme,
	     *row);
      printf("    %x %x %x %x %x %x\n",rowtriple[0], rowtriple[1], 
	     rowtriple[2], rowtriple[3], rowtriple[4], rowtriple[5]);
#endif
      memcpy(&*row, &rowtriple, 6);
      row+=6;


    }  /*  build png rows - x loop */

    /*  right side edge  ??
    for (y=0;y<6;y++)
      rowtriple[y]=0;
    rowtriple[6]='\0';
    memcpy(&*row, &rowtriple, 6);
    */

  }  /*  build png rows - y loop */

  /* Write the image data to "fp". */

  png_init_io (png_ptr, PNGFile);
  png_set_rows (png_ptr, info_ptr, row_pointers);

  if (depth > 8)
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_SHIFT | PNG_TRANSFORM_SWAP_ENDIAN, NULL);
  else 
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_SHIFT, NULL);

  png_write_end(png_ptr, info_ptr);

  /* The routine has successfully written the file, so we set
     "status" to a value which indicates success. */

  status = 0;
    
  for (y = 0; y < resY; y++) {
    png_free (png_ptr, row_pointers[y]);
  }
  png_free (png_ptr, row_pointers);

 png_failure:
 png_create_info_struct_failed:
  png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
  fclose (PNGFile);
 fopen_failed:
  return status;

}


void ColorMap(NumColorsUsed)
int NumColorsUsed;
{
  int i,j;
  /*  int ColorChoice;*/
  /*  double colorinc,redstart,greenstart,bluestart;*/
  /*  double redinc,greeninc,blueinc;*/
  /*  double RedStart, RedEnd, GreenStart, GreenEnd, BlueStart, BlueEnd;*/
  /*  double ColorRedInc, ColorGreenInc, ColorBlueInc;*/
  double RedValue, GreenValue, BlueValue;
  double RedStart1, RedStart2, RedStart3, RedEnd1, RedEnd2, RedEnd3;
  double RedStart4, RedEnd4;
  double GreenStart1, GreenStart2, GreenStart3, GreenEnd1, GreenEnd2, GreenEnd3;
  double GreenStart4, GreenEnd4;
  double BlueStart1, BlueStart2, BlueStart3, BlueEnd1, BlueEnd2, BlueEnd3;
  double BlueStart4, BlueEnd4;
  double ColorRedInc1, ColorRedInc2, ColorRedInc3, ColorRedInc4;
  double ColorGreenInc1, ColorGreenInc2, ColorGreenInc3, ColorGreenInc4;
  double ColorBlueInc1, ColorBlueInc2, ColorBlueInc3, ColorBlueInc4;
#ifdef CMDEBUG
  FILE *File;
#endif

  /*  4 way colormap */

  /*#define COLORMAP04*/
  /*#define COLORMAP06*/
  /*#define COLORMAP16*/
  /*#define COLORMAP99*/

#define COLORMAP48


#define PRINTCOLORMAPS

#ifdef COLORMAP01   /* primaries ending in purple */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap01");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=65535.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=0.0; GreenEnd2=65534.0; BlueEnd2=0.0;

  RedStart3=0.0; GreenStart3=65534.0; BlueStart3=0.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=65534.0;

  RedStart4=0.0; GreenStart4=0.0; BlueStart4=65534.0;
  RedEnd4=40000.0; GreenEnd4=500.0; BlueEnd4=60000.0;
#endif

#ifdef COLORMAP02  /* red to orange to black */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap02");
  RedStart1=65534.0; GreenStart1=18504.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=33924.0; BlueEnd1=0.0;

  RedStart2=65534.0; GreenStart2=33924.0; BlueStart2=0.0;
  RedEnd2=65534.0; GreenEnd2=52428.0; BlueEnd2=0.0;

  RedStart3=65534.0; GreenStart3=52428.0; BlueStart3=0.0;
  RedEnd3=65534.0; GreenEnd3=61680.0; BlueEnd3=0.0;

  RedStart4=65534.0; GreenStart4=61680.0; BlueStart4=0.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP03  /* red to purple to black */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap03");
  RedStart1=33667.0; GreenStart1=19018.0; BlueStart1=20303.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=6168.0;

  RedStart2=65534.0; GreenStart2=0.0; BlueStart2=6168.0;
  RedEnd2=65534.0; GreenEnd2=0.0; BlueEnd2=24672.0;

  RedStart3=65534.0; GreenStart3=0.0; BlueStart3=24672.0;
  RedEnd3=45489.0; GreenEnd3=16191.0; BlueEnd3=39064.0;

  RedStart4=45489.0; GreenStart4=16191.0; BlueStart4=39064.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP04  /* white to red to red to magenta to bluish */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP04\n");
#endif

  strcat(PNGFileName,"-CMap04");
  RedStart1=65534.0; GreenStart1=65534.0; BlueStart1=65534.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=65534.0; GreenStart2=0.0; BlueStart2=0.0;
  /*  RedEnd2=0.0; GreenEnd2=65534.0; BlueEnd2=0.0;*/
  RedEnd2=24415.0; GreenEnd2=7967.0; BlueEnd2=1542.0;

  RedStart3=24415.0; GreenStart3=7967.0; BlueStart3=1542.0;
  /*  RedStart3=0.0; GreenStart3=65534.0; BlueStart3=0.0;*/
  /*  RedEnd3=0.0; GreenEnd2=0.0; BlueEnd3=65534.0;*/
  RedEnd3=20945.0; GreenEnd3=6810.0; BlueEnd3=11051.0;

  RedStart4=20945.0; GreenStart4=6810.0; BlueStart4=11051.0;
  /*  RedStart4=0.0; GreenStart4=0.0; BlueStart4=65534.0;*/
  /*  RedEnd4=65536.0; GreenEnd4=65534.0; BlueEnd4=65534.0;*/
  RedEnd4=17476.0; GreenEnd4=5654.0; BlueEnd4=20560.0;
#endif

#ifdef COLORMAP05  /* primaries ending in black */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap05");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=65534.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=0.0; GreenEnd2=65534.0; BlueEnd2=0.0;

  RedStart3=0.0; GreenStart3=65534.0; BlueStart3=0.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=65534.0;

  RedStart4=0.0; GreenStart4=0.0; BlueStart4=65534.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP06  /* purple to blue to black */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap06");
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
  strcat(PNGFileName,"-CMap07");
  RedStart1=28013.0; GreenStart1=3084.0; BlueStart1=27242.0;
  RedEnd1=21845.0; GreenEnd1=3084.0; BlueEnd1=27242.0;

  RedStart2=21845.0; GreenStart2=3084.0; BlueStart2=27242.0;
  RedEnd2=13621.0; GreenEnd2=3084.0; BlueEnd2=27242.0;

  RedStart3=13621.0; GreenStart3=3084.0; BlueStart3=27242.0;
  RedEnd3=65534.0; GreenEnd3=3084.0; BlueEnd3=27242.0;

  RedStart4=65534.0; GreenStart4=3084.0; BlueStart4=27242.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP08  /* purple to bright blue to black */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap08");
  RedStart1=28013.0; GreenStart1=3084.0; BlueStart1=27242.0;
  RedEnd1=21845.0; GreenEnd1=3084.0; BlueEnd1=27242.0;

  RedStart2=21845.0; GreenStart2=3084.0; BlueStart2=27242.0;
  RedEnd2=13621.0; GreenEnd2=3084.0; BlueEnd2=27242.0;

  RedStart3=13621.0; GreenStart3=3084.0; BlueStart3=27242.0;
  RedEnd3=3084.0; GreenEnd3=3084.0; BlueEnd3=65534.0;

  RedStart4=3084.0; GreenStart4=3084.0; BlueStart4=65534.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP09  /* purple to red to blue to black */
#define QUADCOLOR
  strcat(PNGFileName,"-CMap09");
  RedStart1=17476.0; GreenStart1=5654.0; BlueStart1=20560.0;
  RedEnd1=20560.0; GreenEnd1=5654.0; BlueEnd1=5654.0;

  RedStart2=20560.0; GreenStart2=5654.0; BlueStart2=5654.0;
  RedEnd2=65534.0; GreenEnd2=3084.0; BlueEnd2=7242.0;

  RedStart3=65534.0; GreenStart3=3084.0; BlueStart3=7242.0;
  RedEnd3=3084.0; GreenEnd3=3084.0; BlueEnd3=65534.0;

  RedStart4=3084.0; GreenStart4=3084.0; BlueStart4=65534.0;
  RedEnd4=0.0; GreenEnd4=0.0; BlueEnd4=0.0;
#endif

#ifdef COLORMAP10  /* purple to red to blue to black */
#define TRICOLOR
  strcat(PNGFileName,"-CMap10");
  RedStart1=17476.0; GreenStart1=5654.0; BlueStart1=20560.0;
  RedEnd1=65534.0; GreenEnd1=3084.0; BlueEnd1=7242.0;

  RedStart2=65534.0; GreenStart2=3084.0; BlueStart2=7242.0;
  RedEnd2=3084.0; GreenEnd2=3084.0; BlueEnd2=65534.0;

  RedStart3=3084.0; GreenStart3=3084.0; BlueStart3=65534.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=0.0;
#endif

#ifdef COLORMAP11  /* fire attempt 1 */
#define TRICOLOR

  /* burnt red - 24415, 7967, 1542 */
  /* orange - 64764, 29555, 15677 */
  /* red - 65535, 3084, 7242 */

  strcat(PNGFileName,"-CMap11");
  RedStart1=24415.0; GreenStart1=7967.0; BlueStart1=1542.0;
  RedEnd1=64764.0; GreenEnd1=29555.0; BlueEnd1=15677.0;

  RedStart2=64764.0; GreenStart2=29555.0; BlueStart2=15677.0;
  RedEnd2=65534.0; GreenEnd2=3084.0; BlueEnd2=7242.0;

  RedStart3=65534.0; GreenStart3=3084.0; BlueStart3=7242.0;
  RedEnd3=0.0; GreenEnd3=0.0; BlueEnd3=0.0;
#endif

#ifdef COLORMAP12  /* black to blue to magenta to red */
#define TRICOLOR

  strcat(PNGFileName,"-CMap12");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=3084.0; GreenEnd1=3084.0; BlueEnd1=65534.0;

  RedStart2=3084.0; GreenStart2=3084.0; BlueStart2=65534.0;
  RedEnd2=17476.0; GreenEnd2=5654.0; BlueEnd2=20560.0;

  RedStart3=17476.0; GreenStart3=5654.0; BlueStart3=20560.0;
  RedEnd3=65534.0; GreenEnd3=3084.0; BlueEnd3=7242.0;

#endif

#ifdef COLORMAP13  /* black to blue to blue to purple to red */
#define QUADCOLOR

  strcat(PNGFileName,"-CMap13");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=3084.0; GreenEnd1=3084.0; BlueEnd1=65534.0;

  RedStart2=3084.0; GreenStart2=3084.0; BlueStart2=65534.0;
  RedEnd2=8738.0; GreenEnd2=1542.0; BlueEnd2=32768.0;

  RedStart3=8738.0; GreenStart3=1542.0; BlueStart3=32768.0;
  RedEnd3=17476.0; GreenEnd3=5654.0; BlueEnd3=20560.0;

  RedStart4=17476.0; GreenStart4=5654.0; BlueStart4=20560.0;
  RedEnd4=65534.0; GreenEnd4=3084.0; BlueEnd4=7242.0;

#endif

#ifdef COLORMAP14  /* black to red to red to red */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP14\n");
#endif

  strcat(PNGFileName,"-CMap14");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=50000.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=50000.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=55000.0; GreenEnd2=0.0; BlueEnd2=0.0;

  RedStart3=55000.0; GreenStart3=0.0; BlueStart3=0.0;
  RedEnd3=60000.0; GreenEnd3=0.0; BlueEnd3=0.0;

  RedStart4=60000.0; GreenStart4=0.0; BlueStart4=0.0;
  RedEnd4=65534.0; GreenEnd4=0.0; BlueEnd4=0.0;

#endif

#ifdef COLORMAP15  /*  cmap04 reverse white to red  */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP15\n");
#endif

  strcat(PNGFileName,"-CMap15");
  /*  RedStart1=20945.0; GreenStart1=6810.0; BlueStart1=1542.0;*/
  RedStart1=20945.0; GreenStart1=600.0; BlueStart1=1542.0;
  /*  RedEnd1=24415.0; GreenEnd1=7867.0; BlueEnd1=1542.0;*/
  RedEnd1=24415.0; GreenEnd1=800.0; BlueEnd1=1542.0;

  RedStart2=24415.0; GreenStart2=800.0; BlueStart2=1542.0;
  /*  RedStart2=24415.0; GreenStart2=7867.0; BlueStart2=1542.0;*/
  RedEnd2=65534.0; GreenEnd2=0.0; BlueEnd2=0.0;

  RedStart3=65534.0; GreenStart3=0.0; BlueStart3=0.0;
  RedEnd3=65534.0; GreenEnd3=65534.0; BlueEnd3=65534.0;

  RedStart4=65534.0; GreenStart4=65534.0; BlueStart4=65534.0;
  RedEnd4=2000.0; GreenEnd4=500.0; BlueEnd4=10560.0;
  /*  RedEnd4=2000.0; GreenEnd4=500.0; BlueEnd4=20560.0;*/
  /*  RedEnd4=17476.0; GreenEnd4=5654.0; BlueEnd4=20560.0;*/

#endif

#ifdef COLORMAP16
#define TRICOLOR

  strcat(PNGFileName,"-CMap16");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=3084.0; BlueEnd1=7242.0;

  RedStart2=65534.0; GreenStart2=3084.0; BlueStart2=7242.0;
  RedEnd2=24415.0; GreenEnd2=7967.0; BlueEnd2=1542.0;

  RedStart3=24415.0; GreenStart3=7967.0; BlueStart3=1542.0;
  RedEnd3=64764.0; GreenEnd3=29555.0; BlueEnd3=15677.0;

#endif

#ifdef COLORMAP17  /*  red to black to dark red to orange to yellow */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP17\n");
#endif

  strcat(PNGFileName,"-CMap17");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=0.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=15000.0; GreenEnd2=0.0; BlueEnd2=0.0;

  RedStart3=15000.0; GreenStart3=0.0; BlueStart3=0.0;
  RedEnd3=60395.0; GreenEnd3=28013.0; BlueEnd3=4883.0;

  RedStart4=60395.0; GreenStart4=28013.0; BlueStart4=4883.0;
  RedEnd4=61680.0; GreenEnd4=60652.0; BlueEnd4=3084.0;

#endif

#ifdef COLORMAP18  /*  red to white to dark red to orange to yellow */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP18\n");
#endif

  strcat(PNGFileName,"-CMap18");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=65534.0; BlueEnd1=65534.0;

  RedStart2=65534.0; GreenStart2=65534.0; BlueStart2=65534.0;
  RedEnd2=15000.0; GreenEnd2=0.0; BlueEnd2=0.0;

  RedStart3=15000.0; GreenStart3=0.0; BlueStart3=0.0;
  RedEnd3=60395.0; GreenEnd3=28013.0; BlueEnd3=4883.0;

  RedStart4=60395.0; GreenStart4=28013.0; BlueStart4=4883.0;
  RedEnd4=61680.0; GreenEnd4=60652.0; BlueEnd4=3084.0;

#endif

#ifdef COLORMAP19  /*  red to dark orange to dark red to orange to yellow */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP19\n");
#endif

  strcat(PNGFileName,"-CMap19");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=49891.0; GreenEnd1=26728.0; BlueEnd1=14135.0;

  RedStart2=49891.0; GreenStart2=26728.0; BlueStart2=14135.0;
  RedEnd2=15000.0; GreenEnd2=0.0; BlueEnd2=0.0;

  RedStart3=15000.0; GreenStart3=0.0; BlueStart3=0.0;
  RedEnd3=60395.0; GreenEnd3=28013.0; BlueEnd3=4883.0;

  RedStart4=60395.0; GreenStart4=28013.0; BlueStart4=4883.0;
  RedEnd4=61680.0; GreenEnd4=60652.0; BlueEnd4=3084.0;

#endif

#ifdef COLORMAP20  /*  red to dark red to orange to dark orange to yellow */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP20\n");
#endif

  strcat(PNGFileName,"-CMap20");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=15000.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=15000.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=60395.0; GreenEnd2=28013.0; BlueEnd2=4883.0;

  RedStart3=60395.0; GreenStart3=28013.0; BlueStart3=4883.0;
  RedEnd3=31354.0; GreenEnd3=14649.0; BlueEnd3=2827.0;

  RedStart4=31354.0; GreenStart4=14649.0; BlueStart4=2827.0;
  RedEnd4=61680.0; GreenEnd4=60652.0; BlueEnd4=3084.0;

#endif

#ifdef COLORMAP21  /*  blue to dark green to light green to grey to white  */
#define QUADCOLOR

#ifdef CMDEBUG
  printf("\n  COLORMAP21\n");
#endif

  strcat(PNGFileName,"-CMap21");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=65534.0;
  RedEnd1=11565.0; GreenEnd1=29555.0; BlueEnd1=8224.0;

  RedStart2=11565.0; GreenStart2=29555.0; BlueStart2=8224.0;
  RedEnd2=29041.0; GreenEnd2=61680.0; BlueEnd2=32639.0;

  RedStart3=29041.0; GreenStart3=61680.0; BlueStart3=32639.0;
  RedEnd3=45746.0; GreenEnd3=47288.0; BlueEnd3=46003.0;

  RedStart4=45746.0; GreenStart4=47288.0; BlueStart4=46003.0;
  RedEnd4=65534.0; GreenEnd4=65534.0; BlueEnd4=65534.0;

#endif

#ifdef COLORMAP22  /* white to red */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP22\n");
#endif

  strcat(PNGFileName,"-CMap22");
  RedStart1=65534.0; GreenStart1=65534.0; BlueStart1=65534.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP23  /* blue to red */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP23\n");
#endif

  strcat(PNGFileName,"-CMap23");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=65534.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP24  /* blue to yellow */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP24\n");
#endif

  strcat(PNGFileName,"-CMap24");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=65534.0;
  RedEnd1=65534.0; GreenEnd1=65534.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP25  /* red to white */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP25\n");
#endif

  strcat(PNGFileName,"-CMap25");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=65534.0; BlueEnd1=65534.0;

#endif

#ifdef COLORMAP26  /* red to blue */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP26\n");
#endif

  strcat(PNGFileName,"-CMap26");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=65534.0;

#endif

#ifdef COLORMAP27  /* yellow to blue */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP27\n");
#endif

  strcat(PNGFileName,"-CMap27");
  RedStart1=65534.0; GreenStart1=65534.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=65534.0;

#endif

#ifdef COLORMAP28  /* red to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP28\n");
#endif

  strcat(PNGFileName,"-CMap28");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP29  /* black to red */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP29\n");
#endif

  strcat(PNGFileName,"-CMap29");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP30  /* green to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP30\n");
#endif

  strcat(PNGFileName,"-CMap30");
  RedStart1=0.0; GreenStart1=65534.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP31  /* black to green */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP31\n");
#endif

  strcat(PNGFileName,"-CMap31");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=65534.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP32  /* blue to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP32\n");
#endif

  strcat(PNGFileName,"-CMap32");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=65534.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP33  /* black to blue */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP33\n");
#endif

  strcat(PNGFileName,"-CMap33");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=65534.0;

#endif

#ifdef COLORMAP34  /* yellow to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP34\n");
#endif

  strcat(PNGFileName,"-CMap34");
  RedStart1=65534.0; GreenStart1=65534.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP35  /* black to yellow */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP35\n");
#endif

  strcat(PNGFileName,"-CMap35");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65534.0; GreenEnd1=65534.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP36  /* majenta to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP36\n");
#endif

  strcat(PNGFileName,"-CMap36");
  RedStart1=65534.0; GreenStart1=0.0; BlueStart1=65534.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP37  /* black to majenta */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP37\n");
#endif

  strcat(PNGFileName,"-CMap37");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=65535.0; GreenEnd1=0.0; BlueEnd1=65534.0;

#endif

#ifdef COLORMAP38  /* light teal to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP38\n");
#endif

  strcat(PNGFileName,"-CMap38");
  RedStart1=0.0; GreenStart1=65534.0; BlueStart1=65535.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP39  /* black to light teal */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP39\n");
#endif

  strcat(PNGFileName,"-CMap39");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=0.0; GreenEnd1=65534.0; BlueEnd1=65534.0;

#endif

#ifdef COLORMAP40  /* purple to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP40\n");
#endif

  strcat(PNGFileName,"-CMap40");
  RedStart1=51657.0; GreenStart1=6939.0; BlueStart1=57568.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef COLORMAP41  /* black to purple */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP41\n");
#endif

  strcat(PNGFileName,"-CMap41");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=51657.0; GreenEnd1=6939.0; BlueEnd1=57568.0;

#endif

#ifdef COLORMAP42  /* green to teal */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP42\n");
#endif

  strcat(PNGFileName,"-CMap42");
  /*  RedStart1=3084.0; GreenStart1=38036.0; BlueStart1=10023.0;*/
  RedStart1=4112.0; GreenStart1=22873.0; BlueStart1=7967.0;
  RedEnd1=5140.0; GreenEnd1=46517.0; BlueEnd1=58339.0;

#endif

#ifdef COLORMAP43  /* teal to green */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP43\n");
#endif

  strcat(PNGFileName,"-CMap43");
  RedStart1=5140.0; GreenStart1=46517.0; BlueStart1=58339.0;
  RedEnd1=4112.0; GreenEnd1=22873.0; BlueEnd1=7967.0;
  /*  RedEnd1=3084.0; GreenEnd1=38036.0; BlueEnd1=10023.0;*/

#endif


  /*  try these 3 colors (colorpicker.com)
        10591F - 18, 89, 31 - 4626, 22873, 7967
	1F1059 - 31, 16, 89 - 7967, 4112, 22873
	591F10 - 89, 31, 16 - 22873, 7967, 4112
  */

#ifdef COLORMAP44
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP44\n");
#endif

  strcat(PNGFileName,"-CMap44");
  RedStart1=4626.0; GreenStart1=22873.0; BlueStart1=7967.0;
  RedEnd1=7967.0; GreenEnd1=4112.0; BlueEnd1=22873.0;

  RedStart2=7967.0; GreenStart2=4112.0; BlueStart2=22873.0;
  RedEnd2=22873.0; GreenEnd2=7967.0; BlueEnd2=4112.0;

#endif

#ifdef COLORMAP45
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP45\n");
#endif

  strcat(PNGFileName,"-CMap45");
  RedStart1=22873.0; GreenStart1=7967.0; BlueStart1=4112.0;
  RedEnd1=7967.0; GreenEnd1=4112.0; BlueEnd1=22873.0;

  RedStart2=7967.0; GreenStart2=4112.0; BlueStart2=22873.0;
  RedEnd2=4626.0; GreenEnd2=22873.0; BlueEnd2=7967.0;

#endif

#ifdef COLORMAP46
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP46\n");
#endif

  strcat(PNGFileName,"-CMap46");
  RedStart1=7967.0; GreenStart1=4112.0; BlueStart1=22873.0;
  RedEnd1=4626.0; GreenEnd1=22873.0; BlueEnd1=7967.0;

  RedStart2=4626.0; GreenStart2=22873.0; BlueStart2=7967.0;
  RedEnd2=22873.0; GreenEnd2=7967.0; BlueEnd2=4112.0;

#endif

#ifdef COLORMAP47
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP47\n");
#endif

  strcat(PNGFileName,"-CMap47");
  RedStart1=21588.0; GreenStart1=3598.0; BlueStart1=21074.0;
  /*  RedEnd1=45000.0; GreenEnd1=1285.0; BlueEnd1=1285.0;*/
  RedEnd1=45000.0; GreenEnd1=1285.0; BlueEnd1=10285.0;

  RedStart2=45000.0; GreenStart2=1285.0; BlueStart2=10285.0;
  RedEnd2=21588.0; GreenEnd2=3598.0; BlueEnd2=21074.0;

#endif

#ifdef COLORMAP48
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP48\n");
#endif

  strcat(PNGFileName,"-CMap48");
  RedStart1=45000.0; GreenStart1=1285.0; BlueStart1=10285.0;
  RedEnd1=21588.0; GreenEnd1=3598.0; BlueEnd1=21074.0;

  RedStart2=21588.0; GreenStart2=3598.0; BlueStart2=21074.0;
  RedEnd2=45000.0; GreenEnd2=1285.0; BlueEnd2=10285.0;

#endif

#ifdef COLORMAP49
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP49\n");
#endif

  strcat(PNGFileName,"-CMap49");
  RedStart1=21588.0; GreenStart1=3598.0; BlueStart1=21074.0;
  RedEnd1=31354.0; GreenEnd1=1285.0; BlueEnd1=1285.0;

  RedStart2=31354.0; GreenStart2=1285.0; BlueStart2=1285.0;
  RedEnd2=21588.0; GreenEnd2=3598.0; BlueEnd2=21074.0;

#endif

#ifdef COLORMAP50
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP50\n");
#endif

  strcat(PNGFileName,"-CMap50");
  RedStart1=31354.0; GreenStart1=1285.0; BlueStart1=1285.0;
  RedEnd1=21588.0; GreenEnd1=3598.0; BlueEnd1=21074.0;

  RedStart2=21588.0; GreenStart2=3598.0; BlueStart2=21074.0;
  RedEnd2=31354.0; GreenEnd2=1285.0; BlueEnd2=1285.0;

#endif

#ifdef COLORMAP51
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP51\n");
#endif

  strcat(PNGFileName,"-CMap51");
  RedStart1=21588.0; GreenStart1=3598.0; BlueStart1=21074.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

  RedStart2=0.0; GreenStart2=0.0; BlueStart2=0.0;
  RedEnd2=21588.0; GreenEnd2=3598.0; BlueEnd2=21074.0;

#endif

#ifdef COLORMAP52
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP52\n");
#endif

  strcat(PNGFileName,"-CMap52");
  RedStart1=0.0; GreenStart1=0.0; BlueStart1=0.0;
  RedEnd1=21588.0; GreenEnd1=3598.0; BlueEnd1=21074.0;

  RedStart2=21588.0; GreenStart2=3598.0; BlueStart2=21074.0;
  RedEnd2=0.0; GreenEnd2=0.0; BlueEnd2=0.0;

#endif

#ifdef COLORMAP53
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP53\n");
#endif

  strcat(PNGFileName,"-CMap53");
  RedStart1=21588.0; GreenStart1=3598.0; BlueStart1=21074.0;
  RedEnd1=65534.0; GreenEnd1=65534.0; BlueEnd1=65534.0;

  RedStart2=65534.0; GreenStart2=65534.0; BlueStart2=65534.0;
  RedEnd2=21588.0; GreenEnd2=3598.0; BlueEnd2=21074.0;

#endif

#ifdef COLORMAP54
#define DUALCOLOR

#ifdef CMDEBUG
  printf("  COLORMAP54\n");
#endif

  strcat(PNGFileName,"-CMap54");
  RedStart1=65534.0; GreenStart1=65534.0; BlueStart1=65534.0;
  RedEnd1=21588.0; GreenEnd1=3598.0; BlueEnd1=21074.0;

  RedStart2=21588.0; GreenStart2=3598.0; BlueStart2=21074.0;
  RedEnd2=65534.0; GreenEnd2=65534.0; BlueEnd2=65534.0;

#endif






#ifdef COLORMAP99  /* white to black */
#define ONECOLOR

#ifdef CMDEBUG
  printf("  COLORMAP99\n");
#endif

  strcat(PNGFileName,"-CMap99");
  RedStart1=65534.0; GreenStart1=65534.0; BlueStart1=65534.0;
  RedEnd1=0.0; GreenEnd1=0.0; BlueEnd1=0.0;

#endif

#ifdef QUADCOLOR

  double fNumColorsUsed, QuarterColors, ThirdColors, HalfColors, FifthColors;
  fNumColorsUsed = (float)NumColorsUsed;
  QuarterColors = fNumColorsUsed / 4.0;
  FifthColors = fNumColorsUsed / 5.0; ThirdColors = fNumColorsUsed / 3.0;
  HalfColors = fNumColorsUsed / 2.0;

  printf("  NumColorsUsed/4:         %f\n",QuarterColors);

  ColorRedInc1 = (RedEnd1-RedStart1)/((float)NumColorsUsed/4.0);
  ColorGreenInc1 = (GreenEnd1-GreenStart1)/((float)NumColorsUsed/4.0);
  ColorBlueInc1 = (BlueEnd1-BlueStart1)/((float)NumColorsUsed/4.0);

  ColorRedInc2 = (RedEnd2-RedStart2)/((float)NumColorsUsed/4.0);
  ColorGreenInc2 = (GreenEnd2-GreenStart2)/((float)NumColorsUsed/4.0);
  ColorBlueInc2 = (BlueEnd2-BlueStart2)/((float)NumColorsUsed/4.0);

  ColorRedInc3 = (RedEnd3-RedStart3)/((float)NumColorsUsed/4.0);
  ColorGreenInc3 = (GreenEnd3-GreenStart3)/((float)NumColorsUsed/4.0);
  ColorBlueInc3 = (BlueEnd3-BlueStart3)/((float)NumColorsUsed/4.0);

  ColorRedInc4 = (RedEnd4-RedStart4)/QuarterColors;
  ColorGreenInc4 = (GreenEnd4-GreenStart4)/(fNumColorsUsed/4.0f+0);
  ColorBlueInc4 = (BlueEnd4-BlueStart4)/(fNumColorsUsed/4.0f+0);

  /*
  printf("\nRed4  :  %f - %f = %f (%d)\n",RedEnd4, RedStart4, (RedEnd4-RedStart4), NumColorsUsed);
  printf("Green4:  %f %f %d\n",GreenEnd4, GreenStart4, NumColorsUsed);
  printf("Blue4 :  %f %f %d\n",BlueEnd4, BlueStart4, NumColorsUsed);
  */


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

  fprintf(StatsFile,"\n\n  Colormaps:\n");
  fprintf(StatsFile,"    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  fprintf(StatsFile,"    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	 BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);
  fprintf(StatsFile,"    %f %f %f --> %f  %f %f\n",RedStart3, GreenStart3,
	 BlueStart3, RedEnd3, GreenEnd3, BlueEnd3);
  fprintf(StatsFile,"    %f %f %f --> %f  %f %f\n",RedStart4, GreenStart4,
	 BlueStart4, RedEnd4, GreenEnd4, BlueEnd4);

  fprintf(StatsFile,"    Inc1's: %f, %f, %f\n", 
	 ColorRedInc1, ColorGreenInc1, ColorBlueInc1);
  fprintf(StatsFile,"    Inc2's: %f, %f, %f\n", 
	 ColorRedInc2, ColorGreenInc2, ColorBlueInc2);
  fprintf(StatsFile,"    Inc3's: %f, %f, %f\n", 
	 ColorRedInc3, ColorGreenInc3, ColorBlueInc3);
  fprintf(StatsFile,"    Inc4's: %f, %f, %f\n", 
	 ColorRedInc4, ColorGreenInc4, ColorBlueInc4);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<(NumColorsUsed/4);
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign first quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of first fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of first fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<(NumColorsUsed/2);
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign second quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of second fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of second fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
       i<(NumColorsUsed*3/4);
       i++,
	 RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	 BlueValue+=ColorBlueInc3) {
    
    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign third quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of third fourth:   (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of third fourth:   (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  int StepCounter = 0;

  RedValue=RedStart4; GreenValue=GreenStart4; BlueValue=BlueStart4;

  for (/*RedValue=RedStart4,GreenValue=GreenStart4,BlueValue=BlueStart4*/;
       i<NumColorsUsed;
       i++ ) {
	 /*
	 RedValue+=ColorRedInc4,GreenValue+=ColorGreenInc4,
	 BlueValue+=ColorBlueInc4) {
	 */

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

    RedValue = RedValue + ColorRedInc4;
    GreenValue = GreenValue + ColorGreenInc4;
    BlueValue = BlueValue + ColorBlueInc4;

    StepCounter++;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign fourth quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of fourth fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  printf("    Actual 4rth quater steps:  %d\n",StepCounter);
  fprintf(StatsFile,"    Out of fourth fourth:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Actual 4rth quater steps:  %d\n",StepCounter);
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

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign first quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of first third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of first third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<(NumColorsUsed*2/3);
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign second quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of second third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of second third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart3,GreenValue=GreenStart3,BlueValue=BlueStart3;
       i<NumColorsUsed;
       i++,
	 RedValue+=ColorRedInc3,GreenValue+=ColorGreenInc3,
	 BlueValue+=ColorBlueInc3) {
    
    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign third quarter colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of third third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of third third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

#endif

#ifdef DUALCOLOR

  ColorRedInc1 = (RedEnd1-RedStart1)/(float)(NumColorsUsed/2);
  ColorGreenInc1 = (GreenEnd1-GreenStart1)/(float)(NumColorsUsed/2);
  ColorBlueInc1 = (BlueEnd1-BlueStart1)/(float)(NumColorsUsed/2);

  ColorRedInc2 = (RedEnd2-RedStart2)/(float)(NumColorsUsed/2);
  ColorGreenInc2 = (GreenEnd2-GreenStart2)/(float)(NumColorsUsed/2);
  ColorBlueInc2 = (BlueEnd2-BlueStart2)/(float)(NumColorsUsed/2);

#ifdef PRINTCOLORMAPS
  printf("\n\n  Colormaps:\n");
  printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  printf("    %f %f %f --> %f  %f %f\n",RedStart2, GreenStart2, 
	 BlueStart2, RedEnd2, GreenEnd2, BlueEnd2);

  printf("    Inc1's: %f, %f, %f\n", 
	 ColorRedInc1, ColorGreenInc1, ColorBlueInc1);
  printf("    Inc2's: %f, %f, %f\n", 
	 ColorRedInc2, ColorGreenInc2, ColorBlueInc2);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<(NumColorsUsed/2);
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign first half colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of first half:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of first third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

  for (RedValue=RedStart2,GreenValue=GreenStart2,BlueValue=BlueStart2;
       i<NumColorsUsed;
       i++,
	 RedValue+=ColorRedInc2,GreenValue+=ColorGreenInc2,
	 BlueValue+=ColorBlueInc2) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

  }  /* assign second half colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of second half:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of second third:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
#endif

#endif


#ifdef ONECOLOR

  ColorRedInc1 = (RedEnd1 - RedStart1)/(float)(NumColorsUsed);
  ColorGreenInc1 = (GreenEnd1 - GreenStart1)/(float)(NumColorsUsed);
  ColorBlueInc1 = (BlueEnd1 - BlueStart1)/(float)(NumColorsUsed);

#ifdef PRINTCOLORMAPS
  printf("\n\n  Colormaps:\n");
  printf("    %f %f %f --> %f  %f %f\n",RedStart1, GreenStart1, 
	 BlueStart1, RedEnd1, GreenEnd1, BlueEnd1);
  printf("    Inc1's: %f, %f, %f\n", 
	 ColorRedInc1, ColorGreenInc1, ColorBlueInc1);
#endif

  for (i=0,RedValue=RedStart1,GreenValue=GreenStart1,BlueValue=BlueStart1;
       i<NumColorsUsed;
       i++,
	 RedValue+=ColorRedInc1,GreenValue+=ColorGreenInc1,
	 BlueValue+=ColorBlueInc1) {

    ColorRed[i]=RedValue;
    ColorGreen[i]=GreenValue;
    ColorBlue[i]=BlueValue;

#ifdef CMDEBUGASSIGN
    printf("Assigned color (%f, %f, %f) - %d\n",
	   ColorRed[i], ColorGreen[i], ColorBlue[i], i);
#endif

  }  /* assign all colors */

#ifdef PRINTCOLORMAPS
  printf("    Out of loop:  (%d) %f, %f, %f\n",
	 (i-1), ColorRed[i-1], ColorGreen[i-1], ColorBlue[i-1]);
  fprintf(StatsFile,"    Out of loop:  (%d) %f, %f, %f\n",
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

