rm_temp_results = function(){
  system(paste("cd",tempdir(),"&& rm -rf madingley_outs*"))
  system(paste("cd",tempdir(),"&& rm -rf input"))
  system(paste("cd",tempdir(),"&& rm -rf spatial_inputs"))
}