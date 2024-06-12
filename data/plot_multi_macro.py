import numpy as np
from datetime import date
import os
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r"./latest_run.npy"
print("Looking for " + os.path.abspath(table_path))
# table_path = r'data/wall_thickness_optimization/eff_v_angle_per_wall_g4glasslead.npy'
phi_ind = 0
theta_spacing = 5  # deg
pore_widths = np.array([40])
# pore_widths = np.array([50])
wall_thicknesses = np.linspace(5, 495, 99, dtype=int)  # um
# wall_thicknesses = np.array([75])
dependent_var = "wall"  # can be 'pore' or 'wall'


##############################################
###                  CODE                  ###
##############################################

table = np.load(table_path)
print(table.shape)
if dependent_var == "pore":
    table = table[0, :, :, phi_ind]
    worked_data = np.empty((len(pore_widths), 2))
    worked_data[:, 0] = pore_widths
    label = "Pore widths (um)"
elif dependent_var == "wall":
    table = table[:, 0, :, phi_ind]
    # table = table[:,:,phi_ind]
    worked_data = np.empty((len(wall_thicknesses), 2))
    worked_data[:, 0] = wall_thicknesses
    label = "τ-α (μm)"
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
    max_val = 100 * data_bspline(extrema)
    ax.axvline(extrema, label="Maximum: {:.2f} um".format(extrema))
    ax.axhline(max_val, label="Maximum: {:.2f} %".format(max_val))
    alpha = 40
    extrema = alpha
    max_val = 100 * data_bspline(extrema)
    ax.axvline(extrema, label="τ=2α: {:.2f} um".format(extrema + alpha), color="orange")
    ax.axhline(max_val, label="τ=2α: {:.2f} %".format(max_val), color="orange")
    extrema = 2 * alpha
    max_val = 100 * data_bspline(extrema)
    ax.axvline(extrema, label="τ=3α: {:.2f} um".format(extrema + alpha), color="green")
    ax.axhline(max_val, label="τ=3α: {:.2f} %".format(max_val), color="green")
except:
    print("no extrema")
ax.set_xlabel(label, fontdict=dict(size=12))
ax.set_ylabel("Average zenith efficiency (%)", fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()


lamina_depth = "1 in"
photon_energy = "511 KeV"
xlabel = "Wall Thickness"
lamina_thickness = 100
###Code
file_name = str(date.today())
file_name += ", " + xlabel + " vs " + "Efficiency Single Lamina Thickness"
plot_title = "W: " + lamina_depth + ", "
plot_title += "Photon Energy: " + photon_energy + "\n"
plot_title += "λ=β=α=" + "40" + " μm, "
plot_title += "B33 Glass"

plt.title(plot_title)
plt.text(0.99, 0.99, str(date.today()), ha="right", va="top", transform=ax.transAxes)
fig.canvas.get_default_filename = lambda: file_name


plt.show()
