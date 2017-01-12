#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>
#include <values.h>

#define Radians(Angle)    (double)(abs(Angle%360))*0.01745329252
#define F3(Delta,X0,X1,X2) (float)((X0+X1+X2)/3.0+Delta*Gauss())
#define F4(Delta,X0,X1,X2,X3) (float)((X0+X1+X2+X3)/4.0+Delta*Gauss())

#include "X11Landscape.h"

int  angle,rangle,xangle,yangle,zangle,size,sgn,maxlevel,numrand,seed,
  getmaxx,getmaxy,arand,XMap[520][520],YMap[520][520],dougee,dougeen,
  XMax,YMax,XMin,YMin,Rangex,Rangey,DougeeShift;
float delta,gaussadd,gaussfac,h,oldh,sigma,oldsigma,maxz,maxz1,maxz2,maxz3,
  maxz4,ZMap[520][520];
float alpha,theta;
float LightTheta, LightPhi, ViewPhi;
/* theta is the angle that the light is coming in at */
/* alpha is the angle between view vector and reflected vector (?) */
char s[133];

void InitGauss(),InitMap(),InitZMap(),BLine(),Intro(),Show();
void DisplayHeightField();
float I();
float Gauss(),Sign();
extern double drand48();
extern void srand48();
float DougeeScale;

main (argc,argv)
int argc;
char *argv[];
{

  getmaxx=1024;getmaxy=768;
  Intro();
  Show();

printf("Done\n");scanf("%s",choice);

  DisplayHeightField();

printf("Really Done\n");scanf("%s",choice);

XDestroyWindow(Dougee_Display,Dougee_Window);XSync(Dougee_Display,(int)1);
}

float Gauss()
{
  int i;
  double dougeesum;

  dougeesum=0.0;for (i=1;i<=numrand;i++) dougeesum = dougeesum + drand48();
  return((gaussfac * (float)dougeesum) - gaussadd);
}

void InitGauss()
{
  int seeed;
  gaussadd=(float)sqrt(3.0*(double)numrand);
  gaussfac=2.0*gaussadd/(float)(numrand*arand);/*seeed=time(0);srand48(seeed);*/
/*printf("\nseed is %d",seeed);*/
  srand48(seed);
}

float Sign(N)
float N;
{
  if (N=0.0) return(1.0); else return((float)abs((int)N)/(int)N);
}

void AddMap()
{
  int x,y;

  for (x=0;x<=dougeen;x++) {
    for (y=0;y<=dougeen;y++) {
      if (ZMap[x][y] > 0.0) YMap[x][y] -= (int)(ZMap[x][y]+0.5);
    }
  }
}

void GetMaxZ()
{
  int x,y;

  maxz=0.0;
  for (x=0;x<=dougeen;x++) {
    for (y=0;y<=dougeen;y++) {
      if (ZMap[x][y] > maxz) maxz=ZMap[x][y];
    }
  }
  maxz1=maxz*0.30;maxz2=maxz*0.60;maxz3=maxz*0.75;maxz4=maxz*0.80;
}

void InitMap()
{
  int i,j,x,y;

/*  xangle=335;yangle=205;*/  /* 25  works with 48, 64, 128 grids */
/*  offset of 36 works with 512 */
/*  offset of 33 works with 256 */
/*  offset of 25 works with 128 */
/*  offset of 23 works with 64 */
/*  offset of 21 works with 48 */
/*  xangle =360-x, yangle=180+x */
/*  xangle=350;yangle=190;*/  /* 10 */
xangle=360-DougeeShift;yangle=180+DougeeShift;
  size=(int)((float)getmaxy/(float)dougeen+0.5);
  for (i=0;i<dougeen;i++) {
    x=0; y=getmaxy/3;
    x += (int)((float)i*(float)cos((double)Radians(xangle))*(float)size*DougeeScale+0.5);
    y += (int)((float)i*(float)sin((double)Radians(xangle))*(float)size+0.5);
    for (j=0;j<dougeen;j++) {
      XMap[i][j]=x; YMap[i][j]=y;
      x += (int)((float)cos((double)Radians(yangle))*(float)size*DougeeScale+0.5);
      y += (int)((float)sin((double)Radians(yangle))*(float)size+0.5);
    }
  }
}

