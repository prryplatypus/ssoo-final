#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LINE 100

int main(){
	int pipe;
	char line[MAX_LINE];
	
    mkfifo("/tmp/myFIFO", 0666);          // Crea el pipe si no existe
	
    pipe = open("/tmp/myFIFO", O_WRONLY); // abre el pipe en escritura
   
    for (int i=10;i<20;i++)
    {
	   sprintf(line,"%04X",i);
	   write(pipe, line, strlen(line));
	   sleep(2);
	}
   close(pipe);
   return(0);
}   

