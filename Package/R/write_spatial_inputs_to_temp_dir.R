write_spatial_inputs_to_temp_dir = function(spatial_inputs,XY_window="0 0 0 0",crop=TRUE,input_dir,silenced=FALSE){

  # debug code, raster to terra update
  if(FALSE){
    
    s1 = runif(360*180); s2 = runif(360*180*12)
    
    # raster
    spatial_inputs = list(
      'test_raster' = raster::raster(matrix(s1,180,360)),
      'test_rasterbrick' = raster::brick(array(s2, c(180,360,12)))
    )
    raster::extent(spatial_inputs[['test_raster']]) = c(-180,180,-90,90)
    raster::extent(spatial_inputs[['test_rasterbrick']]) = c(-180,180,-90,90)
    
    # terra
    spatial_inputs = list(
      'test_raster' = terra::rast(matrix(s1,180,360)),
      'test_rasterbrick' = terra::rast(array(s2, c(180,360,12)))
    )
    terra::ext(spatial_inputs[['test_raster']]) = c(-180,180,-90,90)
    terra::ext(spatial_inputs[['test_rasterbrick']]) = c(-180,180,-90,90)

    XY_window = "-11 25 35 60"; crop = TRUE; input_dir = tempdir(); silenced = FALSE
    browseURL(input_dir)
  
  }
  
  # sp window
  XY_window_num = as.numeric(unlist(strsplit(XY_window, " ")))
  XY_window_adjusted = c(XY_window_num[1],
                         XY_window_num[2],
                         XY_window_num[3],
                         XY_window_num[4])

  # inout dir
  input_sp_dir = paste0(input_dir,"/spatial_inputs/1deg/")
  unlink(input_sp_dir, recursive = TRUE)
  dir.create(input_sp_dir, showWarnings = FALSE, recursive = TRUE)

  csv_namas = names(spatial_inputs)
  cntr = 1
  i = csv_namas[1]
  for(i in csv_namas){
    # old raster pkg: if(class(spatial_inputs[[i]])[1]=="RasterLayer"){
    if(class(spatial_inputs[[i]])[1]=="SpatRaster" & terra::nlyr(spatial_inputs[[i]])==1){

      # extract raster
      r = spatial_inputs[[i]]

      # create df
      # old raster pkg: df = as.data.frame(cbind(r[],raster::xyFromCell(r, 1:length(r[]), spatial=FALSE)))
      df = as.data.frame(cbind(r[],terra::xyFromCell(r, 1:terra::ncell(r))))
      names(df) = c('var','x','y')
      df$var = ifelse(is.na(df$var),-999,df$var)

      # sort df for madingley
      df = df[with(df, order(y, x)),]
      head(df)

      # crop df by XY_window_adjusted
      if(crop){
        df = df[which(df$x>XY_window_adjusted[1]&
                      df$x<XY_window_adjusted[2]&
                      df$y>XY_window_adjusted[3]&
                      df$y<XY_window_adjusted[4]),]
      }

      # write to csv
      if(cntr==1 & !silenced) cat(paste0("Processing: ",i,", "))
      if(cntr>1 & cntr<4 & !silenced) cat(paste0(i,", "))
      if(cntr==4 & !silenced) cat(paste0(i,"\n"))
      if(cntr==5 & !silenced) cat(paste0("Processing: ",i,", "))
      if(cntr>5 & cntr<8 & !silenced) cat(paste0(i,", "))
      if(cntr==8 & !silenced) cat(paste0(i,"\n"))
      write.csv(df,paste0(input_sp_dir,paste0(i,'.csv')),row.names = FALSE)
    }
    # old raster pkg: if(class(spatial_inputs[[i]])[1]=="RasterBrick"){
    if(class(spatial_inputs[[i]])[1]=="SpatRaster" & terra::nlyr(spatial_inputs[[i]])==12){
      for(j in 1:12){
        # extract raster
        r = spatial_inputs[[i]][[j]]

        # create df
        # old code raster pkg: df = as.data.frame(cbind(r[],raster::xyFromCell(r, 1:length(r[]), spatial=FALSE)))
        df = as.data.frame(cbind(r[],terra::xyFromCell(r, 1:terra::ncell(r))))
        names(df) = c('var','x','y')
        df$var = ifelse(is.na(df$var),-999,df$var)

        # sort df for madingley
        df = df[with(df, order(y, x)),]
        head(df)

        # crop df by XY_window_adjusted
        if(crop){
          df = df[which(df$x>XY_window_adjusted[1]&
                          df$x<XY_window_adjusted[2]&
                          df$y>XY_window_adjusted[3]&
                          df$y<XY_window_adjusted[4]),]
        }

        # write to csv
        if(j==1 & !silenced) cat(paste0("Processing: ",i,"_1-12",'\n'))
        write.csv(df,paste0(input_sp_dir,paste0(i,"_",j,'.csv')),row.names = FALSE)
      }
    }
    cntr = cntr + 1
  }

}
