# Laminated Microchannel Plate (LMCP)

### How to build and run the simulation
1. Compile and link to generate an executable with cmake.

```
        % cd LMCP_basic/                      # go to directory which contains your example
        % mkdir build                         # make a directoy for the build
        % cd build/
        % cmake ..
        % make -j N                           # "N" is the number of processes 
```

2. Execute in 'BATCH' mode from macro files.

```
        % cd LMCP_basic/build
        % ./startSIM -m ../macros/Run_0.mac    # run macro Run_0.mac
```

3. Execute in 'INTERACTIVE MODE' with visualization.

```
       % cd cd LMCP_basic/build
       % ./startSIM
       PreInit> /run/initialize 
       Idle>    /run/beamOn 1
       ...
       Idle>    exit
```

4. Cameron doing quick test...
