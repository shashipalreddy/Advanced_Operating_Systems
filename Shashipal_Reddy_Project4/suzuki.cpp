//*********************************************************************************************************************
// SHASHIPAL REDDY PINGILI
// Advanced Operating Systems
// Programming Project #4:  Implementation of Suzuki and Kasami's algorithm
// Due date: 04-29-2019 11:59p.m CST 
// Instructor: Dr. Mohammed Yassine Belkhouche
// File name: suzuki.cpp
//*********************************************************************************************************************
#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
using namespace std;

//Global variables which has the intial token holder and the status of the processes
int initHolder = rand();
int startime = time(NULL);
int released = 1;
int requested = 2;
int granted = 3;

//*************************************************************************************************************
//
// Main Function
//
// Only main function is used in this program. No other functions are used. 
// This program is written to implement the suzuki and kasami's algorithm
// This will receive two inputs. Simulation time and the maximum sleep time. Three main state are used in this program.
// The process which are all in released state will request for the token. The process with grant state will enter into 
// critical section and it will send the token to the process in the queue. Once all process will raise request it will
// move to the requested state. This process will continue until the simulation time is over. Once the simulation time
// is over no requests will be raised and the pending requests will be addressed  
//
//
// Input Parameter
// --------------------
//
// argv[1]     char			This parameter specifies the simulation time.
// argv[2]     char			This parameter specifies the maximum sleep time for each process before the request
//
//***************************************************************************************************************

