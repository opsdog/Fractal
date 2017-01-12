#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <X11/Xlib.h>  /* includes <X11/X.h> */

#include "OpenWindow.h"

void openwindow_(nx,ny,ReturnDisplay,ReturnDrawable,ReturnGC,ReturnGCValues,ReturnWindow)
int *nx,*ny,*ReturnDisplay,*ReturnDrawable,*ReturnGC,*ReturnGCValues,*ReturnWindow;
{
  int i,j,Width,Height;
  char ToDisplay[100];

  printf("Where you be? ");scanf("%s",ToDisplay);
  strcat(ToDisplay,":0.0");

  if ((Dougee_Display = XOpenDisplay(ToDisplay)) == NULL) {
    printf("\n\ndisplay not opened\n\n"); exit(2);
  }

  Width= *nx; Height= *ny;

  Dougee_Screen=XDefaultScreen(Dougee_Display);
  Dougee_Visual=XDefaultVisual(Dougee_Display,Dougee_Screen);
  Dougee_Depth=XDefaultDepth(Dougee_Display,Dougee_Screen);
  num_entries_colormap=XDisplayCells(Dougee_Display,0);
printf("there be %d colours\n",num_entries_colormap);
/*  num_entries_colormap=80;*/

#include "../Stuff/Choice.c"

  Dougee_Attributes=(XSetWindowAttributes *)malloc(sizeof(XSetWindowAttributes));
  border_width=1;x=0;y=0;Dougee_Attributes->backing_store=Always;
  Dougee_Attributes->background_pixel=0;
  Dougee_Attributes->colormap=Dougee_Colormap;
  Dougee_Attributes->override_redirect=False;
  Dougee_Attributes->event_mask=ButtonPressMask;

  Dougee_Window = XCreateWindow(Dougee_Display,DefaultRootWindow(Dougee_Display),
				x,y,Width,Height,border_width,Dougee_Depth,InputOutput,
				(Visual *) CopyFromParent,
	     CWBackPixel | CWEventMask | CWColormap | CWBackingStore | CWOverrideRedirect,
				Dougee_Attributes);

  XMapWindow(Dougee_Display,Dougee_Window); XFlush(Dougee_Display);sleep(2);
  XMoveWindow(Dougee_Display,Dougee_Window,1,1); XSync(Dougee_Display,False);

  Dougee_Drawable=Dougee_Window; Dougee_GCValues=(XGCValues *)malloc(sizeof(XGCValues));
  Dougee_GCValues->foreground=0; Dougee_GCValues->background=1;
  Drawing_GC=XCreateGC(Dougee_Display,Dougee_Drawable,GCForeground | GCBackground,
		       Dougee_GCValues);

*ReturnDisplay=(int)Dougee_Display;
*ReturnDrawable=(int)Dougee_Drawable;
*ReturnGC=(int)Drawing_GC;
*ReturnGCValues=(int)Dougee_GCValues;
*ReturnWindow=(int)Dougee_Window;
}

void plotpoint_(plotx,ploty,IntDisplay,IntDrawable,IntGC)
int *plotx,*ploty,*IntDisplay,*IntDrawable,*IntGC;
{
  Display *DisplayToUse; Drawable DrawableToUse; GC GCToUse;
  XGCValues *GCValuesToUse;

  DisplayToUse=(Display *)*IntDisplay; DrawableToUse=(Drawable)*IntDrawable;
  GCToUse=(GC)*IntGC;

  XDrawPoint(DisplayToUse,DrawableToUse,GCToUse,*plotx,*ploty);
}

void setcolour_(colour,IntDisplay,IntGC,IntGCValues)
int *colour,*IntDisplay,*IntGC,*IntGCValues;
{
  Display *DisplayToUse; Drawable DrawableToUse; GC GCToUse;
  XGCValues *GCValuesToUse;

  DisplayToUse=(Display *)*IntDisplay; GCValuesToUse=(XGCValues *)*IntGCValues;
  GCToUse=(GC)*IntGC;

  GCValuesToUse->foreground=(unsigned long)(*colour);
  XChangeGC(DisplayToUse,GCToUse,GCForeground,GCValuesToUse);
}

void tester_(IntDisplay,IntDrawable,IntGC,IntGCValues)
int *IntDisplay,*IntDrawable,*IntGC,*IntGCValues;
{
  Display *DisplayToUse; Drawable DrawableToUse;
  GC GCToUse; XGCValues *GCValuesToUse;

  DisplayToUse=(Display *)*IntDisplay; GCValuesToUse=(XGCValues *)*IntGCValues;
  DrawableToUse=(Drawable)*IntDrawable; GCToUse=(GC)*IntGC;
}

void closewindow_(IntDisplay,IntWindow)
int *IntDisplay,*IntWindow;
{
  Display *DisplayToUse; Window WindowToUse;

  DisplayToUse=(Display *)*IntDisplay; WindowToUse=(Window)*IntWindow;

  XSync(DisplayToUse,False);

  printf("Done\n");scanf("%s",choice);

  XDestroyWindow(DisplayToUse,WindowToUse);
  XSync(DisplayToUse,(int) 1);
}