void BLine()
{
  int u,v,w,x,y;

  Dougee_GCValues->foreground=(unsigned long)7;
  XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  x=0; y=(getmaxy/3)+15;
  v=x+(int)((float)cos((double)Radians(xangle))*(float)getmaxx*DougeeScale);
  w=y+(int)((float)sin((double)Radians(xangle))*(float)getmaxy*DougeeScale);
/*  v=x+(int)((float)cos((double)Radians(xangle))*(float)getmaxx);*/
/*  w=y+(int)((float)sin((double)Radians(xangle))*(float)getmaxy);*/
  XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,x,y,v,w);
  v=x+(int)((float)cos((double)Radians(yangle-3))*(float)getmaxx*DougeeScale);
  w=y+(int)((float)sin((double)Radians(yangle-3))*(float)getmaxy*DougeeScale);
/*  v=x+(int)((float)cos((double)Radians(yangle-3))*(float)getmaxx);*/
/*  w=y+(int)((float)sin((double)Radians(yangle-3))*(float)getmaxy);*/
  XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,x,y,v,w);
}

void CLine(u,v,w,x,y)
int u,v,w,x,y;
{
  if (ZMap[x][y] < 0.0) {
    Dougee_GCValues->foreground=(unsigned long)2; /* 4 */
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
    }
  else if (ZMap[x][y] < maxz1) {
    Dougee_GCValues->foreground=(unsigned long)3; /* 5 */
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  }
  else if (ZMap[x][y] < maxz2) {
    Dougee_GCValues->foreground=(unsigned long)4; /* 28 */
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  }
  else if (ZMap[x][y] < maxz3) {
    Dougee_GCValues->foreground=(unsigned long)5; /* 24 */
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  }
  else if (ZMap[x][y] < maxz4) {
    Dougee_GCValues->foreground=(unsigned long)7; /* 30 */
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  }
  else {
    Dougee_GCValues->foreground=(unsigned long)7; /* 1 */
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  }
}

void XLine(u,v,w,x,y)
int u,v,w,x,y;
{
  CLine(u,v,w,x,y);
  if (y==0) {
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x+1][y],YMap[x+1][y]);
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x][y],YMap[x][y]+u);
  }
  else if (y==(dougeen-1)) {
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x+1][y],YMap[x+1][y]);
  }
  else if (YMap[x][y] <= YMap[x][y-1]) {
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x+1][y],YMap[x+1][y]);
  }
}

void YLine(u,v,w,x,y)
int u,v,w,x,y;
{
  CLine(u,v,w,x,y);
  if (x==0) {
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x][y+1],YMap[x][y+1]);
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x][y],YMap[x][y]+u);
  }
  else if (x==dougeen-1) {
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x][y+1],YMap[x][y+1]);
  }
  else if (YMap[x][y] <= YMap[x-1][y]) {
    XDrawLine(Dougee_Display,Dougee_Drawable,Drawing_GC,XMap[x][y],YMap[x][y],
	      XMap[x][y+1],YMap[x][y+1]);
  }
}

void DrawMap()
{
  int u,v,w,x,y,shit,hell;

  Initialize(getmaxx,getmaxy);
  GetMaxZ();

XMax=YMax=0;XMin=YMin=MAXINT;
for (shit=0;shit<=dougeen;shit++) {
  for (hell=0;hell<=dougeen;hell++) {
    if (XMap[shit][hell]>XMax) XMax=XMap[shit][hell];
    if (YMap[shit][hell]>YMax) YMax=YMap[shit][hell];
    if (XMap[shit][hell]<XMin) XMin=XMap[shit][hell];
    if (YMap[shit][hell]<YMin) YMin=YMap[shit][hell];
  }
}
/*printf("     MAXES\n%d,%d,%f\n",XMax,YMax,maxz);*/
/*printf("     MINS\n%d,%d,0.0\n",XMin,YMin);*/
Rangex=XMax-XMin;Rangey=YMax-YMin;
/*printf("Range of x %d\nRange of y %d\n",Rangex,Rangey);*/
for (shit=0;shit<=dougeen;shit++) {
  for (hell=0;hell<=dougeen;hell++) {
    XMap[shit][hell]=(int)((float)XMap[shit][hell]*((float)getmaxx/(float)Rangex))+(getmaxx/2);
    YMap[shit][hell]=(int)((float)YMap[shit][hell]*((float)getmaxy/(float)Rangey))+(getmaxy/2);
  }
}

  for (x=dougeen-1;x>=0;x--) {
    for (y=dougeen-1;y>=0;y--) {
      if (ZMap[x][y] > 0.0) u=(int)(ZMap[x][y]+15.5);
      else u=15;
      if ((y<dougeen-1) & (x<dougeen-1)) {
	XLine(u,v,w,x,y);
	YLine(u,v,w,x,y);
      }
      if ((y==dougeen-1) & (x<dougeen-1)) XLine(u,v,w,x,y);
      if ((y<dougeen-1) & (x==dougeen-1)) YLine(u,v,w,x,y);
    }
  }
XSync(Dougee_Display,False);
}

