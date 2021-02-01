compile_src = function(wd) {
  
  cd_base_dir = paste("cd",wd)
  cd_src_dir = "cd src"
  make = "make" 
  madingley_make = paste(cd_base_dir,"&&",cd_src_dir,"&&",make)
  
  switch(Sys.info()[['sysname']],
         Windows= {
           print("Compiling Madingley not possible on Windows")
           },
         Linux  = {
           print("Compiling Madingley C++ source code on Linux")
           system(madingley_make)
           },
         Darwin = {
           print("Compiling Madingley C++ source code on MacOS")
           system(madingley_make)
           }
         )

}