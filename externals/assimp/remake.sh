#!/bin/bash

# Assimp library is huge, so i decided not to store it in project
# If you want to remake it - launch bash script:
# bash remake.sh
# it will download assimp library from github, unpack, make, copy to bridge and use in current project 

# download assimp library
wget https://github.com/assimp/assimp/archive/v4.0.1.tar.gz

# unpack
tar -xvf v4.0.1.tar.gz
cd assimp-4.0.1

# make
cmake CMakeLists.txt -G 'Unix Makefiles'
make

# coping to bridge
BRIDGE=../../../bridge
mkdir $BRIDGE/lib
mkdir -p $BRIDGE/include
cp -r lib/* $BRIDGE/lib
cp -r include/* $BRIDGE/include