void Intro()
{
  arand=1.0;numrand=4;maxlevel=20;DougeeShift=55;seed=667521591;
  oldh=.8;oldsigma=33.3;dougeen=128;getmaxx=1000;getmaxy=760;DougeeScale=5.0;
  printf("\n\n           The Fractal Landscapes\n");
/*  printf("\nThe severity of the Landscapes are determined by\n");*/
/*  printf("the formula   Dimensions = 3 - H\n");*/
/*  printf("\nTypical values for H are 0.2 to 0.9 with 0.8 suggested\n");*/
  printf("    Please input H "); scanf("%f",&oldh);
/*  printf("Individual details are averaged using an initial standard\n");*/
/*  printf("deviation termed Sigma\n");*/
/*  printf("\nTypical values are 10.1 to 40.4 with 33.3 suggested\n");*/
  printf("    Please input Sigma "); scanf("%f",&oldsigma);
  printf("\n\nGrid Density? "); scanf("%d",&dougeen);
  oldh=0.8;oldsigma=33.3;theta=45.0*.0174524;alpha=90.0*.0174524;
/*  dougeen=2048;*/
/*  printf("\nWindow size horizontal? "); scanf("%d",&getmaxx);*/
/*  printf("\nWindow size vertical? "); scanf("%d",&getmaxy);*/
/*  printf("\nDegree shift? "); scanf("%d",&DougeeShift);*/
/*  printf("\nScale factor? "); scanf("%f",&DougeeScale);*/
  switch (dougeen) {
  case  4 : DougeeShift=21;DougeeScale=1.0;break;
  case  8 : DougeeShift=21;DougeeScale=1.0;break;
  case 16 : DougeeShift=21;DougeeScale=1.0;break;
  case 32 : DougeeShift=21;DougeeScale=1.0;break;
  case 64 : DougeeShift=23;DougeeScale=1.0;break;
  case 128 : DougeeShift=25;DougeeScale=5.0;break;
  case 256 : DougeeShift=30;DougeeScale=5.0;break;
  case 512 : DougeeShift=70;DougeeScale=20.0;break;
/*  case 640 : DougeeShift=70;DougeeScale=50.0;break;*/
  case 1024 : DougeeShift=30;DougeeScale=50.0;break;
  case 2048 : DougeeShift=30;DougeeScale=10.0;break;
  }
}

