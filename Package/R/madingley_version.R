madingley_version = function(R=TRUE, CPP=TRUE) {
  
  R_package_version = packageVersion("MadingleyR")
  
  df=data.frame(matrix(NA,ncol=2,nrow=2))
  names(df) = NULL
  rownames(df) = c("MadingleyR","Madingley C++ source")
  df[,1] = "->"
  df[1,2] = as.character(R_package_version)

  # Call the C++ code (version check only)
  switch(Sys.info()[['sysname']],

    # run on windows
    Windows = {

      # setup windows executable path
      win_dist_dir = paste0(get_lib_path(),'/win_exec/')
      madingley_exec = paste0('"',win_dist_dir,'madingley.exe" version')

      # create madingley.bat file for running the C++ code
      bat_l1 = "ECHO off"
      bat_l2 =  paste(madingley_exec)
      bat_l2 = gsub("/", "\\\\", bat_l2)
      run_exec = paste0(win_dist_dir,"madingley.bat")
      writeLines(c(bat_l1,bat_l2), run_exec)
      run_exec = paste0('"',run_exec,'"')
      version_CPP = system(run_exec,intern=T)

      # return the version data
      df[2,2] = version_CPP
      return(df)

    },

    # run on linux
    Linux = {

      # setup linux executable path
      lin_dist_dir = paste0('"',get_lib_path(),'/lin_exec/','"')
      system(paste('cd',lin_dist_dir,"&&","chmod u+x madingley"))
      madingley_exec = paste('cd',lin_dist_dir,"&&","./madingley version")
      version_CPP = system(madingley_exec,intern=T)

      # return the version data
      df[2,2] = version_CPP
      return(df)

    },

    # run on mac
    Darwin = {

      # setup mac executable path
      mac_dist_dir = paste0('"',get_lib_path(),'/mac_exec/','"')
      system(paste('cd',mac_dist_dir,"&&","chmod u+x madingley"))
      madingley_exec = paste('cd',mac_dist_dir,"&&","./madingley version")
      version_CPP = system(madingley_exec,intern=T)

      # return the version data
      df[2,2] = version_CPP
      return(df)

    }
  )

}
