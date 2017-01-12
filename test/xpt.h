/*****************************************************************************
/* FILE         : xpt.h
/* DATE         : August 25, 1989.
/* AUTHOR       : Paul Sharpe @ DEC, Reading (OSCR-Europe).
/* FUNCTION     : header file to 'toolkit' of common functions.
/*
/*   Copyright (c) Digital Equipment Corporation 1990  All rights reserved.
/*   Copyright is claimed in the computer program and user interface thereof.
/*
/*   Digital Equipment Corporation cannot accept any responsibility for
/*   use, misuse, or abuse of this software.
/*
/*****************************************************************************/

#define DEFDSPLY        "unix:0.0"

#define DEBUG(x)        {printf x; fflush(stdout);}
#define ROOTWNDW(d)	RootWindow((d),DefaultScreen((d)))
#define DEFDEPTH(d)	DefaultDepth((d),DefaultScreen((d)));

#define TRUE	1
#define ON	1
#define YES	1
#define	SET	1

#define	FALSE	0
#define OFF	0
#define NO	0
#define UNSET	0

#define ARGS(X)	(args[(X)].valstr)
struct opts {
    char        flagstr[32];	/* Full argument flag string */
    char	aflagstr[8];	/* Abbreviated argument flag string */
    char	*valstr;	/* Argument command line value. */
    char	*usage;		/* Usage descriptor. */
};
