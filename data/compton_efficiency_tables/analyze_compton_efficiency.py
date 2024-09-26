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
    "Electron Energy vs Pore Entry Efficiency",
    "NIST Lead Glass",
    'T = 1"',
]
graph_title = "\n".join(text_lines)

min_energy = 0
max_energy = 520
num_bins = 50
# graph_title = ("Pb Glass\n\u03B1 = \u03B2 = \u03B3 = \u03C4/2 = " + str(alpha) + "$ \mu$m\nT = 1\"")


data_directory = r"../../raw_data/latest_run/"

output_name = "b33_effs.csv"

lam_bins = np.array([0] * num_bins)
pore_bins = np.array([0] * num_bins)
bin_edges = np.linspace(min_energy, max_energy, num_bins + 1)
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2


def index(a, x):
    "Locate the leftmost value exactly equal to x"
    i = bisect.bisect_left(a, x)
    if i != len(a) and a[i] == x:
        return i
    raise ValueError


c = 0
for file_num, file_name in enumerate(os.listdir(data_directory)):
    with uproot.open(data_directory + "/" + file_name) as f:
        c += 1
        pore_tree = f["pore"]
        pore_branches = pore_tree.arrays(library="ak")
        lam_tree = f["lamina"]
        lam_branches = lam_tree.arrays(library="ak")

        first_scatter_cut = lam_branches["TrackID"] == 2
        first_scatter_energies = np.array(
            lam_branches["EKin"][first_scatter_cut][ak.any(first_scatter_cut, axis=1)][
                :, 0
            ]
        )

        lam_eventnums = lam_branches["EventNumber"][ak.any(first_scatter_cut, axis=1)]
        pore_eventnums = pore_branches["EventNumber"][
            ak.any(pore_branches["TrackID"] == 2, axis=1)
        ]

        # Builds cut on initial_energies based on events that reached pore
        initial_energies_cut = []
        for i, eventnum in enumerate(pore_eventnums):
            try:
                initial_energies_cut.append(index(lam_eventnums, eventnum))
            except:
                continue
        print(str(c) + "/" + str(len(os.listdir(data_directory))))
        initial_energies_cut = np.array(initial_energies_cut)

        first_scatter_energies_pore = first_scatter_energies[initial_energies_cut]
        nlam, _ = np.histogram(first_scatter_energies, bins=bin_edges)
        npore, _ = np.histogram(first_scatter_energies_pore, bins=bin_edges)
        lam_bins += nlam
        pore_bins += npore

total = np.sum(lam_bins)
sufficient_data_cut = np.where(lam_bins > 0.01 * total / num_bins)[0]
lam_bins = lam_bins[sufficient_data_cut]
pore_bins = pore_bins[sufficient_data_cut]
bin_filter = bin_centers[sufficient_data_cut]
yaxis = []
for i in range(len(lam_bins)):
    yaxis.append(float(pore_bins[i]) / lam_bins[i])
yaxis = np.array(yaxis)
fig, ax = plt.subplots()
full = np.interp(bin_centers, bin_filter, yaxis)
np.savetxt(output_name, full, delimiter=",")
ax.plot(bin_centers, full)


# aesthetic stuff
ax.xaxis.set_ticks_position("both")
ax.xaxis.set_ticks_position("both")
plt.minorticks_on()
ax.set_xlabel("Electron Energy (KeV)")
ax.set_ylabel("Probability of Reaching Pore")
plt.title(graph_title)

plt.show()
