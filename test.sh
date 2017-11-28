#!/bin/bash

# Make project
make

cd build/bin

# Launch project
echo
echo Visuzalization begins
./visualize --config_file config_file