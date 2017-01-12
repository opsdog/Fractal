#include <starbase.c.h>
#include <stdio.h>
#include <math.h>

#define MAXARRAYSIZE 800

static int fildes;
int nx,ny,maxiter;
double MSet[MAXARRAYSIZE][MAXARRAYSIZE];
double xmin,xmax,ymin,ymax;
double MSetPot();
void plot4();

main()
{
    int ix,iy;
    double cx,cy;

    printf("\nnx?"); scanf("%d",&nx);
    printf("ny?"); scanf("%d",&ny);
    printf("maxiter?"); scanf("%d",&maxiter);
    printf("minx?"); scanf("%f",&xmin);
    printf("miny?"); scanf("%f",&ymin);
    printf("maxx?"); scanf("%f",&xmax);
    printf("maxy?"); scanf("%f",&ymax);

    for (iy=0;iy<ny-1;iy++) {
	cy = ymin + iy * (ymax - ymin) / (ny - 1.0);
	for (ix=0;ix<nx-1;ix++) {
	    cx = xmin + ix * (xmax - xmin) / (nx - 1.0);
	    MSet[ix][iy] = MSetPot(cx,cy,maxiter);
printf("color will be %d",(int)MSet[ix][iy]);
	}  /* for ix */
    } /* for iy */

    plot4(nx,ny);
}

double MSetPot(cx,cy,maxiter)
double cx,cy;
int maxiter;
{
    int iter;
    double x,y,x2,y2,temp,potential;

    x = cx; x2 = x * x;
    y = cy; y2 = y * y;
    iter = 0;
    while ((iter<maxiter) & (x2 + y2 < 10000.0)) {
	temp = x2 - y2 + cx;
	y = 2.0 * x * y + cx;
	x = temp;
	x2 = x * x;
	y2 = y * y;
	iter++;
    }  /* while */
printf("%d %d\n",iter,maxiter);
    if (iter < maxiter)
	potential = 0.5 * log(x2 + y2)/pow(2.0,(double)iter);
    else
	potential = 0.0;
    return(potential);
}

void plot4(nx,ny)
int nx,ny;
{
    double max;
    int x,y,color;

printf("\nmade it into plot4");
    if ((fildes=gopen("/dev/crt",OUTDEV,"hp98550",RESET_DEVICE)) == -1) {
 	printf("\ngopen died");	exit(-1);
    }

    view_port(fildes,0.0,0.0,1.0,1.0);
    view_window(fildes,0.0,0.0,(float)nx,(float)ny);

    for (x=0;x<nx;x++) {
	for (y=0;y<ny;y++) {
/*	    color=(int)MSet[x][y];*/
/*	    line_color_index(fildes,color);*/
	    line_color_index(fildes,(int)MSet[x][y]);
/*printf("\ncolor is %d",color);*/
	    move2d(fildes,(float)x,(float)y);
	    draw2d(fildes,(float)x,(float)y);
	} /* for y */
	make_picture_current(fildes);
    } /* for x */
    make_picture_current(fildes);
}

