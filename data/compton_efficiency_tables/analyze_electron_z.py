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
    "Electron Height Distribution at Pore Entry",
    "B33",
    'T = 1"',
]
graph_title = "\n".join(text_lines)

min_z = -20
max_z = 20
num_bins = 50
# graph_title = ("Pb Glass\n\u03B1 = \u03B2 = \u03B3 = \u03C4/2 = " + str(alpha) + "$ \mu$m\nT = 1\"")


data_directory = r"../../raw_data/latest_run/"

SAVE_ARRAY = True
output_name = "z_height"

bins = np.array([0] * num_bins)
bin_edges = np.linspace(min_z, max_z, num_bins + 1)
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2


c = 0
for file_num, file_name in enumerate(os.listdir(data_directory)):
    with uproot.open(data_directory + "/" + file_name) as f:
        c += 1
        pore_tree = f["pore"]
        pore_branches = pore_tree.arrays(library="ak")

        first_scatter_cut = pore_branches["TrackID"] == 2
        zs = np.array(
            pore_branches["PosZ"][first_scatter_cut][ak.any(first_scatter_cut, axis=1)][
                :, 0
            ]
        )
        print(str(c) + "/" + str(len(os.listdir(data_directory))))

        npore, _ = np.histogram(zs, bins=bin_edges)
        bins += npore

normalize = float(num_bins) / (np.sum(bins) * (max_z - min_z))
yaxis = bins * normalize
fig, ax = plt.subplots()
ax.plot(bin_centers, yaxis)

# aesthetic stuff
ax.xaxis.set_ticks_position("both")
plt.minorticks_on()
ax.set_xlabel("Height")
ax.set_ylabel("Frequency")
plt.title(graph_title)

plt.show()
