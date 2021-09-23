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
	//imprimir_matriz(M, &tablero [M][M]);

	return 0;
}*/
int N=3;

enum boolean {FALSE=0, TRUE=1};

struct caja {
    //estos atributos almacenan info de 1 bit (0 o 1)
    unsigned int abierta:1;  //indica si la caja tiene todas sus paredes
    unsigned int ARRIBA :1;
    unsigned int ABAJO  :1;
    unsigned int IZQ    :1;
    unsigned int DER    :1;
};


//Imprime la caja
void PrintBox(struct caja caja){
    if(caja.DER){
        printf("jaja %d\n",caja.DER);
    }
    printf("soy una caja\n");
}


int puntos[] = {0, 0};  //arr[0] Usuario | arr[1] PC
void InitBoxes(struct caja cajas[][N]);
void IniciarPartida(int N, int turno);
void AgregarPared(struct caja tablero[][N], int x, int y, int p);
int CajasAbiertas(struct caja cajas[][N]);




int main(int argc, char *argv[]){
    printf("Hello World\n");
    IniciarPartida(N, 1);
	return 0;
}





void IniciarPartida(int N, int turno){
	struct caja tablero[N][N];
	//inicializar
    InitBoxes(tablero);
    int f, c, p = 0;
    while (CajasAbiertas(tablero) != 0){
    	puts("bueno");
    	printf("Ingrese las coordenadas de la caja\n");
		scanf("%d, %d", &f, &c);
		while(!tablero[f][c].abierta){
			printf("WRONG\n");
			scanf("%d, %d", &f, &c);
		}
		printf("Pared a cerrar:\n1:Arriba\n2:Abajo\n3:Derecha\n4:Izquierda\n");
		scanf("%d", &p);
		while(p != 1 && p != 2 && p != 3 && p != 4){
			printf("WRONG\n");
			scanf("%d", &p);
		}
    	AgregarPared(tablero, f, c, p);
    	puntos[turno] +=10;
    	turno = (turno)? 0 : 1;
    }
}



void InitBoxes(struct caja cajas[][N]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			cajas[i][j].abierta = FALSE;
			cajas[i][j].ARRIBA  = FALSE;
			cajas[i][j].ABAJO   = FALSE;
			cajas[i][j].DER     = FALSE;
			cajas[i][j].IZQ     = FALSE;
		}
	}
}





int CajasAbiertas(struct caja cajas[][N]){
	for(int i=0; i < N; i++){
		for(int j=0; j < N; j++){
			if(cajas[i][j].abierta){
				return 1;
			}
		}
	}
	return 0;
}






void AgregarPared(struct caja tablero[][N], int x, int y, int p){
	switch(p){
		case 1:
			tablero[x][y].ARRIBA=TRUE;
			break;
		case 2:
			tablero[x][y].ABAJO=TRUE;
			break;
		case 3:
			tablero[x][y].DER=TRUE;
			break;
		case 4:
			tablero[x][y].IZQ=TRUE;
			break;
	}
}
