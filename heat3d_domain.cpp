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
                if (i == 0 || j == 0 || k == 0 || i == N - 1 || j == N - 1 || k == N - 1)
                    heat[i][j][k] = 100.0; // Boundary condition
                else
                    heat[i][j][k] = 0.0; // Initial condition
            }
        }
    }
}

void heat3D_domain(grid &heat, int N, int T) {
    grid newheat(N, vector<vector<double>>(N, vector<double>(N)));

    int num_threads;
    #pragma omp parallel 
    {
        num_threads = omp_get_num_threads();
    }

    int chunk_size = N / num_threads; // Dividing the grid along the z-axis

    for (int t = 0; t < T; t++) {
        #pragma omp parallel
        {
            int thread_id = omp_get_thread_num();
            int start_k = thread_id * chunk_size;
            int end_k = (thread_id == num_threads - 1) ? N - 1 : start_k + chunk_size;

            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    for (int k = max(1, start_k); k < min(N - 1, end_k); k++) {
                        newheat[i][j][k] = 0.125 * (heat[i+1][j][k] + heat[i-1][j][k] 
                        + heat[i][j+1][k] + heat[i][j-1][k] + heat[i][j][k+1] 
                        + heat[i][j][k-1] - 6 * heat[i][j][k]) + heat[i][j][k];
                    }
                }
            }
        }
        #pragma omp barrier
        swap(heat, newheat);
    }
}

int main() {
    int N, T = 1000;
    cin >> N;

    grid heat(N, vector<vector<double>>(N, vector<double>(N)));
    initialize(heat, N);
    
    // Domain Decomposition version timing
    auto start = Clock::now();
    heat3D_domain(heat, N, T);
    auto end = Clock::now();
    auto domain_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Domain Decomposition Duration: " << domain_duration << " ms" << endl;

    return 0;
}