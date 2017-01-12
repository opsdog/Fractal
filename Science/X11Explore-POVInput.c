#include <stdio.h>
#include <math.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

int num_entries_colormap;
int maxiter;

void Initialize();

int MSet[2000][2000];
int MSetLevel();

#define MAX_COLORS 65535

float ColorRed[MAX_COLORS];
float ColorGreen[MAX_COLORS];
float ColorBlue[MAX_COLORS];

double dougee;


main(argc,argv)
int argc;
char *argv[];
{
    int nx,ny,ix,iy,shit;
    int RetVal;
    FILE *File;
    double xmin,xmax,ymin,ymax,cx,cy,lengthx,lengthy,centerx,centery;
    double distx,disty,incx,incy;
    char choice[80];

    num_entries_colormap=256;

    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474;

    centerx= -0.75; centery= -0.06; distx=1.0; disty=1.0;
    centerx= -0.75; centery= -0.040; distx=0.250; disty=0.250;

/* centerx=0.3584; centery=0.644;distx=0.006;disty=0.006;*/

/* centerx= -1.4578; centery=0.0; distx=0.04; disty=0.08;*/
/* centerx= -1.4578; centery=0.0; distx=0.006; disty=0.006;*/
/* centerx= -1.4578; centery=0.0; distx=0.0006; disty=0.0006;*/
/* centerx= -1.4578; centery=0.0; distx=0.0002; disty=0.0002;*/
/* centerx= -1.4578; centery=0.0; distx=0.0001; disty=0.0001;*/
/* centerx= -1.4578; centery=0.0; distx=0.00004; disty=0.00004;*/

/* centerx= -1.48; centery=0.0; distx=0.001; disty=0.001;*/

/* this is a NEAT sequence */
/* centerx= -0.7779324; centery=0.135055500998; distx=1.5; disty=1.5;*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.002; disty=0.002;*/
/*** centerx= -0.7779324; centery=0.135055500998; distx=0.00002; disty=0.00002;*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.000002; disty=0.000002;*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.0000002; disty=0.0000002;*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.000000000002; disty=0.000000000002;*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.0000000000002; disty=0.0000000000002;*/
/* centerx= -0.7779324; centery=0.135055500998; distx=0.00000000000005; disty=0.00000000000005;*/

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

/* centerx= -1.8323762603103880; centery=0.0014286451041698; distx=2.0; disty=2.0;*/
/* centerx= -1.8323762603103880; centery=0.0014286451041698; distx=0.00002; disty=0.00002;*/

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
/* centerx= -1.8323698334092280; centery=0.0014357434457388; distx=0.00000000004; disty=0.00000000004;*/

/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=1.75; disty=1.75;*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.1; disty=0.1;*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.0008; disty=0.0008;*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.00002; disty=0.00002;*/
/* centerx= -1.2334552407264710; centery= 0.3167717270553112; distx=0.000002; disty=0.000002;*/

/*  'John Dewey Jones midget' on the spike  (Ken Philip) */
/* centerx= -1.99638; centery=0.0; distx=1.5; disty=1.5;*/
/* centerx= -1.99638; centery=0.0; distx=0.05; disty=0.05;*/
/* centerx= -1.99638; centery=0.0; distx=1.43e-5; disty=1.43e-5;*/

/*  Quad spiral in East Valley - NEAT!! (Ken Philip) */
/* centerx=0.2913061419; centery=0.0151788696; distx=1.5; disty=1.5;*/
/* centerx=0.2913061419; centery=0.0151788696; distx=1.23e-8; disty=1.23e-8;*/
/* centerx=0.2913061419; centery=0.0151788696; distx=0.000000005; disty=0.000000005;*/
/* centerx=0.2913061419; centery=0.0151788696; distx=0.0000000008; disty=0.0000000008;*/

/*  Sea horse tail in SHV - neat, at least 50000 dwell  (Ken Philip) */
/* centerx= -0.7499385; centery=0.0169434; distx=1.5; disty=1.5;*/
/* centerx= -0.7499385; centery=0.0169434; distx=1.056e-5; disty=1.056e-5;*/

/*  Rabbit Valley, between north atom and its head  (Ken Philip) */
/*  not impressed */
/* centerx= -0.117824; centery=0.838990; distx=1.5; disty=1.5;*/
/* centerx= -0.117824; centery=0.838990; distx=0.00152671; disty=0.00152671;*/

/*  near small midget on the spike  (Ken Philip) */
/* centerx= -1.711638721; centery=0.000449787; distx=1.5; disty=1.5;*/
/* centerx= -1.711638721; centery=0.000449787; distx=1.4124e-7; disty=1.4124e-7;*/

    nx=102; ny=78; maxiter=3000;
    nx=204; ny=156; maxiter=3000;
/*    nx=300; ny=234; maxiter=3000;*/
/*    nx=575; ny=450; maxiter=3000;*/
/*    nx=768; ny=576; maxiter=3000;*/
/**    nx=1148; ny=898; maxiter=20000;*/

/*    printf("nx?"); scanf("%d",&nx);*/
/*    printf("ny?"); scanf("%d",&ny);*/
/*    printf("maxiter?"); scanf("%d",&maxiter);*/
/*    printf("center x?"); scanf("%f",&centerx);*/
/*    printf("center y?"); scanf("%f",&centery);*/
/*    printf("distance x?"); scanf("%f",&distx);*/
/*    printf("distance y?"); scanf("%f",&disty);*/

    xmin=centerx-distx; xmax=centerx+distx;
    ymin=centery-disty; ymax=centery+disty;
    lengthx = (double)sqrt((xmin*xmin)+(xmax*xmax));
    lengthy = (double)sqrt((ymin*ymin)+(ymax*ymax));
    incx=(xmax-xmin)/((double)nx-1.0); incy=(ymax-ymin)/((double)ny-1.0);
    if(xmax==xmin) {exit(-1);}
/*printf("Center (%e,%e)\n",centerx,centery);*/
/*printf("Distance (%e,%e)\n",distx,disty);*/
/*printf("(%e,%e) (%e,%e) %e ",xmin,xmax,ymin,ymax,(xmax-xmin));*/
/*printf("Incs:  (%e,%e)\n",incx,incy);*/

    for (iy=0;iy<ny-1;iy++) {
printf("\n%d",iy);
/*	cy=ymin+iy*(ymax-ymin)/(ny-1.0);*/
	cy=ymin+iy*incy;
	for (ix=0;ix<nx-1;ix++) {
/*	    cx=xmin+ix*(xmax-xmin)/(nx-1.0);*/
	    cx=xmin+ix*incx;
/*printf("  (%d,%d)(%e,%e)",ix,iy,cx,cy);*/
	    MSet[ix][iy]=(int)MSetLevel(cx,cy,maxiter);
/*printf(" %d",MSet[ix][iy]);*/
	} /* for ix */
    } /* for iy */

    printf("length of x %f, length of y %f",lengthx,lengthy);
    printf("\ncenter point is (%f,%f)",centerx,centery);
    printf("\ndist is (%f,%f)",distx,disty);
    printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
    printf("\nnx %d, ny %d, dwell %d\n",nx,ny,maxiter);

    /* write out the POV scene file */

    Initialize();

    printf("\n\nWriting scene file...");

    RetVal=(int)(File=fopen("Fractal.pov","w"));
    if (RetVal == -1 ) {
      printf("Couldn't open scene file - exiting.\n");
      exit(RetVal);
    }

    fprintf(File,"#include \"colors.inc\"\n");
    fprintf(File,"#include \"textures.inc\"\n");
    fprintf(File,"plane {y, 0 pigment {color White} }\n");

    /*
    fprintf(File,
      "camera { location <%f, 50, %f> direction y look_at <%f, -10, %f>}\n",
	    centerx, centery, centerx, centery);
    */

    fprintf(File,
	    /*
      "camera { location <5, 20, 5> direction y look_at <%d, -20, %d>}\n",
	    (nx/2), (ny/2));
	    */
      "camera { location <%d, 20, %d> direction y look_at <%d, -20, %d>}\n",
	    (nx/2), (ny/2), (nx/2), (ny/2));

    fprintf(File,"light_source { <0, 500, 0> White}\n");

    fprintf(File,
	    "light_source { <0, 500, %d> White}\n",
	    ny);
    fprintf(File,
	    "light_source { <%d, 500, 0> White}\n",
	    nx);
    fprintf(File,
	    "light_source { <%d, 500, %d> White}\n",
	    nx,ny);

	    
    fprintf(File,"\n\n");
    for (iy=0;iy<ny-1;iy++) {
	for (ix=0;ix<nx-1;ix++) {

	  /*
	  printf("Sphere (%d,%d) should have color %d\n",
		  ix,iy,MSet[ix][iy]);
	  */

	  /* flat spheres
	  fprintf(File,
		  "sphere { <%d, 0, %d>, .75 pigment { rgb <%f, %f, %f> } }\n",
		  ix,iy,ColorRed[MSet[ix][iy]],ColorGreen[MSet[ix][iy]],
		  ColorBlue[MSet[ix][iy]]);
	  */

	  /* height scaled spheres - works well
	  fprintf(File,
		 "sphere { <%d, %f, %d>, %f texture { pigment { color rgb <%f, %f, %f> transmit 0.25 } finish { ambient .8 diffuse .2 } } }\n",
		  ix,
		  ( (float)MSet[ix][iy]/(float)maxiter*10.0 ),
		  iy,
		  0.5,
		  ColorRed[MSet[ix][iy]],ColorGreen[MSet[ix][iy]],
		  ColorBlue[MSet[ix][iy]]);
	  */


	  /* height scaled boxes - glass - works well
	  fprintf(File,
		  "box { <%d, 0, %d>, <%d, %f, %d> texture { pigment { color rgb <%f, %f, %f> transmit 0.25 } finish { ambient .8 diffuse .2 } } }\n",
		  ix, iy, (ix+1), 
		  ( (float)MSet[ix][iy]/(float)maxiter*10.0 ),
		  (iy+1),
		  ColorRed[MSet[ix][iy]],
		  ColorGreen[MSet[ix][iy]],
		  ColorBlue[MSet[ix][iy]]);
	  */

	  fprintf(File,
		"cone { <%f, 0, %f>, .5 <%f, %f, %f>, .2 texture { pigment { color rgb <%f, %f, %f> transmit 0.25 } finish { ambient .8 diffuse .2 } } }\n",
		  ( (float)ix+.5 ),( (float)iy+.5) ,
		  ( (float)ix+.5 ),
		  ( (float)MSet[ix][iy]/(float)maxiter*10.0 ),
		  ( (float)iy+.5 ),
		  ColorRed[MSet[ix][iy]],
		  ColorGreen[MSet[ix][iy]],
		  ColorBlue[MSet[ix][iy]]); 


	}
    }


    fclose(File);
    printf("Done.\n");

/*system("xwd -name Untitled -out Fractal.xwd");*/

}

