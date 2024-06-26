import numpy as np
from datetime import date
import os
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r"./latest_run.npy"
print("Looking for " + os.path.abspath(table_path))
compute_max = False
##############################################
###                  CODE                  ###
##############################################

table = np.load(table_path)
print(table.shape)

integrated_effs = []
i = 0

spline_tuple = splrep(table[:, 0], table[:, 1], k=3)
data_bspline = BSpline(*spline_tuple)
ddata_bspline = data_bspline.derivative()
spline_domain = np.linspace(table[0, 0], table[-1, 0], 1000)
dcubic_spline = CubicSpline(spline_domain, ddata_bspline(spline_domain))

fig, ax = plt.subplots()
ax.scatter(table[:, 0], 100 * table[:, 1], color="black", label="Raw data")
ax.plot(
    spline_domain, 100 * data_bspline(spline_domain), color="red", label="Spline fit"
)
if compute_max:
    extrema = dcubic_spline.solve(0, extrapolate=False)[0]
    ax.axvline(extrema, label="Maximum: {:.2f} um".format(extrema))
    ax.axhline(
        data_bspline(extrema) * 100,
        label="Maximum: {:.2f} um".format(data_bspline(extrema) * 100),
    )
ax.set_ylim(bottom=0, top=105 * table[:, 1].max())
ax.set_xlim(left=0)
ax.set_xlabel("Alpha", fontdict=dict(size=12))
ax.set_ylabel("Pore Entry Efficiency (%)", fontdict=dict(size=12))
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
file_name += "160 tau Lead Glass Semistandard Geometry"
plot_title = "T: " + lamina_depth + ", "
plot_title += "Photon Energy: " + photon_energy + "\n"
plot_title += "τ=160" + " μm, "
plot_title += "Lead Glass\n"
plot_title += "Angle of Incidence From Normal: 0-20 Degrees"

plt.title(plot_title)
plt.text(0.02, 0.02, str(date.today()), ha="left", va="top", transform=ax.transAxes)
fig.canvas.get_default_filename = lambda: file_name


plt.show()
