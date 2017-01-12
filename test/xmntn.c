/*****************************************************************************
/* FILE		: xmntn.c
/* AUTHOR	: Paul Sharpe @ DEC (OSCR-Europe, Reading, England).
/* DATE		: July 20, 1989
/* FUNCTION	: X-windows Fractal Brownian-Motion mountains.
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
#include "xmntn.h"

main(argc,argv)
int	argc;
char	*argv[];
{
    xpt_getargs(args,NUMARGS,argc,argv);
    init();
    gen_mntndata(mntndata.elevs,square, (double)(yscale*square - 5));
    elevs2coords();
    process_events();			/* Main event-processing loop. */
}

init()
{
    init_cmn();
    if ((xscale = atoi(ARGS(13))) < 1)
        xscale = 1;
    if ((yscale = atoi(ARGS(14))) < 1)
        yscale = 1;
    screenwidth  = square*(xscale*2)+(XOFFSET*3);
    screenheight = square*yscale*3;
    basey = yscale*3*square;
    init_X();
}

elevs2coords()
{
register int	i,j, x,y, rowy;

    DEBUG(("Converting elevations to Y-coordinates..."));

/* To speed up, convert elevations for those points having the same Y-pixel
 * coordinate: i.e. those points on the diagonals.
 */
    for (i=0; i<square; i++) {
	rowy = PIXELY(i,0) - HEIGHT;
	for (j=0; j<=i; j++) {
	    x = i - j;
	    y = j;
	    mntndata.px[x][y] = PIXELX(x,y);
	    mntndata.py[x][y] = rowy - ((mntndata.elevs[x][y] <= (double)0.0)? 0:(int)floor(mntndata.elevs[x][y]));
	}
    }
    for (i=1; i<square; i++) {
	rowy = PIXELY(square-1,i) - HEIGHT;
	for (j=square-1; j>=i; j--) {
	    x = j;
	    y = i + square - j - 1;
	    mntndata.px[x][y] = PIXELX(x,y);
	    mntndata.py[x][y] = rowy - ((mntndata.elevs[x][y] <= (double)0.0)? 0:(int)floor(mntndata.elevs[x][y]));
	}
    }
    DEBUG(("DONE\n"));
}

display()
{
    draw_grid();
    draw_heights();
}

draw_grid()
{
register int	i;
static XPoint	pnts[6];

/* Colour the main grid ('the sea') polygon. */
    PNTS(0, PIXELX(square-1,square-1),	PIXELY(square-1,square-1));
    PNTS(1, PIXELX(square-1,0),		PIXELY(square-1,0));
    PNTS(2, pnts[1].x,			pnts[1].y - HEIGHT);
    PNTS(3, pnts[0].x,			PIXELY(0,0) - HEIGHT);
    PNTS(4, PIXELX(0,square-1),		pnts[2].y);
    PNTS(5, pnts[4].x,			pnts[1].y);
    XFillPolygon(dsply,wndw,seagc,pnts,6,Nonconvex,CoordModeOrigin);
    
    for (i=0; i<square; i++) {
	PNTS(0, PIXELX(square-1, i),	PIXELY(square-1, i));
	PNTS(1, pnts[0].x,		pnts[0].y - HEIGHT);
	PNTS(2, PIXELX(0, i),		PIXELY(0, i) - HEIGHT);
	XDrawLines(dsply,wndw,maingc,pnts,3,CoordModeOrigin);

	PNTS(0, PIXELX(i, square-1),	PIXELY(i, square-1));
	PNTS(1, pnts[0].x,		pnts[0].y - HEIGHT);
	PNTS(2, PIXELX(i, 0),		PIXELY(i, 0) - HEIGHT);
	XDrawLines(dsply,wndw,maingc,pnts,3,CoordModeOrigin);
    }

/* Bottom two lines of the block. */
    PNTS(0, PIXELX(0,square-1),		PIXELY(0,square-1));
    PNTS(1, PIXELX(square-1,square-1),	PIXELY(square-1,square-1));
    PNTS(2, PIXELX(square-1,0),		PIXELY(square-1,0));
    XDrawLines(dsply,wndw,maingc,pnts,3,CoordModeOrigin);
}

/******************************************************************************
 * Fill the quadilateral faces along lines parallel to a far edge.
 ******************************************************************************/
draw_heights()
{
register int	x,y;
static XPoint	pnts[4];

    for (x=0; x<square; x++)
	for (y=0; y<square; y++)
	    if (x>0 && y>0)
		if (mntndata.elevs[x][y] > 0   || mntndata.elevs[x-1][y] > 0 ||
		    mntndata.elevs[x][y-1] > 0 || mntndata.elevs[x-1][y-1] > 0){
		    PNTS(0, mntndata.px[x][y],    mntndata.py[x][y]);
		    PNTS(1, mntndata.px[x][y-1],  mntndata.py[x][y-1]);
		    PNTS(2, mntndata.px[x-1][y-1],mntndata.py[x-1][y-1]);
		    PNTS(3, mntndata.px[x-1][y],  mntndata.py[x-1][y]);
		    draw_face(pnts,mc);
		}
}

#ifdef NOTUSED
/******************************************************************************
 * Fill the quadilateral faces along diagonal lines (parallel to the viewer).
 ******************************************************************************/
heights_by_diagonal()
{
register int	x,y;
static XPoint	pnts[5];

/* Top diagonal half of the complete square. */
    for (i=0; i<square; i++) {
	for (j=0; j<=i; j++) {
	    if ((x = i - j) > 0 && (y = j) > 0)
		if (mntndata.elevs[x][y] > 0   || mntndata.elevs[x-1][y] > 0 ||
		    mntndata.elevs[x][y-1] > 0 || mntndata.elevs[x-1][y-1] > 0){
		    PNTS(0, mntndata.px[x][y],    mntndata.py[x][y]);
		    PNTS(1, mntndata.px[x-1][y],  mntndata.py[x-1][y]);
		    PNTS(2, mntndata.px[x-1][y-1],mntndata.py[x-1][y-1]);
		    PNTS(3, mntndata.px[x][y],  mntndata.py[x][y]);
		    draw_face(pnts,mc);
		}
	}
    }

/* Bottom diagonal half of the complete square. */
    for (i=1; i<square; i++) {
	for (j=square-1; j>=i; j--) {
	    if ((x = j) > 0 && (y = i + square - j - 1) > 0)
		if (mntndata.elevs[x][y] > 0   || mntndata.elevs[x-1][y] > 0 ||
		    mntndata.elevs[x][y-1] > 0 || mntndata.elevs[x-1][y-1] > 0){
		    PNTS(0, mntndata.px[x][y],    mntndata.py[x][y]);
		    PNTS(1, mntndata.px[x-1][y],  mntndata.py[x-1][y]);
		    PNTS(2, mntndata.px[x-1][y-1],mntndata.py[x-1][y-1]);
		    PNTS(3, mntndata.px[x][y],  mntndata.py[x][y]);
		    draw_face(pnts,mc);
		}
	}
    }
}
#endif
