/****************************************************************************
/* FILE:	gen_mntndata.c
/* AUTHOR:	Paul Sharpe @ DEC, OSCR_Europe, reading, England.
/* DATE:	October 11, 1989.
/* INSPIRATION:	'The Science Of factal Images'
/*
/*   Copyright (c) Digital Equipment Corporation 1990  All rights reserved.
/*   Copyright is claimed in the computer program and user interface thereof.
/*
/*   Digital Equipment Corporation cannot accept any responsibility for
/*   use, misuse, or abuse of this software.
/*
/****************************************************************************/

#include <stdio.h>
#include <math.h>

#include "xpt.h"

#define F3(delta,x0,x1,x2)      ((x0+x1+x2)/3 + delta*Gauss())
#define F4(delta,x0,x1,x2,x3)   ((x0+x1+x2+x3)/4 + delta*Gauss())

int		level;
char		addition = 1;
double		sigma, H;
static int	Nrand, Arand;
static double	GaussFac, GaussAdd;
double		Gauss();
static double	numer, denom; /* Replaces use of GaussFac for precision... */

gen_mntndata(elevs, square, top)
double	**elevs, top;
int	square;
{
    MidPointFM2D(elevs,level,sigma,H,addition,(int)time((long *)0));
    scale(elevs, square, top);
}

scale(elevs, square, top)
double	**elevs;
int	square;
double	top;
{
register int	x,y;
double		max = 0.0;

    for (x=0; x<square; x++)
	for (y=0; y<square; y++)
	    if (elevs[x][y] > max)
		max = elevs[x][y];
    DEBUG(("Scaling: %f -> %f.\n",max,top));
    if (max > 0.0)			/* Just in case summat goes wrong... */
	for (x=0; x<square; x++)
	    for (y=0; y<square; y++)
		elevs[x][y] = elevs[x][y]*top/max;
}

/*==========================================================================
 * Routines 'interpreted' from the code in 'The Science Of Fractal Images.'
 *==========================================================================*/
InitGauss(seed)
int	seed;
{
    Nrand = 4;
    Arand = (1<<31)-1;		/* random() gives results in [0,Arand] */

    GaussAdd = (double)sqrt((double)(3.0*Nrand));
/* NB: GaussFac is no longer used: it loses too much precision!
 *   GaussFac = (double)(2.0 * GaussAdd / ((double)Nrand * (double)Arand));
 */
/* NB: We have to do this, as we seem to loose too much precision in the method
 *     laid out in 'The Scienc Of Fractal Images.'
 */
    numer = GaussAdd + GaussAdd;
    denom = (double)((double)Nrand * (double)Arand);

    srandom(seed);
}

double
Gauss()
{
register int	i;
register double	sum = 0;
extern long	random();

/* See the above comment in InitGauss()... */
    for (i=0; i<Nrand; i++)
	sum += (double)random();
    sum = numer*(sum/denom);
    return(sum - GaussAdd);
}

MidPointFM2D(X,maxlevel,sigma,H,addition,seed)
double	**X;
int	maxlevel;
double	sigma;
double	H;
char	addition;
int	seed;
{
register int	i, N, x,y, D,d;
double		delta;

    InitGauss(seed);
    N = 1<<maxlevel;
    DEBUG(("MidPointFM2D: (Dim:%dx%d) (Sigma:%f) (H:%f).\n",N,N,sigma,H));

    delta = sigma;
    X[0][0] = delta * Gauss();
    X[0][N] = delta * Gauss();
    X[N][0] = delta * Gauss();
    X[N][N] = delta * Gauss();

    D = N;
    d = N/2;

    for (i=0; i<maxlevel; i++) {

/* Going from Grid type I to Grid type II */
	delta = delta * pow((double)0.5,(double)(0.5*H));
	for (x=d; x<=N-d; x+=D)		/* Interpolate and offset points. */
	    for (y=d; y<=N-d; y+=D)
		X[x][y] = F4(delta,X[x+d][y+d],X[x+d][y-d],X[x-d][y+d],X[x-d][y-d]);

/* Displace other points also if needed. */
	if (addition == 1)
	    for (x=0; x<=N; x+=D)
		for (y=0; y<=N; y+=D)
		    X[x][y] = X[x][y] + delta*Gauss();

/* Going from Grid type II to Grid type I */
	delta = delta * pow(0.5,0.5*H);
/* Interpolate and offset boundary grid points. */
	for (x=d; x<=N-d; x+=D) {
	    X[x][0] = F3(delta,X[x+d][0],X[x-d][0],X[x][d]);
	    X[x][N] = F3(delta,X[x+d][N],X[x-d][N],X[x][N-d]);
	    X[0][x] = F3(delta,X[0][x+d],X[0][x-d],X[d][x]);
	    X[N][x] = F3(delta,X[N][x+d],X[N][x-d],X[N-d][x]);
	}

/* Interpolate and offset interior grid points. */
	for (x=d; x<=N-d; x+=D)
	    for (y=D; y<=N-d; y+=D)
		X[x][y] = F4(delta,X[x][y+d],X[x][y-d],X[x+d][y],X[x-d][y]);

	for (x=D; x<=N-d; x+=D)
	    for (y=d; y<=N-d; y+=D)
		X[x][y] = F4(delta,X[x][y+d],X[x][y-d],X[x+d][y],X[x-d][y]);

/* Displace other points also if needed. */
	if (addition == 1) {
	    for (x=0; x<=N; x+=D)
		for (y=0; y<=N; y+=D)
		    X[x][y] = X[x][y] + delta*Gauss();
	    for (x=d; x<=N-d; x+=D)
		for (y=d; y<=N-d; y+=D)
		    X[x][y] = X[x][y] + delta*Gauss();
	}

	D = D/2;
	d = d/2;
    }
}
