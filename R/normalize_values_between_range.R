normalize_values_between_range = function(vals,max_r,min_r){
  vals2 = (vals-min(vals))/(max(vals)-min(vals)) * (max_r-min_r) + min_r
  return(vals2)
}