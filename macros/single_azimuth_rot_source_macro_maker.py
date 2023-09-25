# Helper script to generate a Geant4 macro file with a rotating source but held at a constant azimuth
import numpy as np

output_file_name = r'macros/Run_diff_thicknesses_VIS_TEST.mac'
source_distance_from_lmcp_center = 22       # mm
theta_increment = 30     # degrees (multiple of 90)
phi = 90                # degrees
lmcp_dimensions = [25.4, 25.4, 25.4]        # mm
pore_dimensions = [2.54, 0.0508]            # mm
lamina_thicknesses = [10, 500, 2000, 5000]               # um

num_threads = 1
num_events = 1
num_events_prog_report = 50000
data_output_directory_path = r'../raw_data/latest_run/'


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
#run/numberOfThreads {num_threads}

######################################################
## Overlap Checking
######################################################
#/user/det/setOverlapChecking false

######################################################
## Physics
# Option0 - Electromagnetic
# Option1 - Electromagnetic & Hadronic
# Option2 - Electromagnetic & Hadronic with high precision option
######################################################
#/user/phy/setPhysics Option0
#/user/phy/setPhysics Option1
#/user/phy/setPhysics Option2

# Setting up source
/gps/particle gamma
/gps/ene/mono 511. keV
/gps/pos/type Plane
/gps/pos/shape Square
/gps/pos/halfx 2. mm
/gps/pos/halfy 2. mm

######################################################
# RANDOM NUMBER
######################################################
/random/setSeeds 165127 668831

######################################################
# START LOOPS
#####################################################            
''')

    thetas = np.linspace(0, 90, int(90/theta_increment)+1)
    for i, thickness in enumerate(lamina_thicknesses):
        for j, theta in enumerate(thetas):
            x = source_distance_from_lmcp_center*np.sin(np.pi*theta/180)*np.cos(np.pi*phi/180)
            y = source_distance_from_lmcp_center*np.sin(np.pi*theta/180)*np.sin(np.pi*phi/180)
            z = source_distance_from_lmcp_center*np.cos(np.pi*theta/180)

            rhat = np.array([
                x/source_distance_from_lmcp_center,
                y/source_distance_from_lmcp_center,
                z/source_distance_from_lmcp_center
            ])
            f.write(f'''# Setting the LMCP geometry
/user/det/setSlabDimensions {lmcp_dimensions[0]} {lmcp_dimensions[1]} {lmcp_dimensions[2]} mm
/user/det/setPoreDimensions {pore_dimensions[0]} {pore_dimensions[1]} {lmcp_dimensions[2]} mm
/user/det/setLaminaThickness {thickness} um
# Azimuth: {phi} deg, Zenith: {theta} deg
/gps/pos/centre {round(x, 5)} {round(y, 5)} {round(z, 5)} mm
/gps/direction {round(-rhat[0], 5)} {round(-rhat[1], 5)} {round(-rhat[2], 5)}
/analysis/setFileName {data_output_directory_path}Run_thickness_{thickness}um_theta{j}.root
/run/printProgress {num_events_prog_report}
/run/beamOn {num_events}

''')