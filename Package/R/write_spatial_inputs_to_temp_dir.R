write_spatial_inputs_to_temp_dir = function(spatial_inputs,XY_window="0 0 0 0",crop=T,input_dir,silenced=F){

  XY_window_num = as.numeric(unlist(strsplit(XY_window, " ")))
  XY_window_adjusted = c(XY_window_num[1],
                         XY_window_num[2],
                         XY_window_num[3],
                         XY_window_num[4])

  input_sp_dir = paste0(input_dir,"/spatial_inputs/1deg/")
  unlink(input_sp_dir, recursive = T)
  dir.create(input_sp_dir, showWarnings = F, recursive = T)

  csv_namas = names(spatial_inputs)
  cntr = 1
  for(i in csv_namas){
    if(class(spatial_inputs[[i]])[1]=="RasterLayer"){

      # extract raster
      r = spatial_inputs[[i]]

      # create df
      df = as.data.frame(cbind(r[],raster::xyFromCell(r, 1:length(r[]), spatial=FALSE)))
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
      write.csv(df,paste0(input_sp_dir,paste0(i,'.csv')),row.names = F)
    }
    if(class(spatial_inputs[[i]])[1]=="RasterBrick"){
      for(j in 1:12){
        # extract raster
        r = spatial_inputs[[i]][[j]]

        # create df
        df = as.data.frame(cbind(r[],raster::xyFromCell(r, 1:length(r[]), spatial=FALSE)))
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
        write.csv(df,paste0(input_sp_dir,paste0(i,"_",j,'.csv')),row.names = F)
      }
    }
    cntr = cntr + 1
  }

}
