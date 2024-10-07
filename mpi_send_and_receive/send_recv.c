#include <mpi.h>
#include <stdio.h>

int main() {
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2) {
        fprintf(stderr, "World size must be 2\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        return -1;
    }

    int number;
    if (world_rank == 0) {
        number = -1;
        MPI_Send(
            /* data         */ &number,
            /* count        */ 1,
            /* datatype     */ MPI_INT,
            /* dest         */ 1,
            /* tag          */ 0,
            /* communicator */ MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        MPI_Recv(
            /* data         */ &number,
            /* count        */ 1,
            /*datatype      */ MPI_INT,
            /* source       */ 0,
            /* tag          */ 0,
            /* communicator */ MPI_COMM_WORLD,
            /* status       */ MPI_STATUS_IGNORE
        );

        printf("Process 1 received number %d from process 0\n", number);
    }

    MPI_Finalize();

    return 0;
}
