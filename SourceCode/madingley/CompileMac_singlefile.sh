#!/usr/bin/env bash 

mkdir dist_$(date +%Y%m%d)_mac
cd src
/usr/local/bin/g++-7 -std=c++11 -w -c -g -fopenmp -static-libgcc -static-libstdc++ $1;


mv *.o binaries_mac
cd binaries_mac
/usr/local/bin/g++-7 -fopenmp -static-libgcc -static-libstdc++ -o madingley *.o
mv madingley ../../dist_$(date +%Y%m%d)_mac/
cd ../../
echo "single source compiled"

cd dist_$(date +%Y%m%d)_mac/
cp madingley /Users/osx/Documents/GitHub/MadingleyR/inst/mac_exec/madingley
cp madingley /Library/Frameworks/R.framework/Versions/3.6/Resources/library/MadingleyR/mac_exec/madingley
cd ../
echo "files copied to R package dir and installation dir"