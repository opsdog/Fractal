/*****************************************************************************
/* FILE		: cmn_mntncode.c
/* AUTHOR	: Paul Sharpe @ DEC (OSCR-Europe, Reading, England).
/* DATE		: July 20, 1989
/* FUNCTION	: Common code in the mountain-generating programs.
/* INSPIRATION	: 'The Science Of Fractal Images.'
/*
/*   Copyright (c) Digital Equipment Corporation 1990  All rights reserved.
/*   Copyright is claimed in the computer program and user interface thereof.
/*
/*   Digital Equipment Corporation cannot accept any responsibility for
/*   use, misuse, or abuse of this software.
/*
/*****************************************************************************/

#include <stdio.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "xpt.h"

#define	RECALC		"New Mountains"
#define QUIT		"Quit"

#define PNTS(i,px,py)	{pnts[(i)].x=(px);pnts[(i)].y=(py);}

#define ABS(x)		(((x)<0) ? -(x) : (x))
#define SW(s)           (XTextWidth(fs,(s),strlen((s))))

int	screenwidth, screenheight;
int	basey;
int	square;

struct mntn {
    double	**elevs;
    int		**px;
    int		**py;
} mntndata;
extern double	sigma, H;
extern int	level;

extern char	*calloc();
extern char	*bckgrnd, *progtitle, *iconfile;

Display		*dsply;		/* The display connected to. */
Window		wndw;		/* The window in which to display */
Window		hwndw,swndw, twndw, rwndw, qwndw;
GC 		maingc, mntngc,seagc, bttngc;
unsigned long	mc;		/* Mountain colour(s?) */
Colormap	clmap;		/* colourmap for colour display. */
Pixmap		pntrbtm;
Font		fnt;
XFontStruct	*fs;
int		wdth, hght;
int		depth;		/* Display depth: no. of planes. */
int		numcols;
int		mntndist, scrndist;

static char	*titles[] = {"Brownian Motion","Mountains",
			     "Paul Sharpe @ DEC"};

extern struct opts     args[];

init_cmn()
{
    sigma = atof(ARGS(11));	/* Scaling, or standard deviation. */
    H = atof(ARGS(10));		/* Sets the fractal dimension. */
    numcols = atoi(ARGS(7));
    if ((level = atoi(ARGS(12))) < 1)
	level = 1;
    square = (1<<level) + 1;
 
    init_arrays();
}

init_arrays()
{
register int	i;

    mntndata.elevs =(double **)calloc(square,(unsigned)sizeof(double *));
    mntndata.px = (int **)calloc(square,(unsigned)sizeof(int *));
    mntndata.py = (int **)calloc(square,(unsigned)sizeof(int *));
    for (i=0; i<square; i++) {
        mntndata.elevs[i] = (double *)calloc(square,(unsigned)sizeof(double));
        mntndata.px[i] = (int *)calloc(square,(unsigned)sizeof(int));
        mntndata.py[i] = (int *)calloc(square,(unsigned)sizeof(int));
    }
}

process_events()
{
    for(;;)
	do_event();
}

do_event()
{
XEvent		event;

    XNextEvent(dsply, &event);
    switch (event.type) {
	case Expose:
	case GraphicsExpose:
	    if (event.xexpose.count == 0)
        	do_expose((XExposeEvent *)&event);
	    break;
	case ButtonPress:	/* Get a new Dimension, and redisplay! */
	    do_bttn((XButtonPressedEvent *)&event);
	    break;
	default:
	    break;
    }
}

do_expose(event)
XExposeEvent    *event;
{
    if (event->window == qwndw)
	XDrawString(dsply,qwndw,bttngc,2,fs->ascent+2,QUIT,strlen(QUIT));
    if (event->window == rwndw)
	XDrawString(dsply,rwndw,bttngc,2,fs->ascent+2,RECALC,strlen(RECALC));
    if (event->window == swndw)
	draw_scale();
    if (event->window == hwndw)
	draw_H();
    if (event->window == twndw)
	draw_titles();
    if (event->window == wndw)
	display();
}

do_bttn(bttn)
XButtonPressedEvent     *bttn;
{
    if (bttn->window == qwndw)
	exit(1);
    if (bttn->window == rwndw) {
	gen_mntndata(mntndata.elevs,square, (double)(square-5));
	elevs2coords();
	XClearWindow(dsply,wndw);
	display();
    }
    if (bttn->window == swndw) {
	if (bttn->x < 300)
	    H = (double)((double)(bttn->x)/(double)100.0);
	XClearWindow(dsply,swndw);
	XClearWindow(dsply,hwndw);
	draw_scale();
	draw_H();
    }
}

