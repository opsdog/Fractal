#include <stdio.h>
#include <math.h>


main()
{
   int counta,countb,color,i,j;
   float a,b,stepa,stepb,rangex,rangey,MINX,MINY,MAXX,MAXY;
   FILE *map;

   if ((map=fopen("mandel","w"))==-1) {
	printf("\n\noutput file not open\n");
	exit(-1);
   }

/*   printf("\ncounta?  "); scanf("%d",&counta);*/
/*   printf("\ncountb?  "); scanf("%d",&countb);*/
  
/*   counta=1024; countb=780;*/
     counta=100; countb=100;

/*   MINX= -2.0; MINY= -2.0;*/
/*   MAXX=2.0; MAXY=2.0;*/
     MINX= -1.781; MINY= 0.0;
     MAXX= -1.764; MAXY= 0.013;
/*   MINX= -1.5;   MINY= -0.08;*/
/*   MAXX= -1.42;  MAXY=  0.08;*/
/*   MINX=  0.235025; MINY=  0.82712;/*      /* (.235125,.82722) */
/*   MAXX=  0.235225; MAXY=  0.82732;*/
/*   MINX= -0.750; MINY= 0.0986;*/
/*   MAXX= -0.746; MAXY= 0.1014;*/
/*   MINX= 0.31186; MINY= 0.75322;*/
/*   MAXX= 0.31458; MAXY= 0.75594;*/
/*   MINX= -0.29920; MINY= 0.67046;*/
/*   MAXX= 0.91480; MAXY= 1.06707;*/
/*     MINX= -0.19920; MINY= -0.12954;*/     /* this one is neat */
/*     MAXX= 1.01480;  MAXY= 1.06707;*/
/*   MINX= -0.75104; MAXX= 0.10511;*/
/*   MINY= -0.7408; MAXY= 0.11536;*/
/*   printf("\nMINX?  "); scanf("%f",&MINX);*/
/*   printf("\nMAXX?  "); scanf("%f",&MAXX);*/
/*   printf("\nMINY?  "); scanf("%f",&MINY);*/
/*   printf("\nMAXY?  "); scanf("%f",&MAXY);*/
   

   rangex=MAXX-MINX; rangey=MAXY-MINY; stepa=rangex/counta; stepb=rangey/countb;

   fprintf(map,"%f %f %f %f %f %f",MINX,MAXX,MINY,MAXY,stepa,stepb);

   for (a=MINX-stepa,i=0;i<counta;i++) {
/*printf("\ncounta=%d",i);*/	a=a+stepa;
	for (b=MINY-stepb,j=0;j<countb;j++) {
                b=b+stepb; figure_stuff(a,b,&color);
		fprintf(map,"%f %f %d",a,b,color);
	}
   }
}

figure_stuff(a,b,c)
float a,b;
int *c;
{
   int counter,count;
   float x,y;

   counter=500; x=0.0; y=0.0; *c=5;
   for (*c=0,count=0;count<counter;count++) {
	x=(x*x)-(y*y)+a;
	y=2*x*y+b;
        (*c)++;

	if (x >  4.0) { *c=(*c)%7;return;}
	if (x < -4.0) { *c=(*c)%7;return;}
	if (y >  4.0) { *c=(*c)%7;return;}
	if (y < -4.0) { *c=(*c)%7;return;}
   }
   *c=7;
}

