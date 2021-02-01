vs_src = function(wd){
  cd_base_dir = paste("cd",wd)
  cd_src_dir = "cd src"
  system(paste(cd_base_dir,"&&",cd_src_dir,"&& code ."))
}
