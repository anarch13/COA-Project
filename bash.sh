#!/bin/bash

N=$1;
# Compile the sequential and parallel codes
g++ heat3d_seq.cpp -o heat3d_seq
g++ -fopenmp heat3d_par.cpp -o heat3d_par
g++ -O3 -mavx heat3d_simd.cpp -o heat3d_simd

# Run the sequential code and capture the time output
echo "Running sequential version..."
seq_time=$(./heat3d_seq <<< "$N")
echo "${seq_time}"

# Run the parallel code and capture the time output
echo "Running parallel version..."
par_time=$(./heat3d_par <<< "$N")
echo "${par_time}"

echo "Running SIMD version..."
simd_time=$(./heat3d_simd <<< "$N")
echo "${simd_time}"
