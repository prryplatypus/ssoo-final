/**
 * Práctica Final Sist. Operativos 19-20
 * Raúl Sancho, Javier Martínez y Néstor Pérez
 */
#define _GNU_SOURCE // Required to use getline()

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "global.h"

#define MAX_LINE 4

int main(int argc,char *argv[])
{
	int pid = fork();

	if (pid == 0) { // Child process
		printf("I am the child process. My PID is %d\n", pid);
		char* arg[] = { "tlb", NULL };
		execv("./tlb", arg);
	}
	else {
		int pipe;
		mkfifo(PIPE, 0666);
		if ((pipe = open(PIPE, O_WRONLY)) < 0)
		{
			printf("Error opening pipe\n");
			return -1;
		}
		
		FILE *fp = fopen(FILENAME, "r");
		if (fp == NULL){
			printf("Could not open file %s", FILENAME);
			return 1;
		}
		
		char line[MEMADDR_SIZ + 1];
		while (fgets(line, MEMADDR_SIZ + 1, fp) != NULL){
			printf("Sent %04X\n", *line);
			write(pipe, line, strlen(line));
			fgets(line, 2, fp);
			sleep(2);
		}

		fclose(fp);
		
		
		/*char line[MEMADDR_SIZ];
		for (int i=10;i<20;i++)
		{
			sprintf(line,"%04X",i);
			write(pipe, line, strlen(line));
			sleep(2);
		}*/
		
		close(pipe);
		waitpid(pid, NULL, 0); // Wait for child process to finish (blocks this one)
	}
	
	return 0;
}
