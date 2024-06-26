import numpy as np
import os
import uproot
import awkward as ak

data_directory = r"../raw_data/latest_run/"
num_histories_per_run = 10000

eff_v_xaxis = {}


def log_file(xaxis, efficiency):
    if xaxis in eff_v_xaxis:
        eff_v_xaxis[xaxis].append(efficiency)
    else:
        eff_v_xaxis[xaxis] = [efficiency]


def get_conversion_efficiency(pore_branches):
    num_electrons_reached_pore = ak.sum(ak.any(pore_branches["PDGID"] == 11, axis=1))

    eff = float(num_electrons_reached_pore) / num_histories_per_run
    return eff


def get_efficiency_given_interacted(pore_branches):
    num_electrons_reached_pore = ak.sum(ak.any(pore_branches["PDGID"] == 11, axis=1))

    lamina_tree = f["lamina"]
    lamina_branches = lamina_tree.arrays(library="ak")
    if len(lamina_branches["EventNumber"]) == 0:
        eff = 0
    eff = float(num_electrons_reached_pore) / len(lamina_branches["EventNumber"])
    return eff


def get_interacted_efficiency(pore_branches):
    num_electrons_reached_pore = ak.sum(ak.any(pore_branches["PDGID"] == 11, axis=1))

    lamina_tree = f["lamina"]
    lamina_branches = lamina_tree.arrays(library="ak")
    if len(lamina_branches["EventNumber"]) == 0:
        eff = 0
    eff = len(lamina_branches["EventNumber"]) / num_histories_per_run
    return eff


for file_name in os.listdir(data_directory):
    file_path = data_directory + file_name
    with uproot.open(file_path) as f:
        fname_components = file_name.split("_")
        xaxis = int("".join(c for c in fname_components[0] if c.isdigit()))
        pore_tree = f["pore"]
        pore_branches = pore_tree.arrays(library="ak")
        log_file(xaxis, get_interacted_efficiency(pore_branches))
        # num_events_reached_pore = len(f['pore']['EventNumber'].array(library='np'))
        # num_electrons_reached_pore = len(pore_branches['EventNumber', pore_branches['PDGID'] == 11])
xaxis_values = list(eff_v_xaxis)
xaxis_values.sort()
xaxis_values = np.array(xaxis_values)
efficiencies = [np.mean(eff_v_xaxis[c]) for c in xaxis_values]
efficiencies = np.array(efficiencies)
data = np.empty((len(xaxis_values), 2))
data[:, 0] = xaxis_values
data[:, 1] = efficiencies
print("saving to " + data_directory.split("/")[-2])
np.save(data_directory.split("/")[-2], data)
