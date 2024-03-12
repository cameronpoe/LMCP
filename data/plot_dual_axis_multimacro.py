import numpy as np
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r"./wp_data.npy"
phi_ind = 0
theta_spacing = 5  # deg
wall_thicknesses = np.linspace(5, 195, 39, dtype=int)
pore_widths = np.linspace(5, 145, 15, dtype=int)


##############################################
###                  CODE                  ###
##############################################

table = np.load(table_path)
print(table.shape)
table = table[:, :, :, phi_ind]

integrated_effs = np.ones((len(wall_thicknesses), len(pore_widths)))
zenith_angles = np.linspace(0, 90, int(90 / theta_spacing + 1), dtype=int)
for i, subtable in enumerate(table):
    for j, effs in enumerate(subtable):
        avg_eff = (1 / 90) * trapezoid(effs, zenith_angles)
        integrated_effs[i, j] = avg_eff * 100


fig, ax = plt.subplots()
colors = ax.imshow(
    integrated_effs, interpolation=None, origin="lower", extent=(0, 145, 0, 145)
)
cbar = fig.colorbar(colors, ax=ax)
cbar.set_label("Conversion efficiency (%)", fontdict=dict(size=12.5))
ax.set_xlabel("Wall thicknesses (um)", fontdict=dict(size=12.5))
ax.set_ylabel("Pore width (um)", fontdict=dict(size=12.5))
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()
plt.show()

fig2, ax2 = plt.subplots()


def quick_func(t, w):
    return 4 / (t * t + 2 * t * w)


test_array = np.empty((15, 15))
i = 0
while i < 15:
    j = 0
    while j < 15:
        test_array[i, j] = quick_func(i + 20, j + 20)
        j += 1
    i += 1

colors = ax2.imshow(test_array, interpolation=None, origin="lower")
cbar = fig2.colorbar(colors, ax=ax2)

exit()

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
extrema = dcubic_spline.solve(0, extrapolate=False)[0]

fig, ax = plt.subplots()
ax.scatter(worked_data[:, 0], 100 * worked_data[:, 1], color="black", label="Raw data")
ax.plot(
    spline_domain, 100 * data_bspline(spline_domain), color="red", label="Spline fit"
)
ax.set_ylim(bottom=0, top=105 * worked_data[:, 1].max())
ax.set_xlim(left=0)
ax.axvline(extrema, label="Maximum: {:.2f} um".format(extrema))
ax.set_xlabel(label, fontdict=dict(size=12))
ax.set_ylabel("Average zenith efficiency (%)", fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()
plt.show()
