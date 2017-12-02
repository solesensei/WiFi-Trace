#!/bin/bash

if [ $# -eq 1 ] # if args
then
    # Remake project
    sudo make clean
    sudo make
    cd build/bin
    # Launch project with config file parameters
    echo 
    echo "Test config"
    echo CONFIG: config_file$1.cfg
    echo
    sudo ./visualize --config_file config_file$1.cfg
else # no args
    # Make project
    sudo make
    cd build/bin
    # Launch project with default parameters
    echo 
    echo "Test default"
    echo
    sudo ./visualize 
fi