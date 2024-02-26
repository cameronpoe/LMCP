# Helper script to generate a Geant4 macro file with a rotating source.
# Tiny Change
# tinier change
import numpy as np

output_file_name = r"macros/Run_rot_source_proc_analysis.mac"
num_events = 1000
threads = 1
# wall_thicknesses = np.linspace(150, 150, 1, dtype=int)
wall_thicknesses = np.array([50])
pore_widths = np.array([50])
source_distance_from_lmcp_center = 22  # mm
SINGLE_ZENITH = True
theta_increment = 5  # degrees (multiple of 90)
SINGLE_AZIMUTH = True
phi_increment = 3  # degrees (multiple of 90)

with open(output_file_name, "w") as f:
    f.write(
        f"""######################################################
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
"""
    )

    for wall_thickness in wall_thicknesses:
        for pore_width in pore_widths:
            f.write(
                f"""/user/det/setWallThickness {wall_thickness} um
/user/det/setPoreDimensions {round(pore_width, 3)} {round(pore_width, 3)} 25400 um 
/user/det/update
                
######################################################
# START ANGULAR LOOPS
######################################################            
"""
            )

            if SINGLE_ZENITH:
                thetas = np.array([45])
            else:
                thetas = np.linspace(0, 90, int(90 / theta_increment) + 1)
            if SINGLE_AZIMUTH:
                phis = np.array([0])
            else:
                phis = np.linspace(0, 90, int(90 / phi_increment) + 1)

            for i, theta in enumerate(thetas):
                z = source_distance_from_lmcp_center * np.cos(np.pi * theta / 180)
                for j, phi in enumerate(phis):
                    x = (
                        source_distance_from_lmcp_center
                        * np.sin(np.pi * theta / 180)
                        * np.cos(np.pi * phi / 180)
                    )
                    y = (
                        source_distance_from_lmcp_center
                        * np.sin(np.pi * theta / 180)
                        * np.sin(np.pi * phi / 180)
                    )

                    rhat = np.array(
                        [
                            x / source_distance_from_lmcp_center,
                            y / source_distance_from_lmcp_center,
                            z / source_distance_from_lmcp_center,
                        ]
                    )
                    f.write(
                        f"""# Azimuth: {phi} deg, Zenith: {theta} deg
/gps/pos/centre {round(x, 5)} {round(y, 5)} {round(z, 5)} mm
/gps/direction {round(-rhat[0], 5)} {round(-rhat[1], 5)} {round(-rhat[2], 5)}
/analysis/setFileName ../raw_data/latest_run/Run_wall{wall_thickness}_pore_{pore_width}_theta{i}_phi{j}.root
/run/printProgress 10000
/run/beamOn {num_events}

"""
                    )
