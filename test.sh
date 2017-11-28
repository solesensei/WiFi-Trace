#!/bin/bash

# Make project
make

cd build/bin

# Launch project
echo 
echo Visuzalization begins
echo CONFIG: $1
./visualize --config_file $1