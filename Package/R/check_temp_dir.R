check_temp_dir = function(){
  if(Sys.info()[['sysname']]=="Darwin"){
    system(paste("open",tempdir()))
  }else{
    cat(tempdir())
  }
}