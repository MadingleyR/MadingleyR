#!/usr/bin/env bash 

cd dist_$(date +%Y%m%d)_linux/
mv ../../../Package/inst/lin_exec/madingley ../../../Package/inst/lin_exec/madingley_old
cp madingley ../../../Package/inst/lin_exec/madingley
cp madingley /home/selwyn/R/x86_64-pc-linux-gnu-library/3.6/MadingleyR/lin_exec/madingley
cd ../
echo "files copied to MadingleyR/Package/inst/lin_exec"
