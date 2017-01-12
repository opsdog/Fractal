#include <starbase.c.h>
#include <stdio.h>
#include <math.h>

static int fildes;

main()
{
   int color;
   float a,b,MINX,MINY,MAXX,MAXY,stepa,stepb;
   FILE *map;

   if ((map=fopen("mandel","r"))==NULL) {
	printf("\n\noutput file not open\n");
	exit(-1);
   }

   fscanf(map,"%f %f %f %f %f %f",&MINX,&MAXX,&MINY,&MAXY,&stepa,&stepb);

printf("%f %f %f %f",MINX,MAXX,MINY,MAXY); fscanf(stdin,"%c",&color);

   if ((fildes=gopen("/dev/crt",OUTDEV,"hp98550",RESET_DEVICE)) == NULL) {
	printf("\ngopen died");	exit(-1);
   }

   view_port(fildes,0.0,0.0,1.0,1.0); view_window(fildes,MINX,MINY,MAXX,MAXY);
/*   view_port(fildes,MINX,MINY,MAXX,MAXY);*/

   while ( map!= NULL) {
	fscanf(map,"%f %f %d",&a,&b,&color); line_color_index(fildes,color);
	move2d(fildes,a,b); draw2d(fildes,a,b);
	make_picture_current(fildes);
   }
}

