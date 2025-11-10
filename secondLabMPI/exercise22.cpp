#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    int exit_code;
    int msg;
    int i;
    MPI_Status status;

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
        msg = rank;
        for (i = 0; i < size; i++) {
            if (i != rank) {
                MPI_Send(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        for (i = 0; i < size; i++) {
            if (i != rank) {
                MPI_Recv(&msg, 1, MPI_INT, i, 0,
                         MPI_COMM_WORLD, &status);
                std::cout << "[" << rank << "]: receive message '"
                          << msg << "' from " << i << std::endl;
            }
        }
    }

    MPI_Finalize();
    return exit_code;
}
