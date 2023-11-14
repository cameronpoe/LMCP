import numpy as np
from matplotlib import pyplot as plt
import pickle
import awkward as ak

pickle_object_path = r'data/analyze_energy_distribution/g4_glass_lead/g4_glass_lead_energy_spec.pkl'
gamma_ray_energies = np.linspace(10, 600, 60, dtype=int)
wall_thicknesses = np.linspace(5,200,40,dtype=int)
substrate = 'G4_GLASS_LEAD'






label_conv_dict = {
    'overall': 'Overall',
    'eIoni': 'e- ionization',
    'compt': 'Compton',
    'phot': 'Photoelectric'
}

with open(pickle_object_path, 'rb') as f:

    hist_dict = pickle.load(f)
    
    for wall_energy in hist_dict:
        subhist_dict = hist_dict[wall_energy]
        wall, energy = wall_energy.split('-')
        wall = int(wall[1:])
        energy = int(energy[1:])

        hist_array = []
        hist_labels = []
        for proc in subhist_dict:
            hist_array.append(subhist_dict[proc])
            hist_labels.append(label_conv_dict[proc])
        all_electrons = ak.flatten(hist_array)

        fig, ax = plt.subplots()
        bins = np.linspace(0,520,53,dtype=int)
        width = 0.25*(bins[1]-bins[0])
        for i, single_hist_array in enumerate(hist_array):
            # vals, __ = np.histogram(single_hist_array, bins=bins)
            # vals = vals/len(all_electrons)*100
            # cur_bins = bins + i*width
            # ax.bar(cur_bins[:-1], vals, width, align='edge', label=hist_labels[i])

            vals, __ = np.histogram(single_hist_array, bins=bins)
            vals = vals/len(all_electrons)*100
            ax.plot(bins[:-1], vals, label=hist_labels[i])
            ax.scatter(bins[:-1], vals, marker='.')
        ax.legend()
        ax.set_xlabel('Electron energy (keV)', fontdict=dict(size=12.5))
        ax.set_ylabel('Fraction of electrons (% per 10 keV bin)', fontdict=dict(size=12.5))
        ax.set_title(f'{substrate}, {wall} um', fontdict=dict(size=14))
        ax.xaxis.set_ticks_position('both')
        ax.yaxis.set_ticks_position('both')
        plt.minorticks_on()
        plt.show()

