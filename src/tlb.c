/**
 * Práctica Final Sist. Operativos 19-20
 * Raúl Sancho, Javier Martínez y Néstor Pérez
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "global.h"

int main(int argc,char *argv[])
{	
	int pipe;
	while ((pipe = open(PIPE, O_RDONLY)) < 0); // Wait for pipe to open
	
	unsigned line;	
	while(read(pipe, &line, sizeof(line)))
	{
	    printf("He leido: %04X\n", line);
	}
	    
	close(pipe);
	
	return 0;
}
