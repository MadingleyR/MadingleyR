plot_foodweb = function(madingley_data,max_flows=10,colour=T,sample_data=100,col=c("#74add1","#a50026","#fdae61"),node_opacity=0.5,...){

  # store default par
  def_par = par(no.readonly = TRUE)
  
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

  # paths
  # needed madingley_data!!
  if(length(list.files(paste0('/private',gsub("//", "/", out_dir, fixed=TRUE),madingley_data$out_dir_name)))==0){
    base_path = paste0(gsub("//", "/", out_dir, fixed=TRUE),madingley_data$out_dir_name)
  }else{
    base_path = paste0('/private',gsub("//", "/", out_dir, fixed=TRUE),madingley_data$out_dir_name)
  }

  # determine sample fraction
  sample_fraction = sample_data/100

  # make file paths
  cohort_path = paste0(base_path,'cohort_properties')
  flow_path = paste0(base_path,'consumption_statistics')

  # list files
  flow_files = list.files(flow_path,pattern="PreyBinnedFoodwebConnections",full.names = T)
  bins_files = list.files(cohort_path,pattern="BinnedCohortStatistics",full.names = T)

  # check if required files were exported
  if(!length(flow_files)>1){ # not exist
    stop("Required files were not exported during model run")
  }
  if(!length(bins_files)>1){ # not exist
    stop("Required files were not exported during model run")
  }

  # init loop
  iterator = 1:length(list.files(flow_path,pattern="PreyBinnedFoodwebConnections"))
  flows_l = list()
  bins_l = list()

  # loop over time steps
  for(timestep in iterator){

    # list files and load the last 120 csvs
    flow_files = list.files(flow_path,pattern="PreyBinnedFoodwebConnections",full.names = T)
    bins_files = list.files(cohort_path,pattern="BinnedCohortStatistics",full.names = T)
    #cat("Processing: ",flow_files[timestep])
    #cat("Processing: ",bins_files[timestep])
    flows = read.csv(flow_files[timestep])
    bins = read.csv(bins_files[timestep])

    # sample data
    if(sample_fraction!=1){
      nrow_flows = nrow(flows); nrow_bins = nrow(bins)
      flows_sample = sample(1:nrow_flows,round(nrow_flows*sample_fraction))
      bins_sample = sample(1:nrow_bins,round(nrow_bins*sample_fraction))
      flows = flows[flows_sample,]
      bins = bins[bins_sample,]
    }

    # process bins
    bins = aggregate(bins,by=list(bins$FunctionalGroupIndex,bins$LowerBodyMassBin,bins$UpperBodyMassBin),FUN=sum)
    bins = bins[bins$TotalBiomass_kg!=0,]
    bins[,c("FunctionalGroupIndex","LowerBodyMassBin","UpperBodyMassBin")] = NULL
    names(bins)[1:3] = c("FunctionalGroupIndex","LowerBodyMassBin","UpperBodyMassBin")

    guilds = as.vector(madingley_data$cohort_def$DEFINITION_Nutrition.source)
    guilds = guilds[bins$FunctionalGroupIndex+1]
    bins$guilds = guilds
    thermo = as.vector(madingley_data$cohort_def$DEFINITION_Endo.Ectotherm)
    thermo = thermo[bins$FunctionalGroupIndex+1]
    bins$thermo = thermo
    guilds[guilds == "Herbivore"] = 1
    guilds[guilds == "Omnivore"] = 2
    guilds[guilds == "Carnivore"] = 3
    thermo[thermo == "Endotherm"] = 0.5
    thermo[thermo == "Ectotherm"] = 0
    bins$y = as.numeric(guilds) + as.numeric(thermo)
    bins$x = log10(bins$LowerBodyMassBin) + 0.5
    bins = bins[,c("x","y","TotalAbundance","TotalBiomass_kg")]

    # process flows
    flows$Prey_Diet = as.vector(flows$Prey_Diet)
    flows$Prey_Thermoregulation = as.vector(flows$Prey_Thermoregulation)
    flows[flows$Prey_Thermoregulation == "Ecto",]$Prey_Thermoregulation = 0
    flows[flows$Prey_Thermoregulation == "Endo",]$Prey_Thermoregulation = 0.5
    flows[flows$Prey_Diet == "H",]$Prey_Diet = 1
    flows[flows$Prey_Diet == "O",]$Prey_Diet = 2
    flows[flows$Prey_Diet == "C",]$Prey_Diet = 3
    flows$Prey_Diet = as.numeric(flows$Prey_Diet)
    flows$Prey_Thermoregulation = as.numeric(flows$Prey_Thermoregulation)
    flows$y_prey = flows$Prey_Diet + flows$Prey_Thermoregulation

    flows$Prey_UpperBodyMassBin = flows$Prey_LowerBodyMassBin*10
    flows$LOG10Prey_LowerBodyMassBin = log10(flows$Prey_LowerBodyMassBin)
    flows$LOG10Prey_UpperBodyMassBin = log10(flows$Prey_UpperBodyMassBin)
    flows$x_prey = flows$LOG10Prey_LowerBodyMassBin + 0.5

    bins_size = c(1 %o% 10^(-10:10))
    bin_alloc = rep(NA,length(flows$Pred_LowerBodyMassBin))
    for(row in 1:nrow(flows)) bin_alloc[row] = which(flows$Pred_AdlBM[row]<bins_size)[1]
    flows$Pred_LowerBodyMassBin = bins_size[bin_alloc-1]
    flows$Pred_UpperBodyMassBin = bins_size[bin_alloc]
    flows$LOG10Pred_LowerBodyMassBin = log10(flows$Pred_LowerBodyMassBin)
    flows$LOG10Pred_UpperBodyMassBin = log10(flows$Pred_UpperBodyMassBin)
    flows$x_pred = flows$LOG10Pred_LowerBodyMassBin + 0.5

    guilds = as.vector(madingley_data$cohort_def$DEFINITION_Nutrition.source)
    guilds = guilds[flows$Pred_FunctionalGroupIndex+1]
    thermo = as.vector(madingley_data$cohort_def$DEFINITION_Endo.Ectotherm)
    thermo = thermo[flows$Pred_FunctionalGroupIndex+1]
    guilds[guilds == "Omnivore"] = 2
    guilds[guilds == "Carnivore"] = 3
    thermo[thermo == "Endotherm"] = 0.5
    thermo[thermo == "Ectotherm"] = 0
    flows$y_pred = as.numeric(guilds) + as.numeric(thermo)

    flows = flows[,c("x_pred","y_pred","x_prey","y_prey","Prey_BiomassConsumed")]
    flows = aggregate(flows,by=list(flows[,"x_pred"],flows[,"y_pred"],flows[,"x_prey"],flows[,"y_prey"]),FUN=sum)
    flows = cbind(flows[,c(1:4)],flows$Prey_BiomassConsumed)
    names(flows) = c("x_pred","y_pred","x_prey","y_prey","Prey_BiomassConsumed")

    # put in list
    flows_l[[timestep]] = flows
    bins_l[[timestep]] = bins

  }

  flows = do.call(rbind,flows_l)
  bins = do.call(rbind,bins_l)

  # average over years
  flows = aggregate(flows,by=list(flows[,"x_pred"],flows[,"y_pred"],flows[,"x_prey"],flows[,"y_prey"]),FUN=mean)
  flows = flows[,grep("Group",names(flows),invert = T)]
  bins = aggregate(bins,by=list(bins[,"x"],bins[,"y"]),FUN=mean,drop=T)
  bins = bins[,grep("Group",names(bins),invert = T)]
  bins$x = bins$x + 3
  bins$points_cex = bins$TotalBiomass_kg/max(bins$TotalBiomass_kg)

  # filter
  if(max_flows<8){
    flows$PredID = paste0(flows$x_pred,"_",flows$y_pred)
    filder_max_con = max_flows
    filer_ids = names(table(flows$PredID))[table(flows$PredID)>filder_max_con]
    for(i in 1:length(filer_ids)){
      #print(filer_ids[i])
      idx = which(flows$PredID==filer_ids[i])
      nidx = length(idx)
      idx2 = idx[order(flows[idx,]$Prey_BiomassConsumed)[(nidx-filder_max_con):nidx]]

      if(i==1) {
        new_flows =  flows[idx2,]
      }else{
        new_flows = rbind(new_flows,flows[idx2,])
      }
    }
    idx = which(flows$PredID%in%names(table(flows$PredID))[table(flows$PredID)<=filder_max_con])
    #cat(idx)
    flows = rbind(new_flows,flows[idx,])
  }

  norm_min_range = 1
  norm_max_range = 15
  flows$lwd_norm = (flows$Prey_BiomassConsumed-min(flows$Prey_BiomassConsumed))/
    (max(flows$Prey_BiomassConsumed)-min(flows$Prey_BiomassConsumed)) * (norm_max_range-norm_min_range) + norm_min_range
  plot(c(-1.7,8),c(0.7,4),col="white",xaxs="i", yaxs="i",axes=F,xlab="",ylab="",...)
  arrowcolors = rev(paste0("grey",44:81))
  norm_min_range = 1
  norm_max_range = length(arrowcolors)
  flows$lwd_norm_color = round((flows$Prey_BiomassConsumed-min(flows$Prey_BiomassConsumed))/
    (max(flows$Prey_BiomassConsumed)-min(flows$Prey_BiomassConsumed)) * (norm_max_range-norm_min_range) + norm_min_range )
  flows$lwd_norm_color = arrowcolors[flows$lwd_norm_color]

  x_min_val = -0.5
  for(i in 1:nrow(flows)){
    if(flows$x_prey[i]>x_min_val&flows$x_pred[i]>x_min_val){
      try(
        # diagram::curvedarrow(
        #   c(flows$x_prey[i],flows$y_prey[i]),
        #   c(flows$x_pred[i],flows$y_pred[i]),
        #   lwd = flows$lwd_norm[i],
        #   lty = 1,
        #   lcol = flows$lwd_norm_color[i],
        #   arr.width = 1e-20,
        #   arr.pos = 1,
        #   curve = 0.1,
        #   dr = 0.0001,
        #   arr.col="blue",
        #   arr.lwd=1e-20,
        #   endhead = FALSE,
        #   segment = c(0, 1))
        add_food_web_connection(
          c(flows$x_prey[i],flows$y_prey[i]),
          c(flows$x_pred[i],flows$y_pred[i]),
          lwd = flows$lwd_norm[i],
          lty = 1,
          lcol = flows$lwd_norm_color[i],
          arr.width = 1e-20,
          arr.pos = 1,
          curve = 0.1,
          dr = 0.0001,
          arr.col="blue",
          arr.lwd=1e-20,
          endhead = FALSE,
          segment = c(0, 1))
        ,silent = TRUE)
    }
  }

  quant = quantile(bins$points_cex,0.8)
  bins$points_cex[bins$points_cex>quant]=quant
  norm_min_range = 2.0
  norm_max_range = 8.0
  bins$points_cex_norm = (bins$points_cex-min(bins$points_cex))/
    (max(bins$points_cex)-min(bins$points_cex)) * (norm_max_range-norm_min_range) + norm_min_range

  if(length(col)!=3){
    p_colors = c("#00B358","#FF1300","#104BA9")
    cat("please provide 3 colors, default colors used...")
  }else{
    p_colors = col
  }
  
  p_colors = col2rgb(p_colors)/255
  
  
  for(i in 1:nrow(bins)){
    if(bins$x[i]>(-0.5)){
      if(colour){
        
        points(x=bins$x[i],y=bins$y[i],cex=bins$points_cex_norm[i],
               col=rgb(0,0,0,0.001),bg=rgb(1,1,1,node_opacity),type="p",pch=21) #
        
        if(bins$y[i]<2){
          points(x=bins$x[i],y=bins$y[i],cex=bins$points_cex_norm[i],
                 col=rgb(0,0,0,0.001),bg=rgb(p_colors[1,1],p_colors[2,1],p_colors[3,1],0.7),type="p",pch=21) # herb
        }else if(bins$y[i]<3){
          points(x=bins$x[i],y=bins$y[i],cex=bins$points_cex_norm[i],
                 col=rgb(0,0,0,0.001),bg=rgb(p_colors[1,3],p_colors[2,3],p_colors[3,3],0.7),type="p",pch=21) # carn
        }else{
          points(x=bins$x[i],y=bins$y[i],cex=bins$points_cex_norm[i],
                 col=rgb(0,0,0,0.001),bg=rgb(p_colors[1,2],p_colors[2,2],p_colors[3,2],0.7),type="p",pch=21) # omni
        }
      }else{
        points(x=bins$x[i],y=bins$y[i],cex=bins$points_cex_norm[i],
               col=rgb(0,0,0,0.001),bg=rgb(0,0,0,0.5),type="p",pch=21) #
      }

    }
  }

  xlabs = c(expression(paste("10"^"-3"*"-10"^"-2")),
            expression(paste("10"^"-2"*"-10"^"-1")),
            expression(paste("10"^"-1"*"-10"^"0")),
            expression(paste("10"^"0"*"-10"^"1")),
            expression(paste("10"^"1"*"-10"^"2")),
            expression(paste("10"^"2"*"-10"^"3")),
            expression(paste("10"^"3"*"-10"^"4")))

  #plot(-3:3+3,1:7) # used for testing
  text(cex=0.9, x=c(-2.5,-1.5,-0.5,0.5,1.5,2.5,3.5)+3, y=0.45, xlabs, xpd=TRUE, srt=45)
  axis(1, at=c(-2.5,-1.5,-0.5,0.5,1.5,2.5,3.5)+3,padj=0.5,cex.axis=0.9,labels = F)

  mtext("Body mass bin [kg]",side = 1,at = 3.5,padj = 5.5)

  axis(2,at=c(1.0,1.5,2.0,2.5,3.0,3.5),labels=rep(" ",6),pos=-0.1)

  ycat_font_size = 1
  x_placement = -1
  text(x_placement,1.0,"Ectotherm\nherbivore",cex=ycat_font_size, xpd=T)
  text(x_placement,1.5,"Endotherm\nherbivore",cex=ycat_font_size, xpd=T)
  text(x_placement,2.0,"Ectotherm\nomnivore",cex=ycat_font_size, xpd=T)
  text(x_placement,2.5,"Endotherm\nomnivore",cex=ycat_font_size, xpd=T)
  text(x_placement,3.0,"Ectotherm\ncarnivore",cex=ycat_font_size, xpd=T)
  text(x_placement,3.5,"Endotherm\ncarnivore",cex=ycat_font_size, xpd=T)
  
  
  # return to default par
  par(def_par)

}
