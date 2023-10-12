import numpy as np
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid

table_path = r'data/eff_v_angle_per_wall.npy'
phi_ind = 0
wall_thicknesses = np.linspace(10, 200, 20, dtype=int)



# code
table = np.load(table_path)

integrated_effs = []
zenith_angles = np.linspace(0,90,19, dtype=int)
for effs in table[:,:,0]:
    avg_eff = (1/90)*trapezoid(effs, zenith_angles)
    integrated_effs.append(avg_eff)
integrated_effs = np.array(integrated_effs)

fig, ax = plt.subplots()
ax.scatter(wall_thicknesses, integrated_effs, color='black')

ax.set_xlabel('Wall thickness (um)', fontdict=dict(size=14))
ax.set_ylabel('Average efficiency (zenith)', fontdict=dict(size=14))
ax.xaxis.set_ticks_position('both')
ax.yaxis.set_ticks_position('both')
plt.minorticks_on()
plt.show()

