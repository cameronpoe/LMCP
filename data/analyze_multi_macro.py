import numpy as np
import os
import uproot
import awkward as ak

data_directory = r"../raw_data/wp_data/"
num_histories_per_run = 500000
wall_array = np.linspace(5, 195, 39, dtype=int)
# wall_array = np.array([50])
pore_array = np.linspace(5, 145, 15, dtype=int)
# pore_array = np.array([50])
theta_increment = 5
phi_increment = 3
SINGLE_AZIMUTH = True
# if this is true will use the number of interacted gamma instead of the number of gammas created
EFFICIENCY_INTERACTED = True
SAVE_FOR_PET = False
SINGLE_LAMINA_THICKNESS = False


theta_array = np.linspace(0, 90, int(90 / theta_increment + 1))
theta_array = np.array([0])
phi_array = np.linspace(0, 90, int(90 / phi_increment + 1))
if SINGLE_AZIMUTH:
    phi_array = np.array([0])
if SINGLE_LAMINA_THICKNESS:
    pore_array = np.array([0])
if SAVE_FOR_PET:
    eff_v_angle = np.empty((len(theta_array), len(phi_array)))
else:
    eff_v_angle = np.empty(
        (len(wall_array), len(pore_array), len(theta_array), len(phi_array))
    )
print(f"eff_v_angle shape: {eff_v_angle.shape}")


for file_name in os.listdir(data_directory):
    file_path = data_directory + file_name

    with uproot.open(file_path) as f:
        fname_components = file_name.split("_")
        if SINGLE_LAMINA_THICKNESS:
            wall_ind = int(
                (int(fname_components[2][1:]) - wall_array[0]) / np.diff(wall_array)[0]
            )
            pore_ind = 0
        elif not SAVE_FOR_PET:
            wall_ind = int(
                (int(fname_components[2][1:]) - wall_array[0]) / np.diff(wall_array)[0]
            )
            pore_ind = int(
                (int(fname_components[3][1:]) - pore_array[0]) / np.diff(pore_array)[0]
            )
        theta_ind = int(fname_components[4][5:])
        phi_ind = int(fname_components[5][3:-5])

        pore_tree = f["pore"]
        pore_branches = pore_tree.arrays(library="ak")

        # num_events_reached_pore = len(f['pore']['EventNumber'].array(library='np'))
        # num_electrons_reached_pore = len(pore_branches['EventNumber', pore_branches['PDGID'] == 11])
        num_electrons_reached_pore = ak.sum(
            ak.any(pore_branches["PDGID"] == 11, axis=1)
        )

        if EFFICIENCY_INTERACTED:
            lamina_tree = f["lamina"]
            lamina_branches = lamina_tree.arrays(library="ak")
            if len(lamina_branches["EventNumber"]) == 0:
                eff = 0
            eff = float(num_electrons_reached_pore) / len(
                lamina_branches["EventNumber"]
            )
        else:
            eff = float(num_electrons_reached_pore) / num_histories_per_run
        if SAVE_FOR_PET:
            eff_v_angle[theta_ind][phi_ind] = eff
        else:
            eff_v_angle[wall_ind][pore_ind][theta_ind][phi_ind] = eff

np.save(data_directory.split("/")[-2], eff_v_angle)
