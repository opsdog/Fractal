/*****************************************************************************
/* FILE		: xlmntn.h
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

/*****************************************/
/* CONFIGURE THIS SECTION AS REQUIRED... */
/*****************************************/

/*#define FNTFILE         "fg-18"*/
#define FNTFILE         "fixed"
#define PROGTITLE	"Xlmntn"

/* The argument list processing structure... */
#define NUMARGS	16
struct opts	args[NUMARGS] = {
{"-display",	"-ds", "",		"-display serverstring"},
{"-foreground", "-fg", "white",		"-foreground colour"},
{"-background", "-bg", "red",		"-background colour"},
{"-textfg",  	"-tf", "white",		"-textfg  colour"},
{"-textbg",  	"-tb", "blue",		"-textbg  colour"},
{"-seacolour",  "-sc", "blue",		"-seacolour  colour"},
{"-mntncolour", "-mc", "forestgreen",	"-mntncolour colour"},
{"-numcolours", "-nc", "50",		"-numcolours integer"},
{"-colours",	"-cl", "g",		"-colours r|g|b"},
{"-fontfile",	"-fn", FNTFILE,		"-fontfile fontfile"},
{"-H",		"-hg", "0.9",		"-H double"},
{"-scale", 	"-sl", "2.5",		"-scale double"},
{"-level",	"-lv", "7",		"-level integer[3-~8]"},
{"-mntndistance","-md", "100",		"-mntndistance integer"},
{"-scrndistance","-sd", "500",		"-scrndistance integer"},
{"-height",	"-ht", "120",		"-height  integer"},
};

/*****************************************/
/* END OF CONFIGURATION SECTION...       */
/*****************************************/

#define PNTS(i,px,py)    {pnts[(i)].x=(px);pnts[(i)].y=(py);}
#define PIXELX(x,y)	((((x)-square/2)*scrndist)/(mntndist+(y))+(screenwidth/2))
#define	PIXELY(x,y,e)	(zeroy - ((e)-obsheight)*scrndist/(mntndist+(y)))

char    *progtitle = PROGTITLE;
char    *bckgrnd   = BCKGRND;
char    *iconfile  = ICONFILE;

int     zeroy;
int     obsheight;
int	scrndist, mntndist;

extern int	screenwidth, screenheight;
extern int	square, yscale;
extern struct mntn {
    double      **elevs;
    int         **px;
    int         **py;
} mntndata;
extern int	basey;
extern Window	wndw;
extern GC	maingc, seagc;
extern Display	*dsply;
extern long	mc;
