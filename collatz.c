#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	int i;
	printf("Enter the integer:");
	scanf("%d",&i);

	while(i<0) /* Constrain that runs until we input a positive value */
	{
		printf("Enter a positive integer:");
		scanf("%d",&i);
	}
	/* fork a child process */
	pid = fork ();
	if (pid < 0) /* error occurred */
	{ 
	fprintf(stderr, "Fork Failed");
	return 1;
	}
	else if (pid == 0) /* child process */
	{
		//execlp ("/bin/ls","ls",NULL); 
		while(i!=1)
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
