#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h> //Defines time interval structure
#include <stdlib.h>  // Defines varieties of functions for memory allocation, process control
#include <sys/types.h> // Includes definitions of Process ids
#include <unistd.h> // Provides access to POSIX OS API
#include <string.h> // Contains string operation functions
#include <sys/wait.h> // Contains wait and other similar calls 


int main(int argc, char *argv[]) // main function is defined with arguement count and aruguement vector for taking arguements from the command line
{
	struct timeval start, end; // structure of time containing seconds, milliseconds, day, daytime defined from Jan 1st 1970
	const int SIZE = 4096; //The size (in bytes) of shared memory object 
	const char *name = "OS"; /* name of the shared memory object */
	int fd; /* shared memory file descriptor */
	long int *ptr; /* pointer to shared memory obect */
	// strings written to shared memory

	int pid =fork(); // Forking to create a child process
	
	
	if (pid<0) 
	{
		perror("Unable to fork\n");  // Print error message as unable to fork a process
		exit(1);
	}

else if(pid==0)  // Child Process
	{
		//close (fd[0]); // Closing the unused end (Here it isw child process)
		gettimeofday(&start, NULL);  // Noting the time before the execution of commands
		/* create the shared memory object */
		fd = shm_open (name,O_CREAT | O_RDWR, 0666);
		/* configure the size of the shared memory object */
		ftruncate(fd, SIZE);
		/* memory map the shared memory object */
		ptr = (long int *)
		mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		/* write to the shared memory object */
		execvp(argv[1], argv +1);  // Executing the commands from the command line
		
		sprintf(*ptr,"%ld", start.tv_sec);
		ptr++;
		sprintf(*ptr,"%ld",start.tv_usec);
		ptr++;

	}
	
else  // Parent Process
	{
		double timetaken;
		long int startsec, startusec;
		fd = shm_open (name, O_RDONLY, 0666);
		/* memory map the shared memory object */
		ptr = (long int *)
		mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		/* read from the shared memory object */
		startsec = (*ptr);
		ptr++;
		startusec = (*ptr);
		gettimeofday(&end,NULL); //Noting the time after execution of commands
		timetaken = (end.tv_sec-startsec)*1e6;  // Calculating the time difference
		timetaken = timetaken + (end.tv_usec-startusec)*1e-6;
		shm_unlink (name); /* remove the shared memory object */
		printf("Elapsed time:%0.6lf sec\n", timetaken); // Printing the elapsed time
		wait(NULL); // Wait call for the parent until the child completes its process
	}




	return 0;
}
