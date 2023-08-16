#!/bin/bash

# function to execute a program
execute_program() {
    local program=$1
    local pid
    local sum=0

    echo "" > output.txt

    timeout --signal=SIGINT 5 ./${program} &
    pid=$!
    sleep 5.5
    kill -0 $pid 2>/dev/null && kill -9 $pid

    # get the first line, extract the numbers, add them and store in sum
    read num1 num2 < output.txt
    sum=$((num1 + num2))
    echo "${program} -> Value of sum for ${program}: $sum" >> results.txt
}

pause() {
    read -n 1 -s -r -p "Press any key to continue"
}

# compile the programs
g++ -DEVAL -std=c++11 -O2 -pipe -static -s -o got2 grader.cpp got2.cpp
g++ -DEVAL -std=c++11 -O2 -pipe -static -s -o got2Against grader.cpp got2Against.cpp

if [ $? -eq 0 ]; then
    echo "Compilation successful"

    # for each file in the input directory
    for file in input/*; do
        # print the current file being tested
        echo "==============================" >> results.txt
        echo "Test: $file" >> results.txt
        echo "==============================" >> results.txt

        # copy the file to input.txt
        cp "$file" input.txt

        # run each program and process its output sequentially
        execute_program got2
        pause
        execute_program got2Against
        echo "==============================" >> results.txt
        echo "" >> results.txt

        pause
    done

    echo "Done"
else
    echo "An error occurred during compilation"
fi
