#!/bin/bash

N_array=(10 16 20 25 32 37 40 45 50 64 85 100 128 150)

g++ heat3d_seq.cpp -o heat3d_seq
g++ -fopenmp heat3d_doacross.cpp -o heat3d_doacross
g++ -O3 -mavx heat3d_simd.cpp -o heat3d_simd
g++ -fopenmp heat3d_domain.cpp -o heat3d_domain

output_file="time.csv"

if [ ! -f "$output_file" ]; then
    echo "Size(n),Seq(ms),Doacross(ms),SIMD(ms),Domain(ms)" > "$output_file"
fi

for N in "${N_array[@]}"; do

    seq_time=$(./heat3d_seq <<< "$N")

    doacross_time=$(./heat3d_doacross <<< "$N")

    simd_time=$(./heat3d_simd <<< "$N")

    domain_time=$(./heat3d_domain <<< "$N")

    echo "$N,$seq_time,$doacross_time,$simd_time,$domain_time" >> "$output_file"
done