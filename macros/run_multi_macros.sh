#!/bin/bash

<<<<<<< HEAD
a=(/local/d1/iangoldberg/claudio_lmcp/macros/multi_macros/*)
=======
a=(/home/claud/Documents/Work/LMCP/macros/multi_macros/*)
>>>>>>> origin/claudiobf/newsims
N=${#a[@]}

threads=63
iter=$((N/threads))
max_full=$((threads*iter))
cd m    
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
