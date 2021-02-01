load_input_csvs = function(dir){
  files = paste0(dir,list.files(dir))
  inputs_list = lapply(files,read.csv)
  names(inputs_list) = list.files(dir)
  return(inputs_list)
}