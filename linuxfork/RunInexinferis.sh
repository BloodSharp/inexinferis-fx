#!/bin/bash

GAMEROOT=$PWD
#GAMEEXE=./hl_linux

export LD_LIBRARY_PATH=${GAMEROOT}:$LD_LIBRARY_PATH

#Cheat...
export LD_PRELOAD=${GAMEROOT}/inexinferis/libinexinferisfx.so

ulimit -n 2048

./hl_linux -game cstrike
