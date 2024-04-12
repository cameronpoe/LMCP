import numpy as np
import random
import os
import sys
import colorsys
from matplotlib import pyplot as plt
from datetime import date
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r"wp_data.npy"
print("Looking for " + os.path.abspath(table_path))
# table_path = r'data/wall_thickness_optimization/eff_v_angle_per_wall_g4glasslead.npy'
theta_spacing = 5  # deg
# all the important variables that can be varied:
pore_widths = np.linspace(5, 145, 15, dtype=int)
wall_thicknesses = np.linspace(5, 195, 39, dtype=int)
zenith_angles = np.linspace(0, 90, int(90 / theta_spacing + 1), dtype=int)
azumith_angles = np.linspace(0, 90, int(90 / theta_spacing + 1), dtype=int)
lamina_thickness_as_variable = True
compare_physics = False  # whether to separate compton scattering, photoelectric effect, and other physical effects
###KEY FOR THESE THINGS
# 0=wall
# 1=pore or lamina thickness
# 2=zenith
# 3=theta

# now we tell the script which variable to use as x axis and which variable to vary:
independant_var = 0  # can be 0,1,2,3
vary_var = 1  # can be 0,1,2,3 but can't be the same as dependant_var
# indexes to use for other parameters, -1 means to average across them
other_param1 = 0
other_param2 = 0
# indexes for vary_var
increment = 4
start = 1
end = 16
##############################################
###                  CODE                  ###
##############################################


def combine(a, b):
    c = []
    for i in a:
        for j in b:
            if (i + j) not in c:
                c.append(i + j)
    c.sort()
    return np.array(c)


# defining all the variables
lamina_thicknesses = combine(wall_thicknesses, pore_widths)
xlabel = ""
vlabel = ""
other_param1_label = ""
other_param2_label = ""
# defining all the parameter arrays
independant_array = []
vary_array = []
other_param1_array = []
other_param2_array = []

if lamina_thickness_as_variable:
    label_array = [
        "Wall Thickness",
        "Lamina Thickness",
        "Zenith Angle",
        "Azumith Angle",
    ]
    array_array = [wall_thicknesses, lamina_thicknesses, zenith_angles, azumith_angles]
else:
    label_array = ["Wall Thickness", "Pore Width", "Zenith Angle", "Azumith Angle"]
    array_array = [wall_thicknesses, pore_widths, zenith_angles, azumith_angles]
xlabel = label_array[independant_var]
vlabel = label_array[vary_var]
independant_array = array_array[independant_var]
vary_array = array_array[vary_var]
other_param1_index = [i for i in range(4) if i not in [vary_var, independant_var]][0]
other_param2_index = [i for i in range(4) if i not in [vary_var, independant_var]][1]
other_param1_array = array_array[other_param1_index]
other_param2_array = array_array[other_param2_index]


def getSmallTable(vary_parameter, independant_parameter):
    values = [slice(None)] * 4
    values[vary_var] = vary_parameter
    values[independant_var] = independant_parameter
    if other_param1 != -1:
        values[other_param1_index] = other_param1
    if other_param2 != -1:
        values[other_param2_index] = other_param2
    if values[1] == slice(None) and lamina_thickness_as_variable:
        sys.exit("cannot average over lamina thickness")
    return values


def generateData(vary_parameter, table):
    worked_data = []
    i = 0
    while i < table.shape[independant_var]:
        indexes = getSmallTable(vary_parameter, i)
        effs = table[indexes[0], indexes[1], indexes[2], indexes[3]]
        # removing wall thicknesses that don't correspond to anything.
        if (
            lamina_thickness_as_variable
            and indexes[0] != slice(None)
            and lamina_thicknesses[indexes[1]]
            not in combine([wall_thicknesses[indexes[0]]], pore_widths)
        ):
            i += 1
            continue
        elif lamina_thickness_as_variable and indexes[0] == slice(None):
            j = 0
            while j < effs.shape[0]:
                if lamina_thicknesses[indexes[1]] not in combine(
                    [wall_thicknesses[j]], pore_widths
                ):
                    effs = np.delete(effs, i, 0)
                else:
                    j += 1
        if effs.size == 0:
            i += 1
            continue
        x_coordinate = independant_array[indexes[independant_var]]
        worked_data.append([x_coordinate, np.mean(effs)])
        i += 1
    worked_data = np.array(worked_data)
    spline_tuple = splrep(worked_data[:, 0], worked_data[:, 1], k=3)
    data_bspline = BSpline(*spline_tuple)
    ddata_bspline = data_bspline.derivative()
    spline_domain = np.linspace(worked_data[0, 0], worked_data[-1, 0], 1000)
    dcubic_spline = CubicSpline(spline_domain, ddata_bspline(spline_domain))
    return [worked_data, spline_domain, data_bspline]


