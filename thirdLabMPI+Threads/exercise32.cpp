#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <iomanip>

int main(int argc, char** argv) {
    int rank;
    int size;
    int provided;
    long long N;
    double h;
    double local_sum;
    double global_sum;

    N = 0;
    local_sum = 0.0;
    global_sum = 0.0;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cin >> N;  // точность
    }

    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (N > 0) {
        h = 1.0 / static_cast<double>(N);
        local_sum = 0.0;

#pragma omp parallel for reduction(+:local_sum)
        for (long long k = rank; k < N; k += size) {
            double x = (static_cast<double>(k) + 0.5) * h;
            local_sum += 4.0 / (1.0 + x * x);
        }

        MPI_Reduce(
                &local_sum,
                &global_sum,
                1,
                MPI_DOUBLE,
                MPI_SUM,
                0,
                MPI_COMM_WORLD
        );

        if (rank == 0) {
            double pi = h * global_sum;
            std::cout << std::fixed << std::setprecision(8) << pi << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
