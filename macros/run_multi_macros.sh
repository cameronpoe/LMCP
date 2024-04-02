#!/bin/bash

a=(/home/cameronpoe/Desktop/research/frisch/hgmt_simulation/hgmt_sim_geant/LMCP_Full/macros/multi_macros/*)
N=${#a[@]}

threads=1
iter=$((N/threads))
max_full=$((threads*iter))

num_files=0
counter=0
while [ $counter -le $iter ]
do
    if [ $counter -eq $iter ] 
    then
        subset=(${a[@]:$max_full})
    else
        start=$((counter*threads))
        subset=(${a[@]:start:$threads})
    fi
    
    for var in "${subset[@]}"
    do 
        ../build/startSIM -m "$var" &
    done
    wait

    ((counter++))
done
