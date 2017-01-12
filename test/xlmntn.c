/*****************************************************************************
/* FILE		: xlmntn.c
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
#include "xlmntn.h"

main(argc,argv)
int	argc;
char	*argv[];
{
    xpt_getargs(args,NUMARGS,argc,argv);
    init();
    gen_mntndata(mntndata.elevs,square, (double)(square-5));
    elevs2coords();
    process_events();			/* Main event-processing loop. */
}

init()
{
    init_cmn();
    obsheight = atoi(ARGS(13));
    if ((mntndist = atoi(ARGS(13))) < 1)
	mntndist = 200;
    if ((scrndist = atoi(ARGS(14))) < 1)
	scrndist = 2000;
    screenwidth = square*scrndist/mntndist;
    screenheight= screenwidth;
    zeroy = screenheight - obsheight*scrndist/mntndist;
    init_X();
}

elevs2coords()
{
register int	x,y;

    basey = square;
    DEBUG(("Converting elevations to Y-coordinates...\n"));

    for (y=0; y<square; y++) {
	for (x=0; x<square; x++) {
	    if (mntndata.elevs[x][y] <= 0.0)
		mntndata.elevs[x][y] = 0.000000;
	    mntndata.px[x][y] = PIXELX(x,y);
	    mntndata.py[x][y] = PIXELY(x,y,(int)floor(mntndata.elevs[x][y]));
	}
    }
}

display()
{
    draw_grid();
    draw_heights();
}

draw_grid()
{
XPoint	pnts[4];

/* Colour the main grid ('the sea') polygon. */
    PNTS(0, PIXELX(0,0),	   PIXELY(0,0,0));
    PNTS(1, PIXELX(square,0),	   PIXELY(square,0,0));
    PNTS(2, PIXELX(square,square), PIXELY(square,square,0));
    PNTS(3, PIXELX(0,square),	   PIXELY(0,square,0));
    XFillPolygon(dsply,wndw,seagc,pnts,4,Nonconvex,CoordModeOrigin);
}

draw_heights()
{
register int	x,y;
XPoint		pnts[5];

    for (y=square-1; y>=0; y--) {
	for (x=0; x<square; x++) {
	    if (y < square-1 && x > 0) {
		if (mntndata.elevs[x][y] > 0   || mntndata.elevs[x-1][y] > 0 ||
		    mntndata.elevs[x][y+1] > 0 || mntndata.elevs[x-1][y+1] >0) {

		    PNTS(0, mntndata.px[x][y],     mntndata.py[x][y]);
		    PNTS(1, mntndata.px[x-1][y],   mntndata.py[x-1][y]);
		    PNTS(2, mntndata.px[x-1][y+1], mntndata.py[x-1][y+1]);
		    PNTS(3, mntndata.px[x][y+1]  , mntndata.py[x][y+1]);
		    draw_face(pnts, mc);

		    if (y == 0) {
			PNTS(2, mntndata.px[x-1][y], PIXELY(x-1,y,0));
			PNTS(3, mntndata.px[x][y]  , PIXELY(x  ,y,0));
			draw_face(pnts, -1);
		    }
		} /* if elevs... */
	    } /* if y */
	}
    }
}
