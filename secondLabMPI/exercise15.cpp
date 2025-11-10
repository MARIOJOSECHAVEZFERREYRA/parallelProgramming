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

    if (size > 0) {
        std::cout << "I am " << rank
                  << " process from " << size
                  << " processes!" << std::endl;
    } else {
        exit_code = 1;
    }

    MPI_Finalize();
    return exit_code;
}
