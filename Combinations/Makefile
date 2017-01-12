#
#  Makefile for various fractal programs
#

F77= gfortran
FFLAGS= # -fpic -fPIC -m64 # -v

CC= gcc
CFLAGS= 

##
##  c support file locations
##

INCLUDEDIRS= -I/usr/GNU/include
LIBDIRS= -L/usr/GNU/lib

##
##  NO USER SERVICEABLE PARTS BEYOND THIS POINT
##

.c.o:
	$(CC) -c ${CFLAGS} ${COPTS} ${INCLUDEDIRS} ${DEBUGFLAGS} $<

.f.o:
	$(F77) -c $(FFLAGS) $<

FTargets= XPot+Mandel TestStuff Merge-XPot+Mandel

XPMTargets= XPot.o XMSet.o

all: $(FTargets)

XPot+Mandel: XPot+Mandel.o XPot.o XMSet.o
	$(F77) -o XPot+Mandel XPot+Mandel.o $(XPMTargets) # -fPIC -Wl,-v -Wl,-pie
	chmod 0755 XPot+Mandel && echo

Merge-XPot+Mandel: Merge-XPot+Mandel.o
	$(F77) -o Merge-XPot+Mandel Merge-XPot+Mandel.o $(XPMTargets) # -fPIC -Wl,-v -Wl,-pie
	chmod 0755 Merge-XPot+Mandel && echo

TestStuff: TestStuff.o
	$(F77) -o TestStuff TestStuff.o
	chmod 0755 TestStuff && echo


clean:
	rm -f $(FTargets)
	rm -f *.o
	rm -f *~ #*



#
#  depends relationships
#

XPot+Mandel.o:  XPot+Mandel.f
Merge-XPot+Mandel.o:  Merge-XPot+Mandel.f
XPot.o:  XPot.f
XMSet.o:  XMSet.f
TestStuff.o:  TestStuff.f
