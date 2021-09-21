/*
 ============================================================================
 Name        : TPF_D&B.c
 Author      : R.Benitez || V.Estigarribia
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funciones.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	nombre();
	jugador();
	color();
	int M = dim_matriz();
	printf("\nLa matriz es de: %d x %d \n", M, M);
	char tablero[M][M];
	tablero[M][M] = crear_matriz(M);
	return 0;
}
