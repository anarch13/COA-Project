#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using Clock = chrono::high_resolution_clock;
using grid = vector<vector<vector<double>>>;

void initializeheat(grid &heat, int N) {
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

void heat3D_parallel(grid &heat, int N, int T) {
    grid newheat(N, vector<vector<double>>(N, vector<double>(N)));

    for (int t = 0; t < T; t++) {
        #pragma omp parallel for ordered(3)
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                for (int k = 1; k < N-1; k++) {
                    #pragma omp await depend(i-1,j,k) depend(i,j-1,k) depend (i,j,k-1)
                    newheat[i][j][k] = 0.125 * (heat[i+1][j][k] + heat[i-1][j][k] 
                    + heat[i][j+1][k] + heat[i][j-1][k] + heat[i][j][k+1] 
                    + heat[i][j][k-1] - 6 * heat[i][j][k]) + heat[i][j][k];
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
    initializeheat(heat, N);

    // Parallel version timing
    auto start = Clock::now();
    heat3D_parallel(heat, N, T);
    auto end = Clock::now();
    auto par_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Parallel Duration: " << par_duration << " ms" << endl;

    return 0;
}
