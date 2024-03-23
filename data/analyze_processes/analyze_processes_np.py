import numpy as np
import os
import uproot
import awkward as ak

data_directory = r"../../raw_data/wp_data/"
print("Looking In " + os.path.abspath(data_directory))
num_histories_per_run = 10000
wall_array = np.linspace(5, 195, 39, dtype=int)
# wall_array = np.array([50])
pore_array = np.linspace(5, 145, 15, dtype=int)
# pore_array = np.array([50])
theta_increment = 5
phi_increment = 3
SINGLE_AZIMUTH = True
SINGLE_LAMINA_THICKNESS = False


theta_array = np.linspace(0, 90, int(90 / theta_increment + 1))
phi_array = np.linspace(0, 90, int(90 / phi_increment + 1))
if SINGLE_AZIMUTH:
    phi_array = np.array([0])
if SINGLE_LAMINA_THICKNESS:
    pore_array = np.array([0])
print(
    f"eff_v_angle shape: {(len(wall_array), len(pore_array), len(theta_array), len(phi_array))}"
)


# adds piece of data to appropiate table or creates a new table
def addData(dictionary, process, efficiency, index):
    if process in dictionary:
        dictionary[process][index] = efficiency
    else:
        dictionary[process] = np.empty(
            (len(wall_array), len(pore_array), len(theta_array), len(phi_array))
        )
        dictionary[process][index] = efficiency


process_table_dict = {}
for file_num, file_name in enumerate(os.listdir(data_directory)):
    print(f"Working on file number {file_num}")
    file_path = data_directory + file_name

    with uproot.open(file_path) as f:
        fname_components = file_name.split("_")
        if SINGLE_LAMINA_THICKNESS:
            wall_ind = int(
                (int(fname_components[2][1:]) - wall_array[0]) / np.diff(wall_array)[0]
            )
            pore_ind = 0
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
        pore_branches["CreatorProc"] = ak.str.split_pattern(
            pore_branches["CreatorProc"], "\n"
        )

        labels = np.unique(np.array(ak.flatten(pore_branches["CreatorProc"])))
        for label in labels:
            num_electrons_reached_pore = len(
                pore_branches["CreatorProc"][
                    ak.any(pore_branches["CreatorProc"] == label, axis=1)
                ]
            )
            eff = num_electrons_reached_pore / num_histories_per_run
            addData(
                process_table_dict, label, eff, (wall_ind, pore_ind, theta_ind, phi_ind)
            )
        overall_events_converted = len(pore_branches["EventNumber"])
        total_eff = overall_events_converted / num_histories_per_run
        addData(
            process_table_dict,
            "overall",
            total_eff,
            (wall_ind, pore_ind, theta_ind, phi_ind),
        )
        # num_events_reached_pore = len(f['pore']['EventNumber'].array(library='np'))
        # num_electrons_reached_pore = len(pore_branches['EventNumber', pore_branches['PDGID'] == 11])

for process in process_table_dict:
    np.save(process + "_" + data_directory.split("/")[-2], process_table_dict[process])
