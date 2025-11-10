#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm parent_comm;
    MPI_Comm_get_parent(&parent_comm);

    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (parent_comm == MPI_COMM_NULL) {
        std::cout << "I am " << rank
                  << " process from " << size
                  << " processes!" << std::endl;
        std::cout << "My parent is none." << std::endl;

        int n = 0;
        if (rank == 0) {
            std::cin >> n;
        }

        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Comm intercomm;
        MPI_Comm_spawn(
                argv[0],
                MPI_ARGV_NULL,
                n,
                MPI_INFO_NULL,
                0,
                MPI_COMM_WORLD,
                &intercomm,
                MPI_ERRCODES_IGNORE
        );

        MPI_Comm_free(&intercomm);
    } else {
        int child_rank = rank;
        int child_size = size;
        int parent_rank = 0;

        std::cout << "I am " << child_rank
                  << " process from " << child_size
                  << " processes!" << std::endl;
        std::cout << "My parent is " << parent_rank << "." << std::endl;

        MPI_Comm_free(&parent_comm);
    }

    MPI_Finalize();
    return 0;
}
