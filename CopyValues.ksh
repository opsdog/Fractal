#!/bin/ksh
##
##  copy all values files to 01-Values
##

for Dir in [A-Z]* [a-z]*
do
  if [ -d $Dir ]
  then
    find $Dir -name "*.values" -exec cp -p {} 01-Values/ \;
    find $Dir -name "*.values.gz" -exec cp -p {} 01-Values/ \;
  fi
done
