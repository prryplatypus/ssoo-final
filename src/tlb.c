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

typedef struct{
	short int pagina;
	short int marco;
	short int valida;
	int tiempo;
} T_TLB;

void printTLB( T_TLB *tlb) { // TEMPORAL
	int i;
	printf("El estado de la tlb es:\n");
	for(i = 0; i < 4; ++i)
	{
		printf("\tp:%X ", tlb[i].pagina);
		printf("\tm:%X ", tlb[i].marco);
		printf("\tv:%d ", tlb[i].valida);
		printf("\tt:%d ", tlb[i].tiempo);
		printf("\n");
	}
}

int comprobarValida( T_TLB*tlb, int pagina) {
	int i;
	for(i = 0; i < 4; ++i){
			if(pagina == tlb[i].pagina && tlb[i].valida == 1){
				return 1;
			}
	}
	return 0;
}

// Falta lo de armar la SIGUSR2

int main()
{
	T_TLB tlb[4];
	int i;
	int pipe;
	int pagina;
	int offset;
	
	int tiempoGlobal = 0;
	int numFallos = 0;
	
	unsigned line;	
	
	for(i = 0; i < 4; ++i)
	{	// Se rellena la tlb con valores por defecto
		tlb[i].pagina = 255;
		tlb[i].marco = (2+i) % 4;
		tlb[i].valida = 0;
		tlb[i].tiempo = 0;
	}

	if( (pipe = open(PIPE, O_RDONLY)) < 0 )
	{
		printf("Sa matao...\n");
		return -1;
	}
	while(read(pipe, &line, MEMADDR_SIZ))
	{
		
		printTLB(tlb);
		
		printf("Estoy leyendo %04X\n", line);
		pagina  = ((line & 0xF000) >> 12);
		offset = ((line) & 0x0FFF);
		printf("La pagina es %X y el offset es %03X\n", pagina, offset);
		
		tiempoGlobal++;
		
		int valida = comprobarValida(tlb, pagina);
		
		if(valida == 0) // == No hay traduccion valida
		{
			numFallos++;
			for(i = 0; i < 4; ++i)
			{
				if( tlb[i].valida == 0)
				{
					tlb[i].pagina = pagina;
					tlb[i].tiempo = tiempoGlobal;
					tlb[i].valida = 1;
					printf("%X, Fallo de TLB %X, VADDR %04X pagina %X offset %03X\n", tiempoGlobal, numFallos, line, pagina, offset);
					tiempoGlobal++;
					comprobarValida(tlb, pagina);
					break;
				}
				
			}
			
			// Aqui iria lo de decidir a que pagina expulsar
		
		}
		
	}
	printf("TLB ya\n");
	close(pipe);
	return 0;
}
