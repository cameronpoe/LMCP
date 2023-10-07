# Helper script to generate a Geant4 macro file with a rotating source.
import numpy as np

output_file_name = r'macros/Run_var_dims_rot_source.mac'
num_events = 100000
threads = 60
wall_thicknesses = np.linspace(50, 200, 39, dtype=int)
source_distance_from_lmcp_center = 22       # mm
theta_increment = 3     # degrees (multiple of 90)
SINGLE_AZIMUTH = True
phi_increment = 3       # degrees (multiple of 90)

with open(output_file_name, 'w') as f:
    f.write(f'''######################################################
## redirect output from each thread
######################################################
/control/cout/setCoutFile lmcp.out
/control/cout/setCerrFile lmcp.err

######################################################
## Verbosity
######################################################
/control/verbose 0
/process/verbose 0
/run/verbose 0
/event/verbose 0
/tracking/verbose 0

######################################################
## Change the default number of threads (in multi-threaded mode)
######################################################
/run/numberOfThreads {threads}

######################################################
## Overlap Checking
######################################################
/user/det/setOverlapChecking false

######################################################
## Physics
# Option0 - Electromagnetic
# Option1 - Electromagnetic & Hadronic
# Option2 - Electromagnetic & Hadronic with high precision option
######################################################
#/user/phy/setPhysics Option0
/user/phy/setPhysics Option1
#/user/phy/setPhysics Option2

######################################################
## Initialize kernel
######################################################
/run/initialize

######################################################
# Source Particle
######################################################
/gps/particle gamma
/gps/ene/mono 511. keV

######################################################
# Source Geometry
######################################################
/gps/pos/type Plane
/gps/pos/shape Square
/gps/pos/halfx 2. mm
/gps/pos/halfy 2. mm

######################################################
# RANDOM NUMBER
######################################################
/random/setSeeds 165127 668831

######################################################
# START WALL THICKNESS LOOPS
######################################################            
''')
    
    for wall_thickness in wall_thicknesses:
        f.write(f'''/user/det/setWallThickness {wall_thickness} um
/user/det/update
                
######################################################
# START ANGULAR LOOPS
######################################################            
# ''')

        if SINGLE_AZIMUTH:
            phis = np.array([0])
        else:
            phis = np.linspace(0, 90, int(90/phi_increment)+1)
        thetas = np.linspace(0, 90, int(90/theta_increment)+1)
        for i, theta in enumerate(thetas):
            z = source_distance_from_lmcp_center*np.cos(np.pi*theta/180)
            for j, phi in enumerate(phis):
                x = source_distance_from_lmcp_center*np.sin(np.pi*theta/180)*np.cos(np.pi*phi/180)
                y = source_distance_from_lmcp_center*np.sin(np.pi*theta/180)*np.sin(np.pi*phi/180)

                rhat = np.array([
                    x/source_distance_from_lmcp_center,
                    y/source_distance_from_lmcp_center,
                    z/source_distance_from_lmcp_center
                ])
                f.write(f'''# Azimuth: {phi} deg, Zenith: {theta} deg
/gps/pos/centre {round(x, 5)} {round(y, 5)} {round(z, 5)} mm
/gps/direction {round(-rhat[0], 5)} {round(-rhat[1], 5)} {round(-rhat[2], 5)}
/analysis/setFileName ../data/eff_v_angle_data/Run_wall{wall_thickness}_theta{i}_phi{j}.root
/run/printProgress 10000
/run/beamOn {num_events}

''')