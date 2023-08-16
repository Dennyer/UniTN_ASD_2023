#!/bin/bash

# run the g++ command
g++ -DEVAL -std=c++11 -O2 -pipe -static -s -o got2 grader.cpp got2.cpp

# check if compilation was successful
if [ $? -eq 0 ]
then
    echo "Compilation successful"
    timeout --signal=SIGINT 5 ./got2 &
    pid=$!
    sleep 5.5
    kill -0 $pid 2>/dev/null && kill -9 $pid
    echo "Done"
else
    echo "An error occurred during compilation"
fi
