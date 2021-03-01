madingley_compile = function(madingley_path, install_new_binary=FALSE) {
  
  current_wd = getwd() # store current wd
  
  if(grepl("src", madingley_path)){
    setwd(madingley_path)
    setwd("../")
  }else{
    setwd(madingley_path)
  }
  
  switch(Sys.info()[['sysname']],
         Windows= {
           cat("Compiling Madingley not possible on Windows\n")
           },
         Linux  = {
              if(!any(grepl("CompileLinux.sh",list.files()))){
                
                 stop("CompileLinux.sh not found")
              
              }else{
                 cat("Compiling Madingley C++ source code on Linux\n")
                 system("sh CompileLinux.sh")
              }
           
              if(install_new_binary) {
                 cat("installing binary into MadingleyR package library: \n")
                 cat(get_lib_path())
                 
                 # copies the compiled source to package library
                 install = paste0("cd dist_$(date +%Y%m%d)_linux/"," && ","cp madingley ",get_lib_path(),"/lin_exec/madingley")
                 system(install)
              }
    
           },
         Darwin = {
           cat("Compiling Madingley not possible on Mac\n")
           system(madingley_make)
           }
         )
  
  setwd(current_wd) # change wd back

}