draw_face(pnts, col)
XPoint	pnts[];
int	col;		/* Colour for the filled area. */
{
int		dy;
static long	lastcol = -1;

/* Pseudo-shading determined by gradient of the face. */
    if (col >= 0 && depth > 1 && numcols > 1) {
	dy = (int)(pnts[1].y - pnts[3].y);
	col = (pnts[0].y - pnts[2].y) + (20 - ABS(dy));
        if (col >= numcols)
	    col = numcols - 1;
    }
    if (col < 0)
	col = 0;

/* Try to speed up the display by only setting colours as necessary... */
    if (col != lastcol) {
	XSetForeground(dsply,mntngc,col);
	lastcol = col;
    }
    XFillPolygon(dsply,wndw,mntngc,pnts,4,Nonconvex,CoordModeOrigin);

/* Draw the gridcolour lines around the filled quadilateral... */
    if (numcols <= 1 || depth <= 1) {
	pnts[4].x = pnts[0].x;
	pnts[4].y = pnts[0].y;
	XDrawLines(dsply,wndw,maingc,pnts,5,CoordModeOrigin);
    }
}

init_X()
{
int             x,y, len;
Font            fnt;
extern Window	xpt_window();
extern Display  *xpt_open_display();

    dsply = xpt_open_display(ARGS(0));

    depth = DEFDEPTH(dsply);

/* If we have multi-planes, allow for some shading of the mountains. */
    if (depth == 1 || numcols <= 1)
	clmap = DefaultColormapOfScreen(DefaultScreenOfDisplay(dsply));
    else {
	clmap = XCreateColormap(dsply,ROOTWNDW(dsply),DefaultVisualOfScreen(DefaultScreenOfDisplay(dsply)),AllocNone);
	set_cols(numcols);
    }
    set_GCs();

    x = screenwidth + 20;
    y = screenheight + 20;
    DEBUG(("Creating window: %dx%d.\n",x,y));
    wndw  = xpt_window(dsply,ROOTWNDW(dsply),0,0,x,y,0);
    XSetWindowColormap(dsply,wndw,clmap);
    len = DisplayWidth(dsply,DefaultScreen(dsply));
    if (x > len)
	x = len;
    xpt_windowcols(dsply,wndw,maingc,clmap,ARGS(2),"white",bckgrnd);

    init_font();

    len = SW("0.000000");
    hwndw = xpt_window(dsply,wndw,x-len-20,10,len,hght+2,ButtonPressMask);
    xpt_windowcols(dsply,hwndw,bttngc,clmap,ARGS(4),ARGS(3),NULL);
    swndw = xpt_window(dsply,wndw,x-len-300-20,10,300,hght+2,ButtonPressMask);
    xpt_windowcols(dsply,swndw,bttngc,clmap,ARGS(4),ARGS(3),NULL);
    set_pointer();		/* Create a Pixmap holding the arrow pointer. */

/* BUTTON: Recalculate and draw new mountains. */
    len = SW(RECALC) + 2;
    rwndw = xpt_window(dsply,wndw,x-len-20,hght+20,len,hght+4,ButtonPressMask);
    xpt_windowcols(dsply,rwndw,bttngc,clmap,ARGS(4),ARGS(3),NULL);

/* BUTTON: Quit from mountain program. */
    len = SW(QUIT) + 2;
    qwndw = xpt_window(dsply,wndw,x-len-20,2*hght+30,len,hght+4,ButtonPressMask);
    xpt_windowcols(dsply,qwndw,bttngc,clmap,ARGS(4),ARGS(3),NULL);

/* Title block for the mountains, author etc. */
    len = XTextWidth(fs,titles[2],strlen(titles[2])) + 4;
    twndw = xpt_window(dsply,wndw,10,10,len,3*hght+4,ButtonPressMask);
    xpt_windowcols(dsply,twndw,bttngc,clmap,ARGS(4),ARGS(3),NULL);

    XMapSubwindows(dsply,wndw);
    XMapWindow(dsply,wndw);

    xpt_icon(dsply,wndw,progtitle,iconfile);
}

init_font()
{
    fnt = XLoadFont(dsply,ARGS(9));
    XSetFont(dsply,bttngc,fnt);
    fs = XQueryFont(dsply,fnt);
    wdth = fs->max_bounds.width;
    hght = 2 + fs->ascent + fs->descent;
}

