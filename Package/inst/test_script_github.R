# unload pkg and remove installation
detach('package:MadingleyR', unload=TRUE)
remove.packages('MadingleyR')
rm(list=ls())

# install pkg
devtools::install_github("SHoeks/MadingleyR", auth_token = "099f9209beb876055f3adc0f8e0c791cafaff6d1")

# test run
library(MadingleyR)
?madingley_init
??MadingleyR

# open installation dir
system(paste('open',get_lib_path()))

# open temp dir
system(paste('open',tempdir()))

# set model params
spatial_window = c(31,35,-5,-1) # Serengeti

# init model
m_data = madingley_init(spatial_window = spatial_window)

mm = madingley_inputs("model parameters")

# run model
#m_data2 = madingley_run(out_dir='C:/test test/',madingley_data = m_data, years = 4)
m_data2 = madingley_run(madingley_data = m_data, years = 1, dispersal_off = T, noise_cohort_order = 0.35,
                        output_timestep = c(99,99,99,99), parallel = F, model_parameters = mm)

# plots
plot_timelines(m_data2,select='functional groups',ylim=c(5.5,11.5))
plot_timelines(m_data2,select='feeding guilds',ylim=c(8,17))
plot_foodweb(m_data2,max_flows=5)

plot_densities(m_data2,weighted='biomass',xlim=c(-4,5))
plot_densities(m_data2,weighted='abundance',xlim=c(-4,5))
plot_trophicpyramid(m_data2)
plot_spatialabundances(m_data2,functional_filter=T)
plot_spatialbiomass(m_data2,functional_filter=T)


# non default run
spatial_window = c(110,160,-50,0) # Australia
sptl_inp = madingley_inputs('spatial inputs')
chrt_def = madingley_inputs('cohort definition')
stck_def = madingley_inputs('stock definition')
mdl_prms = madingley_inputs('model parameters')
plot_spatialwindow(spatial_window,sptl_inp[[1]])

cname = paste0(m_data2$cohort_def[, 2], '-', m_data2$cohort_def[, 7], '-', m_data2$cohort_def[, 6])
plot_spatialabundances(m_data2, functional_filter = T, box=F, axes=F, names = cname)


# init model
m_data = madingley_init(cohort_def=chrt_def,
                        stock_def=stck_def,
                        spatial_inputs=sptl_inp,
                        spatial_window=spatial_window,
                        max_cohort = 200)

m_data3 = madingley_run(madingley_data = m_data,
                        years = 4,
                        max_cohort = 50,
                        cohort_def = chrt_def,
                        stock_def = stck_def,
                        spatial_inputs = sptl_inp,
                        model_parameters = mdl_prms)


# plots for paper
plot_spatialwindow(m_data3$spatial_window,sptl_inp[[1]])
plot_timelines(m_data3,select='feeding guilds',ylim=c(8,17))
plot_densities(m_data3,weighted='biomass',xlim=c(-4,5))
plot_foodweb(m_data3,max_flows=5)
plot_trophicpyramid(m_data3)
plot_spatialbiomass(m_data3,functional_filter=F,zlim=c(7,15))
