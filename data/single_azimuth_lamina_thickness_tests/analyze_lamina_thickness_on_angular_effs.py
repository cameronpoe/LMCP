import numpy as np
import uproot
import os
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

########### PARAMETERS ###########
num_events = 10000
parent_directory = r'data/single_azimuth_lamina_thickness_tests/pbglass_data/'
num_diff_thetas = 31
##################################



########### Code ###########
angle_domain = np.linspace(0, 90, num_diff_thetas, dtype=int)

thicknesses = []
eff_v_angle_tables = []
integrals = []

for data_directory in os.listdir(parent_directory):

    data_directory += '/'

    if '.root' in data_directory or '.txt' in data_directory or '.png' in data_directory:
        continue

    eff_v_angle = np.empty(num_diff_thetas)
    directory_contents = os.listdir(parent_directory + data_directory)
    for file_name in directory_contents:
        with uproot.open(parent_directory + data_directory + file_name) as f:
            
            theta_ind = int(file_name[9:-5])

            num_events_reached_pore = len(f['pore']['EventNumber'].array(library='np'))
                  
            eff = num_events_reached_pore/num_events*100

            eff_v_angle[theta_ind] = eff
    
    if len(directory_contents) != 0:
        lamina_thickness = float(data_directory[:-8])
        thicknesses.append(lamina_thickness)
        eff_v_angle_tables.append(eff_v_angle)
        single_integral = trapezoid(eff_v_angle, x=angle_domain)
        integrals.append(single_integral)

thicknesses = np.array(thicknesses)
eff_v_angle_tables = np.array(eff_v_angle_tables)
integrals = np.array(integrals)

reorder = np.argsort(thicknesses)
thicknesses = thicknesses[reorder]
eff_v_angle_tables = eff_v_angle_tables[reorder]
integrals = integrals[reorder]

color_list = [
    '#FF0000',
    '#FF7B00',
    '#FFB400',
    '#DAFF00',
    '#00FF37',
    '#00FFB6',
    '#00AFFF',
    '#003BFF',
    '#7500FF',
    '#DA00FF',
    '#FF007B',
    '#250044'
]

fig, ax = plt.subplots()
fig.set_size_inches(10, 7)
for i, table in enumerate(eff_v_angle_tables):
    ax.scatter(angle_domain, table, marker='.', color=color_list[i], label=f'{thicknesses[i]} um')
    ax.plot(angle_domain, table, color=color_list[i])
ax.set_ylim(bottom=0)
ax.legend(ncols=4)
ax.set_xlabel('Angle from normal (degrees)', fontdict=dict(size=12))
ax.set_ylabel('Percentage of events generating e- in a pore', fontdict=dict(size=12))
ax.xaxis.set_ticks_position('both')
ax.yaxis.set_ticks_position('both')
plt.minorticks_on()

spline_tuple = splrep(thicknesses, integrals, k=3, s=10000)
data_bspline = BSpline(*spline_tuple)
ddata_bspline = data_bspline.derivative()
spline_domain = np.linspace(thicknesses[0], thicknesses[-1], 1000)
dcubic_spline = CubicSpline(spline_domain, ddata_bspline(spline_domain))
extrema = dcubic_spline.solve(0, extrapolate=False)[0]

fig2, ax2 = plt.subplots()
ax2.scatter(thicknesses, integrals, color='black', label='Raw data')
ax2.plot(spline_domain, data_bspline(spline_domain), color='red', label='Spline fit')
ax2.axvline(extrema, color='blue', label=f'Extrema ({round(extrema, 1)} um)')
ax2.legend()
ax2.set_xlabel('Lamina thickness (um)')
ax2.set_ylabel('Integral of eff v. angle curve')
ax2.xaxis.set_ticks_position('both')
ax2.yaxis.set_ticks_position('both')
plt.minorticks_on()

plt.show()

