list_output_paths = function(madingley_data,full_path=T){

  if(is.null(madingley_data$out_path)) stop('No outputs available, out_dir was not specified in madingley_run?')
  if(!dir.exists(madingley_data$out_path)) stop('No outputs available, out_dir was not specified in madingley_run?')

  out_path = madingley_data$out_path

  # remove slashes from out_dir
  if(substr(out_path,(nchar(out_path)+1)-1,nchar(out_path))=='/')  out_path=substr(out_path,1,nchar(out_path)-1)
  if(substr(out_path,(nchar(out_path)+1)-1,nchar(out_path))=='\\') out_path=substr(out_path,1,nchar(out_path)-1)

  # create return list
  out_path_main = paste0(out_path,madingley_data$out_dir)
  return_list = list()

  # add cohort_properties
  if(dir.exists(paste0(out_path_main,'cohort_properties'))){
    temp = list.files(paste0(out_path_main,'cohort_properties'),pattern= "csv",full.names = full_path)
    if(length(temp)!=0) return_list$cohort_properties = temp
  }

  # add stock_properties
  if(dir.exists(paste0(out_path_main,'stock_properties'))){
    temp = list.files(paste0(out_path_main,'stock_properties'),pattern= "csv",full.names = full_path)
    if(length(temp)!=0) return_list$stock_properties = temp
  }

  # add consumption_statistics
  if(dir.exists(paste0(out_path_main,'consumption_statistics'))){
    temp = list.files(paste0(out_path_main,'consumption_statistics'),pattern= "csv",full.names = full_path)
    if(length(temp)!=0) return_list$consumption_statistics = temp
  }

  # add grid_properties
  if(dir.exists(paste0(out_path_main,'grid_properties'))){
    temp = list.files(paste0(out_path_main,'grid_properties'),pattern= "csv",full.names = full_path)
    if(length(temp)!=0) return_list$grid_properties = temp
  }

  # add timelines
  if(dir.exists(paste0(out_path_main,'timelines'))){
    temp = list.files(paste0(out_path_main,'timelines'),pattern= "csv",full.names = full_path)
    if(length(temp)!=0) return_list$timelines = temp
  }

  return(return_list)
  
}
