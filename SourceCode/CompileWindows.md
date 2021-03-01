## Compile source code for Windows (10)

To compile the source on Windows mingw is required with mingw32-pthreads-w32. see http://mingw-w64.org/doku.php. 

After installing mingw, run the following in ```cmd``` to compile and link entire source (src) directory 

```bash
cd c:/madingley/
mkdir dist_%date:~-4,4%%date:~-10,2%%date:~7,2%_win
cd src
for /r %i in (*.cpp) do g++ -std=c++11 -w -c -g -fopenmp -static-libgcc -static-libstdc++ %i
g++ -fopenmp -static-libgcc -static-libstdc++ -o madingley.exe *.o
del /S *.o
move madingley.exe ..
cd ..
move /y madingley.exe dist_%date:~-4,4%%date:~-10,2%%date:~7,2%_win
```

Next move the created executable (madingley.exe) into the MadingleyR package library (e.g. into ```MadingleyR/win_exec```). The following R code can be used to obtain the path of the MadingleyR library (function is part of the MadingleyR pacakage):

```R
get_lib_path()
```