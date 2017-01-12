#include <stdio.h>
/*#include <math.h>*/
/*#include <fcntl.h>*/
#include <starbase.c.h>

static int fildes;

int ix[100],iy[100],id,sides,i,j,px,py;
float cx=400,cy,sx,sy,sin(),cos(),pip5,as,ax,ay,adiv,temp;
unsigned long random=32767, mult=69069;
main(argc,argv)
int argc;
char *argv[];
{
    printf("Enter # of sides: ");
    scanf("%d",&sides);

   if ((fildes=gopen("/dev/crt",OUTDEV,"hp98550",RESET_DEVICE)) == -1) {
	printf("\ngopen died");	exit(-1);
   }

    view_port(fildes,0.0,0.0,1.0,1.0); view_window(fildes,0.0,0.0,695.0,480.0);
    as=3./(sides+3.);
    pip5 = 3.14159265/2.;
/*    calculate multiplier to scale "bottom" point to bottom of screen */
    j=(sides-1)/2;
    cy=2./(1.+sin(((4.*(j+1.)/sides)-1.)*pip5));
/*    calculate scale factors and y center point            */
    sx=cy*260.;
    sy=cy*239.5;
    cy=sy+.5;
/*    calculate and plot polygon corners                */
    for (i=0; i<sides; i++)
    {
        temp=((4.*(i+1.)/sides)-1.)*pip5;
        ix[i]=ax=cx+sx*cos(temp);
        iy[i]=ay=cy+sy*sin(temp);
printf("p[%d,%d]v[]\n",ix[i],iy[i]);
printf("%d,%d\n",ix[i],iy[i]);
        move2d(fildes,ix[i],iy[i]);
        draw2d(fildes,ix[i],iy[i]);
    }
/*    pick a starting point, use 32 bit pseudo-random generator    */
    random=random*mult+1;
    px = random%520+140;
    random=random*mult+1;
    py = random%480;
    while (1)
    {
        random=random*mult+1;
        i = random%sides;
        px = ix[i] + (px - ix[i])*as;
        py = iy[i] + (py - iy[i])*as;
/*        printf("p[%d,%d]v[]\n",px,py);*/
/*        printf("%d,%d\n",px,py);*/
        move2d(fildes,px,py);
        draw2d(fildes,px,py); make_picture_current(fildes);
    }
}
