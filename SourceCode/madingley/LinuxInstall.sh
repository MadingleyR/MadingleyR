#!/usr/bin/env bash 

cd dist_$(date +%Y%m%d)_linux/
mv ../../../Package/inst/lin_exec/madingley ../../../Package/inst/lin_exec/madingley_old
cp madingley ../../../Package/inst/lin_exec/madingley
cd ../
echo "files copied to MadingleyR/Package/inst/lin_exec"
