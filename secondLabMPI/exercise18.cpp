#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    int message;
    int exit_code;

    exit_code = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            std::cout << "Need at least 2 processes." << std::endl;
        }
        exit_code = 1;
    } else {
        if (rank == 0) {
            message = rank;
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

            MPI_Recv(&message, 1, MPI_INT, size - 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "[" << rank << "]: receive message '"
                      << message << "'" << std::endl;
        } else {
            MPI_Recv(&message, 1, MPI_INT, rank - 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::cout << "[" << rank << "]: receive message '"
                      << message << "'" << std::endl;

            message = message + 1;
            MPI_Send(&message, 1, MPI_INT, (rank + 1) % size,
                     0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return exit_code;
}
