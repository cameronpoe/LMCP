import numpy as np
import os
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r"./wp_data.npy"
print("Looking for " + os.path.abspath(table_path))
# table_path = r'data/wall_thickness_optimization/eff_v_angle_per_wall_g4glasslead.npy'
phi_ind = 0
theta_spacing = 5  # deg
pore_widths = np.linspace(5, 145, 15, dtype=int)
# pore_widths = np.array([50])
wall_thicknesses = np.linspace(5, 195, 39, dtype=int)
# wall_thicknesses = np.array([75])
dependent_var = "pore"  # can be 'pore' or 'wall'


##############################################
###                  CODE                  ###
##############################################

table = np.load(table_path)
print(table.shape)
if dependent_var == "pore":
    table = table[8, :, :, phi_ind]
    worked_data = np.empty((len(pore_widths), 2))
    worked_data[:, 0] = pore_widths
    label = "Pore widths (um)"
elif dependent_var == "wall":
    table = table[:, 8, :, phi_ind]
    # table = table[:,:,phi_ind]
    worked_data = np.empty((len(wall_thicknesses), 2))
    worked_data[:, 0] = wall_thicknesses
    label = "Wall thicknesses (um)"
else:
    print(f"'{dependent_var}' is not an allowed choice!")
    exit()

integrated_effs = []
zenith_angles = np.linspace(0, 90, int(90 / theta_spacing + 1), dtype=int)
i = 0
while i < table.shape[0]:
    effs = table[i, :]
    # fig, ax = plt.subplots()
    # ax.scatter(zenith_angles, effs)
    # plt.show()
    avg_eff = (1 / 90) * trapezoid(effs, zenith_angles)
    integrated_effs.append(avg_eff)
    i += 1
worked_data[:, 1] = np.array(integrated_effs)

spline_tuple = splrep(worked_data[:, 0], worked_data[:, 1], k=3)
data_bspline = BSpline(*spline_tuple)
ddata_bspline = data_bspline.derivative()
spline_domain = np.linspace(worked_data[0, 0], worked_data[-1, 0], 1000)
dcubic_spline = CubicSpline(spline_domain, ddata_bspline(spline_domain))

fig, ax = plt.subplots()
ax.scatter(worked_data[:, 0], 100 * worked_data[:, 1], color="black", label="Raw data")
ax.plot(
    spline_domain, 100 * data_bspline(spline_domain), color="red", label="Spline fit"
)
ax.set_ylim(bottom=0, top=105 * worked_data[:, 1].max())
ax.set_xlim(left=0)
try:
    extrema = dcubic_spline.solve(0, extrapolate=False)[0]
    ax.axvline(extrema, label="Maximum: {:.2f} um".format(extrema))
except:
    print("no extrema")
ax.set_xlabel(label, fontdict=dict(size=12))
ax.set_ylabel("Average zenith efficiency (%)", fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()
plt.show()
