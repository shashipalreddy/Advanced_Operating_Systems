/*SHASHIPAL REDDY PINGILI
Instructor: Dr. Mohammed Yassine Belkhouche
COSC 6352, Advanced Operating Systems SPRING 2019,
Project # 1
 */

//compile:mpicc myallgather.c -o ./gather
//run: mpirun -np 6 ./gather
//jumpshot:mpecc -mpilog myallgather.c -o ./gather



#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main ( int argc, char *argv[] )
{
   int size,myrank,i,j,k;
   double start,end,min=100000000000.00,max=0;
   MPI_Init(&argc,&argv);
   
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
   int l[size];
	j = 2*myrank;//assigning values
	
	start=MPI_Wtime();//initialized starting time
	for(i=0;i<size;i++)
		MPI_Send(&j,1,MPI_INT, i,0,MPI_COMM_WORLD);
	for(i=0;i<size;i++)
		MPI_Recv(&l[i],1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	end=MPI_Wtime();//ending times
	
	printf("\n \n data in %d is",size);//the data is printed
	for(k=0;k<size;k++)
		printf(" %d", l[k]);//the data is printed
	printf("\n");

	MPI_Finalize();
	 if(min > start){
        min = start;//assigned min start value is stored in minimum time
    }

    if(max < end){
        max = end; //maximum time out of all process is allocated
    }
    if(myrank == 0){
        printf("\n total time %.8f\n",max-min);//total designated time is printed
    }
   return 0;
}