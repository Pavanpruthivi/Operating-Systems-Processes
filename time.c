#include <stdio.h>
#include <sys/time.h> //Defines time interval structure
#include <stdlib.h>  // Defines varieties of functions for memory allocation, process control
#include <sys/types.h> // Includes definitions of Process ids
#include <unistd.h> // Provides access to POSIX OS API
#include <string.h> // Contains string operation functions
#include <sys/wait.h> // Contains wait and other similar calls

int main(int argc, char *argv[]) // main function is defined with arguement count and aruguement vector for taking arguements from the command line
{
	struct timeval start, end; // structure of time containing seconds, milliseconds, day, daytime defined from Jan 1st 1970
	int fd[2];  // File descriptor for read-end fd[0] and write-end of the pipe fd[1]
	pipe(fd); // Establishing pipe connection between two process for one way communication
	int pid =fork(); // Forking to create a child process
	
	
	if (pid<0) 
	{
		perror("Unable to fork\n");  // Print error message as unable to fork a process
		exit(1);
	}
	
	
	else if(pid==0)  // Child Process
	{
		close (fd[0]); // Closing the unused end (Here it isw child process)
		gettimeofday(&start, NULL);  // Noting the time before the execution of commands
		char buffer[100]; //Creating a memory space
		sprintf(buffer,"%ld %ld", start.tv_sec, start.tv_usec); // Writing to the buffer
		printf("Child: %ld\n", start.tv_usec); 
		write(fd[1], buffer,strlen(buffer) +1); // Writing the pipe
		execvp(argv[1], argv +1);  // Executing the commands from the command line
		close(fd[1]);
	}
	
	
	else  // Parent Process
	{
		double timetaken;
		long int startsec, startusec;
		close(fd[1]);  // Closing the unused end (Here it is the parent process)
		//char buffer[100]; 
		dup2(fd[0],0); //This duplicates the file descriptor to the write end of the pipe
		//read(fd[0], buffer,100); // Reading from the pipe
		scanf("%ld",&startsec);
		scanf("%ld",&startusec);
		//printf("%ld %ld",startsec, startusec);
		printf("Parent:%ld\n",startusec);
		gettimeofday(&end,NULL); //Noting the time after execution of commands
		timetaken = (end.tv_sec-startsec)*1e6;  // Calculating the time difference
		timetaken = timetaken + (end.tv_usec-startusec)*1e-6;
		close(fd[0]); // Closing the read end of the pipe
		//printf("Parent:%ld\n",end.tv_usec); 
		printf("Elapsed time:%0.6lf sec\n", timetaken); // Printing the elapsed time
		
		wait(NULL); // Wait call for the parent until the child completes its process
	}
}
