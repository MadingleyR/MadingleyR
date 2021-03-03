#detach('package:MadingleyR', unload=TRUE)
#remove.packages('MadingleyR')
#library(’remotes’) # or use library(’devtools’)
#install_github(’MadingleyR/MadingleyR’, subdir=’Package’)

#### Load packages
library('MadingleyR')
library('tinytest')

#### Load MadingleyR default inputs
sptl_inp = madingley_inputs("spatial inputs")
chrt_def = madingley_inputs("cohort definition")
stck_def = madingley_inputs("stock definition")
mdl_prms = madingley_inputs("model parameters") 

#### Init test model
mdata = madingley_init(spatial_window = spatial_window,
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp)

#### Run the Madingley model for 10 years
mdata2 = madingley_run(madingley_data = mdata,
                       years = 1,
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp,
                       model_parameters = mdl_prms)

#### Check if package works as before based in madingley_init() data generated

# test if all results are returned
expect_equal(names(mdata), 
             c("cohorts", "stocks", "cohort_def", 
               "stock_def", "spatial_window", "grid_size"))

# check if spatial window was set correctly
expect_equal(length(mdata$spatial_window), 4)

# check if the number of initialised cohorts is equal the length of the number expected from the cohort definitions
expect_equal(nrow(mdata$cohorts)/length(unique(mdata$cohorts$GridcellIndex)),
             sum(mdata$cohort_def$PROPERTY_Initial.number.of.GridCellCohorts))

# check if the number of cohorts per functional group is correct (based on cohort definition input file)
expected_n_cohorts_per_fg_init = structure(c(`0` = 880L, `1` = 880L, `2` = 880L, `3` = 880L, `4` = 880L, 
            `5` = 880L, `6` = 880L, `7` = 880L, `8` = 880L), .Dim = 9L, .Dimnames = structure(list(
              c("0", "1", "2", "3", "4", "5", "6", "7", "8")), .Names = ""), class = "table")
expect_identical(table(mdata$cohorts$FunctionalGroupIndex),expected_n_cohorts_per_fg_init)

#### Check if package works as before based in madingley_run() data generated

# check if >7000 are still alive (keep in mind simulation is stochastic)
expect_true(nrow(mdata2$cohorts)>7000)

# check if the number of cohorts per functional group is above 400
expect_false(any(table(mdata2$cohorts$FunctionalGroupIndex)<rep(400,9)))

# check if one of the cohort properties makes sense
expect_true(exp(max(mdata2$cohorts$LogOptimalPreyBodySizeRatio))<1.5)

# check if the body mass of the cohorts is still around the maximum set by the cohort definitions
expect_false(any(aggregate(mdata2$cohorts$AdultMass,by=list(mdata2$cohorts$FunctionalGroupIndex),max)[,2]>chrt_def$PROPERTY_Maximum.mass+50))
