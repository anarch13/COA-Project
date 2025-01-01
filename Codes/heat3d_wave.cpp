#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

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
    // Set a heat source in the center
    heat[N/2][N/2][N/2] = 100.0;
}

void heat3D_wavefront(grid &heat, int N, int T) {
    grid newheat(N, vector<vector<double>>(N, vector<double>(N)));

    for (int t = 0; t < T; t++) {
        for (int wave = 2; wave < 2*(N-1); wave++) {
            #pragma omp parallel for collapse(2)
            for (int i = 1; i < N-1; i++) {
                for (int j = 1; j < N-1; j++) {
                    int k = wave - i - j;
                    if(k >= 1 and k < N-1) {
                        newheat[i][j][k] = 0.125 * (heat[i+1][j][k] + heat[i-1][j][k] 
                        + heat[i][j+1][k] + heat[i][j-1][k] + heat[i][j][k+1] 
                        + heat[i][j][k-1] - 6 * heat[i][j][k]) + heat[i][j][k];
                    }
                }
            }
            #pragma omp barrier
        }
        swap(heat, newheat);
    }
}

int main() {
    int N, T = 1000;
    cin >> N;

    grid heat(N, vector<vector<double>>(N, vector<double>(N)));
    initialize(heat, N);
    
    // Wavefront version timing
    auto start = Clock::now();
    heat3D_wavefront(heat, N, T);
    auto end = Clock::now();
    auto wave_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Wavefront Duration: " << wave_duration / 1000.0 << " s" << endl;

    return 0;
}