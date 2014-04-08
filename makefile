EXECS=mpi_hello_world
MPICC?=mpicc

all: ${EXECS}

mpi_hello_world: mpi_prime.c
	${MPICC} -o mpi_prime mpi_prime.c

clean:
	rm ${EXECS}