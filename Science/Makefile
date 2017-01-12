#
#  Makefile for various fractal programs
#

CC= gcc

CFLAGS= 

##
##  for debugging
##

#  generally enough to debug execution flow problems

DEBUGFLAGS= # -DDEBUG -DSHOWAXIS -DDUMPDATA 

##
##  where to find GNU plot include files
##

INCLUDEDIRS= -I/usr/GNU/include
PLOTLIB= -L/usr/GNU/lib -lplot
PNGLIB= -L/usr/GNU/lib -lm -lpng

##
##  NO USER SERVICEABLE PARTS BEYOND THIS POINT
##

Targets= LandFromValues X11Explore-ArrayDump XPotential-ArrayDump \
	PlotFromValues PNGFromValues

.c.o:
	$(CC) -c ${CFLAGS} ${COPTS} ${INCLUDEDIRS} ${DEBUGFLAGS} $<

all:  ${Targets}

LandFromValues: LandFromValues.o
	$(CC) -o LandFromValues LandFromValues.o 
	chmod 0755 LandFromValues && echo

PlotFromValues: PlotFromValues.o
	$(CC) -o PlotFromValues PlotFromValues.o ${PLOTLIB}
	chmod 0755 PlotFromValues && echo

PNGFromValues: PNGFromValues.o
	$(CC) -o PNGFromValues PNGFromValues.o ${PNGLIB}
	chmod 0755 PNGFromValues 
	cp -p PNGFromValues ~/bin/ && echo

X11Explore-ArrayDump: X11Explore-ArrayDump.o
	$(CC) -o X11Explore-ArrayDump X11Explore-ArrayDump.o 
	chmod 0755 X11Explore-ArrayDump && echo

XPotential-ArrayDump: XPotential-ArrayDump.o
	$(CC) -o XPotential-ArrayDump XPotential-ArrayDump.o 
	chmod 0755 XPotential-ArrayDump && echo



clean:
	rm -f $(Targets)
	rm -f *.o
	rm -f *~ #*

#
#  depends relationships
#

LandFromValues.o:  LandFromValues.c
PlotFromValues.o:  PlotFromValues.c
PNGFromValues.o:  PNGFromValues.c
X11Explore-ArrayDump.o:  X11Explore-ArrayDump.c
XPotential-ArrayDump.o:  XPotential-ArrayDump.c