int MSetLevel(cx,cy,maxiter)
double cx,cy;
int maxiter;
{
    int iter;
    double x,y,x2,y2,temp;

/*printf("(%e,%e)\n",cx,cy);*/
    x = 0; y = 0; x2 = 0; y2 = 0; iter = 0;
    while ((iter<maxiter) & (x2+y2<250000.0)) {
	temp = x2 - y2 + cx; y = 2.0 * x * y + cy;
	x = temp; x2 = x * x; y2 = y * y; iter++;
    } /* while */
    if (iter==maxiter) {
/*      printf("maxed out ");*/
/*      return(num_entries_colormap);*/
      return(maxiter);
    } else {
      /*      return((iter%num_entries_colormap));*/
      return(iter);
    }
}

void Initialize(nx,ny)
int nx,ny;
{
  int i,j,ColorChoice;
  float colorinc,redstart,greenstart,bluestart;
  float redinc,greeninc,blueinc;
  float RedValue, GreenValue, BlueValue;
  float RedStart, RedEnd, GreenStart, GreenEnd, BlueStart, BlueEnd;
  float ColorRedInc, ColorGreenInc, ColorBlueInc;
  FILE *File;

  printf("\n\nCOLORMAP CHOICES:");
  printf("\n\n  0)  Default Colormap");
  printf("\n  1)  Greyscale, Black Background");
  printf("\n  2)  Greyscale, White Background");
  printf("\n  3)  Experimental\n");
  printf("\nChoose a Colormap:  ");
  scanf("%d",&ColorChoice);

  if (ColorChoice) {

    /******************************************************************/
    /*  black to white                                                */
    /******************************************************************/

    if (ColorChoice==1) {
      /*
      redinc=(float)MAX_COLORS/(float)num_entries_colormap;
      greeninc=(float)MAX_COLORS/(float)num_entries_colormap;
      blueinc=(float)MAX_COLORS/(float)num_entries_colormap;
      */

      /*
      redinc=1.0/(float)MAX_COLORS;
      greeninc=1.0/(float)MAX_COLORS;
      blueinc=1.0/(float)MAX_COLORS;
      */

      /*
      redinc=1.0/(float)num_entries_colormap;
      greeninc=1.0/(float)num_entries_colormap;
      blueinc=1.0/(float)num_entries_colormap;
      */

      redinc=1.0/(float)maxiter;
      greeninc=1.0/(float)maxiter;
      blueinc=1.0/(float)maxiter;

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

      redinc=1.0/(float)maxiter;
      greeninc=1.0/(float)maxiter;
      blueinc=1.0/(float)maxiter;

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

  }  /*if ColorChoice*/

  File=fopen("DefinedColors.txt","w");

  for (i=0;i<maxiter;i++)
    fprintf(File,
	    "Color %d:  <%f, %f, %f>\n",
	    i,ColorRed[i],ColorGreen[i],ColorBlue[i]);
  
  fclose(File);




}
