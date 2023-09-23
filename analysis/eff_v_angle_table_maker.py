import numpy as np
import uproot
import os

num_primaries = 5000
data_directory = r'data/eff_v_angle_data/'
num_diff_thetas = 31
num_diff_phis = 31

SAVE_ARRAY = True
array_save_name = r'eff_by_angle_ecomass'


eff_v_angle = np.empty((num_diff_thetas, num_diff_phis))
for file_name in os.listdir(data_directory):
    with uproot.open(data_directory + file_name) as f:
        indices = file_name.split('_')
        theta_ind = int(indices[1][5:])
        phi_ind = int(indices[2][3:][:-5])

        secondary_ids = f['pore']['ID'].array(library='np')
        num_secondaries = 0
        for secondary_id_array in secondary_ids:
            num_secondaries += len(secondary_id_array)
        eff = num_secondaries/num_primaries*100

        eff_v_angle[theta_ind, phi_ind] = eff

np.save('data/' + array_save_name, eff_v_angle)