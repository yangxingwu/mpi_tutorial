EXECS=mpi_hello_world
MPICC?=mpicc

all: ${EXECS}

mpi_hello_world: mpi_hello_world.c
	${MPICC} ${CFLAGS} ${LDFLAGS} -o mpi_hello_world mpi_hello_world.c

clean:
	rm -rf ${EXECS}
