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
#include <signal.h>
#include <string.h>

#include "global.h"

typedef struct{
	short int pagina;
	short int marco;
	short int valida;
	int tiempo;
} T_TLB;

void printTLB( T_TLB *tlb) { // TEMPORAL
	int i;
	printf("El estado de la tlb es:\n");
	for(i = 0; i < TLB_SIZE; ++i)
	{
		printf("\tp:%X ", tlb[i].pagina);
		printf("\tm:%X ", tlb[i].marco);
		printf("\tv:%d ", tlb[i].valida);
		printf("\tt:%d ", tlb[i].tiempo);
		printf("\n");
	}
	printf("\n");
}

int comprobarValida( T_TLB*tlb, int pagina) {
	int i;
	for(i = 0; i < TLB_SIZE; ++i){
			if(pagina == tlb[i].pagina && tlb[i].valida == 1){
				return 1;
			}
	}
	return 0;
}

void sig_handler(int signo)
{
  if (signo == SIGUSR2)
    printf("Recibida SIGUSR2, fin del proceso tlb\n");
    exit(0);
}

int main()
{
	T_TLB tlb[TLB_SIZE];
	int i;
	int pipe;
	int pagina;
	int offset;
	
	unsigned tiempoGlobal = 0;
	unsigned numFallos = 0;
	
	unsigned line;	

	signal(SIGUSR2, sig_handler);
	
	for(i = 0; i < TLB_SIZE; ++i)
	{	// Se rellena la tlb con valores por defecto
		tlb[i].pagina = 255;
		tlb[i].marco = (2+i) % TLB_SIZE;
		tlb[i].valida = 0;
		tlb[i].tiempo = 0;
	}

	if( (pipe = open(PIPE, O_RDONLY)) < 0 )
	{
		printf("No se ha podido abrir...\n");
		return -1;
	}

	// Imprime el estado inicial de la tlb
	printTLB(tlb);

	while(read(pipe, &line, sizeof(line)))
	{
		pagina  = ((line & 0xF000) >> 12);
		offset = ((line) & 0x0FFF);
		
		tiempoGlobal++;
		
		int valida = comprobarValida(tlb, pagina);
		
		if(valida == 0) // == No hay traducción válida
		{
			numFallos++;
			printf("%d, Fallo de TLB %X, VADDR %04X pagina %X offset %04X\n", tiempoGlobal, numFallos, line, pagina, offset);

			for(i = 0; i < TLB_SIZE; ++i)
			{
				if( tlb[i].valida == 0)
				{
					tlb[i].pagina = pagina;
					tlb[i].tiempo = tiempoGlobal;
					tlb[i].valida = 1;
					tiempoGlobal++;
					valida = comprobarValida(tlb, pagina);
					break;
				}
				
			}
			
			if(i == TLB_SIZE && valida == 0){
				int j;
				valida = 1;
				int tiempoMenor = tlb[0].tiempo, tlbMenor = 0;
				
				for(j = 0; j < TLB_SIZE; ++j){
					if(tlb[j].tiempo < tiempoMenor){
						tiempoMenor = tlb[j].tiempo;
						tlbMenor = j;
					}
				}
				printf("%d, Expulsada pagina %04X\n", tiempoGlobal, tlb[tlbMenor].pagina);
				i = tlbMenor;
				tlb[tlbMenor].pagina = (short)pagina;
				tlb[tlbMenor].tiempo = tiempoGlobal;

				tiempoGlobal++;
			}
		
		
		}
			// Es válida
			printf("%d, Acierto de TLB, VADDR %04X pagina %X offset %04X marco %X => PHYSADDR %01X%03X\n", tiempoGlobal, line, pagina, offset, tlb[i].marco, tlb[i].marco, offset);
			tlb[i].tiempo = tiempoGlobal;

			printTLB(tlb);
	}
	printf("TLB ya\n");
	close(pipe);
	return 0;
}
