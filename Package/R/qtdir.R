qtdir = function(dir) {


  switch(Sys.info()[['sysname']],
  Windows = {
    return(paste0('\\',paste0('"',dir,'\\\"')))
  },
  Linux = {
    return(paste0('"',dir,'"'))
  },
  Darwin = {
    return(paste0('"',dir,'"'))
  } )





}
