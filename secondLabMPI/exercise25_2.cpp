#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    int rank;
    int size;
    int n;
    int i;
    int j;
    int k;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n = 0;
    if (rank == 0) {
        std::cin >> n;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = n / size;
    if (rows_per_proc == 0 || (n % size) != 0) {
        if (rank == 0) {
            std::cout << "n must be divisible by number of processes" << std::endl;
        }
        MPI_Finalize();
        return 0;
    }

    int block_size = rows_per_proc * n;

    std::vector<double> A;
    std::vector<double> B;
    std::vector<double> C;

    if (rank == 0) {
        A.resize(n * n);
        B.resize(n * n);
        C.resize(n * n);

        for (i = 0; i < n * n; i++) {
            std::cin >> A[i];
        }
        for (i = 0; i < n * n; i++) {
            std::cin >> B[i];
        }
    } else {
        B.resize(n * n);
    }

    // всем нужна полная матрица B
    MPI_Bcast(B.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> local_A(block_size);
    std::vector<double> local_C(block_size);

    // рассылаем блоки A точка-точка
    if (rank == 0) {
        for (i = 1; i < size; i++) {
            MPI_Send(
                    &A[i * block_size],
                    block_size,
                    MPI_DOUBLE,
                    i,
                    0,
                    MPI_COMM_WORLD
            );
        }
        for (i = 0; i < block_size; i++) {
            local_A[i] = A[i];
        }
    } else {
        MPI_Recv(
                local_A.data(),
                block_size,
                MPI_DOUBLE,
                0,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
        );
    }

    // локальное умножение
    for (i = 0; i < rows_per_proc; i++) {
        for (j = 0; j < n; j++) {
            double sum = 0.0;
            for (k = 0; k < n; k++) {
                sum += local_A[i * n + k] * B[k * n + j];
            }
            local_C[i * n + j] = sum;
        }
    }

    //  собираем C точка-точка
    if (rank == 0) {
        for (i = 0; i < block_size; i++) {
            C[i] = local_C[i];  // свой блок
        }
        for (i = 1; i < size; i++) {
            MPI_Recv(
                    &C[i * block_size],
                    block_size,
                    MPI_DOUBLE,
                    i,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE
            );
        }

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                std::cout << C[i * n + j];
                if (j + 1 < n) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    } else {
        MPI_Send(
                local_C.data(),
                block_size,
                MPI_DOUBLE,
                0,
                0,
                MPI_COMM_WORLD
        );
    }

    MPI_Finalize();
    return 0;
}
