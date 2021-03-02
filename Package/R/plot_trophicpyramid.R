plot_trophicpyramid = function(madingley_data,flow_round=2,mass_round=2,col=c("#a50026","#fdae61","#74add1","#313695"),
                               text_col=rep("white",4),...){

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


  # get base path
  md = madingley_data
  if(length(list.files(paste0('/private',gsub("//", "/", out_dir, fixed=TRUE),md$out_dir_name)))==0){
    base_path = paste0(gsub("//", "/", out_dir, fixed=TRUE),md$out_dir_name)
  }else{
    base_path = paste0('/private',gsub("//", "/", out_dir, fixed=TRUE),md$out_dir_name)
  }

  # cons path
  cons_path = paste0(base_path,'consumption_statistics')
  cl = list()
  cfiles = list.files(cons_path,pattern="CohortConsumptionSummary",full.names = T)

  # check if required files were exported
  if(!length(cfiles)>1){ # not exist
    stop("Required files were not exported during model run")
  }

  years = 0:((length(cfiles)/12)-1)
  if(length(cfiles)==12) years = 0
  months = 1:12
  for(y in years){
    for(m in months){
      i = y*12+m
      cf_raw = read.csv(cfiles[i])
      if(m==1){
        cf = aggregate(cf_raw[,grep("Cohort_Consumed_",names(cf_raw))],by=list(cf_raw$Cohort_FunctionalGroupIndex),FUN=sum)
      }else{
        cf = cf + aggregate(cf_raw[,grep("Cohort_Consumed_",names(cf_raw))],by=list(cf_raw$Cohort_FunctionalGroupIndex),FUN=sum)
      }
    }
    if(y==0){
      cf_year = cf
    }else{
      cf_year = cf_year + cf
    }
  }
  cf_year=cf_year/length(years)
  cf_year$Cohort_FunctionalGroup = md$cohort_def$DEFINITION_Nutrition.source[(cf_year[,1]/12+1)]
  cf_year$Group.1 = NULL
  cons = aggregate(cf_year[,grep("Cohort_Consumed_",names(cf_year))],by=list(cf_year$Cohort_FunctionalGroup),FUN=sum)


  tl = as.data.frame(t(md$time_line_cohorts[,3:ncol(md$time_line_cohorts)]))
  tl$fg = as.vector(md$cohort_def$DEFINITION_Nutrition.source)
  tl = tl[rowSums(tl[,1:(nrow(tl)-1)])>0,]
  col_select = ncol(tl)-length(years)*12
  tl = tl[,col_select:ncol(tl)] # select years of interest
  for(y in years) tl[,paste0("year_",(y+1),"_sum")] = rowSums(tl[,(y*12+1:12)])
  tl = tl[,c(grep("fg",names(tl)),grep("_sum",names(tl)))]
  if(length(years)>1) {
    tl$yearly_mean = rowMeans(tl[,grep("_sum",names(tl))])
    mass = aggregate(tl$yearly_mean,by=list(tl$fg),FUN=sum)
  }else{
    mass = aggregate(tl$year_1_sum,by=list(tl$fg),FUN=sum)
  }

  mass = rbind(mass,c(NA,NA))
  mass[nrow(mass),1] = "Autotrophs"
  tl = md$time_line_stocks
  tl = aggregate(tl,by=list(tl$Year),FUN=sum)
  tl = tl[(nrow(tl)-length(years)+1):nrow(tl),]
  mass[nrow(mass),2] = mean(tl$TotalStockBiomass)
  names(mass) = c("g","Biomass_kg")


  mar = plot_trophicpyramid_internal(mass[mass$g=="Carnivore",2],
                                     mass[mass$g=="Omnivore",2],
                                     mass[mass$g=="Herbivore",2],
                                     mass[mass$g=="Autotrophs",2],
                                     col,...)


  # add flow labels
  flow_a_h = (cons$Cohort_Consumed_Autotroph[cons$Group.1=="Herbivore"]/mass[mass$g=="Autotrophs",2])*100
  text(mar[1]-15,25,paste0(round(flow_a_h,flow_round),"%"))

  flow_h_o = (cons$Cohort_Consumed_Herbivore[cons$Group.1=="Omnivore"]/mass[mass$g=="Herbivore",2])/10
  text(mar[2]-20,45,paste0(round(flow_h_o,flow_round),"%"))

  flow_o_c = (cons$Cohort_Consumed_Omnivore[cons$Group.1=="Carnivore"]/mass[mass$g=="Omnivore",2])/10
  text(mar[3]-20,65,paste0(round(flow_o_c,flow_round),"%"))

  flow_c_c = (cons$Cohort_Consumed_Carnivore[cons$Group.1=="Carnivore"]/mass[mass$g=="Carnivore",2])/10
  text(mar[4]-15,85,paste0(round(flow_c_c,flow_round),"%"))

  flow_a_o = (cons$Cohort_Consumed_Autotroph[cons$Group.1=="Omnivore"]/mass[mass$g=="Autotrophs",2])/10
  text(100-mar[2]+30,35,paste0(round(flow_a_o,flow_round),"%"))

  flow_h_c = (cons$Cohort_Consumed_Herbivore[cons$Group.1=="Carnivore"]/mass[mass$g=="Herbivore",2])/10
  text(100-mar[2]+30,55,paste0(round(flow_h_c,flow_round),"%"))


  text(50,75,paste0("10^",round(log10(mass[mass$g=="Carnivore",2]),mass_round)," kg"),col=text_col[2])
  text(50,65,"Carnivores",col=text_col[2])
  text(50,55,paste0("10^",round(log10(mass[mass$g=="Omnivore",2]),mass_round)," kg"),col=text_col[3])
  text(50,45,"Omnivores",col=text_col[3])
  text(50,35,paste0("10^",round(log10(mass[mass$g=="Herbivore",2]),mass_round)," kg"),col=text_col[1])
  text(50,25,"Herbivores",col=text_col[1])
  text(50,15,paste0("10^",round(log10(mass[mass$g=="Autotrophs",2]),mass_round)," kg"),col=text_col[4])
  text(50,5,"Autotrophs",col=text_col[4])

}
