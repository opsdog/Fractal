#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <starbase.c.h>
#include </usr/lib/starbase/demos/SBUTILS/sbutils.c.h>
#include </usr/lib/starbase/demos/SBUTILS/sbutils.h>

#define toascii(x) ((x>=0 && x<=9) ? x + '0' : -1)

static int fildes;
int MSet[1025][782];
void plot4();
int MSetPot();

extern int sb_num_pens;
extern float sb_xmax,sb_ymax;

main(argc,argv)
int argc;
char *argv[];
{
  int nx,ny,maxiter,ix,iy;
  float cx,cy,centerx,centery,lengthx,lengthy;
  float distx,disty,xmin,xmax,ymin,ymax;

  centerx=.3584; centery=.644; distx=.006; disty=.006;

  printf("nx?"); scanf("%d",&nx);
  printf("ny?"); scanf("%d",&ny);
  printf("maxiter?"); scanf("%d",&maxiter);
/*  printf("center x?"); scanf("%f",&centerx);*/
/*  printf("center y?"); scanf("%f",&centery);*/
/*  printf("distance x?"); scanf("%f",&distx);*/
/*  printf("distance y?"); scanf("%f",&disty);*/

  xmin = centerx - distx; xmax = centerx + distx;
  ymin = centery - disty; ymax = centery + disty;
  lengthx = (float)sqrt((xmin*xmin)+(xmax*xmax));
  lengthy = (float)sqrt((ymin*ymin)+(ymax*ymax));

  for (iy=0;iy<ny;iy++) {
    printf("\n%d",iy);
    cy=ymin+(float)iy*(ymax-ymin)/((float)ny-1.0);
    for (ix=0;ix<nx;ix++) {
      cx=xmin+(float)ix*(xmax-xmin)/((float)nx-1.0);
      MSet[ix][iy]=MSetPot(cx,cy,maxiter);
/*printf("\n\n     MSet[%d,%d] is %d",ix,iy,MSet[ix][iy]);*/
    }
  }

  printf("length of x %f, length of y %f",lengthx,lengthy);
  printf("\ncenter point is (%f,%f)",centerx,centery);
  printf("\ndist is (%f,%f)",distx,disty);
  printf("\nfrom (%f,%f) to (%f,%f)",xmin,ymin,xmax,ymax);
  printf("\nnx %d, ny %d, dwell %d\n",nx,ny,maxiter);

  plot4(nx,ny,argc,argv);

}

int MSetPot(cx,cy,maxiter)
float cx,cy;
int maxiter;
{
  int iter;
  float x,y,x2,y2,temp,potential;

  x = cx; x2 = x * x;
  y = cy; y2 = y * y;
  iter = 0;

  while ((iter<maxiter) & ((x2+y2)<100000.0)) {
    temp = x2 - y2 + cx;
    y = 2.0 * x * y + cy;
    x = temp;
    x2 = x * x;
    y2 = y * y;
    iter++;
  }

  if (iter<maxiter) 
    potential=0.5*log(x2+y2)/pow(2.0,(float)iter);
  else
    potential=0.0;
/*printf("\npotential is %f",potential);*/
  return(potential*1000);
}

void plot4(nx,ny,argc,argv)
int nx,ny;
int argc;
char *argv[];
{
    float max;
    int x,y,color,inq,temp,tempx,tempy;
    char xstring[10],ystring[10],command[100];

    system("xwdestroy window1");
/*  */
    tempx=nx;tempy=ny;
    temp = (int)tempx/10000; xstring[0] = toascii(temp);
    tempx -= temp*10000; temp = (int)tempx / 1000;
    xstring[1] = toascii(temp); tempx -= temp*1000;
    temp = (int)tempx / 100; xstring[2] = toascii(temp);
    tempx -= temp*100; temp = (int)tempx / 10;
    xstring[3] = toascii(temp); tempx -= temp*10;
    xstring[4] = toascii(tempx); xstring[5] = '\0';
/*  */
    temp = (int)tempy/10000; ystring[0] = toascii(temp);
    tempy -= temp*10000; temp = (int)tempy / 1000;
    ystring[1] = toascii(temp); tempy -= temp*1000;
    temp = (int)tempy / 100; ystring[2] = toascii(temp);
    tempy -= temp*100; temp = (int)tempy / 10;
    ystring[3] = toascii(temp); tempy -= temp*10;
    ystring[4] = toascii(tempy); ystring[5] = '\0';
/*  */
    printf("x is %s, y is %s\n",xstring,ystring);
    strcpy(command,"xwcreate -r -g =");strcat(command,xstring);strcat(command,"x");
    strcat(command,ystring); strcat(command,"+0+0 window1");
    printf("command is %s\n",command);
    system(command);
    if ((fildes=gopen("/dev/screen/window1",OUTDEV,"sox11",RESET_DEVICE)) == -1) {
	printf("\ngopen died"); exit(-1); }

    fill_dither(fildes,16);
    vdc_extent(fildes,0.0,0.0,0.0,(float)(nx-1),(float)(ny-1),1.0);
    view_window(fildes,0.0,0.0,(float)nx,(float)ny);
    buffer_mode(fildes,FALSE);

    for (x=0;x<nx;x++) {
	for (y=0;y<ny;y++) {
	    line_color_index(fildes,(int)MSet[x][y]);
	    move2d(fildes,(float)x,(float)y);
	    draw2d(fildes,(float)x,(float)y);
	} /* for y */
    } /* for x */
    make_picture_current(fildes);
}
