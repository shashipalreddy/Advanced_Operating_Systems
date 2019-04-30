The objective of this project is to compare MPI scatter, gather and allgather functions with your
own scatter, gather and allgather functions. Write three different programs, these programs perform the
followings:
1. Program 1: Implements you own scatter(myscatter) function.
2. Program 2: Implements you own gather(mygatter) function.
3. Program 2: Implements you own allgather(myallgatter) function.
Within each program, collect the time it takes to execute both MPI functions and your functions. Run
the program with different number of processes: 4, 6, 8, 10, 12, 14, 16, 18, 20, 22 and 24. Plot the graph
(time vs # of processes) for each program.
Submission:
Submit a zip file containing:
1. A pdf file containing a detailed description of the design and the implementation of your functions,
the jumpshots(only for experiment with 6 processes and 16 processes) and the graph (time,#of
processes) for each program..
2. Your C/C++ code.

COSC6352: Advanced Operating Systems
Fall 2019, Project #2
Date Assigned: Wednesday, March 27, 2019
Due Date: Friday, April 05, 2019 at 11:59 p.m.
(Total points: 100)
Project description
Write a C or C++ program named berkeley.c or berkeley.cpp (executable name should be berkeley)
to implement the Berkeley time synchronization algorithm as discussed in the class using MPI for
communication. The screenshot from jumpshot should be saved as berkeley.pdf. Read the values for
the clocks at each processor and the coordinator processor from an input file specified as a command
line argument.
Format of the input file
coordinator process rank
time for process with rank 0
time for process with rank 1
time for process with rank 2
.
.
.
.
time for process with rank n
integer value ( an abs ( value ) which deviated by more than this value is ignored )
Example file named berkeley.txt. The content in brackets is not part of the input file.
3 ( coordinator process )
16:20:11 ( time at rank 0)
13:58:23 ( time at rank 1)
14:12:01 ( time at rank 2)
09:48:24 ( time at rank 3)
02:12:34 ( Ignore if the time difference between coordinator and another process is
more than abs (2:12))
Sample Execution: mpirun -np 4 berkeley berkeley.txt
Program output
First print the rank of the process performing the coordinator using a printf() statement like this:
printf ( "I am process with rank %d acting as the coordinator process " , rank );
The coordinator process should then broadcast it’s time read from the file to all other processes. When
the processes receives the time print a message. Use printf() statements like these:
printf ( " Coordinator process is sending time % d\n " , rank );
printf ( " Process % d has received time % s\ n" , rank , time );
Once the processes receive the time they should calculate the differential of their clocks and send it back
to the coordinator process. Use printf() statements like these:
1
printf ( " Process % d is sending time differential value of % d to process % d \ n " , rank , diff , rank );
printf ( " Process % d has received time differential value of % d \ n" , rank , diff );
If the coordinator ignores any values use printf() statements to output the information.
printf ( " Coordinator Process is ignoring time differential value of %d from process %d \n" , diff , rank );
After the coordinator process calculates the average of the time differential’s display it using a printf()
statement
printf ( " Time differential average is % f " , average );
Calculate the amount by which each clock’s value should be adjusted, and send it to the appropriate
processor. Once the processors receive the value adjust the local clocks and display their local time. Use
printf() statements like these to clearly show the process.
printf ( " Coordinator process is sending the clock adjustment value of % f to process %d , value , rank );
printf ( Process % d has received the clock adjustment value of %f , rank , value );
printf ( " Adjusted local time at process % d is % s " , rank , newtime );
Important:
1. Your MPI program will read the file name as command line argument and write the output to
standard output.
2. The only MPI functions you are permitted to use are MPI_Init(), MPI_Finalize(), MPI_Comm_rank(),
MPI_Comm_size(), MPI_Send(), MPI_Recv(), MPI_Bcast() and MPI_Barrier()
Submission:
Submit a zip file containing:
1. Write your name on all files you submit.
2. Document your code.
3. Your C/C++ implementation of the Berkeley algorithm.
4. The jumpshots saved in pdf file.


COSC 6352, Advanced Operating Systems
Spring 2019, Programing Assignment #3
Date Assigned: Monday, April 08, 2019
Due Date: Wednesday, April 17, 2019 at 11:59 p.m. (use blackboard)
DESCRIPTION
PART 1 - LOGICAL CLOCKS
DESCRIPTION
Write a C or C++ program named logical.c or logical.cpp (executable name should be logical) to
implement Lamport's logical clocks using MPI. The screenshot from jumpshot should be saved
as logical.pdf. You will implement a set of primitives to maintain the state of logical clocks. To
show that your logical clocks work correctly, your program should read in a set of simulation
parameters and then print the logical clocks associated with each event. It is recommended that a
single manager process and a set of worker processes be implemented. The manager process will
manage the simulation, and the worker processes will exchange messages and implement the
logical clocks.
INPUT TO THE PROGRAM
The input will be in the following format:
number of processes N
event type pid
event type pid1 pid2 msg
...
end
Process IDs in the input file will range from 1 to N (You need N+1 processors to run this
simulation). There will be exactly one input event per line. There are two event types that may
appear in the input: exec and send. An exec event has no second argument, and indicates that an
execution event has taken place in a process. A send event denotes the sending of a message
from one process to another. The message will be a string of printable ASCII characters
delimited by quotes. The quotes should not be stored as part of the string.
Example file named logical.txt:
3
exec 1
send 1 2 "silly message"
end
Sample Execution:
mpirun -np 4 logical logical.txt
PROGRAM OUTPUT
For execution events, print that the event has occurred and the logical timestamp at the process.
For a send event, print the relevant information at both the sender and receiver. After each event,
you should output any logical clocks updated by the event, and only those. At the end of the run,
print the logical clocks of all processes in the system in ascending rank order.
At the beginning of the simulation, output the number of processes as following:
There are N processes in the system
For each event, you should use the equivalent of the following printf() statements:
printf( "Execution event in process %d\n", pid );
printf( "Message sent from process %d to process %d: %s\n", pid1, pid2, msg );
printf( "Message received from process %d by process %d: %s\n", pid1, pid2, msg );
For any logical clock printed, use the following statement (or similar):
printf( "Logical time at process %d is %d\n", pid, logical_time );
MISCELLANEOUS
1. Start all clocks at time 0.
2. The clock delta, d, should be 1 for all processors.
3. Your MPI program will read the file name as command line argument and write the output to
standard output.
4. The only MPI functions you are permitted to use are MPI_Init(), MPI_Finalize(),
MPI_Comm_rank(), MPI_Comm_size(), MPI_Send(), MPI_Recv(), and MPI_Barrier().
5. You may use the MPI_Probe function if needed.
6. You may use an acknowledgement message to ensure that the current action (e.g. a message
being sent between slave processes) has concluded before the master reads the next action.
PART 2 - VECTOR CLOCKS
DESCRIPTION
Write a C or C++ program named vector.c or vector.cpp (executable name should be aosproj2_1)
to implement vector clocks using MPI. The screenshot from jumpshot should be saved as
vector.pdf. You will implement a set of primitives to maintain the state of vector clocks. To
show that your vector clocks work correctly, your program should read in a set of simulation
parameters and then print the vector clocks associated with each event. It is recommended that a
single manager process and a set of worker processes be implemented. The manager process will 
manage the simulation, and the worker processes will exchange messages and implement the
vector clocks.
INPUT TO THE PROGRAM
The input will be in the following format:
number of processes N
event type pid
event type pid1 pid2 msg
...
end
Process IDs in the input file will range from 1 to N (You need N+1 processors to run this
simulation). There will be exactly one input event per line. There are two event types that may
appear in the input: exec and send. An exec event has no second argument, and indicates that an
execution event has taken place in a process. A send event denotes the sending of a message
from one process to another. The message will be a string of printable ASCII characters
delimited by quotes. The quotes should not be stored as part of the string.
Example file named vector.txt:
3
exec 1
send 1 2 "silly message"
end
Sample Execution:
mpirun -np 4 vector vector.txt
PROGRAM OUTPUT
For execution events, print that the event has occurred and the vector timestamp at the process.
For a send event, print the relevant information at both the sender and receiver. After each event,
you should output any vector clocks updated by the event, and only those. At the end of the run,
print the vector clocks of all processes in the system in ascending rank order.
At the beginning of the simulation, output the number of processes as following:
There are N processes in the system
For each event, you should use the equivalent of the following printf() statements:
printf( "Execution event in process %d\n", pid );
printf( "Message sent from process %d to process %d: %s\n", pid1, pid2, msg );
printf( "Message received from process %d by process %d: %s\n", pid1, pid2, msg );
For any vector clock printed, use the following statement (or similar):
printf( "Vector time at process %d is %d\n", pid, vector_time );
MISCELLANEOUS
1. Start all clocks at time (0,0,....0).
2. The clock delta, d, should be 1 for all processors.
3. Your MPI program will read the file name as command line argument and write the output to
standard output.
4. The only MPI functions you are permitted to use are MPI_Init(), MPI_Finalize(),
MPI_Comm_rank(), MPI_Comm_size(), MPI_Send(), MPI_Recv(), and MPI_Barrier().
5. You may use the MPI_Probe function if needed.
6. You may use an acknowledgement message to ensure that the current action (e.g. a message
being sent between slave processes) has concluded before the master reads the next action.


COSC 6352, Advanced Operating Systems
Spring 2019, Programing Assignment #4
Date Assigned: Wednesday, April 17, 2019
Due Date: Monday, April 29, 2019 at 11:59 p.m. (use blackboard)
DESCRIPTION
Write a C or C++ program named suzuki.c or suzuki.cpp (executable name should be suzuki) to
implement Suzuki and Kasami’s Broadcast Algorithm using MPI. The screenshot from
jumpshot should be saved as suzuki.pdf.
Each process records its state of being outside the critical section (RELEASED), requesting entry
(REQUESTED) or being in the critical section (GRANTED). A process can randomly request
entry to critical section after waiting for some time, if it has not already requested one or is not
currently in the critical section.
In order to get access to the critical section a process must send (N - 1) requests and wait for (N1) reply if it is not already holding the token. At the start of the simulation randomly assign the
token to any process.
When a process is requesting entry to the critical section, it defers processing requests from other
processes until its own requests have been sent to all (N-1) processes.
On every process maintain a data structure which contains the largest sequence number for each
thread received (RN vector). The token comprises of a data structure (LN vector) which contains
the sequence number of the latest executed request from a thread i and another data structure for
maintaining a queue (Q) of requesting threads.
INPUT TO THE PROGRAM
The input to the program is the number of processes, the simulation time and the maximum amount
of time a process waits before requesting access to critical section. The number of processes should
be specified as an argument to your program.
PROGRAM OUTPUT
mpirun –np 4 aosproj3 30 5
• The number of processes requested is 4 (maximum 8 processes). 
• The simulation time is 30 seconds (minimum 10 seconds), after which no process can
request access to critical section.
• The maximum wait time for a process is 5 seconds (Therefore a process can randomly
wait between 0 to 5 seconds before requesting access to critical section again).
Process with rank 1 and sequence number 1 is requesting critical
section
Broadcast message (1:1)
Rank 2 received critical section request from rank 1
Rank 3 received critical section request from rank 1
Rank 0 received critical section request from rank 1
Rank 3 is sending the token to rank 1
Process with rank 2 and sequence number 1 is requesting critical
section
Broadcast message (2:1)
Rank 1 has received the token from Rank 3 and entering into
critical section
Rank 3 received critical section request from rank 2
Rank 0 received critical section request from rank 2
Rank 1 has exited critical section
Rank 1 received critical section request from rank 2
Rank 1 is sending the token to rank 2
Rank 2 has received the token from Rank 1 and entering into
critical section
Rank 2 has exited critical section
.
.
.
.
. so on until the simulation time ends