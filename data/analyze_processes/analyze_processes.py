import numpy as np
import os
import uproot
import awkward as ak
import pickle

data_directory = r'../../raw_data/b33_procs/'
num_histories_per_run = 500000
wall_array = np.array([50])
pore_array = np.array([50])
gamma_ray_energies = np.linspace(10, 600, 60, dtype=int)
SINGLE_ZENITH = True
theta_increment = 5
SINGLE_AZIMUTH = True
phi_increment = 3





if SINGLE_ZENITH:
    theta_array = np.array([45])
else:
    theta_array = np.linspace(0, 90, int(90/theta_increment + 1))
if SINGLE_AZIMUTH:
      phi_array = np.array([0])
else:
    phi_array = np.linspace(0, 90, int(90/phi_increment + 1))


proc_plot_dict = {}

for file_name in os.listdir(data_directory):

    file_path = data_directory + file_name

    with uproot.open(file_path) as f:
            
        fname_components = file_name.split('_')
        energy = int(fname_components[1][1:])
        energy_ind = int((energy - gamma_ray_energies[0])/(gamma_ray_energies[1]-gamma_ray_energies[0]))
        
        pore_tree = f['pore']
        pore_branches = pore_tree.arrays(library='ak')
        pore_branches['CreatorProc'] = ak.str.split_pattern(pore_branches['CreatorProc'], '\n')
        
        labels, counts = np.unique(np.array(ak.flatten(pore_branches['CreatorProc'])), return_counts=True)

        overall_events_converted = len(pore_branches['EventNumber'])
        labels = np.append(labels, 'overall')
        counts = np.append(counts, overall_events_converted)

        proc_fracs = counts/num_histories_per_run

        for label, proc_frac in zip(labels, proc_fracs):
            if label not in proc_plot_dict:
                proc_plot_dict[label] = np.zeros_like(gamma_ray_energies, dtype=np.float64)
            proc_plot_dict[label][energy_ind] = proc_frac

save_file_path = data_directory.split('/')[-2]
with open(save_file_path + '.pkl', 'wb') as f:
    pickle.dump(proc_plot_dict, f, protocol=pickle.HIGHEST_PROTOCOL)



