#!/bin/bash

N=$1;
g++ heat3d_seq.cpp -o heat3d_seq
g++ -fopenmp heat3d_doacross.cpp -o heat3d_doacross
g++ -O3 -mavx heat3d_simd.cpp -o heat3d_simd
g++ -fopenmp heat3d_wave.cpp -o heat3d_wave

echo
echo "Running sequential version..."
seq_time=$(./heat3d_seq <<< "$N")
echo "${seq_time}"

echo
echo "Running doacross version..."
doacross_time=$(./heat3d_doacross <<< "$N")
echo "${doacross_time}"

echo
echo "Running SIMD version..."
simd_time=$(./heat3d_simd <<< "$N")
echo "${simd_time}"

echo
echo "Running Wavefront version..."
wave_time=$(./heat3d_wave <<< "$N")
echo "${wave_time}"
