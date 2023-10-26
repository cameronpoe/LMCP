import numpy as np
from matplotlib import pyplot as plt
import pickle

pickle_object_path = r'data/analyze_processes/constenergy_varwallsize/ecomass_procs_bywallsize.pkl'
gamma_ray_energies = np.linspace(10, 600, 60, dtype=int)
wall_thicknesses = np.linspace(5,200,40,dtype=int)
XAXIS = 'wall'




if XAXIS == 'wall':
    label = 'Wall/pore size (um)'
    xdata = wall_thicknesses

with open(pickle_object_path, 'rb') as f:

    proc_plot_dict = pickle.load(f)

    fig, ax = plt.subplots()
    for proc in proc_plot_dict:
        ax.plot(xdata, 100*proc_plot_dict[proc], label=proc)
        ax.scatter(xdata, 100*proc_plot_dict[proc], marker='.')
    ax.legend()
    ax.set_xlabel(label, fontdict=dict(size=12.5))
    ax.set_ylabel('Conversion efficiency (%)', fontdict=dict(size=12.5))
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.minorticks_on()

    fig, ax = plt.subplots()
    for proc in proc_plot_dict:
        if proc == 'overall':
            continue
        ax.plot(xdata, 100*proc_plot_dict[proc]/proc_plot_dict['overall'], label=proc)
        ax.scatter(xdata, 100*proc_plot_dict[proc]/proc_plot_dict['overall'], marker='.')
    ax.legend()
    ax.set_xlabel(label, fontdict=dict(size=12.5))
    ax.set_ylabel('Fraction of converted electrons (%)', fontdict=dict(size=12.5))
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    plt.minorticks_on()
    plt.show()


