library(MadingleyR)

# Set model params
spatial_window = c(-20, 55, -36, 40) 
#spatial_window = c(32, 34, -4, 0) # smaller test region
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Set years
years_spinup = 200
years_vegred = 5
years_postred = 200

# Load cohort definitions
chrt_def = madingley_inputs('cohort definition')
chrt_def[,13] = 40
max_chrt = sum(chrt_def[,13])

# Initialize model
mdata = madingley_init(spatial_window = spatial_window, 
                       spatial_inputs = sptl_inp,
                       max_cohort = max_chrt,
                       cohort_def = chrt_def)

# Run spin-up of 200 years 
mdata2 = madingley_run(madingley_data = mdata, 
                       years = years_spinup,
                       spatial_inputs = sptl_inp, 
                       max_cohort = max_chrt,
                       cohort_def = chrt_def,
                       output_timestep = rep(years_spinup-1,4),
                       out_dir = "~/Desktop/TempMadingleyOuts/")

plot_spatialbiomass(mdata2)
plot_timelines(mdata2)

save.image("~/Desktop/TempMadingleyOuts/env_sinup.RData")
#load("~/Desktop/TempMadingleyOuts/env_sinup.RData")

# Set scenario parameters
m_params = madingley_inputs('model parameters') # load default model parameters
mdata_list = list(mdata2)

# Loop over land-use intensities
for(i in 1:19) {
  
  # Reduce available stock biomass every iteration by 10%
  m_params[86, 2] = 1 - i * 0.05
  print(m_params[86, 2])
  
  # Run model with reduced stock availability
  mdata_list[[i+1]] = madingley_run(madingley_data = mdata_list[[i]], 
                                    years = years_vegred,
                                    model_parameters = m_params, 
                                    spatial_inputs = sptl_inp,
                                    max_cohort = max_chrt,
                                    cohort_def = chrt_def,
                                    output_timestep = c(999,999,999,999),
                                    silenced = F,
                                    out_dir = "~/Desktop/TempMadingleyOuts/",
                                    dispersal_off = F)
  
  # Rest for a bit
  #Sys.sleep(10)
  
}

save.image("~/Desktop/TempMadingleyOuts/env_reduction.RData")

# Run model for additional 100 years
removal_fraction = 0.95
list_index=removal_fraction/0.05+1
m_params[86, 2] = 1-removal_fraction
mdata4 = madingley_run(madingley_data = mdata_list[[list_index]], 
                       years = years_postred,
                       model_parameters = m_params, 
                       spatial_inputs = sptl_inp,
                       cohort_def = chrt_def,
                       max_cohort = max_chrt,
                       out_dir = "~/Desktop/TempMadingleyOuts/")



# Create plot
herb_bef = plot_spatialbiomass(mdata2, functional_filter = TRUE, plot = FALSE)[[1]]
herb_red = plot_spatialbiomass(mdata4, functional_filter = TRUE, plot = FALSE)[[1]]
r = (10^herb_red/10^herb_bef)*100 # calculate % difference
r[] = ifelse(herb_bef[]==0,0,r[]) # remove values were no herbivores were present
r_max = ceiling(quantile(r[],na.rm=TRUE,seq(0,1,0.05))["95%"]) 
r[] = ifelse(r[]>r_max,r_max+1e-9,r[])
pdf(paste0("~/Desktop/TempMadingleyOuts/env_",removal_fraction,"stabilization.pdf"),width=8,height=5)
plot(r,colNA="white", axes = FALSE, box = FALSE, legend = FALSE,
     col = colorRampPalette(c("#e5f5e0","#31a354"))(20),zlim=c(0,(r_max+1e-9)))
plot(r, legend.only=TRUE, col=colorRampPalette(c("#e5f5e0","#31a354"))(20),
     legend.width=1, legend.shrink=0.75, legend.mar = 42,
     axis.args=list(at=seq(0,r_max,5), labels=seq(0,r_max,5), cex.axis=0.6),
     legend.args=list(text='Herbivores\n remaining (%)', side=3, font=2, line=2.5, cex=0.8))
dev.off()

save.image(paste0("~/Desktop/TempMadingleyOuts/env_",removal_fraction,"_stabilization.RData"))
#load(paste0("~/Desktop/TempMadingleyOuts/env_",0.95,"stabilization.RData"))
