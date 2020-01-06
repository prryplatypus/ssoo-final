/**
 * Práctica Final Sist. Operativos 19-20
 * Raúl Sancho, Javier Martínez y Néstor Pérez
 */
#define _GNU_SOURCE // Removes warning for kill();

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "global.h"

int main(int argc,char *argv[])
{
	pid_t pid = fork();

	if (pid == 0) { // Child process
		char* arg[] = { "tlb", NULL };
		execv("./tlb", arg);
	}
	else { // Parent process
		printf("kerneltyr, mi pid es %d y la de la tlb es %d\n", getpid(), pid);
		
		// Pipe init
		int pipe;
		mkfifo(PIPE, 0666);
		if ((pipe = open(PIPE, O_WRONLY)) < 0)
		{
			printf("Error opening pipe\n");
			return -1;
		}
		
		// File init
		FILE *fp = fopen(FILENAME, "r");
		if (fp == NULL){
			printf("Could not open file %s\n", FILENAME);
			return -1;
		}
		
		// Only allocate memory for this variables if pipe and file opened successfully
		unsigned to_send;
		unsigned read_size = (MEMADDR_SIZ + 3) * sizeof(char); // Line num + '\0' + '\n'
		char *line = (char *)malloc(read_size);
		
		while (fgets(line, read_size, fp) != NULL){
			to_send = (unsigned)strtol(line, NULL, 16); // Convert hex string to its hex value
			write(pipe, &to_send, sizeof(to_send));
			sleep(2);
		}
		
		fclose(fp);
		close(pipe);
		
		kill(pid, SIGUSR2); // Sends SIGUSR2 signal to child
		wait(NULL); // Wait for child process to finish (blocks this one)
	}
	
	return 0;
}
