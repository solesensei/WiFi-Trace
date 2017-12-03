#!/bin/bash

if [ $# -eq 1 ] # if args
then
    if [ $1 -ge 1 ] && [ $1 -le 3 ]
    then
        # Remake project
        sudo make clean
        sudo make
        cd build/bin
        # Launch project with config file parameters
        echo 
        echo "Test config"
        echo CONFIG: config_file$1.cfg
        sudo ./visualize --config_file config_file$1.cfg
    else
        echo no such config file [config_file$1.cfg]. Aborted.
        echo choose \'1-3\' configs, or use default parameters    
    fi
else # no args
    # Make project
    sudo make
    cd build/bin
    # Launch project with default parameters
    echo 
    echo "Test default"
    sudo ./visualize 
fi