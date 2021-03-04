## Compile source code for Windows (10)

To compile the source code on Windows mingw is required with mingw32-pthreads-w32. see http://mingw-w64.org/doku.php. 

After installing mingw, run the following in ```cmd``` to compile and link the entire source (src) directory. The source code can be downloaded [here](https://github.com/MadingleyR/MadingleyR/archive/master.zip).

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

Next, the compiled executable (madingley.exe) can be found in a folder called ```dist_{dateoftoday}_win```, needs to be moved into the MadingleyR package library (e.g. into ```path/to/pacakge/library/MadingleyR/win_exec```). This will make sure the package will call the new C++ executable. The following R code can be used to obtain the path of the MadingleyR library (function is part of the MadingleyR pacakage):

```R
get_lib_path()
```