int main(int argc, char *argv[]) 
{
	int rank,size,state=released,nxtProcess,sleepTime;
	int tokenReqTag=0,probeTag=0,tokenSndTag1=2,tokenSndTag2=3,probeFlag=0,completeReq=100;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	int LN[size],RN[size],Queue[size+1],sn=0,rsn=0;
	time_t stime,endtime;
	initHolder = initHolder % size;

	//Defines the startime and endtime based on the input simulation time
	stime = time(NULL);
	endtime = stime + atoi(argv[1]);
	//Maximum sleep time given as input
	sleepTime = atoi(argv[2]);

	//Initializing the queue, LN and RN values
	for(int i=0;i<size;i++)
	{		
		LN[i]=0;
		RN[i]=0;
		Queue[i]=-1;	
	}
	Queue[size]=-1;

	//Print which process has the token at the start of the program
	if(initHolder == rank)
	{
		state = granted;
		cout << "The process " << rank << " has the token initially" << "\n";

	}

	// This loop will process only for the time duration given as input
	while( stime < endtime)
	{

			//If the state of the processes are released it will raise request for the critical section and will broadcast the sequence no
			if(state == released)
			{
				sleep(rand()%sleepTime);
				sn = sn+1;
				RN[rank]=sn;
				cout <<"Process with rank " << rank << " and sequence number " << sn <<" is requesting critical section" << "\n";
				cout << "Broadcast message(" << rank << ":" << sn << ")" <<"\n";
				for(int i=0;i<size;i++)
				{
					if(i != rank)
						MPI_Send(&sn , 1 , MPI_INT , i , tokenReqTag , MPI_COMM_WORLD);
				}


				//Once it raise the request the state will be changed to requested
				state = requested;

			}
			MPI_Barrier(MPI_COMM_WORLD);

			//This loop will help to receive the critical section request from all other processes except its own
			for(int i=0;i<size;i++)
			{
				MPI_Iprobe(i , probeTag , MPI_COMM_WORLD , &probeFlag , &status);
				if(probeFlag == 1)
				{
					MPI_Recv(&rsn , 1 , MPI_INT , i , tokenReqTag , MPI_COMM_WORLD,&status);	
					cout<<"Rank " << rank << " received critical section request from rank " << i << "\n";
					if(RN[i] < rsn)
					{
						RN[i] = rsn;
					}
					if((state == granted) && (RN[i] == LN[i] + 1))
					{
						
						//Adding the processes to the queue
						for(int z=0;z<size;z++)
						{
							if(Queue[z] == -1)
							{
								Queue[z] = i;
								z=size;
								
							}
						}
									
					}

				}
				
				

			}
			MPI_Barrier(MPI_COMM_WORLD);

			//If the state of the process is granted then the rank will enter into the critical section and sleep for 2 seconds
			if(state == granted)
			{

				cout << "Rank " << rank << " entering into critical section" << "\n";
				LN[rank] = RN[rank];
				state = released;
				//Once it comes out of the critical section its state will be updated to released
				//It will take the next process in the head of the queue and move all other items one step forward
				nxtProcess = Queue[0];
				for(int x=0;x< size;x++)
				{
					Queue[x] = Queue[x+1];

				}

				//It will send the token to the next process which has been taken from the queue
				if(nxtProcess >=0 && nxtProcess != rank)
				{
					cout<<"Rank " << rank << " is sending the token to rank " << nxtProcess << "\n";
					MPI_Send(&Queue , size+1 , MPI_INT , nxtProcess , nxtProcess , MPI_COMM_WORLD);
					MPI_Send(&LN , size+1 , MPI_INT , nxtProcess , nxtProcess , MPI_COMM_WORLD);
				}

			}
			MPI_Barrier(MPI_COMM_WORLD);


			probeFlag = 0;

			//This help the processes to receive the token which has been sent and it will change the status to granted
			MPI_Iprobe(MPI_ANY_SOURCE , rank , MPI_COMM_WORLD , &probeFlag , &status);
			if(probeFlag == 1 && status.MPI_SOURCE != rank && status.MPI_SOURCE >=0 && status.MPI_SOURCE<size)
			{
				cout << "Rank " << rank << " has received the token from rank " << status.MPI_SOURCE << " and entering into critical section" <<"\n";
				MPI_Recv(&Queue , size+1 , MPI_INT , status.MPI_SOURCE , rank , MPI_COMM_WORLD,&status);	
				MPI_Recv(&LN , size+1 , MPI_INT , status.MPI_SOURCE , rank , MPI_COMM_WORLD,&status);	
				state=granted;
			}
			MPI_Barrier(MPI_COMM_WORLD);

	//This will update the time which acts as a countdown for the process	
	stime = time(NULL);

	}

	// This loop will help to process the remaining requests in the queue
	completeReq = 100;
	for(int y=0;y<completeReq;y++)
	{
		if(state == granted)
		{

			cout << "Rank " << rank << " entering into critical section" << "\n";
			cout << "Rank " << rank << " entering into critical section" << "\n";
			LN[rank] = RN[rank];
			state = released;
			nxtProcess = Queue[0];
			for(int x=0;x< size;x++)
			{
				Queue[x] = Queue[x+1];

			}
			if(nxtProcess >=0 && nxtProcess != rank)
			{
				cout<<"Rank " << rank << " is sending the token to rank " << nxtProcess << "\n";
				MPI_Send(&Queue , size+1 , MPI_INT , nxtProcess , nxtProcess , MPI_COMM_WORLD);
				MPI_Send(&LN , size+1 , MPI_INT , nxtProcess , nxtProcess , MPI_COMM_WORLD);
			}

		}
		MPI_Barrier(MPI_COMM_WORLD);
		probeFlag = 0;
		MPI_Iprobe(MPI_ANY_SOURCE , rank , MPI_COMM_WORLD , &probeFlag , &status);
		if(probeFlag == 1 && status.MPI_SOURCE != rank && status.MPI_SOURCE >=0 && status.MPI_SOURCE<size)
		{
			cout << "Rank " << rank << " has received the token from rank " << status.MPI_SOURCE << " and entering into critical section" <<"\n";
			MPI_Recv(&Queue , size+1 , MPI_INT , status.MPI_SOURCE , rank , MPI_COMM_WORLD,&status);	
			MPI_Recv(&LN , size+1 , MPI_INT , status.MPI_SOURCE , rank , MPI_COMM_WORLD,&status);	
			state=granted;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

