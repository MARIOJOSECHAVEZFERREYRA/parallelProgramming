#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    int exit_code;

    exit_code = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cout << size << " processes." << std::endl;
    }

    if (rank > 0) {
        if (rank % 2 == 1) {
            std::cout << "I am " << rank << " process: FIRST!" << std::endl;
        } else {
            std::cout << "I am " << rank << " process: SECOND!" << std::endl;
        }
    }

    MPI_Finalize();
    return exit_code;
}
