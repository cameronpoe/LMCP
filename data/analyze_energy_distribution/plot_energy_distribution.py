import numpy as np
from matplotlib import pyplot as plt
import pickle

pickle_object_path = r'data/analyze_energy_distribution/g4_glass_lead_energy_spec.pkl'
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
    print(hist_dict)

    fig, ax = plt.subplots()
    hist_array = []
    hist_labels = []
    for proc in hist_dict:
        hist_array.append(hist_dict[proc])
        hist_labels.append(proc)
    ax.hist(hist_array, label=hist_labels, bins=np.linspace(0,600,61,dtype=int))
    ax.legend()
    ax.set_xlabel('Gamma ray energy (keV)', fontdict=dict(size=12.5))
    ax.set_ylabel('Number of e-', fontdict=dict(size=12.5))
    ax.set_title(substrate, fontdict=dict(size=14))
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.minorticks_on()

    # fig, ax = plt.subplots()
    # for proc in proc_plot_dict:
    #     if proc == 'overall':
    #         continue
    #     ax.plot(xdata, 100*proc_plot_dict[proc]/proc_plot_dict['overall'], label=label_conv_dict[proc])
    #     ax.scatter(xdata, 100*proc_plot_dict[proc]/proc_plot_dict['overall'], marker='.')
    # ax.legend()
    # ax.set_xlabel(label, fontdict=dict(size=12.5))
    # ax.set_ylabel('Fraction of converted electrons (%)', fontdict=dict(size=12.5))
    # ax.set_title(substrate, fontdict=dict(size=14))
    # ax.xaxis.set_ticks_position('both')
    # ax.yaxis.set_ticks_position('both')
    # plt.minorticks_on()
    plt.show()


