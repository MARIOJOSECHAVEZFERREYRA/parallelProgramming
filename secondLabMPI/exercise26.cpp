#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

int main(int argc, char** argv) {
    int world_rank;
    int world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Group world_group;
    MPI_Group even_group;
    MPI_Comm even_comm;

    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int even_count = (world_size + 1) / 2;  // 0,2,4,...
    std::vector<int> even_ranks(even_count);
    int idx = 0;
    for (int i = 0; i < world_size; i++) {
        if (i % 2 == 0) {
            even_ranks[idx] = i;
            idx++;
        }
    }

    MPI_Group_incl(world_group, even_count, even_ranks.data(), &even_group);
    MPI_Comm_create(MPI_COMM_WORLD, even_group, &even_comm);

    int new_rank = -1;
    int new_size = 0;
    std::string final_message = "no";

    if (even_comm != MPI_COMM_NULL) {
        MPI_Comm_rank(even_comm, &new_rank);
        MPI_Comm_size(even_comm, &new_size);

        char msg_buf[11];

        if (new_rank == 0) {
            std::string input;
            std::cin >> input;
            if (input.size() > 10) {
                input = input.substr(0, 10);
            }
            std::snprintf(msg_buf, 11, "%s", input.c_str());
        }

        MPI_Bcast(msg_buf, 11, MPI_CHAR, 0, even_comm);

        final_message = msg_buf;
    }

    if (even_comm != MPI_COMM_NULL) {
        std::cout << "MPI_COMM_WORLD: " << world_rank
                  << " from " << world_size
                  << ". New comm: " << new_rank
                  << " from " << new_size
                  << ". Message = " << final_message
                  << std::endl;
    } else {
        std::cout << "MPI_COMM_WORLD: " << world_rank
                  << " from " << world_size
                  << ". New comm: no from no. Message = no"
                  << std::endl;
    }

    MPI_Group_free(&even_group);
    MPI_Group_free(&world_group);
    if (even_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&even_comm);
    }

    MPI_Finalize();
    return 0;
}
