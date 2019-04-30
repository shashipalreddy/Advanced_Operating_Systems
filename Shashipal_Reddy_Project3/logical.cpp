//*********************************************************************************************************************
// SHASHIPAL REDDY PINGILI
// Advanced Operating Systems
// Programming Project #3 Calculate the lamport's logical clock based on the input file provided
// Due date: 04-18-2019 11:59p.m CST 
// Instructor: Dr. Mohammed Yassine Belkhouche
// File name: logical.cpp
//*********************************************************************************************************************
#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;
// mpecc -mpicc=mpic++ -mpilog logical.cpp -o logical
// mpirun -np 7 logical logical.txt

//*************************************************************************************************************
//
// Main Function
//
// Only main function is used in this program. No other functions are used. 
// This program is used to calculate the lamport's logical clock for the processes by executing the instructions given in the input file
// The root processes (rank 0) receives the input from the command line. This input specifies the file name from where the instructions need to be read
// Input file contains the number of processes used in this program. Total number of processes are sum of number of processes mentioned in the file and root process 
// Two types of instructions used are exec and send. For exec in a certain process will increase the clock of that particular process by 1. 
// If its a send statement then the sender clock will be increased by one and the receiver clock will be increased by 1 to the maximum clock of sender or receiver.
//
//
// Input Parameter
// --------------------
//
// argv     char			This parameter recives the file name as input through command line argument.
//
//***************************************************************************************************************