set_GCs()
{
    maingc = XDefaultGC(dsply,DefaultScreen(dsply));
    XSetFunction(dsply,maingc,GXcopy);
    XSetPlaneMask(dsply,maingc,AllPlanes);
    XSetGraphicsExposures(dsply,maingc,True);
    XSetForeground(dsply, maingc, xpt_colour(dsply,ARGS(1),clmap));
    XSetBackground(dsply, maingc, xpt_colour(dsply,ARGS(2),clmap));

    bttngc = XCreateGC(dsply,ROOTWNDW(dsply),0,0);
    XCopyGC(dsply,maingc,-1L,bttngc);
    XSetForeground(dsply, bttngc, xpt_colour(dsply,ARGS(3),clmap));
    XSetBackground(dsply, bttngc, xpt_colour(dsply,ARGS(4),clmap));

    seagc = XCreateGC(dsply,ROOTWNDW(dsply),0,0);
    XCopyGC(dsply,maingc,-1L,seagc);
    XSetForeground(dsply, seagc, xpt_colour(dsply,ARGS(5),clmap));

    mntngc = XCreateGC(dsply,ROOTWNDW(dsply),0,0);
    XCopyGC(dsply,maingc,-1L,mntngc);
    XSetForeground(dsply, mntngc,(mc = xpt_colour(dsply,ARGS(6),clmap)));
}

set_cols(numcols)
int	numcols;
{
int		i, dc;
char		str[32], fmt[32];
extern char	*index();

/* Spread the colours out along the available spectrum... */
    sprintf(fmt,"#%s%s%s",(index(ARGS(8),'r')==NULL)?"0000":"%04x",
			  (index(ARGS(8),'g')==NULL)?"0000":"%04x",
			  (index(ARGS(8),'b')==NULL)?"0000":"%04x");
    if (numcols > 1) {
	dc = 65000/numcols;
	for (i=1; i<=numcols; i++) {
	    sprintf(str,fmt,i*dc,i*dc,i*dc);
	    xpt_colour(dsply,str,clmap);
	}
    }
}

draw_titles()
{
int	i, y;

    for (i=0; i<3; i++) {
	y = i*hght + fs->ascent + 1;
	XDrawString(dsply,twndw,bttngc,2,y,titles[i],strlen(titles[i]));
    }
}

draw_scale()
{
int	x, i;

    for (i=100; i<=200; i+=100) {
	XDrawLine(dsply,swndw,bttngc,i,0,i,5);
	XDrawLine(dsply,swndw,bttngc,i,hght,i,hght-5);
    }
    XDrawLine(dsply,swndw,bttngc,300,0,300,hght);
    x = (int)floor(H * (double)100.0) - (hght/2);
    XCopyArea(dsply,pntrbtm,swndw,bttngc,0,0,hght,hght,x,0);
}

draw_H()
{
char	str[16];

    sprintf(str,"%f",H);
    XDrawString(dsply,hwndw,bttngc,2,fs->ascent+2,str,strlen(str));
}

set_pointer()
{
XPoint	pnts[7];

    pntrbtm = XCreatePixmap(dsply,wndw,hght,hght,depth);
    if (pntrbtm == 0) {
	fprintf(stderr,"FATAL: Can't create pointer pixmap!\n");
	exit(2);
    }
/* Set the foreground to the normal button text background colour. */
    XSetForeground(dsply, bttngc, xpt_colour(dsply,ARGS(4),clmap));
/* Clear the Pixmap by filling it as a rectangle with the background colour. */
    XFillRectangle(dsply, pntrbtm,bttngc,0,0,hght,hght);
/* Reset the normal button text foreground colour. */
    XSetForeground(dsply, bttngc, xpt_colour(dsply,ARGS(3),clmap));
    pnts[0].x = hght/2;		pnts[0].y = 0;
    pnts[1].x = 0;		pnts[1].y = hght/2;
    pnts[2].x = hght/4;		pnts[2].y = pnts[1].y;
    pnts[3].x = pnts[2].x;	pnts[3].y = hght;
    pnts[4].x = 3*hght/4;	pnts[4].y = pnts[3].y;
    pnts[5].x = pnts[4].x;	pnts[5].y = pnts[1].y;
    pnts[6].x = hght;		pnts[6].y = pnts[5].y;
    XFillPolygon(dsply,pntrbtm,bttngc,pnts,7,Nonconvex,CoordModeOrigin);
}
