#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank;
    int size;
    int message;
    int exit_code;
    MPI_Request request;
    MPI_Status status;

    exit_code = 0;
    message = 45;  // сообщение, которое пошлёт процесс 0

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
            MPI_Isend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
        }
        if (rank == 1) {
            MPI_Irecv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            std::cout << "receive message '" << message << "'" << std::endl;
        }
    }

    MPI_Finalize();
    return exit_code;
}
