get_a_color = function(n){
  clrs1 = c("pink1", "violet", "mediumpurple1", "slateblue1", "purple", "purple3",
           "turquoise2", "skyblue", "steelblue", "blue2", "navyblue",
           "orange", "tomato", "coral2", "palevioletred", "violetred", "red2",
           "springgreen2", "yellowgreen", "palegreen4",
           "wheat2", "tan", "tan2", "tan3", "brown",
           "grey70", "grey50", "grey30")
  clrs2 = rep(c("#3691f6","#45f285","#f243d0","#ff534a","#f9fb2f"),4)
  return(sort(clrs2[n]))
}
