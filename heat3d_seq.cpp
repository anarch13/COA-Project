#include <iostream>
#include <vector>
#include <chrono>

#define N 100 // heat size (e.g., 100x100x100)
#define T 1000 // Number of time steps

using namespace std;
using Clock = chrono::high_resolution_clock;
using grid = vector<vector<vector<double>>>;

void initializeheat(grid &heat) {
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

void heat3D_sequential(grid &heat) {
    grid newheat = heat;

    for (int t = 0; t < T; t++) {
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                for (int k = 1; k < N-1; k++) {
                    newheat[i][j][k] = 0.125 * (heat[i+1][j][k] + heat[i-1][j][k] + heat[i][j+1][k] 
                    + heat[i][j-1][k] + heat[i][j][k+1] + heat[i][j][k-1] - 6 * heat[i][j][k]) + heat[i][j][k];
                }
            }
        }
        heat = newheat;
    }
}

int main() {
    grid heat(N, vector<vector<double>>(N, vector<double>(N)));
    initializeheat(heat);

    // Sequential version timing
    auto start = Clock::now();
    heat3D_sequential(heat);
    auto end = Clock::now();
    auto seq_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Sequential Duration: " << seq_duration << " ms" << endl;

    return 0;
}
