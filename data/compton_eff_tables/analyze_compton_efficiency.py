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

material = "Schott B33"
alpha = 80  # um
standard_geometry = True

# If "standard_geometry" is set to true, then the alpha = beta = gamma = tau/2
# Otherwise individual parameters may be specified below

beta = 20
gamma = 20
tau = 40


if standard_geometry == True:
    beta = alpha
    gamma = alpha
    tau = int(2*alpha)


graph_title = (str(material) + " LMCP: " +  str(alpha) + "$\mu$m = " + " \u03B1 = \u03B2 = \u03B3 = \u03C4/2, T = 1\" "  )





data_directory = r'/local/d1/iangoldberg/LMCP/raw_data/latest_run/'
# data_directory = r'raw_data/latest_run'
energy_array = np.array([511])
# energy_array = np.array([200,300,400,500])
# theta_array = np.array([0])
# theta_array = np.array([0,30,60,90])
theta_array = np.array([45])
# phi_array = np.array([0,30,60,90])
phi_array = np.array([0])

SAVE_ARRAY = True
output_name = 'b33_effs'

def index(a, x):
    'Locate the leftmost value exactly equal to x'
    i = bisect.bisect_left(a, x)
    if i != len(a) and a[i] == x:
        return i
    raise ValueError

hist_dict = {}


for file_num, file_name in enumerate(os.listdir(data_directory)):

    with uproot.open(data_directory + '/' + file_name) as f:

        # Metadata for the final plots
        fname_components = file_name.split('_')
        energy = int(fname_components[1][1:])   # gamma ray energy
        theta_ind = int(fname_components[4][5:])
        theta = theta_array[theta_ind]
        phi_ind = int(fname_components[5][3:-5])
        phi = phi_array[phi_ind]
        wall_thickness = int(fname_components[2][1:])

        pore_tree = f['pore']
        pore_branches = pore_tree.arrays(library='ak')

        lam_tree = f['lamina']
        lam_branches = lam_tree.arrays(library='ak')

        first_scatter_cut = lam_branches['TrackID'] == 2
        first_scatter_energies = np.array(lam_branches['EKin'][first_scatter_cut][ak.any(first_scatter_cut, axis=1)][:,0])

        lam_eventnums = lam_branches['EventNumber'][ak.any(first_scatter_cut, axis=1)]
        pore_eventnums = pore_branches['EventNumber'][ak.any(pore_branches['TrackID'] == 2, axis=1)]

        # Builds cut on initial_energies based on events that reached pore
        initial_energies_cut = []
        percent = 0
        for i, eventnum in enumerate(pore_eventnums):
            if 100*float(i)/len(pore_eventnums) >= percent:
                print(f'{percent}%')
                percent += 20
            try:
                initial_energies_cut.append(index(lam_eventnums, eventnum))
            except:
                continue
        percent = 0     # resets counter if we use it again
        print('100%\n')
        initial_energies_cut = np.array(initial_energies_cut)

        bins = np.linspace(0,520,27)
        bin_centers = bins[:-1] + 0.5*np.diff(bins)[0]

        first_scatter_energies_pore = first_scatter_energies[initial_energies_cut]
        nlam, _ = np.histogram(first_scatter_energies, bins=bins)
        npore, _ = np.histogram(first_scatter_energies_pore, bins=bins)
        prohibited_cut = nlam >= 0.0005*len(first_scatter_energies)
        nlam = nlam[prohibited_cut]
        npore = npore[prohibited_cut]
        bin_centers = bin_centers[prohibited_cut]
        nlam = np.insert(nlam, 0, 1)
        npore = np.insert(npore, 0, 0)
        bin_centers = np.insert(bin_centers, 0, 0)
        
        # fig, ax = plt.subplots()
        # ax.hist(first_scatter_energies, bins=bins)
        # ax.hist(first_scatter_energies_pore, bins=bins)
        # plt.show()

        hist_dict[f'{energy} keV, $\\theta={theta}\degree$, $\phi={phi}\degree$'] = np.array([bin_centers, npore/nlam*100])

        x, y = bin_centers, npore/nlam*100
        tck = splrep(x,y, k=3)
        bspline = BSpline(*tck, extrapolate=False)
        fig, ax = plt.subplots()
        domain = np.linspace(0,x[-2], 500)
        ax.plot(domain, bspline(domain), color='red', linewidth=3, label='Simulation')
        domain = np.linspace(x[-2], 511, 100)
        ax.plot(domain, np.interp(domain, x[-2:], y[-2:]), linewidth=3, color='green', label='Linear interpolation')
        ax.scatter(x, y, marker='.', s=180, color='black', label='Raw data', zorder=5)
        ax.legend(fontsize=11,loc='lower right')

        # ax.grid(True)

        wrapped_text = "\n".join(textwrap.wrap(graph_title, width=30))

        ax.text(-5, 90, wrapped_text, fontsize=10, verticalalignment='top',
        bbox=dict(facecolor='lightgray', alpha=.5, edgecolor='black'))

        ax.set_xlabel('Primary $e^{-}$ Initial Energy (keV)', fontdict=dict(size=20))
        ax.set_ylabel('Pore Entry Efficiency (%)', fontdict=dict(size=20))

        ax.minorticks_on()
        ax.xaxis.set_minor_locator(MultipleLocator(50))
        ax.yaxis.set_minor_locator(MultipleLocator(10))

        ax.xaxis.set_ticks_position('both')
        ax.yaxis.set_ticks_position('both')





        plt.show()

        energy_points = np.linspace(0, 510, 103)
        effs = []
        for energy in energy_points:
            if energy <= x[-2]:
                effs.append(bspline(energy))
            else:
                effs.append(np.interp(energy, x[-2:], y[-2:]))
        effs = 0.01*np.array(effs)

        fig, ax = plt.subplots()
        ax.scatter(energy_points, effs*100, marker='.', color='black', label='Interpolated data')
        ax.legend(fontsize=11.5)
        ax.set_xlabel('Primary e- initial E (keV)', fontdict=dict(size=15))
        ax.set_ylabel('Efficiency (%)', fontdict=dict(size=15))
        ax.xaxis.set_ticks_position('both')
        ax.yaxis.set_ticks_position('both')
       # plt.show()

        round_effs = np.round(effs, decimals=4) + 0.     # add zero to get rid of -0.0
        eff_string = ''
        for eff in round_effs:
            eff_string += f'{eff}, '
        eff_string = eff_string[:-2]

        with open(os.path.dirname(os.path.realpath(__file__)) + '/' + output_name + '.csv', 'w') as f2:
            f2.write(eff_string)

        exit()



