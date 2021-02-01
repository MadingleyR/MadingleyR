plot_timelines = function(madingley_data,select="functional groups",xlims=0,ylims=0,legend="topright",unclutter_val=0.7,plot=TRUE,timescale="months",...){

  # default colors
  cols_reds = c('#FF1300','#B23F36','#930B00','#FF5143','#FF8379')
  cols_blues = c('#104BA9','#2B4875','#042861','#477FD6','#7098D6')
  cols_greens = c('#00B358','#267D51','#006733','#3ADB89','#68DBA1')


  #--------------------------------------------------------- functional groups

  if(select=="functional groups"){

    l_types = c(sample(1:4,1))
    tl = madingley_data$time_line_cohorts

    if(timescale=="years"){
      # tl = aggregate(tl,by=list(tl$Year),FUN = mean)
      # tl$Month = tl$Year
      # tl$Group.1 = NULL
      # xlabel = "Time in years"
      xlabel = "Time in months"
    }else{
      xlabel = "Time in months"
    }

    tl = tl[, colSums(tl != 0) > 0]
    x_axis_max = round(max(tl$Month)/12)*12
    x_axis_min = 0
    if(length(xlims)==1) xlims = c(x_axis_min,x_axis_max)
    y_axis_max = ceiling(log10(max(madingley_data$time_line_stocks$TotalStockBiomass)))
    y_axis_min = floor(log10(min(tl[,3:ncol(tl)])))
    if(length(ylims)==1) ylims = c(y_axis_min,y_axis_max)
    if(is.infinite(y_axis_min)) ylims[1] = 0


    if(plot==F){
      return(tl)
    }

    if(plot){

      guilds = as.vector(madingley_data$cohort_def$DEFINITION_Nutrition.source)
      random_colors = c()
      for(i in guilds){
        if(i=="Herbivore"){
          random_colors = c(random_colors,sample(cols_greens,1))
        }else if(i=="Carnivore"){
          random_colors = c(random_colors,sample(cols_reds,1))
        }else if(i=="Omnivore"){
          random_colors = c(random_colors,sample(cols_blues,1))
        }else{
          random_colors = c(random_colors,'black')
        }
      }
      colors=random_colors

      plot(tl$Month,log10(tl[,3]),type="l",ylim=ylims,xlim=c(xlims[1],(xlims[2]+max(tl$Year)*2.5)),col=colors[1],
           lty=l_types,lwd=2,xaxs="i", yaxs="i",axes=F,xlab=xlabel,ylab="Log10 biomass [kg]",...)

      counter=2
      for(i in 4:ncol(tl)) {
        cur_color = colors[counter]
        l_type = sample(1:4,1)
        lines(tl$Month,log10(tl[,i]),type="l",col=cur_color,lty=l_type,lwd=2)
        colors = c(colors,cur_color)
        l_types = c(l_types,l_type)
        counter=counter+1
      }

      colors = c(colors,"black")
      lines(tl$Month,log10(madingley_data$time_line_stocks$TotalStockBiomass),lwd=2,col="black")

      l_types = c(l_types,1)

      names = paste("FG",matrix(unlist(strsplit(names(tl)[3:ncol(tl)],"_")),ncol =3, byrow=T)[,3])[]

      order_cohorts = order(tl[nrow(tl),grep('Biomass',colnames(tl))])
      max_cohorts = log10(max(tl)) + 1
      min_cohorts = log10(min(tl[, colSums(tl != 0) > 0][,3:ncol(tl[, colSums(tl != 0) > 0])]))
      n_names = ncol(tl[, colSums(tl != 0) > 0])-2

      if(is.infinite(min_cohorts)) min_cohorts = 1
      names_y_cohort = seq(min_cohorts,max_cohorts,abs(min_cohorts-max_cohorts)/n_names)
      names_y_auto = log10(madingley_data$time_line_stocks$TotalStockBiomass)[nrow(madingley_data$time_line_stocks)]

      names_y_cohort = names_y_cohort[is.finite(names_y_cohort)]

      norm_min_range = min_cohorts
      norm_max_range = max_cohorts


      names_y_cohort = (names_y_cohort-min(names_y_cohort))/
        (max(names_y_cohort)-min(names_y_cohort)) * (norm_max_range-norm_min_range) + norm_min_range
      names_y_cohort = c(names_y_cohort,names_y_auto)


      text_x_c = xlims[2]+max(tl$Year)*1.3
      text_y_c = names_y_cohort[1:length(names)]#[order_cohorts]

      norm_max_range = max(text_y_c)+unclutter_val
      norm_min_range = min(text_y_c)-unclutter_val

      if(norm_min_range>ylims[1]&norm_max_range<ylims[2]){
        text_y_c = ((text_y_c-min(text_y_c))/(max(text_y_c)-min(text_y_c)) )* (norm_max_range-norm_min_range) + norm_min_range
      }

      names = paste("FG",order_cohorts-1)
      text(text_x_c, y = text_y_c, labels = names, cex=0.9)
      text(xlims[2]+max(tl$Year), y = names_y_auto, labels = "Autotrophs",cex=0.9)

      ys_start = sort(log10(tl[nrow(tl),][3:ncol(tl)]))
      ys_stops = sort(text_y_c[1:length(ys_start)])

      colors2 = colors[order_cohorts]
      for(i in 1:length(ys_start)) {
        lines(x=c(xlims[2],(xlims[2]+max(tl$Year)*0.8)),y=c(ys_start[i],ys_stops[i]),col=colors2[i])
      }

      axis(1, at=c(seq(xlims[1],xlims[2],12)))
      if(is.infinite(ylims[1])) {
        axis(2, at=c(seq(0,ylims[2],1)),las=2)
      }else{
        axis(2, at=c(seq(ylims[1],ylims[2],1)),las=2)
      }
    }


  #--------------------------------------------------------- functional groups end

  #--------------------------------------------------------- feeding guilds
  }else if(select=="feeding guilds"){

    guilds = as.vector(madingley_data$cohort_def$DEFINITION_Nutrition.source)
    random_colors = c()
    for(i in guilds){
      if(i=="Herbivore"){
        random_colors = c(random_colors,sample("#00B358",1))
      }else if(i=="Carnivore"){
        random_colors = c(random_colors,sample("#FF1300",1))
      }else if(i=="Omnivore"){
        random_colors = c(random_colors,sample("#104BA9",1))
      }else{
        random_colors = c(random_colors,'black')
      }
    }

    tl = madingley_data$time_line_cohorts
    months = tl$Month
    years = tl$Year
    tl = tl[,3:ncol(tl)]
    fgs_not_zero = which(colSums(tl)!=0)
    tl = tl[,fgs_not_zero]
    guilds = guilds[fgs_not_zero]
    unique_guild = unique(guilds)
    df = as.data.frame(matrix(NA,nrow=nrow(tl),ncol=length(unique_guild)))
    names(df) = unique_guild
    for(i in unique_guild) df[,i] = rowSums(tl[,which(i==guilds)])
    df$Month = months
    df$Year = years

    ##
    if(timescale=="years"){
      df = aggregate(df,by=list(df$Year),FUN = mean)
      df$Month = df$Year
      df$Group.1 = NULL
      xlabel = "Time in years"
    }else{
      xlabel = "Time in months"
    }
    ##

    if(plot==F){
      if(timescale!="years"){
        return(df)
      }else{
        df_re = df
        df_re$Month = NULL
        return(df_re)
      }

    }else{


      colors = random_colors

      l_types = c(sample(1:4,1))

      x_axis_max = round(max(df$Month)/12)*12
      x_axis_min = 0
      if(length(xlims)==1) xlims = c(x_axis_min,x_axis_max)
      y_axis_max = ceiling(log10(max(madingley_data$time_line_stocks$TotalStockBiomass)))
      y_axis_min = floor(log10(min(df[,1:length(unique_guild)])))
      if(length(ylims)==1) ylims = c(y_axis_min,(y_axis_max+((y_axis_max-y_axis_min)/3)))
      if(is.infinite(y_axis_min)) ylims[1] = 0

      yearss = max(xlims)/12
      plot(df$Month,log10(df[,1]),type="l",ylim=ylims,xlim=c(xlims[1],(xlims[2]+yearss*2.5)),
           col=colors[1],lty=l_types,lwd=2,xaxs="i", yaxs="i",
           axes=F,xlab=xlabel,ylab="Log10 biomass [kg]",...)

      for(i in 2:length(unique_guild)) {
        cur_color = colors[i]
        l_type = sample(1:4,1)
        lines(df$Month,log10(df[,i]),type="l",col=cur_color,lty=l_type,lwd=2)
        colors = c(colors,cur_color)
        l_types = c(l_types,l_type)
      }



      colors[4] = "black"
      time_line_stocks = madingley_data$time_line_stocks

      if(timescale=="years") {
        time_line_stocks = aggregate(madingley_data$time_line_stocks,by=list(madingley_data$time_line_stocks$Year),FUN=mean)
        time_line_stocks$Month = time_line_stocks$Year
      }

      lines(df$Month,log10(time_line_stocks$TotalStockBiomass)[1:length(df$Month)],lwd=2,col="black")
      l_types = c(l_types,1)

      names = c(paste0(names(df)[1:length(unique_guild)],"s"),"Autotrophs")
      legend(legend, legend=names,col=colors, lty=l_types, cex=0.9,lwd=2,border = "white",bty = "n")

      names_y = c(log10(df[nrow(df),1:length(unique_guild)]),
                  log10(madingley_data$time_line_stocks$TotalStockBiomass)[nrow(madingley_data$time_line_stocks)])

      axis(1, at=c(seq(xlims[1],xlims[2],12)))
      axis(2, at=c(seq(ylims[1],ylims[2],1)),las=2)
    }
  }
  #--------------------------------------------------------- feeding guilds end


}
