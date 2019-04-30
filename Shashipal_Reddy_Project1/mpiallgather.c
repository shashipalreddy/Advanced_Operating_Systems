/*SHASHIPAL REDDY PINGILI
Instructor: Dr. Mohammed Yassine Belkhouche
COSC 6352, Advanced Operating Systems SPRING 2019,
Project # 1
 */

//compile:mpicc mpiallgather.c -o ./gather
//run: mpirun -np 6 ./gather
//jumpshot:mpecc -mpilog mpiallgather.c -o ./gather


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[])
 {
    int size, rank, mvotes, nvotes[size];
	double start,end,min=100000000000.00,max=0;
    MPI_Init ( &argc, &argv );
	
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );
    mvotes = rank;//initializing data to be sent
	start=MPI_Wtime();//initialized starting time
    /* Allgather to processes*/
    MPI_Allgather(&mvotes, 1, MPI_INT,nvotes, 1, MPI_INT, MPI_COMM_WORLD); 
	end=MPI_Wtime();//ending time
	
	printf("P%d: Gathered\t ", rank);
	for (int i=0; i<size; i++)
		printf("\t%d ", nvotes[i]);
	printf("\n");
      
    MPI_Finalize();
	if(min > start){
		//assigned min start value is stored in minimum time
        min = start;
    }

    if(max < end){
		//maximum time out of all process is allocated
        max = end;
    }
    if(rank == 0){
		//total designated time is printed
        printf("\n total time %.8f\n",max-min);
    }
    return 0;
}