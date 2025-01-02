#!/bin/bash

N_array=(10 16 20 25 32 37 40 45 50 64 85 100 128 150)

g++ heat3d_seq.cpp -o heat3d_seq
g++ -fopenmp heat3d_doacross.cpp -o heat3d_doacross
g++ -O3 -mavx heat3d_simd.cpp -o heat3d_simd
g++ -fopenmp heat3d_wave.cpp -o heat3d_wave

output_file="time.csv"

if [ ! -f "$output_file" ]; then
    echo "Size(N),Seq(s),Doacross(s),SIMD(s),wave(s)" > "$output_file"
fi

for N in "${N_array[@]}"; do

    seq_time=$(./heat3d_seq <<< "$N")
    seq=$(echo "$seq_time" | awk '{print $3}')

    doacross_time=$(./heat3d_doacross <<< "$N")
    doacross=$(echo "$doacross_time" | awk '{print $3}')

    simd_time=$(./heat3d_simd <<< "$N")
    simd=$(echo "$simd_time" | awk '{print $3}')

    wave_time=$(./heat3d_wave <<< "$N")
    wave=$(echo "$wave_time" | awk '{print $3}')

    echo "$N,$seq,$doacross,$simd,$wave" >> "$output_file"
done