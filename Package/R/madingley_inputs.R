madingley_inputs = function(input_type = "print options") {
  
  # input arg options
  input_options = c("spatial inputs","cohort definition","stock definition","model parameters","print options")
  
  # for debugging of "spatial inputs" only
  if(FALSE) input_options = "spatial inputs"

  # stop if incorrect input is provided
  if(!is.element(input_type, input_options)){
    stop("Incorrect input argument, possible input arguments are:",paste0('"',input_options[1:4],'";'))
  }

  # list options
  if(input_type=="print options"){
    cat("possible input arguments are:",paste0('"',input_options[1:4],'";'))
  }

  if(input_type=="spatial inputs") {
    
    output = list()
    
    # old code with raster package
    # pkg = installed.packages()[, "Package"]
    # if(!"rgdal" %in% pkg) stop("Package 'rgdal' not installed") else require(rgdal)
    # if(!"raster" %in% pkg) stop("Package 'raster' not installed") else require(raster)
    
    spatial_path=paste0(get_lib_path(),"/spatial_input_rasters")

    file_names = list.files(spatial_path,full.names = TRUE,recursive = TRUE)
    list_names = gsub("\\..*","",list.files(spatial_path,full.names = FALSE,recursive = TRUE))

    FILES_sp = c("realm_classification","land_mask","hanpp","available_water_capacity",
                 "Ecto_max","Endo_C_max","Endo_H_max","Endo_O_max")
    FILES_sp_temp = c("terrestrial_net_primary_productivity","near-surface_temperature",
                      "precipitation","ground_frost_frequency","diurnal_temperature_range")

    # check if all files are there
    for(i in FILES_sp){
      if(length(grep(i,file_names,value = TRUE))!=1) stop("Could not find raster: ",i,".tif \n")
    }
    for(i in FILES_sp_temp){
      if(length(grep(i,file_names,value = TRUE))!=12){
        stop("Could not find raster all 12 monthly rasters containing data on: ",i,"\n")
      }
    }

    cat('Reading default input rasters from: ',spatial_path)

    # read sp non-temp files
    i = FILES_sp[1]
    for(i in FILES_sp){
      file_name = grep(i,file_names,value =TRUE)
      cat(".")
      #output[[i]] = raster::raster(file_name)
      output[[i]] = terra::rast(file_name)
    }

    # read sp temp files
    i = FILES_sp_temp[1]
    for(i in FILES_sp_temp){
      file_name = grep(i,file_names,value =TRUE)
      # sort files
      if(length(grep("_1.tif",file_name,value =TRUE))==0){
        file_name_sort = file_name
      }else{
        if(substr(spatial_path,nchar(spatial_path),nchar(spatial_path))=="/"){
          file_name_sort = paste0(spatial_path,i,"_",1:12)
        }else{
          file_name_sort = paste0(spatial_path,"/",i,"_",1:12,".tif")
        }
      }
      if(length(file_name_sort)==12){
        # output[[i]] = raster::brick(lapply(file_name_sort,raster::raster))
        output[[i]] = terra::rast(file_name_sort)
      }
      cat(".")
    }
    cat("\n")

    return(output)


  }else if(input_type=="cohort definition"){

    return(get_default_cohort_def())

  }else if(input_type=="stock definition"){

    return(get_default_stock_def())

  }else if(input_type=="model parameters"){

    return(get_default_model_parameters())

  }
}
