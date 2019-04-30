
/*SHASHIPAL REDDY PINGILI
Instructor: Dr. Mohammed Yassine Belkhouche
COSC 6352, Advanced Operating Systems SPRING 2019,
Project # 2
 */

//compile: mpecc -mpilog berkeley.c -o berkeley.
//run: mpirun -np 6 ./berkeley berkeley.
//jumpshot:jumpshot

/*
BERKELEY TIME SYNCHRONIZATION ALGORITHM 
Berkeley’s Algorithm is a clock synchronization technique used in distributed systems. 
The algorithm assumes that each machine node in the network either doesn’t have an accurate time source or doesn’t possess an UTC server.
An individual node is chosen as the coordinator node from a pool nodes in the network.
This node is the main node in the network which acts as a master and rest of the nodes act as slaves.

*/

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h> 
#include <mpi.h>
#include <unistd.h>
#define MAX 500

int main(int argc, char *argv[])
{
	int rank, size, i, j, coordinator, lastLineRead, minutes, seconds, time[3], sec, cMin, diffMin, pMin[MAX], t, iFlag[MAX], sumIFlags, hours, mins;
	FILE *inputFile;
	char line[MAX][8], timeString[8];
	float avgMin, adjMin, temp;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	//open a file in read only mode.
	inputFile = fopen(argv[1], "r");
	if(!inputFile)
	{
		// if no input file print error.
		printf("No input file to read by process %d!\n", rank);
		MPI_Finalize();
		exit;
	}
	
	i = 0;
    while( fgets(line[i], MAX, inputFile) != NULL )
	{
		if(rank == (i-1))
		{
			// calculating the value of time into seconds for each process.
			time[0] = ((line[i][0]-'0')*10) + (line[i][1]-'0'); // hours
			time[1] = ((line[i][3]-'0')*10) + (line[i][4]-'0'); // minutes
			time[2] = ((line[i][6]-'0')*10) + (line[i][7]-'0'); // seconds
			sec = (time[0]*60*60) + (time[1]*60) + time[2]; // convert each process time from string to seconds integer
		}
		i++;
	
	}
	fclose(inputFile);
	lastLineRead = i;
	if(size != lastLineRead - 1)
	{
		printf("Number of processes must be %d\n", lastLineRead-1);
		MPI_Finalize();
		exit;
	}
	// input from user.
	coordinator = atoi(line[0]);
	if(rank == coordinator)
	{
		printf("I am process with rank %d acting as the coordinator process\n", rank);
		j=rank+1;
		strncpy(timeString, line[j],8);
		timeString[8] = '\0';
				
		printf("Coordinator process is sending time %s\n", timeString);
		MPI_Bcast(timeString, 8, MPI_CHAR, coordinator, MPI_COMM_WORLD); 
		MPI_Barrier(MPI_COMM_WORLD); 
		
		for(i=0; i<size-1; i++)
		{
			MPI_Recv(&t,  1,  MPI_INT,  MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			pMin[status.MPI_SOURCE] = t;
			printf("Process %d has received time differential value of %d\n", rank, t);
			if(abs(t) > 7920 ) 
			{
				iFlag[status.MPI_SOURCE] = 0; 
				printf("Coordinator Process is ignoring time differential value of %d from process %d\n", t, status.MPI_SOURCE);
			}
			else
				iFlag[status.MPI_SOURCE] = 1;
		}
		
		avgMin = 0;
		sumIFlags = size;
		for(i=0; i<size; i++)
			if(i != rank)
			{
				avgMin = avgMin + (pMin[i]*iFlag[i]); 
				sumIFlags = sumIFlags + iFlag[i] - 1; 
			}
		avgMin = avgMin / sumIFlags; 
		printf("Time differential average is %f\n", avgMin);
		sec = sec + avgMin; 
		for(i=0; i<size; i++)
			if(i != rank)
			{
				adjMin = pMin[i] - avgMin; 
				printf("Coordinator process is sending the clock adjustment value of %f to process %d\n", adjMin, i);
				MPI_Send(&adjMin, 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD); 
			}
	}
	
	else
	{
		MPI_Bcast(timeString, 8, MPI_CHAR, coordinator, MPI_COMM_WORLD); 
		MPI_Barrier(MPI_COMM_WORLD);
		printf("Process %d has received time %s\n", rank, timeString);
		time[0] = ((timeString[0]-'0')*10) + (timeString[1]-'0'); //hours
		time[1] = ((timeString[3]-'0')*10) + (timeString[4]-'0'); //minutes
		time[2] = ((timeString[6]-'0')*10) + (timeString[7]-'0'); //seconds
		cMin = (time[0]*60*60) + time[1]*60 + time[2]; // calculating everything into seconds
		diffMin = cMin - sec; 
		printf("Process %d is sending time differential value of %d to process \n", rank, diffMin);
		MPI_Send(&diffMin, 1, MPI_INT, coordinator, 0, MPI_COMM_WORLD); 
		MPI_Recv(&adjMin, 1, MPI_FLOAT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status); 
		printf("Process %d has received the clock adjustment value of %f\n", rank, adjMin);
		sec = sec + adjMin; 
		
	}
	
	
	temp = sec;
	hours = temp/3600;
	time[0] = hours;
	
	mins = (temp - hours*3600)/60;
	
	time[1] = mins;
	
	time[2] = (temp-3600*time[0]-time[1]*60);
	
	if(time[2] == 60)
	{
		time[2] = 0;
		time[1]++;
	}
	if(time[1] == 60)
	{
		time[1] = 0;
		time[0]++;
	}
	if(time[0] == 24)
		time[0] = 0;
	
	//converting hours to string
	temp = time[0];
	temp = temp/10;
	hours = temp;
	timeString[0] = hours + '0';
	temp = (temp - hours)*10;
	hours = temp;
	timeString[1] = temp + '0';
	
	timeString[2] = ':';
	
	//converting minutes to string
	temp = time[1];
	temp = temp/10;
	minutes = temp;
	timeString[3] = minutes + '0';
	temp = (temp - minutes)*10;
	minutes = temp;
	timeString[4] = temp + '0';
	timeString[5] = ':';
	
	//converting seconds to string
	temp = time[2];
	temp = temp/10;
	seconds = temp;
	timeString[6] = seconds + '0';
	temp = (temp - seconds)*10;
	seconds = temp;
	timeString[7] = temp + '0';
	

	printf("Adjusted local time at process %d is %s\n", rank, timeString); 
	
	MPI_Finalize();
	return 0;
}