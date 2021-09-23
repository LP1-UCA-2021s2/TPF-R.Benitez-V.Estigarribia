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
int N=0;
#include "funciones.h"



enum boolean {FALSE=0, TRUE=1};

struct caja {
    //estos atributos almacenan info de 1 bit (0 o 1)
    unsigned int abierta:1;  //indica si la caja tiene todas sus paredes
    unsigned int ARRIBA :1;
    unsigned int ABAJO  :1;
    unsigned int IZQ    :1;
    unsigned int DER    :1;
};

int puntos[]= {0,0};

//Imprime la caja
void PrintBox(struct caja caja){
    if(caja.DER){
        printf("jaja %d\n",caja.DER);
    }
    printf("soy una caja\n");
}



//Inicializa y cera las paredes
void InitBoxes(struct caja cajas[][N]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			cajas[i][j].abierta = TRUE;
			cajas[i][j].ARRIBA  = FALSE;
			cajas[i][j].ABAJO   = FALSE;
			cajas[i][j].DER     = FALSE;
			cajas[i][j].IZQ     = FALSE;
		}
	}
}

//Controla si las cajas aun tienen paredes abiertas
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

//Agrega pared
void AgregarPared(struct caja tablero[][N], int x, int y, int p){
	switch(p){
		case 0:
			tablero[x][y].ARRIBA=TRUE;
			break;
		case 1:
			tablero[x][y].ABAJO=TRUE;
			break;
		case 2:
			tablero[x][y].DER=TRUE;
			break;
		case 3:
			tablero[x][y].IZQ=TRUE;
			break;
	}
}

//Controla que las paredes no esten cerradas
int pared_check(struct caja tablero[][N], int x, int y, int p){
	switch(p){
		case 0:
			if (tablero[x][y].ARRIBA==TRUE){
				return 0;
			}
			break;
		case 1:
			if (tablero[x][y].ABAJO==TRUE){
				return 0;
			}
			break;
		case 2:
			if (tablero[x][y].DER==TRUE){
				return 0;
			}
			break;
		case 3:
			if (tablero[x][y].IZQ==TRUE){
				return 0;
			}
			break;
	}
	return 1;
}

//Juega el humano
void IniciarPartida(struct caja tablero[][N], int turno){
	//inicializar las paredes
    InitBoxes(tablero);
    int f = 0;
    int c = 0;
    int p = 0;

    //Controla que aun hayan cajas abiertas
    while (CajasAbiertas(tablero) != 0){
    	printf("\nIngrese las coordenadas de la caja [fila, columna]: ");
		scanf("%d, %d", &f, &c);
		while( f >= N || c >= N){			//Pide el valor hasta que sea el correcto
			printf("\nError. Coordenadas fuera de rango. Vuelva a ingresar: ");
			scanf("%d, %d", &f, &c);
		}
		while(!tablero[f][c].abierta){		//La caja que eligio esta cerrada
			printf("\nLa caja seleccionada esta cerrada. Vuelva a elegir: ");
			scanf("%d, %d", &f, &c);
		}

		//Paredes
		printf("\n0:Arriba\n1:Abajo\n2:Derecha\n3:Izquierda\nPared a cerrar: ");
		scanf("%d", &p);
		while(p != 1 && p != 2 && p != 3 && p != 0){	//Pide el valor hasta que sea el correcto
			printf("\nError. Fuera del rango. Vuelva a elegir la pared: ");
			scanf("%d", &p);
		}
		while(pared_check(tablero, f, c, p)==0){		//La pared seleccionada esta cerrada
			printf("\nError. La pared ya esta cerrada. Vuelva a elegir: ");
			scanf("%d", &p);
		}


		AgregarPared(tablero, f, c, p);
		puntos[turno] +=10; 	//Contador de puntos del jugador
		turno = (turno)? 0 : 1;
		break;
		printf ("\nTURNOOO : %d", turno);
    }
}


//Movimiento de la pc de manera random
void mov_pc(struct caja tablero[][N],int N, int turno){
	srand(time(NULL));
						//TIENE PROBLEMAS, ENTRA EN EL LOOP INFINITO DE Pared_check
						//NO PRINTEA LA LINEA 172


	//Controla 	que aun halla cajas abiertas
	while (CajasAbiertas(tablero) != 0){
		int fi= rand()% N;
		int co = rand()% N;
		while(!tablero[fi][co].abierta){		//Elige hasta encontrar una caja abierta
			fi= rand()% N;
			co = rand()% N;
		}
		printf("		EMTRA");
		printf("\nPieza a mover: %d, %d", fi, co);

		//Paredes
		int pa = rand()% 4;
		while(pared_check(tablero, fi, co, pa)==0){		//Elige hasta encontrar una pared abierta
			pa = rand()% 4;
		}
		printf("\n0:Arriba\n1:Abajo\n2:Derecha\n3:Izquierda\nPared a cerrar: %d", pa);


		AgregarPared(tablero, fi, co, pa);
		puntos[turno] +=10; 	//Contador de puntos del jugador
		turno = (turno)? 0 : 1;
		break;
		printf ("\nTURNOOO : %d", turno);
	}
}

int main(int argc, char *argv[]){
								//FALTA EL LOOP INICIAL PARA PODER TERMINAR EL JUEGO


	struct caja tablero[N][N];
	//nombre();
	int turno = jugador();
	//color();
	N =dim_matriz()-1;
	printf("\nLa matriz es de: %d x %d \n", N, N);

	if (turno == 1){ 	//Juega humano
		IniciarPartida(tablero, turno);
	}else if (turno ==0){
		mov_pc(tablero, N, turno);
	}

	return 0;
}







