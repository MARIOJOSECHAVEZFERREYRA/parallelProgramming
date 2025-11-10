#include <mpi.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex print_mutex;

void thread_function(int thread_id, int rank, int total_threads) {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "I am " << thread_id
              << " thread from " << rank
              << " process. Number of hybrid threads = "
              << total_threads << std::endl;
}

int main(int argc, char** argv) {
    int rank;
    int size;
    int n;
    int total_threads;

    n = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cin >> n;  // количество нитей в каждом процессе
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n <= 0) {
        MPI_Finalize();
        return 0;
    }

    total_threads = n * size;

    std::vector<std::thread> threads;
    threads.reserve(n);

    for (int i = 0; i < n; i++) {
        threads.emplace_back(thread_function, i, rank, total_threads);
    }

    for (int i = 0; i < n; i++) {
        threads[i].join();
    }

    MPI_Finalize();
    return 0;
}
