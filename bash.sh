#!/bin/bash

N=$1;
g++ heat3d_seq.cpp -o heat3d_seq
g++ -fopenmp heat3d_par.cpp -o heat3d_par
g++ -O3 -mavx heat3d_simd.cpp -o heat3d_simd
g++ -fopenmp heat3d_domain.cpp -o heat3d_domain

echo
echo "Running sequential version..."
seq_time=$(./heat3d_seq <<< "$N")
echo "${seq_time}"

echo
echo "Running parallel version..."
par_time=$(./heat3d_par <<< "$N")
echo "${par_time}"

echo
echo "Running SIMD version..."
simd_time=$(./heat3d_simd <<< "$N")
echo "${simd_time}"

echo
echo "Running Domain Decomposition version..."
domain_time=$(./heat3d_domain <<< "$N")
echo "${domain_time}"
