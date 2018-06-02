#!/bin/bash

echo "====> start the tests"

echo "thread_count;best_depth;max_depth;iter;time_taken;no_results" > results.csv
for i in {1..5}
do
    echo "====> iteration number ${i}, sequential implementation"
    ./PCM_seq 40 012345678 1 >> results.csv
    ./PCM_seq 60 012345678 1 >> results.csv
    ./PCM_seq 80 012345678 1 >> results.csv
    ./PCM_seq 100 012345678 1 >> results.csv
    ./PCM_seq 200 012345678 1 >> results.csv
    ./PCM_seq 500 012345678 1 >> results.csv
    ./PCM_seq 1000 012345678 1 >> results.csv
done

echo >> results.csv

for i in {1..5}
do
    echo "====> iteration number ${i}, semi-parallel implementation"
    ./PCM_semi 40 012345678 4 1 >> results.csv
    ./PCM_semi 60 012345678 4 1 >> results.csv
    ./PCM_semi 80 012345678 4 1 >> results.csv
    ./PCM_semi 100 012345678 4 1 >> results.csv
    ./PCM_semi 200 012345678 4 1 >> results.csv
    ./PCM_semi 500 012345678 4 1 >> results.csv
    ./PCM_semi 1000 012345678 4 1 >> results.csv
done

echo >> results.csv

for i in {1..5}
do
    echo "====> iteration number ${i}, parallel implementation"
    ./PCM 40 012345678 4 1 >> results.csv
    ./PCM 60 012345678 4 1 >> results.csv
    ./PCM 80 012345678 4 1 >> results.csv
    ./PCM 100 012345678 4 1 >> results.csv
    ./PCM 200 012345678 4 1 >> results.csv
    ./PCM 500 012345678 4 1 >> results.csv
    ./PCM 1000 012345678 4 1 >> results.csv
done

echo "====> finished!"
