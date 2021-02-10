madingley_run = function(out_dir=tempdir(),
                         madingley_data,
                         years=1,
                         cohort_def=get_default_cohort_def(),
                         stock_def=get_default_stock_def(),
                         spatial_inputs=0,
                         model_parameters=0,
                         output_timestep=c(0,0,0,0),
                         max_cohort=500,
                         dispersal_off=F,
                         silenced=F,
                         parallel=T) {
  
  noise_cohort_order=1.0
  grid_size=0

  if(noise_cohort_order>1.0) {
    cat("please set noise_cohort_order to a decimal value between 0.0 and 1.0, value forced to 1.0")
    cat("\n")
    noise_cohort_order=1.0
  }

  if(noise_cohort_order<0.0) {
    cat("please set noise_cohort_order to a decimal value between 0.0 and 1.0, value forced to 0.0")
    cat("\n")
    noise_cohort_order=0.0
  }
  NoiseThresholdCohortOrder=1-noise_cohort_order

  # check if output dir exists
  if(!dir.exists(out_dir)) stop('Specified output folder does not exist, please make sure out_dir is correct')

  # fixed values in current model build
  overwrite_sp_check = F

  # get spatial window from madingley_data output list
  spatial_window = madingley_data$spatial_window

  # remove slashes from out_dir
  if(substr(out_dir,(nchar(out_dir)+1)-1,nchar(out_dir))=='/')  out_dir=substr(out_dir,1,nchar(out_dir)-1)
  if(substr(out_dir,(nchar(out_dir)+1)-1,nchar(out_dir))=='\\') out_dir=substr(out_dir,1,nchar(out_dir)-1)

  # replace tilde with home folder path
  out_dir = sub("~", Sys.getenv("HOME"), out_dir)
  out_dir_save = out_dir

  options(warn=-1)
  try(sink(),silent = TRUE)
  options(warn=0)

  # load default input rasters if not specified by input function
  if(class(spatial_inputs)!="list") spatial_inputs = madingley_inputs(input_type = "spatial inputs")

  # check if all spatial inputs are provided
  if(length(spatial_inputs)<13) stop('Not all required spatial inputs are provided')
  correct_sp_names = c("realm_classification","land_mask","hanpp","available_water_capacity","Ecto_max","Endo_C_max","Endo_H_max",
    "Endo_O_max","terrestrial_net_primary_productivity","near-surface_temperature","precipitation","ground_frost_frequency",
    "diurnal_temperature_range")
  spatial_inputs = spatial_inputs[match(correct_sp_names,names(spatial_inputs))] # order input layers using the order defined by correct_sp_names
  if(!all.equal(names(spatial_inputs),correct_sp_names)) stop('Not all required spatial inputs names correctly')
  classes = c(rep('RasterLayer',8),rep('RasterBrick',5)); classes_check = rep(F,13)
  for(i in 1:13) classes_check[i] = class(spatial_inputs[[i]])==classes[i]
  if(!all(classes_check)) stop('Not all required spatial inputs formatted correctly')


  # set correct grid cell size (resolution)
  if(grid_size==0) {
    grid_size = res(spatial_inputs$realm_classification)[1]
    if(grid_size==0 | is.na(grid_size)) grid_size = 1
  }
  if(grid_size>1) stop('Grid cell sizes larger than 1 degree currently not supported')
  #print(paste('grid cell size:',grid_size))

  # check if grid cell size of spatial_inputs and madingley_data are equal
  if(grid_size!=madingley_data$grid_size) stop('The grid cell size of madingley_data and spatial_inputs do not match')

  sum_res = 0; for(i in 1:13) sum_res = sum_res + mean(res(spatial_inputs[[i]]))
  if( sum_res!= (grid_size*13) ) stop('Please make sure all input raster have the same resolutaion (0.5 or 1 degree)')

  # check spatial window
  if(spatial_window[1]>spatial_window[2]) stop('spatial_window should be defined as followed c(min long, max long, min lat, max lat)')
  if(spatial_window[3]>spatial_window[4]) stop('spatial_window should be defined as followed c(min long, max long, min lat, max lat)')


  # setup basic C++ input arguments
  spatial_window = paste(spatial_window,collapse=" ") # xmin, xmax, ymin, ymax
  if(sum(output_timestep)==0) {
    output_ts_years = c(years-1,years-1,years-1,years-1)
  }else{
    output_ts_years = output_timestep
  }
  output_ts_months = paste(output_ts_years*12,collapse=" ") # bin cohort, full cohort, bin food-web, full stock
  gridout_bool = 1
  start_t = 0

  # create temporary output dir
  out_dir_name = paste0("/madingley_outs_",format(Sys.time(), "%d_%m_%y_%H_%M_%S"),"/")
  out_dir = paste0(out_dir,out_dir_name)
  dir.create(out_dir, showWarnings = F)

  # creat temporary input dir
  input_dir = paste0(out_dir,"input/")
  unlink(input_dir, recursive = T)
  dir.create(input_dir, showWarnings = F)

  options(warn=-1)
  try(sink(),silent = TRUE)
  options(warn=0)

  # def checks
  if(length(cohort_def)==0) cohort_def = get_default_cohort_def()
  if(class(stock_def)=="list") stock_def = get_default_stock_def()

  # write inputs csv files to temp dir
  options("scipen"=100, "digits"=4)
  write_cohort_def(out_dir,cohort_def)
  write_stock_def(out_dir,stock_def)
  write_simulation_parameters(out_dir)
  write_mass_bin_def(out_dir)
  options("scipen"=0, "digits"=7)

  # write madingley_data cohorts and stocks to temp dir
  write_madingley_data_cohorts_stocks_to_temp_dir_fast(input_dir=input_dir,madingley_data=madingley_data)

  # use user defined model parameters or default
  if(class(model_parameters)!="data.frame"){ # default
    model_params = 0
  }else{ # not default
    model_params = paste(c(1,model_parameters$values),collapse=" ")
  }

  # check if spatial inputs have changed since last write to tempdir (can be overwritten with overwrite_sp_check==T)
  if(overwrite_sp_check) { # not default
    write_spatial_inputs_to_temp_dir(spatial_inputs=spatial_inputs,
                                     XY_window=spatial_window,
                                     crop=T,
                                     input_dir=out_dir,
                                     silenced)
  }else{ # default
    sp_inputs_changed = check_and_rewrite_spatial_inputs(spatial_inputs=spatial_inputs,tempdir=out_dir)
    if(sp_inputs_changed) write_spatial_inputs_to_temp_dir(spatial_inputs=spatial_inputs,
                                                           XY_window=spatial_window,
                                                           crop=T,
                                                           input_dir=out_dir,
                                                           silenced)
  }

  if(dispersal_off) {
    NoDispersal=1
  }else{
    NoDispersal=0
  }

  RunInParallel=parallel
  if(RunInParallel){
    int_RunInParallel = 1
  }else{
    int_RunInParallel = 0
  }

  # Run the C++ code (run)
  switch(Sys.info()[['sysname']],

    # run on windows
    Windows= {

      # setup C++ input arguments
      sp_dir = paste0(out_dir,"spatial_inputs/1deg/")
      exec_args = paste(spatial_window,'"%PATH1%"',
                        output_ts_months,gridout_bool,
                        '"%PATH2%"',max_cohort,
                        '"%PATH3%"','"%PATH4%"',
                        start_t,'"%PATH5%"',
                        grid_size,NoiseThresholdCohortOrder,
                        NoDispersal,int_RunInParallel,model_params)

      # setup windows executable path
      win_dist_dir = paste0(get_lib_path(),'/win_exec/')
      madingley_exec = paste0('"',win_dist_dir,'madingley.exe" run')

      # create madingley.bat file for running the C++ code
      bat_l1 = "ECHO off"
      bat_l2 = paste0("set PATH1=",paste0(gsub("/", "\\\\", out_dir),'\\'))
      bat_l3 = paste0("set PATH2=",paste0(gsub("/", "\\\\", input_dir),'\\'))
      bat_l4 = paste0("set PATH3=",paste0(gsub("/", "\\\\", input_dir),"C.csv"))
      bat_l5 = paste0("set PATH4=",paste0(gsub("/", "\\\\", input_dir),"S.csv"))
      bat_l6 = paste0("set PATH5=",paste0(gsub("/", "\\\\", sp_dir),'\\'))
      bat_l7 =  paste(madingley_exec,years,exec_args)
      bat_l7 = gsub("/", "\\\\", bat_l7)
      run_exec = paste0(win_dist_dir,'madingley.bat')
      writeLines(c(bat_l1,bat_l2,bat_l3,bat_l4,bat_l5,bat_l6,bat_l7), run_exec)
      run_exec = paste0('"',run_exec,'"')

      # init model
      #cat(run_exec)
      if(silenced){
        print_out = system(run_exec,intern=T)
      }else{
        system(run_exec)
      }

      # return data
      out = return_output_list_run(cohort_def,stock_def,out_dir,out_dir_name)
      out$spatial_window = madingley_data$spatial_window
      if(!out_dir_save==tempdir()){
        out$out_dir = out_dir_save
      }
      out$grid_size = grid_size
      return(out)
    },

    # run on linux
    Linux  = {

      # setup C++ input arguments
      sp_dir = paste0(out_dir,"/spatial_inputs/1deg/")
      exec_args = paste(spatial_window,
                        paste0('\"',gsub("\\\\", "/", out_dir),'\"'),
                        output_ts_months,
                        gridout_bool,
                        paste0('\"',gsub("\\\\", "/", input_dir),'\"'),
                        max_cohort,
                        paste0('\"',gsub("\\\\", "/", paste0(input_dir,"C.csv")),'\"'),
                        paste0('\"',gsub("\\\\", "/", paste0(input_dir,"S.csv")),'\"'),
                        start_t,
                        paste0('\"',gsub("\\\\", "/", sp_dir),'\"'),
                        grid_size,
                        NoiseThresholdCohortOrder,
                        NoDispersal,
                        int_RunInParallel,
                        model_params)

      # setup linux executable path
      lin_dist_dir = paste0('"',get_lib_path(),'/lin_exec/','"')
      system(paste('cd',lin_dist_dir,'&&','chmod u+x madingley'))
      madingley_exec = paste('cd',lin_dist_dir,'&&','./madingley run',years,exec_args)


      # init model
      #cat(madingley_exec)
      if(silenced){
        print_out = system(madingley_exec,intern=T)
      }else{
        system(madingley_exec)
      }

      # return data
      out = return_output_list_run(cohort_def,stock_def,out_dir,out_dir_name)
      out$spatial_window = madingley_data$spatial_window
      if(!out_dir_save==tempdir()){
        out$out_path = out_dir_save
      }
      out$grid_size = grid_size
      return(out)

    },

    # run on mac
    Darwin = {

      # setup C++ input arguments
      sp_dir = paste0(out_dir,"/spatial_inputs/1deg/")
      exec_args = paste(spatial_window,
                        paste0('\"',gsub("\\\\", "/", out_dir),'\"'),
                        output_ts_months,
                        gridout_bool,
                        paste0('\"',gsub("\\\\", "/", input_dir),'\"'),
                        max_cohort,
                        paste0('\"',gsub("\\\\", "/", paste0(input_dir,"C.csv")),'\"'),
                        paste0('\"',gsub("\\\\", "/", paste0(input_dir,"S.csv")),'\"'),
                        start_t,
                        paste0('\"',gsub("\\\\", "/", sp_dir),'\"'),
                        grid_size,
                        NoiseThresholdCohortOrder,
                        NoDispersal,
                        int_RunInParallel,
                        model_params)

      # setup linux executable path
      mac_dist_dir = paste0('"',get_lib_path(),'/mac_exec/','"')
      system(paste('cd',mac_dist_dir,'&&','chmod u+x madingley'))
      madingley_exec = paste('cd',mac_dist_dir,'&&','./madingley run',years,exec_args)


      # init model
      #cat(madingley_exec)
      if(silenced){
        print_out = system(madingley_exec,intern=T)
      }else{
        system(madingley_exec)
      }

      # return data
      out = return_output_list_run(cohort_def,stock_def,out_dir,out_dir_name)
      out$spatial_window = madingley_data$spatial_window
      if(!out_dir_save==tempdir()){
        out$out_path = out_dir_save
      }
      out$grid_size = grid_size
      #cat(out$grid_size)
      return(out)

    }
  )
  options(warn=-1)
  try(sink(),silent = TRUE)
  options(warn=0)

}
