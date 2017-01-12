#!/bin/ksh
if [ -z "$1" ]
then
  echo "Need a frame name, dipshit!!"
  exit
fi
if [ -z "$2" ]
then
  Aspect="VGA"
else
  Aspect=$2
fi
make clean >/dev/null 2>&1
make
if [ ! -x ./XPot+Mandel ]
then
  exit
fi
echo
echo
./XPot+Mandel
echo
echo
./Merge-XPot+Mandel XPot+Mandel-${1}-640-${Aspect}-Fractal-1.fractal XPot+Mandel-${1}-640-${Aspect}-Fractal-2.fractal
for i in XPot+Mandel-${1}-640-${Aspect}-Fractal-?.values
do 
echo
echo
  ~/bin/PNGFromValues-CMap18 $i
done

