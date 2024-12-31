#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <immintrin.h>

using namespace std;
using Clock = chrono::high_resolution_clock;
using grid = vector<vector<vector<double>>>;

void initialize(grid &heat, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                heat[i][j][k] = 0.0;
            }
        }
    }
    // Set a heat source
    heat[N/2][N/2][N/2] = 100.0;
}

void heat3D_simd(grid &heat, int N, int T) {
    grid newheat(N, vector<vector<double>>(N, vector<double>(N)));

    for (int t = 0; t < T; t++) {
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                for (int k = 1; k < N-1; k += 4) {
                    // Process 4 elements with AVX2
                    // Load 4 doubles from neighboring cells into SIMD registers
                    __m256d center = _mm256_loadu_pd(&heat[i][j][k]);     
                    __m256d left = _mm256_loadu_pd(&heat[i-1][j][k]);    
                    __m256d right = _mm256_loadu_pd(&heat[i+1][j][k]);
                    __m256d front = _mm256_loadu_pd(&heat[i][j-1][k]);  
                    __m256d back = _mm256_loadu_pd(&heat[i][j+1][k]);  
                    __m256d bottom = _mm256_loadu_pd(&heat[i][j][k-1]);    
                    __m256d top = _mm256_loadu_pd(&heat[i][j][k+1]);   
                    
                    // Compute the new heat value
                    __m256d result = _mm256_mul_pd(center, _mm256_set1_pd(0.25));
                    result = _mm256_add_pd(result, _mm256_mul_pd(left, _mm256_set1_pd(0.125)));
                    result = _mm256_add_pd(result, _mm256_mul_pd(right, _mm256_set1_pd(0.125)));
                    result = _mm256_add_pd(result, _mm256_mul_pd(front, _mm256_set1_pd(0.125)));
                    result = _mm256_add_pd(result, _mm256_mul_pd(back, _mm256_set1_pd(0.125)));
                    result = _mm256_add_pd(result, _mm256_mul_pd(top, _mm256_set1_pd(0.125)));
                    result = _mm256_add_pd(result, _mm256_mul_pd(bottom, _mm256_set1_pd(0.125)));

                    // Store the result back
                    _mm256_storeu_pd(&newheat[i][j][k], result);
                }
            }
        }
        swap(heat, newheat);
    }
}

int main() {
    int N, T = 1000;
    cin >> N;

    grid heat(N, vector<vector<double>>(N, vector<double>(N)));
    initialize(heat, N);

    // SIMD version timing
    auto start = Clock::now();
    heat3D_simd(heat, N, T);
    auto end = Clock::now();
    auto simd_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "SIMD Duration: " << simd_duration << " ms" << endl;

    return 0;
}
