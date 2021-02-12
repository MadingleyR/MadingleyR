#!/usr/bin/env bash 
mkdir dist_$(date +%Y%m%d)_linux
cd src
for file in $(ls *.cpp)
do
	echo $file;
	g++ -std=c++11 -w -c -g -fopenmp -static-libgcc -static-libstdc++ $file;

done

g++ -fopenmp -static-libgcc -static-libstdc++ -o madingley *.o
rm *.o
mv madingley ../dist_$(date +%Y%m%d)_linux/
cd ../

cd dist_$(date +%Y%m%d)_linux/
mv ../../../Package/inst/lin_exec/madingley ../../../Package/inst/lin_exec/madingley_old
cp madingley ../../../Package/inst/lin_exec/madingley
cd ../
echo "files copied to MadingleyR/Package/inst/lin_exec"
