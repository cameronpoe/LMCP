import numpy as np
from matplotlib import pyplot as plt
from scipy.integrate import trapezoid
from scipy.interpolate import splrep, BSpline, CubicSpline

table_path = r'data/wall_thickness_optimization/eff_v_angle_per_wall_g4glasslead-old.npy'
phi_ind = 0
wall_thicknesses = np.linspace(10, 200, 20, dtype=int)



# code
table = np.load(table_path)

integrated_effs = []
zenith_angles = np.linspace(0,90,31, dtype=int)
for effs in table[:,:,0]:
    avg_eff = (1/90)*trapezoid(effs, zenith_angles)
    integrated_effs.append(avg_eff)
integrated_effs = np.array(integrated_effs)

print(wall_thicknesses)
print(integrated_effs)
spline_tuple = splrep(wall_thicknesses, integrated_effs, k=3)
data_bspline = BSpline(*spline_tuple)
ddata_bspline = data_bspline.derivative()
spline_domain = np.linspace(wall_thicknesses[0], wall_thicknesses[-1], 1000)
dcubic_spline = CubicSpline(spline_domain, ddata_bspline(spline_domain))
extrema = dcubic_spline.solve(0, extrapolate=False)[0]
print(extrema)

fig, ax = plt.subplots()
ax.scatter(wall_thicknesses, integrated_effs, color='black', label='Raw data')
ax.plot(spline_domain, data_bspline(spline_domain), color='red', label='Spline fit')
ax.axvline(extrema, label='Maximum: {:.2f} um'.format(extrema))
ax.set_xlabel('Wall thickness (um)', fontdict=dict(size=12))
ax.set_ylabel('Average efficiency (zenith)', fontdict=dict(size=12))
ax.legend()
ax.xaxis.set_ticks_position('both')
ax.yaxis.set_ticks_position('both')
plt.minorticks_on()
plt.show()

