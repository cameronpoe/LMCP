import numpy as np
from matplotlib import pyplot as plt
import pickle

pickle_object_path = r'data/analyze_processes/varenergy_constporesize/peek_procs.pkl'
gamma_ray_energies = np.linspace(10, 600, 60, dtype=int)
wall_thicknesses = np.linspace(5,200,40,dtype=int)
substrate = 'PEEK'
XAXIS = 'energy'        # can be 'wall', 'energy'






label_conv_dict = {
    'overall': 'Overall',
    'eIoni': 'e- ionization',
    'compt': 'Compton',
    'phot': 'Photoelectric'
}

if XAXIS == 'wall':
    label = 'Wall/pore size (um)'
    xdata = wall_thicknesses
elif XAXIS == 'energy':
    label = 'Gamma ray energy (keV)'
    xdata = gamma_ray_energies

with open(pickle_object_path, 'rb') as f:

    proc_plot_dict = pickle.load(f)

    fig, ax = plt.subplots()
    for proc in proc_plot_dict:
        ax.plot(xdata, 100*proc_plot_dict[proc], label=label_conv_dict[proc])
        ax.scatter(xdata, 100*proc_plot_dict[proc], marker='.')
    ax.legend()
    ax.set_xlabel(label, fontdict=dict(size=12.5))
    ax.set_ylabel('Conversion efficiency (%)', fontdict=dict(size=12.5))
    ax.set_title(substrate, fontdict=dict(size=14))
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.minorticks_on()

    fig, ax = plt.subplots()
    for proc in proc_plot_dict:
        if proc == 'overall':
            continue
        ax.plot(xdata, 100*proc_plot_dict[proc]/proc_plot_dict['overall'], label=label_conv_dict[proc])
        ax.scatter(xdata, 100*proc_plot_dict[proc]/proc_plot_dict['overall'], marker='.')
    ax.legend()
    ax.set_xlabel(label, fontdict=dict(size=12.5))
    ax.set_ylabel('Fraction of converted electrons (%)', fontdict=dict(size=12.5))
    ax.set_title(substrate, fontdict=dict(size=14))
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.minorticks_on()
    plt.show()


