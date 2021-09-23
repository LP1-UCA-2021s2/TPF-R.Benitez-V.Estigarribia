/*
 ===========================================================================
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

/*int main(int argc, char *argv[]) {
	nombre();
	jugador();
	color();
	int M = dim_matriz();
	printf("\nLa matriz es de: %d x %d \n", M, M);
	char tablero[M][M];
	tablero[M][M] = crear_matriz(M);
	//imprimir_matriz(M, &tablero /*[M][M]);

	return 0;
}*/
int N=4;
enum boolean {FALSE=0, TRUE=1};

struct caja {
    //estos atributos almacenan info de 1 bit (0 o 1)
    unsigned int cerrada:1;  //indica si la caja tiene todas sus paredes
    unsigned int ARRIBA :1;
    unsigned int ABAJO  :1;
    unsigned int IZQ    :1;
    unsigned int DER    :1;
};

//Imprime la caja
void print_box(struct caja caja){
    if(caja.DER){
        printf("jaja %d\n",caja.DER);
    }
    printf("soy una caja\n");
}

//const int N=2;  //dim matriz de cajas
int main(int argc, char *argv[]){

    printf("Hello World\n");
    struct caja cajas[N][N];
    cajas[0][0].DER = TRUE;
    printf("arr[0]: %d", cajas[0][0].DER);

	return 0;
}