def plotData(ax, vary_parameter, color):
    all_data = generateData(vary_parameter, table)
    worked_data = all_data[0]
    spline_domain = all_data[1]
    data_bspline = all_data[2]
    ax.scatter(worked_data[:, 0], 100 * worked_data[:, 1], color=color)
    data_label = vlabel + ": " + str(vary_array[vary_parameter])
    if vary_var == 0 or vary_var == 1:
        data_label += "um"
    ax.plot(
        spline_domain, 100 * data_bspline(spline_domain), color=color, label=data_label
    )
    ax.set_ylim(bottom=0, top=max(ax.get_ylim()[1], 105 * worked_data[:, 1].max()))


table = np.load(table_path)
print(table.shape)
if lamina_thickness_as_variable:
    new_table = np.zeros(
        [
            len(wall_thicknesses),
            len(lamina_thicknesses),
            len(zenith_angles),
            len(azumith_angles),
        ]
    )
    for index in np.ndindex(table.shape):
        new_table[
            index[0],
            np.where(
                lamina_thicknesses == wall_thicknesses[index[0]] + pore_widths[index[1]]
            ),
            index[2],
            index[3],
        ] = table[index[0]][index[1]][index[2]][index[3]]
    table = new_table

fig, ax = plt.subplots()
# ax.scatter(worked_data[:, 0], 100 * worked_data[:, 1], color="black", label="Raw data")
# ax.plot(
#    spline_domain, 100 * data_bspline(spline_domain), color="red", label="Spline fit"
# )
for i in range(start, end, increment):
    color = colorsys.hsv_to_rgb(
        float(range(start, end, increment).index(i))
        / len(range(start, end, increment)),
        1,
        0.8,
    )
    try:
        plotData(ax, i, color)
    except Exception as error:
        print("invalid vary var for index " + str(i) + ":", error)
ax.set_xlim(left=0)
ax.set_xlabel(xlabel, fontdict=dict(size=12))
ax.set_ylabel("Average efficiency (%)", fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position("both")
ax.yaxis.set_ticks_position("both")
plt.minorticks_on()
### File Naming Parameters:
lamina_depth = "1 in"
photon_energy = "511 KeV"

###Code
file_name = str(date.today())
file_name += ", " + xlabel + " vs " + "Efficiency"
file_name += ", " + "Varying " + vlabel
file_name += ", " + "Multiple Processes"
plot_title = "Lamina Depth: " + lamina_depth + ", "
plot_title += "Photon Energy: " + photon_energy + "\n"
if other_param1 == -1:
    plot_title += (
        label_array[other_param1_index]
        + ": "
        + str(other_param1_array[0])
        + "-"
        + str(other_param1_array[-1])
        + " step: "
        + str(other_param1_array[1] - other_param1_array[0])
        + ", "
    )
else:
    plot_title += (
        label_array[other_param1_index]
        + ": "
        + str(other_param1_array[other_param1_index])
        + ", "
    )
if other_param2 == -1:
    plot_title += (
        label_array[other_param2_index]
        + ": "
        + str(other_param2_array[0])
        + "-"
        + str(other_param2_array[-1])
        + " step "
        + str(other_param2_array[1] - other_param2_array[0])
        + ", "
    )
else:
    plot_title += (
        label_array[other_param2_index]
        + ": "
        + str(other_param2_array[other_param2_index])
    )
plt.title(plot_title)
plt.text(0.01, 0.99, str(date.today()), ha="left", va="top", transform=ax.transAxes)
fig.canvas.get_default_filename = lambda: file_name
plt.show()
