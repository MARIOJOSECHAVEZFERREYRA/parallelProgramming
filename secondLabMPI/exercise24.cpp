#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    long long N;
    double h;
    double local_sum;
    double global_sum;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cin >> N;
    }

    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    h = 1.0 / static_cast<double>(N);
    local_sum = 0.0;

    for (long long k = rank; k < N; k += size) {
        double x = (static_cast<double>(k) + 0.5) * h;
        local_sum += 4.0 / (1.0 + x * x);
    }

    if (rank == 0) {
        global_sum = local_sum;
        for (i = 1; i < size; i++) {
            double tmp;
            MPI_Recv(&tmp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            global_sum += tmp;
        }
        double pi = h * global_sum;
        std::cout.setf(std::ios::fixed);
        std::cout.precision(8);
        std::cout << pi << std::endl;
    } else {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