fig, ax = plt.subplots()
# Loops through like this so the legend is in ascending order
for energy in energy_array:
    for theta in theta_array:
        for phi in phi_array:
            for label in hist_dict:
                if not f'{energy} keV' in label:
                    continue
                elif not f'$\\theta={theta}\degree$' in label:
                    continue
                elif not f'$\phi={phi}\degree$' in label:
                    continue
                ax.scatter(hist_dict[label][0,:], hist_dict[label][1,:], marker='.')
                ax.plot(hist_dict[label][0,:], hist_dict[label][1,:], label=label)
ax.legend(fontsize=12.5)
ax.set_xlabel('Primary e- initial E (keV)', fontdict=dict(size=15))
ax.set_ylabel('Efficiency (%)', fontdict=dict(size=15))
ax.xaxis.set_ticks_position('both')
ax.yaxis.set_ticks_position('both')
plt.minorticks_on()
plt.show()

    
    




# OLD CODE
        # for i, eventnum in enumerate(lam_eventnums):
        #     if ak.any(ak.where(pore_eventnums == eventnum)):
        #         initial_energies_cut.append(i)
        #     if i%500 == 0:
        #         print(i)

        # double_events = []
        # for pore_eventnum in pore_eventnums:
        #     try:
        #         double_events.append(index(lam_eventnums, pore_eventnum))
        #     except:
        #         continue
        # double_events = np.array(double_events)
        # initial_energies_cut = np.copy(double_events)

        # initial_energies = lam_branches['EKin'][lam_branches['TrackID'] == 2][ak.any(lam_branches['TrackID'] == 2, axis=1)][:,0]
