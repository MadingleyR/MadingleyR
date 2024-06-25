install.packages("remotes", repos="http://cran.rstudio.com/")
remotes::install_github("MadingleyR/MadingleyR", subdir="Package")
library('MadingleyR')

# Load MadingleyR default inputs
sptl_inp = madingley_inputs("spatial inputs")
chrt_def = madingley_inputs("cohort definition")
stck_def = madingley_inputs("stock definition")
mdl_prms = madingley_inputs("model parameters") 

# set spatial window
spatial_window = c(31, 35, -5, -1)

mdata = madingley_init(spatial_window = spatial_window,
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp)

mdata2 = madingley_run(madingley_data = mdata,
                       years = 1,
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp,
                       parallel = FALSE)

# Check if package works as before based in madingley_init() data generated
checks = rep(FALSE,5)
checks[1] = length(mdata)==6
checks[2] = !any(names(mdata) != c("cohorts", "stocks", "cohort_def", "stock_def", "spatial_window", "grid_size"))
checks[3] = length(mdata$spatial_window)==4
checks[4] = nrow(mdata$cohorts)/length(unique(mdata$cohorts$GridcellIndex))==sum(mdata$cohort_def$PROPERTY_Initial.number.of.GridCellCohorts)
expected_n_cohorts_per_fg_init = structure(c(`0` = 880L, `1` = 880L, `2` = 880L, `3` = 880L, `4` = 880L, 
            `5` = 880L, `6` = 880L, `7` = 880L, `8` = 880L), .Dim = 9L, .Dimnames = structure(list(
              c("0", "1", "2", "3", "4", "5", "6", "7", "8")), .Names = ""), class = "table")
checks[5] = !any(table(mdata$cohorts$FunctionalGroupIndex)!=expected_n_cohorts_per_fg_init)

for(i in checks){
  if(i){
    cat("check passed\n")
  }else{
    stop("check not passed")
  }
}




