import numpy as np
from matplotlib import pyplot as plt
import pickle
import awkward as ak

pickle_object_path = r'data/analyze_energy_distribution/b33_energy_spec.pkl'
gamma_ray_energies = np.linspace(10, 600, 60, dtype=int)
wall_thicknesses = np.linspace(5,200,40,dtype=int)
substrate = 'B33'






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
        ax.hist(hist_array, label=hist_labels, bins=np.linspace(0,520,53,dtype=int), density=False)
        ax.legend()
        ax.set_xlabel('Gamma ray energy (keV)', fontdict=dict(size=12.5))
        ax.set_ylabel('Number of e-', fontdict=dict(size=12.5))
        ax.set_title(f'{substrate}, {wall} um, {len(all_electrons)} e-', fontdict=dict(size=14))
        ax.xaxis.set_ticks_position('both')
        ax.yaxis.set_ticks_position('both')
        plt.minorticks_on()

        fig, ax = plt.subplots()
        ax.hist(all_electrons, label='Overall', bins=np.linspace(0,520,53,dtype=int), density=False)
        ax.legend()
        ax.set_xlabel('Gamma ray energy (keV)', fontdict=dict(size=12.5))
        ax.set_ylabel('Number of e-', fontdict=dict(size=12.5))
        ax.set_title(f'{substrate}, {wall} um, {len(all_electrons)} e-', fontdict=dict(size=14))
        ax.xaxis.set_ticks_position('both')
        ax.yaxis.set_ticks_position('both')
        plt.minorticks_on()
        plt.show()

