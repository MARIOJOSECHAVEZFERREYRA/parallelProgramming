#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank;
    int size;
    int number;
    int exit_code;

    exit_code = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("Ejecuta el programa con al menos 2 procesos.\n");
        }
        exit_code = 1;
    } else {
        if (rank == 0) {
            number = 123;
            printf("Proceso %d: voy a enviar %d al proceso 1.\n", rank, number);
            MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d: he recibido %d del proceso 0.\n", rank, number);
        }
    }

    MPI_Finalize();
    return exit_code;
}