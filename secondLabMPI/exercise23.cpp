#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    int rank;
    int size;
    int n;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n = 0;
    std::vector<char> buffer;

    if (rank == 0) {
        std::string s;
        std::cin >> n;
        std::cin >> s;

        buffer.assign(s.begin(), s.end());
        n = static_cast<int>(buffer.size());
        for (i = 1; i < size; i++) {
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(buffer.data(), n, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buffer.resize(n);
        MPI_Recv(buffer.data(), n, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int local_counts[26];
    int global_counts[26];

    for (i = 0; i < 26; i++) {
        local_counts[i] = 0;
    }

    for (i = rank; i < n; i += size) {
        char c = buffer[i];
        if (c >= 'a' && c <= 'z') {
            local_counts[c - 'a'] = local_counts[c - 'a'] + 1;
        }
    }

    MPI_Reduce(
            local_counts,
            global_counts,
            26,
            MPI_INT,
            MPI_SUM,
            0,
            MPI_COMM_WORLD
    );

    if (rank == 0) {
        for (i = 0; i < 26; i++) {
            if (global_counts[i] > 0) {
                char c = static_cast<char>('a' + i);
                std::cout << c << " = " << global_counts[i] << std::endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
