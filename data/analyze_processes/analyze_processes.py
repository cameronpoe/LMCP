import numpy as np
import os
import uproot
import awkward as ak
import pickle

data_directory = r'../../raw_data/peek_procs/'
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

for file_num, file_name in enumerate(os.listdir(data_directory)):

    print(f'Working on file number {file_num}')

    file_path = data_directory + file_name

    with uproot.open(file_path) as f:
            
        fname_components = file_name.split('_')
        energy = int(fname_components[1][1:])
        energy_ind = int((energy - gamma_ray_energies[0])/(gamma_ray_energies[1]-gamma_ray_energies[0]))
        
        pore_tree = f['pore']
        pore_branches = pore_tree.arrays(library='ak')
        pore_branches['CreatorProc'] = ak.str.split_pattern(pore_branches['CreatorProc'], '\n')
        
        intermediate_dict = {}
        labels = np.unique(np.array(ak.flatten(pore_branches['CreatorProc'])))
        for label in labels:
            print('New label: ' + str(label))
            intermediate_dict[label] = len(pore_branches['CreatorPorc'][ak.any(pore_branches['CreatorProc'] == label, axis=1)])

        print(intermediate_dict)

        overall_events_converted = len(pore_branches['EventNumber'])

        print('Total converted' + str(overall_events_converted))
        intermediate_dict['overall'] = overall_events_converted

        for label in intermediate_dict:
            if label not in proc_plot_dict:
                proc_plot_dict[label] = np.zeros_like(gamma_ray_energies, dtype=np.float64)
            proc_plot_dict[label][energy_ind] = intermediate_dict[label]/num_histories_per_run

save_file_path = data_directory.split('/')[-2]
with open(save_file_path + '.pkl', 'wb') as f:
    pickle.dump(proc_plot_dict, f, protocol=pickle.HIGHEST_PROTOCOL)



