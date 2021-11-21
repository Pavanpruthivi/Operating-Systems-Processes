#include <stdio.h>
#include <unistd.h> // Provides access to POSIX OS API
#include <sys/types.h> // Includes definitions of Process ids
#include <sys/wait.h> // Contains wait and other similar calls 

int main()
{
	pid_t pid; // process ids
	int i;
	printf("Enter the integer:");
	scanf("%d",&i);

	while(i<0) /* Constrain that runs until we input a positive value */
	{
		printf("Enter a positive integer:");
		scanf("%d",&i);
	}
	/* fork a child process */
	pid = fork (); // Forking to create a child process
	if (pid < 0) /* error occurred */
	{ 
	fprintf(stderr, "Fork Failed");
	return 1;
	}
	else if (pid == 0) /* child process */
	{
		while(i!=1)  // Performing calculations for Collatz conjecture and printing all values
		{
			printf("%d,",i);
			if (i%2==0)
				i/=2;
			else
				i= (3*i) +1;
		}
	printf("%d",i); 
	}

	else /* parent process */
	{ 
		
		wait(NULL); /* parent will wait for the child to complete */
		printf("\nChild Complete\n");
	}
	return 0;
}
