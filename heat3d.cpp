#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

#define N 100 // Grid size (e.g., 100x100x100)
#define T 1000 // Number of time steps
#define ALPHA 0.1 // Diffusion coefficient

using namespace std;
using Clock = chrono::high_resolution_clock;

void initializeGrid(vector<vector<vector<double>>> &grid) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                grid[i][j][k] = 0.0;
            }
        }
    }
    // Set a heat source
    grid[N/2][N/2][N/2] = 100.0;
}

void heat3D_sequential(vector<vector<vector<double>>> &grid) {
    vector<vector<vector<double>>> newGrid = grid;

    for (int t = 0; t < T; t++) {
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                for (int k = 1; k < N-1; k++) {
                    newGrid[i][j][k] = grid[i][j][k] + ALPHA * (
                        grid[i+1][j][k] + grid[i-1][j][k] +
                        grid[i][j+1][k] + grid[i][j-1][k] +
                        grid[i][j][k+1] + grid[i][j][k-1] -
                        6 * grid[i][j][k]
                    );
                }
            }
        }
        grid = newGrid;
    }
}

void heat3D_parallel(vector<vector<vector<double>>> &grid) {
    vector<vector<vector<double>>> newGrid = grid;

    for (int t = 0; t < T; t++) {
        #pragma omp parallel for collapse(3) schedule(static)
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                for (int k = 1; k < N-1; k++) {
                    newGrid[i][j][k] = grid[i][j][k] + ALPHA * (
                        grid[i+1][j][k] + grid[i-1][j][k] +
                        grid[i][j+1][k] + grid[i][j-1][k] +
                        grid[i][j][k+1] + grid[i][j][k-1] -
                        6 * grid[i][j][k]
                    );
                }
            }
        }
        #pragma omp barrier
        grid = newGrid;
    }
}

int main() {
    vector<vector<vector<double>>> grid(N, vector<vector<double>>(N, vector<double>(N)));
    initializeGrid(grid);

    // Sequential version timing
    auto start = Clock::now();
    heat3D_sequential(grid);
    auto end = Clock::now();
    auto seq_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Sequential Duration: " << seq_duration << " ms" << endl;

    // Re-initialize grid for parallel computation
    initializeGrid(grid);

    // Parallel version timing
    start = Clock::now();
    heat3D_parallel(grid);
    end = Clock::now();
    auto par_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Parallel Duration: " << par_duration << " ms" << endl;

    // Performance comparison
    cout << "Speedup: " << (double)seq_duration / par_duration << "x" << endl;

    return 0;
}
