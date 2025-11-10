#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    long long N;
    double h;
    double local_sum;
    double global_sum;
    int exit_code;

    exit_code = 0;
    N = 0;
    local_sum = 0.0;
    global_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cin >> N;
    }

    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (N <= 0) {
        if (rank == 0) {
            std::cout << "N must be positive" << std::endl;
        }
        exit_code = 1;
    }

    if (exit_code == 0) {
        h = 1.0 / static_cast<double>(N);
        local_sum = 0.0;

        for (long long k = rank; k < N; k += size) {
            double x = (static_cast<double>(k) + 0.5) * h;
            local_sum += 4.0 / (1.0 + x * x);
        }

        MPI_Win win;
        if (rank == 0) {
            MPI_Win_create(
                    &global_sum,
                    sizeof(double),
                    sizeof(double),
                    MPI_INFO_NULL,
                    MPI_COMM_WORLD,
                    &win
            );
        } else {
            MPI_Win_create(
                    nullptr,
                    0,
                    1,
                    MPI_INFO_NULL,
                    MPI_COMM_WORLD,
                    &win
            );
        }

        MPI_Win_fence(0, win);

        MPI_Accumulate(
                &local_sum,
                1,
                MPI_DOUBLE,
                0,
                0,
                1,
                MPI_DOUBLE,
                MPI_SUM,
                win
        );

        MPI_Win_fence(0, win);

        if (rank == 0) {
            double pi = h * global_sum;
            std::cout.setf(std::ios::fixed);
            std::cout.precision(8);
            std::cout << pi << std::endl;
        }

        MPI_Win_free(&win);
    }

    MPI_Finalize();
    return exit_code;
}
