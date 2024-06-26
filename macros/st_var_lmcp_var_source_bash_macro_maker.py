# Helper script to generate a Geant4 macro file with a rotating source.
import numpy as np
import os
import shutil


####################################
###          PARAMETERS          ###
####################################

root_output_dir = r"../raw_data/latest_run"
max_threads = 63
num_events = 10000
lmcp_dimensions = np.array([2.54, 2.54, 2.54])  # cm

source_distance_from_lmcp_center = 22  # mm

zenith_angle_increment = 2  # degrees (factor of 90)
azumith_angle_increment = 30  # degrees (factor of 90)


def geometry(x_axis_value):
    energy = 511
    tau = 160
    alpha = x_axis_value
    beta = 40
    gamma = 1000
    return (energy, tau, alpha, beta, gamma)


zenith_angles = np.linspace(0, 20, int(20 / zenith_angle_increment) + 1, dtype=int)
azumith_angles = np.linspace(0, 90, int(90 / azumith_angle_increment) + 1)
x_axis_values = np.linspace(4, 60, 15, dtype=int)
####################################
###             CODE             ###
####################################

cur_directory = os.path.dirname(os.path.realpath(__file__))
new_folder_name = "multi_macros"
new_folder_path = os.path.join(cur_directory, new_folder_name)
try:
    os.mkdir(new_folder_path)
except:
    shutil.rmtree(new_folder_path)
    os.mkdir(new_folder_path)

root_output_dir = os.path.realpath(root_output_dir)
shutil.rmtree(root_output_dir)
os.mkdir(root_output_dir)

new_sh_name = "run_multi_macros.sh"
new_sh_path = os.path.join(cur_directory, new_sh_name)
with open(new_sh_path, "w") as f:
    f.write(
        """#!/bin/bash

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
""".format(bash_folder_path=new_folder_path, thread_count=max_threads)
    )


for x_axis_value in x_axis_values:
    energy, tau, alpha, beta, gamma = geometry(x_axis_value)
    for zenith_angle in zenith_angles:
        for azumith_angle in azumith_angles:
            z = source_distance_from_lmcp_center * np.cos(np.pi * zenith_angle / 180)
            x = (
                source_distance_from_lmcp_center
                * np.sin(np.pi * zenith_angle / 180)
                * np.cos(np.pi * azumith_angle / 180)
            )
            y = (
                source_distance_from_lmcp_center
                * np.sin(np.pi * zenith_angle / 180)
                * np.sin(np.pi * azumith_angle / 180)
            )

            rhat = np.array(
                [
                    x / source_distance_from_lmcp_center,
                    y / source_distance_from_lmcp_center,
                    z / source_distance_from_lmcp_center,
                ]
            )

            output_file_name = f"xaxis{x_axis_value}_eng{energy}_alpha{alpha}_beta{beta}_gamma{gamma}_tau{tau}_zenithangle{zenith_angle}_azumithangle{azumith_angle}"
            output_file_path = os.path.join(new_folder_path, output_file_name + ".mac")

            with open(output_file_path, "w") as f:
                f.write(
                    f"""######################################################
## Slab dimensions: {round(lmcp_dimensions[0], 3)} cm x {round(lmcp_dimensions[1], 3)} cm x {round(lmcp_dimensions[2], 3)} cm
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
/user/det/setPoreDimensions {round(gamma*1e-4, 7)} {round(alpha*1e-4, 7)} {lmcp_dimensions[2]} cm
/user/det/setWallX {round(beta, 3)} um
/user/det/setWallY {round(tau-alpha, 3)} um

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
/gps/ene/mono {energy} keV

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
# Azimuth: {azumith_angle} deg, Zenith: {zenith_angle} deg
/gps/pos/centre {round(x, 5)} {round(y, 5)} {round(z, 5)} mm
/gps/direction {round(-rhat[0], 5)} {round(-rhat[1], 5)} {round(-rhat[2], 5)}
/analysis/setFileName {root_output_dir}/{output_file_name}.root
/run/printProgress 10000
/run/beamOn {num_events}

"""
                )
