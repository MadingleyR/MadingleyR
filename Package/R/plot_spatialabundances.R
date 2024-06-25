# raster plot function
plot_spatialabundances = function(madingley_data,functional_filter=F,plot=TRUE,box=T,axes=T,
                                  names=0,cex_legend_title=1,cex_axis=1.3,legend_mar=20,distance_legend_title=4,legend_width=2.5,
                                  col=c('#fff7ec','#fee8c8','#fdd49e','#fdbb84','#fc8d59','#ef6548','#d7301f','#b30000','#7f0000')){

  # debug code
  if(FALSE){
    madingley_data = mdata2
    col=c('#fff7ec','#fee8c8','#fdd49e','#fdbb84','#fc8d59','#ef6548','#d7301f','#b30000','#7f0000')
  }
  
  
  # get spatial window
  spatial_window=madingley_data$spatial_window

  # check if out_dir was specified manually within madingley_run()
  if(!is.null(madingley_data$out_path)){ # out_dir specified manually
    tdo = madingley_data$out_path
    # remove slashes from out_dir
    if(substr(tdo,(nchar(tdo)+1)-1,nchar(tdo))=='/')  tdo=substr(tdo,1,nchar(tdo)-1)
    if(substr(tdo,(nchar(tdo)+1)-1,nchar(tdo))=='\\') tdo=substr(tdo,1,nchar(tdo)-1)
    if(dir.exists(paste0(tdo,madingley_data$out_dir_name))) {
      out_dir = tdo
      cat(paste0("loading inputs from: ",out_dir,madingley_data$out_dir_name))
    }
  }else{ # use default output dir
    out_dir = tempdir()
    cat(paste0("loading inputs from: ",out_dir,madingley_data$out_dir_name))
  }

  # check if dir exists
  if(!dir.exists(paste0(out_dir,madingley_data$out_dir_name))){ # not exist
    stop("Unable to find output folder")
  }

  # default color pal
  pal = col

  # get cohort data paths
  cohortsRnames = paste0(out_dir,madingley_data$out_dir_name,'cohort_properties/')
  cohortsRnames = grep("FullCohortProperties",list.files(cohortsRnames,full.names=T),value = T)

  # check if required files were exported
  if(!length(cohortsRnames)>1) stop("Required files were not exported during model run")
  
  # create tmp out paths
  cohortsRnames = cohortsRnames[1:(length(cohortsRnames)-1)]
  cohortsRnames = cohortsRnames[(length(cohortsRnames)-11):length(cohortsRnames)] # select last 12 csvs
  csv_out_path = paste0(out_dir,madingley_data$out_dir_name,'Data_proc_abundance.csv')

  if(Sys.info()[['sysname']]=='Windows') {

    # setup exec
    exec_path = paste0('"',get_lib_path(),"/win_exec",'/data_proc_abundance.exe','"')
    exec_command = paste(exec_path,paste0('"%PATH',1:13,'%"', collapse = ' '))

    # create exec bat
    bat_l1 = "ECHO off"
    bat_paths = c(); for(i in 1:12) bat_paths = c(bat_paths,paste0("set PATH",i,"=",paste0(gsub("/", "\\\\", cohortsRnames[i]))))
    bat_out_path = paste0("set PATH",13,"=",paste0(gsub("/", "\\\\", csv_out_path)))
    bat_com = exec_command
    run_exec = paste0(get_lib_path(),"/win_exec/","data_proc_abundance.bat")
    writeLines(c(bat_l1,bat_paths,bat_out_path,bat_com), run_exec)

    # run bat
    run_exec = paste0('"',run_exec,'"')
    log = system(run_exec,intern = TRUE)

  }else if(Sys.info()[['sysname']]=='Darwin'){

    # allow exec to be run
    system(paste0('cd "',get_lib_path(),'/mac_exec"'," && ","chmod u+x data_proc_abundance"))

    # path quotation, fix space issues
    cohortsRnames = paste0('\"',cohortsRnames,'\"')
    csv_out_path_cpp = paste0('\"',csv_out_path,'\"')

    in_files = paste(cohortsRnames, collapse = ' ')
    out_file = csv_out_path_cpp
    command = paste0('cd "',get_lib_path(),'/mac_exec" &&',' ./data_proc_abundance ',in_files,' ',out_file)
    log = system(command,intern = TRUE)

  }else{ # Linux

    # allow exec to be run
    system(paste0('cd "',get_lib_path(),'/lin_exec"'," && ","chmod u+x data_proc_abundance"))

    # path quotation, fix space issues
    cohortsRnames = paste0('\"',cohortsRnames,'\"')
    #cat(cohortsRnames)
    csv_out_path_cpp = paste0('\"',csv_out_path,'\"')
    #cat(csv_out_path_cpp)

    in_files = paste(cohortsRnames, collapse = ' ')
    out_file = csv_out_path_cpp
    command = paste0('cd "',get_lib_path(),'/lin_exec" &&',' ./data_proc_abundance ',in_files,' ',out_file)
    #cat('----------------------------------------------\n')
    #cat(command)
    log = system(command,intern = TRUE)

  }

  # read processed csv from out_dir dir
  d=read.csv(csv_out_path)[,1:3]

  # open raster data
  baseRname = paste(get_lib_path(),'spatial_input_rasters','realm_classification.tif', sep = '/')
  # old code raster pkg: e = raster::extent(spatial_window[1], spatial_window[2], spatial_window[3], spatial_window[4])
  # r = raster::raster(baseRname)
  # r_empty = raster::crop(r,e)
  e = terra::ext(spatial_window[1], spatial_window[2], spatial_window[3], spatial_window[4])
  r = terra::rast(baseRname)
  r_empty = terra::crop(r,e)
  if(madingley_data$grid_size!=1) {
    # old raster code: r_sized = raster::raster(nrow=dim(r_empty)[1]*(1/madingley_data$grid_size),ncol=dim(r_empty)[2]*(1/madingley_data$grid_size))
    # raster::extent(r_sized) = c(spatial_window[1], spatial_window[2], spatial_window[3], spatial_window[4])
    # r_empty = raster::resample(r_empty,r_sized,"bilinear")
    r_sized = terra::rast(nrow=terra::nrow(r_empty)*(1/madingley_data$grid_size),ncol=terra::ncol(r_empty)*(1/madingley_data$grid_size))
    terra::ext(r_sized) = c(spatial_window[1], spatial_window[2], spatial_window[3], spatial_window[4])
    r_empty = terra::resample(r_empty,r_sized,"bilinear")
  }
  # old raster code: r_empty[] = ifelse(r_empty[]==2,1,NA)
  # m_r_empty = raster::as.matrix(r_empty)
  r_empty = terra::ifel(r_empty==2,1,NA)
  m_r_empty = terra::as.matrix(r_empty,wide=TRUE)
  # check against old raster pkg: m_r_emptycheck = raster::as.matrix(raster::raster(r_empty))

  # insert raster values
  FGS = unique(d$FG)
  raster_list = rep(list(r_empty), length(FGS))
  for(fg in 1:length(FGS)){

    ds = d[d$FG==fg-1,]

    m_r_empty[] = NA # reset insert matrix

    lat = 1:nrow(m_r_empty)
    long = 1:ncol(m_r_empty)

    gc_counter = 0
    for(i in lat){
      for(j in long){
        if(!all(!ds$GC==gc_counter)){
          val = log10(ds$Abundance[ds$GC==gc_counter])
          if(is.infinite(val)){
            m_r_empty[i,j] = 0
          }else{
            m_r_empty[i,j] = val
          }

        }else{
          m_r_empty[i,j] = 0
        }
        gc_counter = gc_counter+1
      }
    }
    # old code: raster_list[[fg]][] = apply(m_r_empty, 2, rev) # flip matrix vertically
    etmp = terra::ext(raster_list[[fg]])
    crstemp = terra::crs(raster_list[[fg]])
    raster_list[[fg]] = terra::rast(apply(m_r_empty, 2, rev)) # flip matrix vertically
    terra::ext(raster_list[[fg]]) = etmp
    terra::crs(raster_list[[fg]]) = crstemp
  }

  if(functional_filter){
    # old: raster_stack = raster::stack(raster_list)
    raster_stack = terra::rast(raster_list)
    raster_stack = raster_stack*r_empty
    if(names[1]==0) {
      names(raster_stack) = paste('log10 Abundance\nFunctional Group',FGS)
    }else{
      if(length(names(raster_stack))!=length(names)){
        cat('Using default naming, length input not same as rasters plotted')
        names(raster_stack) = paste('Functional Group',FGS)
      }else{
        names(raster_stack) = names
      }
    }

    if(plot) {
      terra::plot(raster_stack,col=pal,box=box,axes=axes,
                  legend=TRUE
                  # axis.args=list(cex.axis=cex_axis),
                  # legend.args = list(text = 'log10(Abundance)',  side = 4,line = distance_legend_title, cex = cex_legend_title),
                  # legend.mar=legend_mar, legend.width=legend_width
                  )
    }else{
      return(raster_stack)
    }
  }else{
    # old: raster_stack = raster::stack(raster_list)
    raster_stack = terra::rast(raster_list)
    raster_list[[1]] = raster_list[[1]]*r_empty
    raster_list[[1]] = log10(sum(10^(raster_stack)))
    raster_list[[1]] = terra::ifel(is.na(r_empty),NA,raster_list[[1]])
    names(raster_list[[1]]) = paste('Total Abundance')
    if(plot) {
      terra::plot(raster_list[[1]],col=pal,main='Total Abundance',box=box,axes=axes,
                   legend=TRUE
                   # axis.args=list(cex.axis=cex_axis),
                   # legend.args = list(text = 'log10(Abundance)',  side = 4,line = distance_legend_title, cex = cex_legend_title),
                   # legend.mar=legend_mar, legend.width=legend_width
                  )
    }else{
      return(raster_list[[1]])
    }
  }
}