void MidPointFM2D()
{
  int a,i,j,k,l,x,y,d,b,stage,addition;

  InitGauss();InitZMap();addition=True;delta=sigma;ZMap[0][0]=delta*Gauss();
  ZMap[0][dougeen-1]=delta*Gauss(); ZMap[dougeen-1][0]=delta*Gauss();
  ZMap[dougeen-1][dougeen-1]=delta*Gauss(); d=dougeen; b=dougeen/2; /* -1 */

  for (stage=1;stage<=maxlevel;stage++) {
printf("\nstage %d, D is %d, b is %d",stage,d,b);

if (d==0) {
  maxlevel=stage-1;
  goto hell;
/*d=1;*/
}
    delta=delta*(float)pow(0.5,0.5*h);
    x=b;
    while (x<=dougeen-b) {
      y=b;
      while (y<=dougeen-b) {
	ZMap[x][y]=F4(delta,ZMap[x+b][y+b],ZMap[x+b][y-b],ZMap[x-b][y+b],
		      ZMap[x-b][y-b]);
	y+=d;
      } /*  while y */
      x+=d;
    } /*  while x */
/*printf("\n  Checkpoint 1");*/
    if (addition) {
      x=0;
      while (x<dougeen) {
	y=0;
	while (y<dougeen) {
	  ZMap[x][y]+=delta*Gauss();
	  y+=d;
	}  /* while y */
	x+=d;
      } /* while x */
    } /* if addition */
/*printf("\n  Checkpoint 2");*/
    delta=delta*(float)pow(0.5,0.5*h); x=b;
    while (x<=(dougeen-b)) {
      ZMap[x][0]=F3(delta,ZMap[x+b][0],ZMap[x-b][0],ZMap[x][b]);
      ZMap[x][dougeen]=F3(delta,ZMap[x+b][dougeen],ZMap[x-b][dougeen],
			  ZMap[x][dougeen-b]);
      ZMap[0][x]=F3(delta,ZMap[0][x+b],ZMap[0][x-b],ZMap[b][x]);
      ZMap[dougeen][x]=F3(delta,ZMap[dougeen][x+b],ZMap[dougeen][x-b],
			  ZMap[dougeen-b][x]);
      x+=d;
    } /* while x */
/*printf("\n  Checkpoint 3");*/
    x=b;
    while (x<=(dougeen-b)) {
      y=d;
      while (y<=(dougeen-b)) {
	ZMap[x][y]=F4(delta,ZMap[x][y+b],ZMap[x][y-b],ZMap[x+b][y],
		      ZMap[x-b][y]);
	y+=d;
      } /* while y */
      x+=d;
    } /* while x */
/*printf("\n  Checkpoint 4");*/
    x=d;
    while (x<=(dougeen-b)) {
      y=b;
      while (y<=(dougeen-b)) {
	ZMap[x][y]=F4(delta,ZMap[x][y+b],ZMap[x][y-b],ZMap[x+b][y],ZMap[x-b][y]);
	y+=d;
      } /* while y */
      x+=d;
    } /* while x */
/*printf("\n  Checkpoint 5");*/
    if (addition) {
      x=0;
      while (x<=dougeen) {
	y=0;
	while (y<=dougeen) {
	  ZMap[x][y]+=delta*Gauss();
	  y+=d;
	} /* while y */
	x+=d;
      } /* while x */
/*printf("\n  Checkpoint 6");*/
      x=b;
      while (x<=(dougeen-b)) {
	y=b;
	while (y<=(dougeen-b)) {
	  ZMap[x][y]+=delta*Gauss();
	  y+=d;
	} /* while y */
	x+=d;
      } /* while x */
    } /* if addition */
    d=d/2;
    b=b/2;
/*printf("\n  Checkpoint 7");*/
  } /* for stage */
hell:printf("\ndone with midpoint\n");
}

void Show()
{
int DougeeCount;

/*for(DougeeCount=71;DougeeCount<=80;DougeeCount++) {*/
/*printf("\nDougeeCount is %d",DougeeCount);*/
/*srand48(DougeeCount);*/
  sigma=oldsigma; h=oldh;
  InitMap();
  MidPointFM2D();
  AddMap();
  DrawMap();
/*printf("Done\n");scanf("%s",choice);*/
/*}*/
}

void InitZMap()
{
  int i,j;

  for (i=0;i<dougeen;i++)
    for (j=0;j<dougeen;j++)
      ZMap[i][j]=0.0;
}

