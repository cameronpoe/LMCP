import numpy as np
import random
import os
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r"./wp_data.npy"
print("Looking for " + os.path.abspath(table_path))
# table_path = r'data/wall_thickness_optimization/eff_v_angle_per_wall_g4glasslead.npy'
theta_spacing = 5  # deg
pore_widths = np.linspace(5, 145, 15, dtype=int)
# pore_widths = np.array([50])
wall_thicknesses = np.linspace(5, 195, 39, dtype=int)
# wall_thicknesses = np.array([75])
zenith_angles = np.linspace(0, 90, int(90 / theta_spacing + 1), dtype=int)
azumith_angles = np.linspace(0, 90, int(90 / theta_spacing + 1), dtype=int)
###KEY FOR THESE THINGS
# 0=wall
# 1=pore
# 2=zenith
# 3=theta
dependent_var = 0  # can be 1,2,3
vary_var = 2  # can be 1,2,3 but can't be the same as dependant_var
other_param1 = 8  # index to use for other parameter, -1 means to average across it
other_param2 = 0  # index to use for second other parameter, -1 mean to average across it

# indexes for vary_var
increment = 3
start = 0
end = 19
##############################################
###                  CODE                  ###
##############################################

label = ""
table = np.load(table_path)
print(table.shape)
if dependent_var == "pore":
    label = "Pore widths (um)"
elif dependent_var == "wall":
    label = "Wall thicknesses (um)"


def getSmallTable(vary_parameter, dependant_parameter):
    values = [slice(None)] * 4
    values[vary_var] = vary_parameter
    values[dependent_var] = dependant_parameter
    found = False
    for i in range(len(values)):
        if values[i] == slice(None):
            if found:
                if other_param2 != -1:
                    values[i] = other_param2
            else:
                if other_param1 != -1:
                    values[i] = other_param1
                    found = True
    return table[values[0], values[1], values[2], values[3]]


def generateData(vary_parameter, table):
    worked_data = np.empty((table.shape[dependent_var], 2))
    match dependent_var:
        case 0:
            worked_data[:, 0] = wall_thicknesses
        case 1:
            worked_data[:, 0] = pore_widths
        case 2:
            worked_data[:, 0] = zenith_angles
        case 3:
            worked_data[:, 0] = azumith_angles
    integrated_effs = []
    i = 0
    while i < table.shape[dependent_var]:
        effs = getSmallTable(vary_parameter, i)
        # fig, ax = plt.subplots()
        # ax.scatter(zenith_angles, effs)
        # plt.show()
        integrated_effs.append(np.mean(effs))
        i += 1
    worked_data[:, 1] = np.array(integrated_effs)

    spline_tuple = splrep(worked_data[:, 0], worked_data[:, 1], k=3)
    data_bspline = BSpline(*spline_tuple)
    ddata_bspline = data_bspline.derivative()
    spline_domain = np.linspace(worked_data[0, 0], worked_data[-1, 0], 1000)
    dcubic_spline = CubicSpline(spline_domain, ddata_bspline(spline_domain))
    return [label, worked_data, spline_domain, data_bspline]


def plotData(ax, vary_parameter):
    all_data = generateData(vary_parameter, table)
    worked_data = all_data[1]
    spline_domain = all_data[2]
    data_bspline = all_data[3]
    color = (random.random(), random.random(), random.random())
    ax.scatter(worked_data[:, 0], 100 * worked_data[:, 1], color=color, label="")
    data_label = ""
    match vary_var:
        case 0:
            data_label = "Wall Thickness: " + str(wall_thicknesses[vary_parameter]) + "um"
        case 1:
            data_label = "Pore Width: " + str(pore_widths[vary_parameter]) + "um"
        case 2:
            data_label = "Zenith Angle: " + str(zenith_angles[vary_parameter])
        case 3:
            data_label = "Azimuth Angle: " + str(zenith_angles[vary_parameter])
    ax.plot(
        spline_domain, 100 * data_bspline(spline_domain), color=color, label=data_label
    )
    ax.set_ylim(bottom=0, top=max(ax.get_ylim()[1], 105 * worked_data[:, 1].max()))


# all_data = generateData(1)
# worked_data = all_data[1]
# spline_domain = all_data[2]
# data_bspline = all_data[3]
fig, ax = plt.subplots()
# ax.scatter(worked_data[:, 0], 100 * worked_data[:, 1], color="black", label="Raw data")
# ax.plot(
#    spline_domain, 100 * data_bspline(spline_domain), color="red", label="Spline fit"
# )
for i in range(int((end - start) / increment)):
    plotData(ax, increment * i + start)
ax.set_xlim(left=0)
ax.set_xlabel(label, fontdict=dict(size=12))
ax.set_ylabel("Average efficiency (%)", fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()
plt.show()
