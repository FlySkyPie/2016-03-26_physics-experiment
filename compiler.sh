#!/bin/bash
# Program:
# This program use to compile something(the game in this case).
# History:
# 2016/03/15	FlySkyPie
export PATH
echo "Starting compile your soure code...... "
SECONDS=0
echo -n "compiling main.cpp ......"
g++  main.cpp -o voltage -Wall -std=gnu++11 -pthread -lncurses `pkg-config --cflags --libs opencv`
echo "done"
echo "Compile finish "
duration=$SECONDS
echo "The work spend $duration seconds."
read -n1 -r -p "Press any key to continue...(run binary file)" key
./voltage

read -n1 -r -p "Press any key to continue..." key
exit 0