void Initialize(nx,ny)
int nx,ny;
{
  int i,j,colorinc;

  if ((Dougee_Display = XOpenDisplay("dredd:0.0")) == NULL) {
    printf("display not opened"); exit(2);
  }

  Dougee_Screen=XDefaultScreen(Dougee_Display);
  Dougee_Visual=XDefaultVisual(Dougee_Display,Dougee_Screen);
  Dougee_Depth=XDefaultDepth(Dougee_Display,Dougee_Screen);
  num_entries_colormap=XDisplayCells(Dougee_Display,0);
  num_entries_colormap=64;

/*  force colors */

  Dougee_Colormap=XCreateColormap(Dougee_Display,DefaultRootWindow(Dougee_Display),
				  Dougee_Visual,AllocNone);
  XAllocColorCells(Dougee_Display,Dougee_Colormap,False,(unsigned long *) NULL,
		   (unsigned int) 0, pixels, (unsigned int) num_entries_colormap);

  NewColors=(XColor *)malloc(num_entries_colormap*sizeof(XColor));
  TempNewColors=(XColor *)malloc(sizeof(XColor));
  TempNewColors=NewColors; colorinc=65535/num_entries_colormap;

  TempNewColors=NewColors;

  /* color 0 */ /* black */

  TempNewColors->pixel=(unsigned long)0;
  TempNewColors->red=(unsigned long)0;
  TempNewColors->green=(unsigned long)0;
  TempNewColors->blue=(unsigned long)0;
  TempNewColors++;

  /* color 1 */ /* white */

  TempNewColors->pixel=(unsigned long)1;
  TempNewColors->red=(unsigned long)65535;
  TempNewColors->green=(unsigned long)65535;
  TempNewColors->blue=(unsigned long)65535;
  TempNewColors++;

  /* color 2 */ /* done */ /* water */

  TempNewColors->pixel=(unsigned long)2;
  TempNewColors->red=(unsigned long)0;
  TempNewColors->green=(unsigned long)0;
  TempNewColors->blue=(unsigned long)65535;
  TempNewColors->flags=DoRed | DoGreen | DoBlue;
  TempNewColors++;

  /* color 3 */ /* done */ /* first ground level */
  
  TempNewColors->pixel=(unsigned long)3;
  TempNewColors->red=(unsigned long)0;
  TempNewColors->green=(unsigned long)65535;
  TempNewColors->blue=(unsigned long)0;
  TempNewColors->flags=DoRed | DoGreen | DoBlue;
  TempNewColors++;

  /* color 4 */

  TempNewColors->pixel=(unsigned long)4;
  TempNewColors->red=(unsigned long)0;
  TempNewColors->green=(unsigned long)49151;
  TempNewColors->blue=(unsigned long)0;
  TempNewColors->flags=DoRed | DoGreen | DoBlue;
  TempNewColors++;

  /* color 5 */

  TempNewColors->pixel=(unsigned long)5;
  TempNewColors->red=(unsigned long)0;
  TempNewColors->green=(unsigned long)32767;
  TempNewColors->blue=(unsigned long)0;
  TempNewColors->flags=DoRed | DoGreen | DoBlue;
  TempNewColors++;

  /* color 6 */

  TempNewColors->pixel=(unsigned long)6;
  TempNewColors->red=(unsigned long)20303;   /* 0 */  /* 12079 */
  TempNewColors->green=(unsigned long)20303;   /* 16383 */
  TempNewColors->blue=(unsigned long)20303;  /* 0 */
  TempNewColors->flags=DoRed | DoGreen | DoBlue;
  TempNewColors++;

  /* color 7 */ /* done */ /* top */

  TempNewColors->pixel=(unsigned long)7;
  TempNewColors->red=(unsigned long)65535;
  TempNewColors->green=(unsigned long)65535;
  TempNewColors->blue=(unsigned long)65535;
  TempNewColors->flags=DoRed | DoGreen | DoBlue;
  TempNewColors++;


  TempNewColors=NewColors;
  XStoreColors(Dougee_Display,Dougee_Colormap,TempNewColors,num_entries_colormap);
  XInstallColormap(Dougee_Display,Dougee_Colormap);

/*  end of force color */

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
  XMoveWindow(Dougee_Display,Dougee_Window,1,1); /*XSync(Dougee_Display,False);*/
  Dougee_Drawable=Dougee_Window; Dougee_GCValues=(XGCValues *)malloc(sizeof(XGCValues));
  Dougee_GCValues->foreground=0; Dougee_GCValues->background=1;
  Drawing_GC=XCreateGC(Dougee_Display,Dougee_Drawable,GCForeground | GCBackground,
		       Dougee_GCValues);


}

#define P(N,phi,y,z) ((float)N-1.0)*(y*sin(phi)+z*cos(phi))

