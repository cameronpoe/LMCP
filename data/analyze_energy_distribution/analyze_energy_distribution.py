import numpy as np
import os
import uproot
import awkward as ak
import pickle

data_directory = r'../../raw_data/g4_glass_lead_energy_spec/'
num_histories_per_run = 500000

hist_dict = {}

for file_num, file_name in enumerate(os.listdir(data_directory)):

    print(f'Working on file number {file_num}')

    file_path = data_directory + file_name

    with uproot.open(file_path) as f:
        
        fname_components = file_name.split('_')
        energy = int(fname_components[1][1:])
        wall_thickness = int(fname_components[2][1:])
        
        pore_tree = f['pore']
        pore_branches = pore_tree.arrays(library='ak')
        pore_branches['CreatorProc'] = ak.str.split_pattern(pore_branches['CreatorProc'], '\n')
        
        intermediate_dict = {}
        labels = np.unique(np.array(ak.flatten(pore_branches['CreatorProc'])))
        for label in labels:
            intermediate_dict[label] = ak.flatten(pore_branches['EKin', pore_branches['CreatorProc'] == label])

        hist_dict[f'w{wall_thickness}-e{energy}'] = intermediate_dict

save_file_path = data_directory.split('/')[-2]
with open(save_file_path + '.pkl', 'wb') as f:
    pickle.dump(hist_dict, f, protocol=pickle.HIGHEST_PROTOCOL)



