madingley_download_source = function(madingley_path) {
  
  current_wd = getwd() # store current wd
  
  wd = madingley_path
  if(substr(wd,(nchar(wd)+1)-1,nchar(wd))=='/')  wd=substr(wd,1,nchar(wd)-1)
  if(substr(wd,(nchar(wd)+1)-1,nchar(wd))=='\\') wd=substr(wd,1,nchar(wd)-1)
  
  zip_path = paste0(wd,"/MadingleyR.zip")
  rasters_path = paste0(wd,"/MadingleyR-master")
  
  if(!file.exists(paste0(wd,"/MadingleyR.zip"))){
    cat('Downloading zip from github repository: ')
    download.file(url = "https://github.com/MadingleyR/MadingleyR/archive/master.zip", destfile = zip_path); 
  }else{
    cat('Zip already downloaded \n')
  }
  if(!file.exists(paste0(wd,"/MadingleyR-master"))){
    cat('Extracting zip \n')
    setwd(wd)
    unzip('MadingleyR.zip')
  }else{
    cat('Zip already extracted \n')
  }
  
  return(paste0(wd,"/MadingleyR-master/","SourceCode/madingley"))
  
  setwd(current_wd) # change wd back

}