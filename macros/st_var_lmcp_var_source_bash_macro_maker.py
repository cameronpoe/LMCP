# Helper script to generate a Geant4 macro file with a rotating source.
import numpy as np
import os
import shutil

####################################
###          PARAMETERS          ###
####################################

root_output_dir = r'../raw_data/latest_run'
max_threads = 62
num_events = 500000
lmcp_dimensions = np.array([2.54, 2.54, 2.54])              # cm
# pore_widths = np.linspace(5, 80, 16, dtype=int)             # um
pore_widths = np.array([150])				   # um
wall_thicknesses = np.linspace(10, 300, 30, dtype=int)      # um
# wall_thicknesses = np.array([50])                             # um

source_distance_from_lmcp_center = 22       # mm
theta_increment = 5     # degrees (factor of 90)
phi_increment = 3       # degrees (factor of 90)
SINGLE_AZIMUTH = True



####################################
###             CODE             ###
####################################

cur_directory = os.path.dirname(os.path.realpath(__file__))
new_folder_name = 'multi_macros'
new_folder_path = os.path.join(cur_directory, new_folder_name)
try:
    os.mkdir(new_folder_path)
except:
    shutil.rmtree(new_folder_path)
    os.mkdir(new_folder_path)

root_output_dir = os.path.realpath(root_output_dir)
shutil.rmtree(root_output_dir)
os.mkdir(root_output_dir)

new_sh_name = 'run_multi_macros.sh'
new_sh_path = os.path.join(cur_directory, new_sh_name)
with open(new_sh_path, 'w') as f:
    f.write('''#!/bin/bash

a=({bash_folder_path}/*)
N=${{#a[@]}}

threads={thread_count}
iter=$((N/threads))
max_full=$((threads*iter))

num_files=0
counter=0
while [ $counter -le $iter ]
do
    if [ $counter -eq $iter ] 
    then
        subset=(${{a[@]:$max_full}})
    else
        start=$((counter*threads))
        subset=(${{a[@]:start:$threads}})
    fi
    
    for var in "${{subset[@]}}"
    do 
        ../build/startSIM -m "$var" &
    done
    wait

    ((counter++))
done
'''.format(bash_folder_path=new_folder_path, thread_count=max_threads))

thetas = np.linspace(0, 90, int(90/theta_increment)+1)
if SINGLE_AZIMUTH:
    phis = np.array([0])
else:
    phis = np.linspace(0, 90, int(90/phi_increment)+1)

for wall_num, wall_thickness in enumerate(wall_thicknesses):

    for pore_num, pore_width in enumerate(pore_widths):

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

                output_file_name = f'Run_wall{wall_num}_pore{pore_num}_theta{i}_phi{j}.mac'
                output_file_path = os.path.join(new_folder_path, output_file_name)
                
                with open(output_file_path, 'w') as f:
                    f.write(f'''######################################################
## Wall thickness: {round(wall_thickness, 3)} um
## Pore dimensions: xxx
## Slab dimensions: xxx
######################################################

######################################################
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
/run/numberOfThreads 1

######################################################
## Detector parameters
######################################################
/user/det/setOverlapChecking false
/user/det/setSlabDimensions {lmcp_dimensions[0]} {lmcp_dimensions[1]} {lmcp_dimensions[2]} cm
/user/det/setPoreDimensions {pore_width*1e-4} {pore_width*1e-4} {lmcp_dimensions[2]} cm
/user/det/setWallThickness {round(wall_thickness, 3)} um

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
# START ANGULAR LOOPS
######################################################             
# Azimuth: {phi} deg, Zenith: {theta} deg
/gps/pos/centre {round(x, 5)} {round(y, 5)} {round(z, 5)} mm
/gps/direction {round(-rhat[0], 5)} {round(-rhat[1], 5)} {round(-rhat[2], 5)}
/analysis/setFileName {root_output_dir}/Run_w{wall_thickness}_p{pore_width}_theta{i}_phi{j}.root
/run/printProgress 10000
/run/beamOn {num_events}

''')
                
