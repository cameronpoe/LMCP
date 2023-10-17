import numpy as np
import os
import uproot

data_directory = r'../raw_data/g4_glass_lead_varpore_wide/'
num_histories_per_run = 500000
wall_array = np.array([50])
pore_array = np.linspace(5, 80, 16, dtype=int)
theta_increment = 5
phi_increment = 3
SINGLE_AZIMUTH = True



theta_array = np.linspace(0, 90, int(90/theta_increment + 1))
phi_array = np.linspace(0, 90, int(90/phi_increment + 1))
if SINGLE_AZIMUTH:
      phi_array = np.array([0])

eff_v_angle = np.empty((len(wall_array), len(pore_array), len(theta_array), len(phi_array)))

for file_name in os.listdir(data_directory):

    file_path = data_directory + file_name

    with uproot.open(file_path) as f:
            
            fname_components = file_name.split('_')

            wall_ind = 0 #int(int(fname_components[1][1:])/10 - 1)
            pore_ind = int(int(fname_components[2][1:])/5 - 1)
            theta_ind = int(fname_components[3][5:])
            phi_ind = int(fname_components[4][3:-5])

            num_events_reached_pore = len(f['pore']['EventNumber'].array(library='np'))
                  
            eff = num_events_reached_pore/num_histories_per_run

            eff_v_angle[wall_ind][pore_ind][theta_ind][phi_ind] = eff

np.save(data_directory.split('/')[-2], eff_v_angle)
