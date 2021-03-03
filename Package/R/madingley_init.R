madingley_init = function(cohort_def=get_default_cohort_def(),
                          stock_def=get_default_stock_def(),
                          spatial_inputs=0,
                          spatial_window=c(32,35,-4,-1),
                          max_cohort = 500,
                          silenced=F) {

  NotYetAssignedVariableToPassToCPP=0.0
  NoDispersal=0
  RunInParallel=1

  # save for return object
  spatial_window_saved = spatial_window

  # madingley_init always outputs to tempdir() because outputs are the same as the returned object
  out_dir=tempdir()

  # remove slashes from out_dir
  if(substr(out_dir,(nchar(out_dir)+1)-1,nchar(out_dir))=='/')  out_dir=substr(out_dir,1,nchar(out_dir)-1)
  if(substr(out_dir,(nchar(out_dir)+1)-1,nchar(out_dir))=='\\') out_dir=substr(out_dir,1,nchar(out_dir)-1)

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
  grid_size=0
  if(grid_size==0) {
    grid_size = res(spatial_inputs$realm_classification)[1]
    if(grid_size==0 | is.na(grid_size)) grid_size = 1
  }
  if(grid_size>1) stop('Grid cell sizes larger than 1 degree currently not supported')

  sum_res = 0; for(i in 1:13) sum_res = sum_res + mean(res(spatial_inputs[[i]]))
  if( sum_res!= (grid_size*13) ) stop('Please make sure all input raster have the same resolutaion (0.5 or 1 degree)')

  # prepare inputs arguments Madingley C++ wrapper
  if(spatial_window[1]>spatial_window[2]) stop('spatial_window should be defined as followed c(min long, max long, min lat, max lat)')
  if(spatial_window[3]>spatial_window[4]) stop('spatial_window should be defined as followed c(min long, max long, min lat, max lat)')
  spatial_window = paste(spatial_window,collapse=" ") # xmin, xmax, ymin, ymax
  output_ts_years = paste(c(999,999,999,999),collapse=" ") # bin cohort, full cohort, bin food-web, full stock
  gridout_bool = 0 # don't output spatial properties
  cohort_csv = stock_csv = "none" # model init, inputs generated
  start_t = 0 # start at year 0
  use_non_default_mp = 0 # don't use model specific parameters

  # create temporary output dir
  out_dir = paste0(out_dir,"/madingley_outs_",format(Sys.time(), "%d_%m_%y_%H_%M_%S"),"/")
  dir.create(out_dir, showWarnings = F) # linux and mac create dir within C++

  # creat temporary input dir
  input_dir = paste0(out_dir,"/input/")
  unlink(input_dir, recursive = T)
  dir.create(input_dir, showWarnings = F)

  # set correct number of cohorts to init
  n_cohorts_per_fg = floor(max_cohort/nrow(cohort_def[cohort_def$DEFINITION_Realm=="Terrestrial",]))
  cohort_def$PROPERTY_Initial.number.of.GridCellCohorts[cohort_def$DEFINITION_Realm=="Terrestrial"] = n_cohorts_per_fg

  # write inputs files to temp dir
  options("scipen"=100, "digits"=4)
  write_cohort_def_dt(out_dir,cohort_def)
  #write_cohort_def(out_dir,cohort_def)
  write_stock_def(out_dir,stock_def)
  write_simulation_parameters(out_dir)
  write_mass_bin_def(out_dir)
  options("scipen"=0, "digits"=7)
  write_spatial_inputs_to_temp_dir(spatial_inputs=spatial_inputs,
                                   XY_window=spatial_window,
                                   crop=T,
                                   input_dir=out_dir,
                                   silenced) # spatial inputs

  # write spatial inputs statistics to check later if identical data has been written before
  check = sort(as.vector(unlist(lapply(spatial_inputs,function(x) {c(cellStats(x,stat='sd'),cellStats(x,stat='mean'))}))))
  write.csv(data.frame(check=check),paste0(out_dir,"/spatial_inputs/spatial_inputs.csv"))

  # Run the C++ code (init)
  switch(Sys.info()[['sysname']],

    # run on windows
    Windows = {

      # setup C++ input arguments
      spatial_inputs_location = paste0(out_dir,"/spatial_inputs/1deg/")
      exec_args = paste(spatial_window,
                        paste0('\\"',gsub("\\\\", "/", out_dir),'\\\\"'),
                        output_ts_years,
                        gridout_bool,
                        paste0('\\"',gsub("\\\\", "/", input_dir),'\\\\"'),
                        max_cohort,
                        cohort_csv,
                        stock_csv,
                        start_t,
                        paste0('\\"',gsub("\\\\", "/", spatial_inputs_location),'\\\\"'),
                        grid_size,
                        NotYetAssignedVariableToPassToCPP,
                        NoDispersal,
                        RunInParallel,
                        use_non_default_mp)

      # setup windows executable path
      win_dist_dir = paste0(get_lib_path(),'/win_exec/')
      madingley_exec = paste0('"',win_dist_dir,'madingley.exe" spin 0')

      # create madingley.bat file for running the C++ code
      bat_l1 = "ECHO off"
      bat_l2 =  paste(madingley_exec,exec_args)
      bat_l2 = gsub("/", "\\\\", bat_l2)
      run_exec = paste0(win_dist_dir,"madingley.bat")
      writeLines(c(bat_l1,bat_l2), run_exec)
      run_exec = paste0('"',run_exec,'"')

      # init model
      if(silenced){
        print_out = system(run_exec,intern=T)
      }else{
        system(run_exec)
      }

      # return the init data
      ROL = return_output_list_ini(cohort_def,stock_def,out_dir)
      ROL$spatial_window = spatial_window_saved
      ROL$grid_size = grid_size
      cat(" ")
      return(ROL)

    },

    # run on linux
    Linux = {

      # setup C++ input arguments
      spatial_inputs_location = paste0(out_dir,"/spatial_inputs/1deg/")
      exec_args = paste(spatial_window,
                        paste0('\"',gsub("\\\\", "/", out_dir),'\"'),
                        output_ts_years,
                        gridout_bool,
                        paste0('\"',gsub("\\\\", "/", input_dir),'\"'),
                        max_cohort,
                        cohort_csv,
                        stock_csv,
                        start_t,
                        paste0('\"',gsub("\\\\", "/", spatial_inputs_location),'\"'),
                        grid_size,
                        NotYetAssignedVariableToPassToCPP,
                        NoDispersal,
                        RunInParallel,
                        use_non_default_mp)

      # setup linux executable path
      lin_dist_dir = paste0('"',get_lib_path(),'/lin_exec/','"')
      system(paste('cd',lin_dist_dir,"&&","chmod u+x madingley"))
      madingley_exec = paste('cd',lin_dist_dir,"&&","./madingley spin 0",exec_args)

      # init model
      if(silenced){
        print_out = system(madingley_exec,intern=T)
      }else{
        system(madingley_exec)
      }

      # return the init data
      ROL = return_output_list_ini(cohort_def,stock_def,out_dir)
      ROL$spatial_window = spatial_window_saved
      ROL$grid_size = grid_size
      cat(" ")
      return(ROL)

    },

    # run on mac
    Darwin = {

      # setup C++ input arguments
      spatial_inputs_location = paste0(out_dir,"/spatial_inputs/1deg/")
      exec_args = paste(spatial_window,
                        paste0('\"',gsub("\\\\", "/", out_dir),'\"'),
                        output_ts_years,
                        gridout_bool,
                        paste0('\"',gsub("\\\\", "/", input_dir),'\"'),
                        max_cohort,
                        cohort_csv,
                        stock_csv,
                        start_t,
                        paste0('\"',gsub("\\\\", "/", spatial_inputs_location),'\"'),
                        grid_size,
                        NotYetAssignedVariableToPassToCPP,
                        NoDispersal,
                        RunInParallel,
                        use_non_default_mp)

      # setup mac executable path
      mac_dist_dir = paste0('"',get_lib_path(),'/mac_exec/','"')
      system(paste('cd',mac_dist_dir,"&&","chmod u+x madingley"))
      madingley_exec = paste('cd',mac_dist_dir,"&&","./madingley spin 0",exec_args)

      # init model
      #cat(madingley_exec)
      if(silenced){
        print_out = system(madingley_exec,intern=T)
      }else{
        system(madingley_exec)
      }


      # return the init data
      ROL = return_output_list_ini(cohort_def,stock_def,out_dir)
      ROL$spatial_window = spatial_window_saved
      ROL$grid_size = grid_size
      cat(" ")
      return(ROL)

    }
  )

}