int main(int argc, char *argv[]) {
	int rank,size,isFirstWord=0,i=0,clock=0,ack=1,clk=0,nop=0;
	int execTag=0,instListTag=1,instProcListTag = 2,instListSizeTag=3,msgTag=4,clkTag=5;
	
	char buffer[2];
	
	char instList[100],execchar[2],srchar[2],mess[100];
	int instProcList[100],instListSize=0;
	vector<string> msg;
	//vector<int> execListVector;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	string word;
	ifstream fin;
	ifstream fnop;
	fin.open(argv[1], ios::in);
	fnop.open(argv[1], ios::in);
	while (fnop >> word)
    {
		
    	if (isFirstWord == 0)
    	{
			//reads the number of process from the file and will incremented by 1 for root process
			std::stringstream convert;
			convert << word;
			convert >> nop;
			nop = nop + 1;
			
		}
		
		isFirstWord = 1;
	}
	
	
	if(rank == 0)
	{
		cout << "There are " << nop <<" processes in the system" <<"\n";
		int execDest;
		char destinchar[2];
		while (!fin.eof())
		{
			
			string line("");
			//each line of the file will be read and will be stored in an array.
			//message vector will be maintained for each statements
			//below lines classify the statements as exec or send
			if (getline(fin, line))
			{
				
				if (line.find("exec") != std::string::npos) 
				{
					//for execution statement it will just maintian the process number
					strcpy(execchar , line.substr(5).c_str());
					instList[instListSize] = 'e';
					instProcList[instListSize] = atoi(execchar);
					instListSize = instListSize + 1;
					msg.push_back("NA");
				}
				else if (line.find("send") != std::string::npos) 
				{
					string arr[10];
					int i = 0;
					stringstream ssin(line);
					//for send statement it will maintian the process number for both send and receive and also maintains the message which needs to be send
					while (ssin.good() && i < 4){
						ssin >> arr[i];
						++i;
					}
					
					instList[instListSize] = 's';
					strcpy(srchar , arr[1].c_str());					
					instProcList[instListSize] = atoi(srchar);
					msg.push_back(line.substr((line.find('\"')+1), ((line.length())- (line.find('\"')+2))));
					instListSize = instListSize + 1;				
					
					instList[instListSize] = 'r';
					strcpy(srchar , arr[2].c_str());					
					instProcList[instListSize] = atoi(srchar);
					instListSize = instListSize + 1;
					msg.push_back("NA");
					
				}

			}

			
		}	
		
		for(int i=1;i<nop;i++)
		{
				//Each instructions and the process involved in that instructions are maintained and will be send to all the processes.
				MPI_Send(&instList,100,MPI_CHAR,i,instListTag,MPI_COMM_WORLD);
				MPI_Send(&instProcList,100,MPI_INT,i,instProcListTag,MPI_COMM_WORLD);
				MPI_Send(&instListSize,1,MPI_INT,i,instListSizeTag,MPI_COMM_WORLD);
		}
		
		for(int i=0;i<instListSize;i++)
		{
			if(instList[i] == 'e')
			{
				//Root process communicates with the process which needs to perform execution
				MPI_Send(&buffer,2,MPI_CHAR,instProcList [i],execTag,MPI_COMM_WORLD);
				
			}
			else if(instList[i] == 's')
			{
				//Root process communicates with the process which needs to send message
				string message = msg[i];
				strcpy(mess , message.c_str());
				MPI_Send(&mess,100,MPI_CHAR,instProcList[i],msgTag,MPI_COMM_WORLD);
			}
			//
			
		}
				
	}	
	
	
	//Each instructions and the process involved in that instructions are received by all processes.
	if(rank > 0)
	{
		MPI_Recv(&instList,100,MPI_CHAR,0,1,MPI_COMM_WORLD,&status);
		MPI_Recv(&instProcList,100,MPI_INT,0,2,MPI_COMM_WORLD,&status);
		MPI_Recv(&instListSize,1,MPI_INT,0,3,MPI_COMM_WORLD,&status);
	}
	
		
	for(int i=0;i<instListSize;i++)
	{	
		if (rank > 0) 
		{
			if(instList[i] == 'e')
			{
				if (instProcList[i] == rank)
				{
					//If the process receives instruction for execution it will execute and increases its clock by 1
					MPI_Recv(&buffer,2,MPI_CHAR,0,execTag,MPI_COMM_WORLD,&status);
					clock = clock + 1;
					cout << "Execution event in process " << rank << "\n";
					cout << "Logical time at process  " << rank << " is " << clock << "\n";
					
				}
				
				
				
			}
			else if(instList[i] == 's')
			{
				if (instProcList[i] == rank)
				{
					//If the process receives instruction for send it will send the message and increases its clock by 1. It also sends its clock to the receiver process.
					clock = clock+1;
					clk = clock;
					MPI_Recv(&mess,100,MPI_CHAR,0,msgTag,MPI_COMM_WORLD,&status);
					MPI_Send(&mess,100,MPI_CHAR,instProcList[i+1],msgTag,MPI_COMM_WORLD);
					MPI_Send(&clk,1,MPI_INT,instProcList[i+1],clkTag,MPI_COMM_WORLD);
					cout<<"Message sent from process "<< rank <<" to process " << instProcList[i+1] <<": " <<mess <<"\n";
					cout << "Logical time at process  " << rank << " is " << clock << "\n";
				}
				
			}
			else if(instList[i] == 'r')
			{
				if (instProcList[i] == rank)
				{
					//If the process receives instruction to receive it will receive the message and increases its clock by 1 to the maximun of sender or receiver. 
					MPI_Recv(&mess,100,MPI_CHAR,instProcList[i-1],msgTag,MPI_COMM_WORLD,&status);
					MPI_Recv(&clk,1,MPI_INT,instProcList[i-1],clkTag,MPI_COMM_WORLD,&status);
					if(clk <= clock)
					{
						clock = clock + 1;	
					}
					else
					{
						clock = clk + 1;	
					}
					
					cout<<"Message received from process "<< instProcList[i-1] <<" by process " << rank <<": " <<mess <<"\n";
					cout << "Logical time at process  " << rank << " is " << clock << "\n";
				}
				
			}
			
			
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	//Prints the clock of each process in ascending order
	for(int i=0;i<instListSize;i++)
	{
		if((rank > 0) && rank == i)
		{
			
			cout << "Logical time at process " << rank << " at the end of all execution is " << clock << "\n";
			
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
		
	
	
	MPI_Finalize();
}

