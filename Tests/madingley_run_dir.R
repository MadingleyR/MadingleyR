library(MadingleyR)

# load updated function versions
if(FALSE){
    library(devtools)
    load_all("Package")
}


dir.create("out")
init <- madingley_init(spatial_window = c(31, 35, -5, -1))
runout <- madingley_run(out_dir = "out", madingley_data = init, years = 5, silenced = TRUE)
runout2 <- madingley_run(out_dir = "out", madingley_data = runout, years = 5, silenced = TRUE)
str(runout2,1)

