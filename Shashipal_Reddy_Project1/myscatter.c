/*SHASHIPAL REDDY PINGILI
Instructor: Dr. Mohammed Yassine Belkhouche
COSC 6352, Advanced Operating Systems SPRING 2019,
Project # 1
 */

//compile:mpicc myscatter.c -o ./scatter
//run: mpirun -np 6 ./scatter
//jumpshot:mpecc -mpilog myscatter.c -o ./scatter


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main ( int argc, char *argv[] )
{
   int size,myrank,i,j;
   double start,end,min=100000000000.00,max=0;
   MPI_Init(&argc,&argv);
   start=MPI_Wtime();//initialized starting time
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	int l[size];
      l[0] = 2;
      for (i=1; i<size; i++) 
		  l[i] = 2*l[i-1];//assigning values
     start=MPI_Wtime();//initialized starting time
   if(myrank == 0) 
   {             
	  MPI_Send(&l[0], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      for(i=1; i<size; i++) 
		  MPI_Send(&l[i],1,MPI_INT,i,0,MPI_COMM_WORLD);
	  MPI_Recv(&j,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
   }
   else /* every worker receives its j from root */
   {
      MPI_Recv(&j,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);  
   }
   end=MPI_Wtime();//ending time
	printf("the data in %d is %d", myrank, j);//the data is printed
    printf("\n");//the data is printed
   
   MPI_Finalize();
    if(min > start){
        min = start;//assigned min start value is stored in minimum time
    }

    if(max < end){
        max = end;//maximum time out of all process is allocated
    }
    if(myrank == 0){
        printf("\n total time %.8f\n",max-min);//total designated time is printed
    }
   return 0;
}