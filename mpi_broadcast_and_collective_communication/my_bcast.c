#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void my_bcast(void *data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == root) {
        int i;
        for (i = 0; i < world_size; i++) {
            if (i != world_rank)
                MPI_Send(data, count, datatype, i, 0, communicator);
        }
    } else {
        MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
    }
}

int main() {
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int data;
    if (world_rank == 0) {
        data = 100;
        printf("Process 0 broadcasting data %d\n\n", data);
        my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Process %d received %d from root process\n", world_rank, data);
    }

    MPI_Finalize();

    return 0;
}
