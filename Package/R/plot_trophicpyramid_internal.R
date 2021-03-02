plot_trophicpyramid_internal = function(carnbio,omnibio,herbbio,autobio,colors, ...){

  # # check package diagram
  # if (!"diagram" %in% installed.packages()[, "Package"]) {
  #   stop("Package 'diagram' not installed")
  # }else{
  #   require(diagram)
  # }

  # calc box coordinates
  widthauto = 80
  widthherb = 80*herbbio/autobio
  widthomni = 80*omnibio/autobio
  widthcarn = 80*carnbio/autobio
  widths=c(widthherb,widthomni,widthcarn)

  # normalize width
  norm_min_range = 30
  norm_max_range = 60
  widths = normalize_values_between_range(widths,norm_max_range,norm_min_range)
  widthauto = 90
  widthherb = widths[1]
  widthomni = widths[2]
  widthcarn = widths[3]

  # calc box margins
  marginauto = (100-widthauto)/2
  marginherb = (100-widthherb)/2
  marginomni = (100-widthomni)/2
  margincarn = (100-widthcarn)/2

  # add pyramide boxes
  #plot(1,1,col="white",xlim=c(-10,115),ylim=c(-10,115),xlab="",ylab="")
  plot(1,1,col="white",xlim=c(-15,120),ylim=c(-10,100),xaxs="i", yaxs="i",axes=F,xlab="",ylab="", ...)
  polygon(x=c(marginauto,100-marginauto,100-marginauto,marginauto),y=c(0,0,20,20),col=colors[4])
  polygon(x=c(marginherb,100-marginherb,100-marginherb,marginherb),y=c(20,20,40,40),col=colors[3])
  polygon(x=c(marginomni,100-marginomni,100-marginomni,marginomni),y=c(40,40,60,60),col=colors[2])
  polygon(x=c(margincarn,100-margincarn,100-margincarn,margincarn),y=c(60,60,80,80),col=colors[1])

  # add pyramide flows
  bend = -0.9
  dist = 2
  # diagram::curvedarrow
  add_food_web_connection(c(marginauto,15),c(marginherb-dist,25),  # a to h
              lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
              arr.pos = 1, curve = bend, dr = 0.1,arr.col="black",arr.lwd=0.5,
              endhead = FALSE, segment = c(0, 1),arr.type="triangle")
  add_food_web_connection(c(marginherb,35),c(marginomni-dist,45),  # h to o
              lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
              arr.pos = 1, curve = bend, dr = 0.1,arr.col="black",arr.lwd=0.5,
              endhead = FALSE, segment = c(0, 1),arr.type="triangle")
  add_food_web_connection(c(marginomni,55),c(margincarn-dist,65),  # o to c
              lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
              arr.pos = 1, curve = bend, dr = 0.1,arr.col="black",arr.lwd=0.5,
              endhead = FALSE, segment = c(0, 1),arr.type="triangle")
  add_food_web_connection(c(margincarn,75),c(margincarn+10,80+dist), # c to c
              lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
              arr.pos = 1, curve = bend, dr = 0.1,arr.col="black",arr.lwd=0.5,
              endhead = FALSE, segment = c(0, 1),arr.type="triangle")
  #add_food_web_connection(c(100-marginomni,50),c(100-marginomni+dist,55),  # o to o
  #             lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
  #             arr.pos = 1, curve = -bend, dr = 0.1,arr.col="black",arr.lwd=0.5,
  #             endhead = FALSE, segment = c(0, 1),arr.type="triangle")
  add_food_web_connection(c(100-marginauto,15),c(100-marginomni+dist,45),  # a to o
              lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
              arr.pos = 1, curve = -bend/2, dr = 0.1,arr.col="black",arr.lwd=0.5,
              endhead = FALSE, segment = c(0, 1),arr.type="triangle")

  add_food_web_connection(c(100-marginherb,35),c(100-margincarn+dist,65),  # h to c
              lwd =   2, lty = 1, lcol ="black", arr.width = 0.3,
              arr.pos = 1, curve = -bend/2, dr = 0.1,arr.col="black",arr.lwd=0.5,
              endhead = FALSE, segment = c(0, 1),arr.type="triangle")

  # Autotroph input??
  # add_food_web_connection(c(marginauto-dist,40),c(marginauto-dist,5), # auto input 1
  #             lwd =   0, lty = 1, lcol ="black", arr.width = 0.3,
  #             arr.pos = 1, curve = 0.1, dr = 0.1,arr.col="black",arr.lwd=0.5,
  #             endhead = FALSE, segment = c(0, 1),arr.type="triangle")
  # lines(c(0,5),c(5,5),lwd=2,lty=2) # auto input 1
  return(c(marginauto,marginherb,marginomni,margincarn))

}
