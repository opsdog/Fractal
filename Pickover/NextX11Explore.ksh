#!/bin/ksh -x
rm -f X11Explore X11Explore.o 
gfortran -o X11Explore X11Explore.f
./X11Explore 
rm -f X11Explore
mv X11Explore-array1-CMap99.png X11Explore-array1-CMap99-old.png
~/bin/PNGFromValues X11Explore-array1.values
unset DYLD_LIBRARY_PATH
open X11Explore-array1-CMap99*.png # ../Science/Blocky-640-VGA-CMap99.png 
