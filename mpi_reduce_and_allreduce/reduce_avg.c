#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>

float *create_rand_nums(int num_elements) {
    if (num_elements <= 0)
        return NULL;

    float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
    assert(rand_nums);

    int i;
    for (i = 0; i < num_elements; i++)
        rand_nums[i] = (rand() / (float)RAND_MAX);

    return rand_nums;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s num_elements_per_proc\n", argv[0]);
        return -1;
    }

    int num_elements_per_proc = atoi(argv[1]);

    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Create a random array of elements on all processes.
    srand(time(NULL) * world_rank);
    float *rand_nums = create_rand_nums(num_elements_per_proc);
    assert(rand_nums);

    // Sum the numbers locally
    float local_sum = 0;
    int i;
    for (i = 0; i < num_elements_per_proc; i++)
        local_sum += rand_nums[i];

    // Print the random numbers on each processes
    printf("Local sum for process %d - %f, avg = %f\n",
           world_rank, local_sum, local_sum / num_elements_per_proc);

    // Reduce all of the local sums into the global sum
    float global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the result
    if (world_rank == 0) {
        printf("Total sum = %f, avg = %f\n", global_sum,
               global_sum / (world_size * num_elements_per_proc));
    }

    // Clean up
    free(rand_nums);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
