#!/bin/bash

# Make project
make

cd build/bin

# Launch project
echo 
echo Visuzalization begins
echo CONFIG: config_file
./visualize --config_file config_file