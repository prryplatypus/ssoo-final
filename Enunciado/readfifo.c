#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LINE 100

int main()
{
	char line[MAX_LINE];	
	int pipe;

	if ((pipe = open("/tmp/myFIFO",O_RDONLY)) < 0)
	{
		printf("ERROR: Imposible abrir el pipe FIFOTLB\n");
		return -1;       
	}
	while(read(pipe, line, MAX_LINE))
		// Print the read string and close
	    printf("\nHe leido: %s\n", line);
	close(pipe);	
	
	return 0;
}

