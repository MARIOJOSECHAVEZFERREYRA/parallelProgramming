#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    int send_value;
    int recv_value;
    int dest;
    int source;
    int exit_code;
    MPI_Status status;

    exit_code = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            std::cout << "Need at least 2 processes." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    send_value = rank;
    dest = (rank + 1) % size;
    source = (rank - 1 + size) % size;

    MPI_Sendrecv(
            &send_value, 1, MPI_INT, dest, 0,
            &recv_value, 1, MPI_INT, source, 0,
            MPI_COMM_WORLD, &status
    );

    std::cout << "[" << rank << "]: receive message '"
              << recv_value << "'" << std::endl;

    MPI_Finalize();
    return exit_code;
}
