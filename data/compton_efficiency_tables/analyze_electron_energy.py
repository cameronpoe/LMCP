import uproot
import awkward as ak
import numpy as np
import textwrap
from matplotlib import pyplot as plt
from matplotlib.ticker import MultipleLocator
import bisect
import os
from scipy.interpolate import BSpline, CubicSpline, splrep


##################################################
# Make sure to change the title to the material and dimensions you wish to plot below
##################################################


# If "standard_geometry" is set to true, then the alpha = beta = gamma = tau/2
# Otherwise individual parameters may be specified below


text_lines = [
    "Electron Energy in Pore",
    "Schott B33",
    'T = 1"',
]
graph_title = "\n".join(text_lines)

min_energy = 0
max_energy = 520
num_bins = 50
# graph_title = ("Pb Glass\n\u03B1 = \u03B2 = \u03B3 = \u03C4/2 = " + str(alpha) + "$ \mu$m\nT = 1\"")


data_directory = r"../../raw_data/latest_run/"

SAVE_ARRAY = True
output_name = "b33_effs"

hist_dict = {}


bins = np.linspace(0, 520, 27)
bin_centers = bins[:-1] + 0.5 * np.diff(bins)[0]

pore_bins = np.array([0] * num_bins)
bin_edges = np.linspace(min_energy, max_energy, num_bins + 1)
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2


def addDataToHistogram(bins, val):
    if val >= min_energy and val < max_energy:
        index = int(num_bins * (val - min_energy) / (max_energy - min_energy))
        bins[index] += 1


# determines whether to cut a pore event
def keep_pore(pore_event):
    return True


def getScatterEnergy(pore_branch):
    for i in range(len(pore_branch["TrackID"])):
        if pore_branch["TrackID"][i] == 2:
            return pore_branch["EKin"][i]
    return 0


c = 0
for file_num, file_name in enumerate(os.listdir(data_directory)):
    with uproot.open(data_directory + "/" + file_name) as f:
        c += 1
        pore_tree = f["pore"]
        pore_branches = pore_tree.arrays(library="ak")
        print(pore_tree.keys())
        # just fixing some formatting
        for i in range(len(pore_branches)):
            pore_branches[i]["CreatorProc"] = pore_branches[i]["CreatorProc"].split(
                "\n"
            )
        event_energy_dict = {}
        pore_energies = []
        for i in pore_branches:
            energy = getScatterEnergy(i)
            if energy != 0:
                event_energy_dict[i["EventNumber"]] = energy
                pore_energies.append(energy)
        for i in pore_energies:
            addDataToHistogram(pore_bins, i)
        print(str(c) + " / " + str(len(os.listdir(data_directory))))
        break
# Builds cut on initial_energies based on events that reached pore
fig, ax = plt.subplots()
yaxis = []
total = np.sum(pore_bins)
for i in range(len(pore_bins)):
    yaxis.append(float(pore_bins[i]) * num_bins / total)
yaxis = np.array(yaxis)
prohibited_cut = pore_bins >= 0.0005 * total
ax.plot(bin_centers[prohibited_cut], yaxis[prohibited_cut])

# aesthetic stuff
ax.xaxis.set_ticks_position("both")
ax.xaxis.set_ticks_position("both")
plt.minorticks_on()
ax.set_xlabel("Electron Energy (KeV)")
ax.set_xlabel("Frequency")
plt.title(graph_title)

plt.show()