void DisplayHeightField()
{
  int N, i, j, k, horizon, p0, p1, p, COLORS, colorinc;
  float phi, Intens[520][520], x[520], y[520], ih;
  float MaxIntensity, MinIntensity, ColorScale;

/*  stuff for I() */
  LightPhi=45.0*0.0174533;
  LightTheta=330.0*0.0174533;
  ViewPhi=90.0*0.0174533;


  N=dougeen; phi=30.0*.01745;
  for (i=0;i<=N-1;i++) {
    x[i]=(float)i/((float)N-1.0);
    y[i]=(float)i/((float)N-1.0);
  }

/*  this is CHEAP */
  for (i=0;i<N;i++)
    for (j=0;j<N;j++)
      if (ZMap[i][j]<0.0) ZMap[i][j]=0.0;

  for (i=0;i<=N-1;i++) {
    for (k=0;k<1200;k++) { Intens[i][k]=0.0; }
    p0=P(N,phi,y[0],ZMap[i][0]);
/*printf("Dcheck1\n");*/
    Intens[0][p0]=I(x[i],y[0],i,0);
    horizon=p0;
    for (j=1;j<=N-1;j++) {
      p1=(int)P((float)N,phi,y[j],ZMap[i][j]);
      if (p1>horizon) {
/*printf("Dcheck2\n");*/
	Intens[i][p1]=I( x[i], y[j], i, j );
	p=p1-1;
	while (p>horizon) {
	  ih=(p-p0)/(p1-p0);
/*printf("Dcheck3\n");*/
	  Intens[i][p]=(1.0-ih)*I(x[i],y[j-1],i,j-1)+ih*Intens[i][p1];
	  p--;
	}  /* while */
	horizon=p1;
      } /* if */
      p0=p1;
    }
  }

/* Let's see what types of values we get for Intens */

  for (i=0;i<N;i++)
    for (j=0;j<(2*N);j++) {
      if (Intens[i][j]>MaxIntensity) MaxIntensity=Intens[i][j];
      if (Intens[i][j]<MinIntensity) MinIntensity=Intens[i][j];
    }
  printf("\nMax Intensity:  %f; Min Intensity:  %f\n",MaxIntensity,
	 MinIntensity);

  ColorScale=num_entries_colormap/MaxIntensity;
  printf("%d %f %d\n",num_entries_colormap,ColorScale,N);

  printf("Done 2\n");scanf("%s",choice);

/* Install New Colormap */
  TempNewColors=NewColors;colorinc=65535/num_entries_colormap;
  for (i=0,j=65535;i<num_entries_colormap;i++,j-=colorinc,TempNewColors++) {
    TempNewColors->pixel=(unsigned long)i;
    TempNewColors->red=(unsigned long)j;
    TempNewColors->green=(unsigned long)j;
    TempNewColors->blue=(unsigned long)j;
    TempNewColors->flags= DoRed | DoGreen | DoBlue;
  }
  TempNewColors=NewColors;
  XStoreColors(Dougee_Display,Dougee_Colormap,TempNewColors,num_entries_colormap);
  XSync(Dougee_Display,False);

/* Blank out the window */

  Dougee_GCValues->foreground=(unsigned long)0;
  XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
  for(i=0;i<getmaxx;i++)
    for(j=0;j<getmaxy;j++)
      XDrawPoint(Dougee_Display,Dougee_Drawable,Drawing_GC,i,j);

/* plot the fractal */
  
  for (COLORS=1;COLORS<num_entries_colormap;COLORS++) {
    Dougee_GCValues->foreground=(unsigned long)(COLORS);
    XChangeGC(Dougee_Display,Drawing_GC,GCForeground,Dougee_GCValues);
    for (i=0;i<520;i++) {
      for (j=0;j<(1200);j++) {
	if ((int)(ColorScale*Intens[i][j])==COLORS) {
	  if (COLORS>0) printf(" (%d,%d) ",i,j);
	  XDrawPoint(Dougee_Display,Dougee_Drawable,Drawing_GC,j,i);
	}
      }
    }
  }

printf("\n");

}

float I(x,y,i,j)
float x,y;
int i,j; /* subscripts of current ZMap */
{
  float Ia, Fd, Fs, intensity;
  float dx,dy,dz,r,b;
  float CosTheta,CosAlpha;
  struct Vector { float x,y,z };
  struct Vector normal, light, view, reflected;

  Ia=0.5; Fd=0.6; Fs=0.4; b=2.0;

  light.x=sin(LightTheta)*cos(LightPhi);
  light.y=sin(LightTheta)*sin(LightPhi);
  light.z=cos(LightTheta);

  view.x=0.0;
  view.y= -cos(ViewPhi);
  view.z=sin(ViewPhi);

  dx=ZMap[i][j] - ZMap[i+1][j];
  dy=ZMap[i][j-1] - ZMap[i][j];
  dz=1.0/((float)dougeen-1.0);
  r=sqrt(dx*dx+dy*dy+dz*dz);
  normal.x=dx/r; normal.y=dy/r; normal.z=dz/r;

  CosTheta=normal.x*light.x + normal.y*light.y + normal.z*light.z;

  if (CosTheta<0.0) {
    return(Ia);
  } else {
    reflected.x=(normal.x-light.x)*(2.0*CosTheta);
    reflected.y=(normal.y-light.y)*(2.0*CosTheta);
    reflected.z=(normal.z-light.z)*(2.0*CosTheta);
    CosAlpha=view.x*reflected.x+view.y*reflected.y+view.z*reflected.z;
    intensity=Ia + (Fd*CosTheta) + (Fs*pow(CosAlpha,b));
    return(intensity);
  }

}
