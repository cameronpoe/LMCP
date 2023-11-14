import numpy as np

np_filename = r'data/g4_glass_lead_efftable_for_pet.npy'
output_table_name = r'1in_eff_by_angle_g4glasslead_wp50um'
data = np.load(np_filename)
data = np.transpose(data)

with open(output_table_name + '.csv', 'w') as f:
    for phi_row in data:
        row_string = ''
        for theta_eff in phi_row:
            row_string += (f' {round(theta_eff, 5)},')
        row_string = row_string[0:-1] + '\n'
        f.write(row_string)
