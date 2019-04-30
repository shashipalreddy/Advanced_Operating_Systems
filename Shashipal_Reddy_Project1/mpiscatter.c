/*SHASHIPAL REDDY PINGILI
Instructor: Dr. Mohammed Yassine Belkhouche
COSC 6352, Advanced Operating Systems SPRING 2019,
Project # 1
 */

//compile:mpicc mpiscatter.c -o ./scatter
//run: mpirun -np 6 ./scatter
//jumpshot:mpecc -mpilog mpiscatter.c -o ./scatter

#include <stdio.h>
#include <mpi.h>
int main (int argc, char *argv[]) 
{
   int rank,size,i;
   double start,end,min=100000000000.00,max=0;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   double x[size],k;//initialized data to be sent and receive
   if(rank==3)
   {
       for(i=0;i<size;i++) 
	      x[i]=3.0*i;//assigning values
   }
   //initialized starting time
   start=MPI_Wtime();	
   //mpi_scatter function
   MPI_Scatter(x,1,MPI_DOUBLE,&k,1,MPI_DOUBLE,3,MPI_COMM_WORLD);
   //ending time
   end=MPI_Wtime();
   printf("P:%d k is %f\n",rank,k);

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