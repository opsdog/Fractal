#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <X11/Xlib.h>  /* includes <X11/X.h> */

#include "X11Explore.h"

int MSet[1160][910];
int MSetLevel();
void plot4();
double dougee;


main(argc,argv)
int argc;
char *argv[];
{
    int nx,ny,maxiter,ix,iy,shit;
    double xmin,xmax,ymin,ymax,cx,cy,lengthx,lengthy,centerx,centery;
    double distx,disty,incx,incy;

    num_entries_colormap=256;

    centerx= -0.675; centery=0.025; distx=1.425; disty=1.474;

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
/* centerx= -0.7779324; centery=0.135055500998; distx=0.00002; disty=0.00002;*/
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
/* centerx=0.36720359945; centery=0.5922317790; distx=0.0000000080; disty=0.0000000080;*/
/* centerx=0.36720359945; centery=0.5922317794; distx=0.0000000002; disty=0.0000000002;*/
/* centerx=0.36720359945; centery=0.5922317794; distx=0.00000000002; disty=0.00000000002;*/
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
 centerx=0.2913061419; centery=0.0151788696; distx=1.23e-8; disty=1.23e-8;
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

/*    nx=102; ny=78; maxiter=50;*/
/*    nx=204; ny=156; maxiter=300;*/
    nx=300; ny=234; maxiter=1000;
/*    nx=575; ny=450; maxiter=2000;*/
/*    nx=768; ny=576; maxiter=2000;*/
/*    nx=1148; ny=898; maxiter=20000;*/

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

    plot4(nx,ny,argc,argv); 

    printf("length of x %f, length of y %f",lengthx,lengthy);
    printf("\ncenter point is (%f,%f)",centerx,centery);
    printf("\ndist is (%f,%f)",distx,disty);
    printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
    printf("\nnx %d, ny %d, dwell %d\n",nx,ny,maxiter);

/*system("xwd -name Untitled -out Fractal.xwd");*/

    printf("Done\n");scanf("%s",choice);

    XDestroyWindow(Dougee_Display,Dougee_Window);
    XSync(Dougee_Display,(int) 1);

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
/*  15 gives brighter colors, but 63 gives better depth */
/*    if (iter==maxiter) return(63);*/
/*    return((iter%63));*/
    if (iter==maxiter) {
/*      printf("maxed out ");*/
      return(num_entries_colormap);
    } else {
      return((iter%num_entries_colormap));
    }
}

void plot4(nx,ny,argc,argv)
int nx,ny;
int argc;
char *argv[];
{
    double max;
    int x,y,color,inq,shit;

  Initialize(nx,ny);

for (shit=0;shit<=num_entries_colormap;shit++) {
  Dougee_GCValues->foreground=(unsigned long)(shit);
  XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  for (x=0;x<nx;x++) {
    for (y=0;y<ny;y++) {
      if (MSet[x][y]==shit) XDrawPoint(Dougee_Display,Dougee_Drawable,Drawing_GC,x,y);
    }
  }
}
    XSync(Dougee_Display,False);
}

void Initialize(nx,ny)
int nx,ny;
{
  int i,j,ColorChoice,colorinc;
  char ToDisplay[100];

  printf("Where you be? ");scanf("%s",ToDisplay);
  strcat(ToDisplay,":0.0");

  if ((Dougee_Display = XOpenDisplay(ToDisplay)) == NULL) {
    printf("\n\ndisplay not opened\n"); exit(2);
  }

  Dougee_Screen=XDefaultScreen(Dougee_Display);
  Dougee_Visual=XDefaultVisual(Dougee_Display,Dougee_Screen);
  Dougee_Depth=XDefaultDepth(Dougee_Display,Dougee_Screen);
  num_entries_colormap=XDisplayCells(Dougee_Display,0);
printf("\n %d colors\n",num_entries_colormap);

#include "../Stuff/Choice.c"

/*    Define and create the main window */
  Dougee_Attributes=(XSetWindowAttributes *)malloc(sizeof(XSetWindowAttributes));
  border_width=1;x=0;y=0;Dougee_Attributes->backing_store=Always;
  Dougee_Attributes->background_pixel=0;
  Dougee_Attributes->colormap=Dougee_Colormap;
  Dougee_Attributes->override_redirect=False;
  Dougee_Attributes->event_mask=ButtonPressMask;

  Dougee_Window = XCreateWindow(Dougee_Display,DefaultRootWindow(Dougee_Display),
				x,y,nx,ny,border_width,Dougee_Depth,InputOutput,
				(Visual *) CopyFromParent,
	     CWBackPixel | CWEventMask | CWColormap | CWBackingStore | CWOverrideRedirect,
				Dougee_Attributes);

  XMapWindow(Dougee_Display,Dougee_Window); XFlush(Dougee_Display);sleep(2);
  XMoveWindow(Dougee_Display,Dougee_Window,1,1); XSync(Dougee_Display,False);

  Dougee_Drawable=Dougee_Window; Dougee_GCValues=(XGCValues *)malloc(sizeof(XGCValues));
  Dougee_GCValues->foreground=0; Dougee_GCValues->background=1;
  Drawing_GC=XCreateGC(Dougee_Display,Dougee_Drawable,GCForeground | GCBackground,
		       Dougee_GCValues);
}
