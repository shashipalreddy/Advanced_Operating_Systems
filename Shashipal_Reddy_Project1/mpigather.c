/*SHASHIPAL REDDY PINGILI
Instructor: Dr. Mohammed Yassine Belkhouche
COSC 6352, Advanced Operating Systems SPRING 2019,
Project # 1
 */

//compile:mpicc mpigather.c -o ./gather
//run: mpirun -np 6 ./gather
//jumpshot:mpecc -mpilog mpigather.c -o ./gather


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main ( int argc, char **argv ) {

    int rank;
    int size;

    int mvotes;//initialized data to be sent 
    int *nvotes;//initialized data to be received
	double start,end,min=100000000000.00,max=0;

    MPI_Init ( &argc, &argv );
	
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
    MPI_Comm_size ( MPI_COMM_WORLD, &size );

    if (rank == 0)
        nvotes = malloc(size * sizeof(int) );

    /* everyone sets their first mvotes element */
    mvotes = rank+4;
	//initialized starting time
    start=MPI_Wtime();
    /* Gather to process 0 */
    MPI_Gather(&mvotes, 1, MPI_INT, nvotes, 1, MPI_INT, 0, MPI_COMM_WORLD); //mpi_gather function
	//ending time
	end=MPI_Wtime();
	printf("P%d: %d\n", rank, mvotes);
    if (rank == 0) {
        printf("P%d: Gathered ", rank);
        for (int i=0; i<size; i++)
            printf("%d ", nvotes[i]);
        printf("\n");
    }

    if (rank == 0)
        free(nvotes);

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