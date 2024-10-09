#ifndef __PARALLEL_RANK_H__
#define __PARALLEL_RANK_H__

#include <mpi.h>

int TMPI_Rank(void *send_data, void *recv_data, MPI_Datatype datatype, MPI_Comm comm);

#endif
