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
ax.set_xlabel("τ", fontdict=dict(size=12))
ax.set_ylabel("Pore Entry Efficiency Given Interacted (%)", fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()


lamina_depth = "1 in"
photon_energy = "511 KeV"
xlabel = "Wall Thickness"
lamina_thickness = 300

###Code

## Titles and labels:
file_name = str(date.today())
file_name += "tau varying electron penetrating wall"
plot_title = "Prob of electron penetrating ≥1 wall"
plot_info = "511 KeV Gamma\n"
plot_info += "B33\n"
plot_info += "α=5μm β=50μm γ=1000μm\n"
plot_info += "Angle of Incidence From Normal: 0-20 Degrees\n"
plot_info += "T: " + lamina_depth

plt.title(plot_title)
plt.text(0.02, 0.02, str(date.today()), ha="left", va="bottom", transform=ax.transAxes)
plt.text(0.98, 0.98, plot_info, ha="right", va="top", transform=ax.transAxes)
fig.canvas.get_default_filename = lambda: file_name


plt.show()
