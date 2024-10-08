#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void my_bcast(void *data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

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

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_elements> <num_trials>\n", argv[0]);
        return -1;
    }

    int num_elements = atoi(argv[1]);
    int num_trials = atoi(argv[2]);
    int *data = (int *)malloc(sizeof(int) * num_elements);
    assert(data);

    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double total_my_bcast_time = 0.0;
    double total_mpi_bcast_time = 0.0;

    int i;
    for (i = 0; i < num_trials; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        total_my_bcast_time -= MPI_Wtime();
        my_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_my_bcast_time += MPI_Wtime();

        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time -= MPI_Wtime();
        MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time += MPI_Wtime();
    }

    if (world_rank == 0) {
        printf("Data size = %d, Trials = %d\n", num_elements * (int)sizeof(int),
               num_trials);
        printf("Avg my_bcast time = %lf\n", total_my_bcast_time / num_trials);
        printf("Avg MPI_Bcast time = %lf\n", total_mpi_bcast_time / num_trials);
    }

    free(data);
    MPI_Finalize();

    return 0;
}
