#!/usr/bin/env bash 

# install correct compiler using:
# brew install gcc@7

mkdir dist_$(date +%Y%m%d)_mac
cd src
for file in $(ls *.cpp)
do
	echo $file;
	/usr/local/bin/g++-7 -std=c++11 -w -c -g -fopenmp -static-libgcc -static-libstdc++ $file;

done

mv *.o binaries_mac
cd binaries_mac
/usr/local/bin/g++-7 -fopenmp -static-libgcc -static-libstdc++ -o madingley *.o
mv madingley ../../dist_$(date +%Y%m%d)_mac/
cd ../../
echo "source compiled"


cd dist_$(date +%Y%m%d)_mac/
mv ../../../Package/inst/mac_exec/madingley ../../../Package/inst/mac_exec/madingley_old
cp madingley ../../../Package/inst/mac_exec/madingley
cd ../
echo "files copied to MadingleyR/Package/inst/mac_exec"
