/*****************************************************************************
/* FILE		: xpt.c
/* DATE		: August 25, 1989.
/* AUTHOR	: Paul Sharpe @ DEC, Reading (OSCR-Europe).
/* FUNCTION	: 'toolkit' of common functions.
/*
/*   Copyright (c) Digital Equipment Corporation 1990  All rights reserved.
/*   Copyright is claimed in the computer program and user interface thereof.
/*
/*   Digital Equipment Corporation cannot accept any responsibility for
/*   use, misuse, or abuse of this software.
/*
/*****************************************************************************/

#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "xpt.h"

char	*prog;

Display *
xpt_open_display(str)
char	*str;
{
extern char	*getenv();
Display		*dsply;

    if (str == (char *)NULL || *str == '\0')
	if ((str = getenv("DISPLAY")) == NULL)
	    str = DEFDSPLY;
    dsply = XOpenDisplay(str);
    if (dsply == (Display *)NULL) {
	fprintf(stderr,"Sorry - can't open display '%s'\n",str);
	exit(1);
    }
    return(dsply);
}

xpt_getargs(args, numargs, argc,argv)
struct	opts	args[];
int		numargs;
int		argc;
char		*argv[];
{
int	i, gotflag = 0;

    prog = *argv;
    while (--argc > 0) {
	gotflag = 0;
	if (**++argv != '-')
	    xpt_usage(args,numargs,"Value with no flag: %s",*argv);
	for (i=0; i<numargs; i++) {
	    if (strncmp(*argv,args[i].flagstr,strlen(*argv))==0 ||
	        strcmp(*argv,args[i].aflagstr)==0) {

/* Have we already had a match for this argument string ? */
		if (gotflag == 1)
		    xpt_usage(args,numargs,"Ambiguous flag: %s",*argv);
		if (args[i].valstr != (char *)TRUE && args[i].valstr != (char *)FALSE) {
/* We want an argument value from the next argv component. */
		    ++argv;
		    if (--argc <= 0)
			xpt_usage(args,numargs,"Flag with no value: %s",*argv);
		    args[i].valstr = *argv;
		}
		gotflag = 1;
	    }
	}
	if (gotflag == 0)
	    xpt_usage(args,numargs,"Unknown flag: %s",*argv);
    }
}

xpt_usage(args,numargs, fmt,str)
struct opts	args[];
int		numargs;
char		*fmt, *str;
{
int	i;

    fprintf(stderr,fmt,str);
    fprintf(stderr,"\nUsage: %s \n",prog);
    for (i=0; i<numargs; i++)
	fprintf(stderr,"           %s\n",args[i].usage);
    exit(1);
}

Window
xpt_window(dsply,parent,x,y,sx,sy,mask)
Display	*dsply;
Window	parent;
int	x,y,sx,sy,mask;
{
Window			new;
XSetWindowAttributes	attrs;

    attrs.event_mask = mask | ExposureMask;

    new = XCreateWindow(dsply,parent,x,y,sx,sy,2,DefaultDepth(dsply,DefaultScreen(dsply)),InputOutput,CopyFromParent,CWEventMask,&attrs);
    if (new == (Window)NULL) {
	fprintf(stderr,"FATAL: Couldn't create window.\n");
	exit(1);
    }
    return(new);
}

xpt_colour(dsply,name,clmap)
Display		*dsply;
char 		*name;
Colormap	clmap;
{
XColor		colour;
static Colormap	defclmap = NULL;

    if (defclmap == (Colormap)NULL)
	defclmap = DefaultColormapOfScreen(DefaultScreenOfDisplay(dsply));
    if (clmap == (Colormap)NULL)
	clmap = defclmap;

    if (XParseColor(dsply,clmap,name,&colour) == BadColor) {
	fprintf(stderr,"WARNING: Failed to parse colour '%s'...\n",name);
	return(0);
    }
    if (XAllocColor(dsply,clmap,&colour) == 0) {
	fprintf(stderr,"FATAL: Failed to alloc colour '%s'...\n",name);
	exit(1);
    }
    return(colour.pixel);
}

xpt_windowcols(dsply,wndw,gc,clmap,bg,bd,tile)
Display		*dsply;
Window		wndw;
GC		gc;
Colormap	clmap;
char		*bg, *bd, *tile;
{
    if (bg != (char *)NULL && *bg != '\0');
	XSetWindowBackground(dsply,wndw,xpt_colour(dsply,bg,clmap));
    if (bd != (char *)NULL && *bd != '\0');
	XSetWindowBorder(dsply,wndw,xpt_colour(dsply,bd,clmap));
    xpt_tile(dsply,wndw,gc,tile);
}

/* Read a bitmap: if colour, convert to a pixmap; set to window background. */
xpt_tile(dsply,wndw,gc,file)
Display	*dsply;
Window	wndw;
GC	gc;
char	*file;
{
int	h,w,x,y, depth;
Pixmap	b, pix;

    if (file == (char *)NULL || *file == '\0')
	return;
    if (XReadBitmapFile(dsply,wndw,file,&w,&h,&b,&x,&y) != BitmapSuccess) {
	fprintf(stderr,"WARNING: Can't load background bitmap '%s'.\n",file);
	return;
    }
    if ((depth = DefaultDepth(dsply,DefaultScreen(dsply))) == 1)
	XSetWindowBackgroundPixmap(dsply,wndw,b);
    else {
	pix = XCreatePixmap(dsply,wndw,w,h,depth);
	XCopyPlane(dsply,b,pix,gc,0,0,w,h,0,0,1);
	XSetWindowBackgroundPixmap(dsply,wndw,pix);
	XFreePixmap(dsply,pix);
    }
}

xpt_icon(dsply,wndw,title,iconfile)
Display	*dsply;
Window	wndw;
char	*title, *iconfile;
{
Pixmap	b;
int	h,w,x,y;
char	name[1][256];

    strcpy(name[0],title);
    if (XReadBitmapFile(dsply,wndw,iconfile,&w,&h,&b,&x,&y) != BitmapSuccess)
	fprintf(stderr,"WARNING: Can't load icon bitmap '%s'.\n",iconfile);
    else
	XSetStandardProperties(dsply,wndw,*name,*name,b,name,0,NULL);
}
