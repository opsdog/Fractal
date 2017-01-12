###############################################################################
#*
#*   Copyright (c) Digital Equipment Corporation 1990  All rights reserved.
#*   Copyright is claimed in the computer program and user interface thereof.
#*
#*   Digital Equipment Corporation cannot accept any responsibility for
#*   use, misuse, or abuse of this software.
#*
###############################################################################

ICONFILE= -DICONFILE="\"/home/icomp01/fsgreen/fractal/test/mntn.btm\""
BCKGFILE= -DBCKGRND="\"/home/icomp01/fsgreen/fractal/test/bg.btm\""
CFLAGS= -O ${ICONFILE} ${BCKGFILE}

STRIP=touch

HFILES= xpt.h xlmntn.h xmntn.h

CMNO=	xpt.o gen_mntndata.o cmn_mntncode.o
CMNC=	xpt.c gen_mntndata.c cmn_mntncode.c
XLO=	xlmntn.o
XLC=	xlmntn.c
XO=	xmntn.o
XC=	xmntn.c

BTMS=	bg.btm mntn.btm

SHAR= xmntns.sh

PROGS = xmntn xlmntn

X11=	-lX11

all:	${PROGS}

xmntn:	${CMNO} ${XO}
	cc -O -o $@ ${CMNO} ${XO} ${X11} -lm
	@${STRIP} $@
	@ls -l $@

xlmntn:	${CMNO} ${XLO}
	cc -O -o $@ ${CMNO} ${XLO} ${X11} -lm
	@${STRIP} $@
	@ls -l $@

xpt.o:		xpt.h
xmntn.o:	xmntn.h
xlmntn.o:	xlmntn.h

clean:
	rm -f *.o ${PROGS} core

shar:
	shar README Makefile ${HFILES} ${CMNC} ${XC} ${XLC} ${BTMS} > ${SHAR}
	@ls -l ${SHAR}
