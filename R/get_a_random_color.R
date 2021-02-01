get_a_random_color = function(scale){
  if(scale=="color") return(sample(c("#3691f6","#7448c5","#f8b415","#1c5c9c","#f243d0","orange","black"),1))
  if(scale=="greyscale") return(sample(c("#000000","#202020","#707070","#909090","#A9A9A9","#BEBEBE","#C8C8C8"),1))
}