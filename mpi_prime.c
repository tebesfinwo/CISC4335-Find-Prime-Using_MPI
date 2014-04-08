#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//3001134

/**
*
* @param int number to be checked
* @return int 0 if the num is prime else 1;
*
*/
int prime(int num){
	// handling special case
	if ( num < 3 ) {
		if ( num == 2 ) 
			return 0; 
		else 
			return 1;
	} else {
		//even number can't be prime
		if(num % 2 == 0) return 1;
		int sqrt_num = sqrt(num);
		for(int i = 3; i < sqrt_num; i+=2){
			if(num%i == 0) return 1;
		}
	}
	return 0;
}

int check_prime(int nb){
	if(nb < 3){
		if ( nb == 2 )
			return 0;
		else 
			return 1;
	}

	int i, test, limit;
	test =  0; 

	limit = sqrt(nb) + 1;

	if (nb % 2 == 0) 
		test = 1;
	else {
		for(i = 3; i < limit && ! test; i+=2){
			if ( nb % i == 0) 
				test = 1;
		}
	}

	// if (nb < 100) {
	// 	if (!test)
	// 		printf("%d is prime number\n", nb);
	// 	else
	// 		printf("%d is not a prime number\n",nb);
	// }

	return test;

}


int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

		//total number size
	long number = 50000000;

	//local count 
	long count = 0;

	//global count
	long total_count = 0;

	//timings
	double time_start, time_end, min_start_time, max_end_time;

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//get the starting time
    MPI_Barrier(MPI_COMM_WORLD);
    time_start = MPI_Wtime();

	long size = number / world_size;
	long start = size * world_rank;
	long finish = start + size;

	// printf("This is rank %d \n", world_rank);
	// printf("start at %ld \n", start);
	// printf("finish at %ld \n", finish);

	for(long i = start; i <= finish; i++){
		if( check_prime(i) == 0 ) 
			count++;
	}

	MPI_Reduce(&count, &total_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	//get the ending time
    MPI_Barrier(MPI_COMM_WORLD);
    time_end = MPI_Wtime();

    printf("Process %d starts at %f and ends at %f. Its elapse time is %f\n", world_rank, time_start, time_end, time_end - time_start);

    /*
	Since all processes are running in parallel, we just need to get the minimun starting time and maximum starting time to calculate the elapse time.
    */
    MPI_Reduce(&time_start, &min_start_time, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time_end,  &max_end_time,1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	MPI_Finalize();


	if ( world_rank == 0 ) {
		printf("The total sum of prime number is %ld. \n", total_count);
		printf("The processes start at %f and finish  at %f. Their elapse time is %f\n", min_start_time, max_end_time, max_end_time - min_start_time);
	}
}
