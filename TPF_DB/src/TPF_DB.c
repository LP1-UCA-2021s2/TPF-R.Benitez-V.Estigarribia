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
void PrintBox(struct caja caja[][N]){
	puts("\n--------------------------------------");
	printf("soy una caja\n");
	for(int i=0; i<N; i++){
			for(int j=0; j<N; j++){
				printf("\narr[%d][%d]\n", i, j);
				printf("ARRIBA:%d, ABAJO:%d, DER:%d, IZQ:%d, abierta:%d\n",caja[i][j].ARRIBA, caja[i][j].ABAJO,caja[i][j].DER,caja[i][j].IZQ, caja[i][j].abierta);
			}
	}
    puts("\n--------------------------------------");
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
	//si en la direccion de p hay una caja, a esa caja se le agrega una pared en la dir opuesta
	switch(p){
		case 0:
			tablero[x][y].ARRIBA=TRUE;
			if(x-1>=0)
				tablero[x-1][y].ABAJO=TRUE;
			break;
		case 1:
			tablero[x][y].ABAJO=TRUE;
			if(x+1<N)
				tablero[x+1][y].ARRIBA=TRUE;
			break;
		case 2:
			tablero[x][y].DER=TRUE;
			if(y+1<N)
				tablero[x][y+1].IZQ=TRUE;
			break;
		case 3:
			tablero[x][y].IZQ=TRUE;
			if(y-1>=0)
				tablero[x+1][y].DER=TRUE;
			break;
	}
	puts("\n\t\tpared agregada\n");

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
void IniciarPartida(struct caja tablero[][N]){
	//inicializar las paredes
    int f = 0;
    int c = 0;
    int p = 0;

    //Controla que aun hayan cajas abiertas
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
	bueno:
		scanf("%d", &p);
		while(p != 1 && p != 2 && p != 3 && p != 0){	//Rango de pared
			printf("\nError. Fuera del rango. Vuelva a elegir la pared: ");
			scanf("%d", &p);
		}
		while(pared_check(tablero, f, c, p)==0){		//La pared seleccionada esta cerrada
			printf("\nError. La pared ya esta cerrada. Vuelva a elegir: ");
			goto bueno;
		}

	AgregarPared(tablero, f, c, p);

}


//Movimiento de la pc de manera random
void mov_pc(struct caja tablero[][N]){
	srand(time(NULL));
						//TIENE PROBLEMAS, ENTRA EN EL LOOP INFINITO DE Pared_check
						//NO PRINTEA LA LINEA 172


	//Controla 	que aun halla cajas abiertas

	int fi= rand()% N;
	int co = rand()% N;
	while(!tablero[fi][co].abierta){		//Elige hasta encontrar una caja abierta
		fi= rand()% N;
		co = rand()% N;
	}
	printf("		EMTRA %d, %d", fi, co);

	//Paredes
	int pa = rand()% 4;
	while(pared_check(tablero, fi, co, pa)==0){		//Elige hasta encontrar una pared abierta
		pa = rand()% 4;
	}
	printf("\n\n0:Arriba\n1:Abajo\n2:Derecha\n3:Izquierda\nPared a cerrar: %d", pa);


	AgregarPared(tablero, fi, co, pa);
}

int main(int argc, char *argv[]){  //FALTA EL LOOP INICIAL PARA PODER TERMINAR EL JUEGO
	//nombre();
	//int turno = jugador();
	int turno=0;
	//color();
	//N =dim_matriz()-1;
	N=3;
	struct caja tablero[N][N];
	InitBoxes(tablero);
	PrintBox(tablero);
	printf("\nLa matriz es de: %d x %d \n", N+1, N+1);
	while (CajasAbiertas(tablero) != 0){
		PrintBox(tablero);
		if (turno == 1){ 	//Juega humano
			IniciarPartida(tablero);
		}else if (turno ==0){
			mov_pc(tablero);
		}
		puntos[turno] +=10; 	//Contador de puntos del jugador
		turno = !turno;
		printf ("\nTURNOOO : %d", turno);
	}

	return 0;
}
