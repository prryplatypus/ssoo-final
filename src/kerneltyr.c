/**
 * Práctica Final Sist. Operativos 19-20
 * Raúl Sancho, Javier Martínez y Néstor Pérez
 */

#include <fcntl.h>
#include <stdio.h> // DEBUG
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 100

int main(int argc,char *argv[])
{
	int pid = fork();

	if (pid == 0) // Child process
	{
		printf("I am the child process. My PID is %d\n", pid);
		char* arg[] = { "tlb", NULL };
		execv("./tlb", arg);
	}
	else // Parent process
	{
		char line[MAX_LINE];
	
		// Create and open pipe (blocking until read)
		mkfifo("/tmp/myFIFO", 0666);
		int pipe = open("/tmp/myFIFO", O_WRONLY);
		
		for (int i=10; i<20; ++i)
		{
			sprintf(line,"%04X",i);
			write(pipe, line, strlen(line));
		}
		
		close(pipe);
		waitpid(pid, NULL, 0); // Wait for child process to finish (blocks this one)
	}
	
	return 0;
}
