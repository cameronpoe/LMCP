import numpy as np
from matplotlib import pyplot as plt

file_path = r'data/eff_by_angle_ecomass.npy'
# file_path = r'data/eff_by_angle_lmcp_paper.npy'
phi_ind_of_interest = 30
num_diff_thetas = 31

eff_v_angle = np.load(file_path)
ydata = eff_v_angle[:,phi_ind_of_interest]
xdata = np.linspace(0,90,num_diff_thetas)
fig, ax = plt.subplots()
ax.scatter(xdata, ydata, marker='o', label='Geant4')
ax.plot(xdata, ydata)
ax.set_ylim(bottom=0, top=1.1*ydata.max())
ax.set_xlabel('Angle from normal (degrees)')
ax.set_ylabel('Efficiency (%)')
ax.set_title('Lead glass angular efficiencies')
ax.xaxis.set_ticks_position('both')
ax.yaxis.set_ticks_position('both')
plt.minorticks_on()

if False:
    extra = np.load(r'data/eff_by_angle_ecomass.npy')
    ydata_extra = extra[:,phi_ind_of_interest]
    ax.scatter(xdata, ydata_extra, marker='s', label='Extra')
    ax.plot(xdata, ydata_extra)

topas_version = r'/home/cameronpoe/Desktop/hybrid_gamma_multiplier/eff_by_angle_code/1in_eff_by_angle_ecomass.csv'
# topas_version = r'/home/cameronpoe/Desktop/hybrid_gamma_multiplier/hgm_lor_creator/1in_eff_by_angle_data.csv'
topas_effs = np.loadtxt(topas_version, delimiter=',', dtype=float)
topas_ydata = np.flip(topas_effs[:,30]*100)
ax.scatter(xdata, topas_ydata, marker='D', label='TOPAS')
ax.plot(xdata, topas_ydata)

ax.legend(loc='lower right')
plt.show()