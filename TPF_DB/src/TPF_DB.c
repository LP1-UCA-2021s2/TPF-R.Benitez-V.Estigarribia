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
int N=0;
enum boolean {FALSE=0, TRUE=1};

int puntos[2];

struct caja {
    //estos atributos almacenan info de 1 bit (0 o 1)
    unsigned int abierta:1;  //indica si la caja tiene todas sus paredes
    unsigned int ARRIBA :1;
    unsigned int ABAJO  :1;
    unsigned int IZQ    :1;
    unsigned int DER    :1;
};



//Imprime la caja
void PrintBox(struct caja cajas[][N]){
	printf("\n");
    for(int i=0; i < N; i++){
        printf("+");
        for(int j=0; j < N; j++){  //ARRIBA
            printf("%s+",cajas[i][j].ARRIBA?"----":"    ");
        }
        puts(" ");
        for(int j=0; j < N; j++){  //IZQ
            printf("%s    ", cajas[i][j].IZQ?"|":" ");
        }
        printf("%s", cajas[i][N-1].DER?"|":" "); //DER ultima columna
        puts(" ");
    }
    printf("+");
    for(int j=0; j < N; j++){  //ABAJO ultima fila
        printf("%s+",cajas[N-1][j].ABAJO?"----":"    ");
    }
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
int AgregarPared(struct caja tablero[][N], int x, int y, int p){
	switch(p){
		case 0:
			tablero[x][y].ARRIBA=TRUE;
			//Se agrega la pared a la cajas adyacente de la caja actual
			if(x-1>=0)
			{
				if(tablero[x-1][y].ABAJO==FALSE){  //condicion de ruptura (funcion recursiva)
					AgregarPared(tablero, x-1, y, 1);
				}

				if(!tablero[x-1][y].abierta){
					return 1;
				}
			}
			break;
		case 1:
			tablero[x][y].ABAJO=TRUE;

			if(x+1<N)
			{
				if(tablero[x+1][y].ARRIBA==FALSE){  //condicion de ruptura (funcion recursiva)
					AgregarPared(tablero, x+1, y, 0);
				}

				if(!tablero[x+1][y].abierta){
					return 1;
				}
			}
			break;
		case 2:
			tablero[x][y].DER=TRUE;

			if(y+1<N)
			{
				if(tablero[x][y+1].IZQ==FALSE){  //condicion de ruptura (funcion recursiva)
					AgregarPared(tablero, x, y+1, 3);
				}
				if(!tablero[x][y+1].abierta){
					return 1;
				}
			}
			break;
		case 3:
			tablero[x][y].IZQ=TRUE;

			if(y-1>=0)
			{
				if(tablero[x][y-1].DER==FALSE){  //condicion de ruptura (funcion recursiva)
					AgregarPared(tablero, x, y-1, 2);
				}

				if(!tablero[x][y-1].abierta){
					return 1;
				}
			}
			break;
	}
	//Controla si la jugada realizada cerro una caja

	if (tablero[x][y].ARRIBA && tablero[x][y].ABAJO && tablero[x][y].DER && tablero[x][y].IZQ){
		tablero[x][y].abierta = FALSE;
		return 1;
	}

	return 0;

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
int mov_usuario(struct caja tablero[][N]){
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
			goto bueno;		//-->Vuelve a controlar si el ingresado esta dentro del rango
		}
	int cerro;
	cerro = AgregarPared(tablero, f, c, p);
	//Si la caja se cerro, suma los puntos
	if (cerro){
		puntos[1] += 10;
		printf("\n	Se te han sumado 10 puntos. Tienes %d puntos.", puntos[1]);
		return 1;
	}
	return 0;

}


//Movimiento de la pc de manera random
int mov_pc(struct caja tablero[][N]){
	srand(time(NULL));
	int fi= rand()% N;
	int co = rand()% N;
	while(!tablero[fi][co].abierta){		//Elige hasta encontrar una caja abierta
		fi= rand()% N;
		co = rand()% N;
	}
	printf("\nCoordenadas de la caja: %d, %d", fi, co);

	//Paredes
	int pa = rand()% 4;
	while(pared_check(tablero, fi, co, pa)==0){		//Elige hasta encontrar una pared abierta
		pa = rand()% 4;
	}
	printf("\n\n0:Arriba\n1:Abajo\n2:Derecha\n3:Izquierda\nPared a cerrar: %d", pa);

	int cerro;
	cerro = AgregarPared(tablero, fi, co, pa);
	//Si la caja se cerro, suma los puntos
	if (cerro){
		puntos[0] += 10;
		printf("\n	Se le han sumado 10 puntos a la computadora. Tiene %d puntos.", puntos[0]);
		return 1;
	}
	return 0;

}

int main(int argc, char *argv[]){  //FALTA EL LOOP INICIAL PARA PODER TERMINAR EL JUEGO
	//nombre();
	int turno = jugador();
	//color();
	N = dim_matriz()-1;
	puntos[0] = 0;
	puntos[1] = 0;

	struct caja tablero[N][N];
	InitBoxes(tablero);
	PrintBox(tablero);
	printf("\nLa matriz es de: %d x %d \n", N+1, N+1);
	int repite=0;
	while (CajasAbiertas(tablero)){
		if (turno == 0){
			printf("\n\n 		Juega la computadora\n");
		}else{
			printf("\n\n 		Juega usted\n");
		}
		if (turno == 1){ 	//Juega humano
			repite = mov_usuario(tablero);
		}else if (turno == 0){
			repite = mov_pc(tablero);
		}
		PrintBox(tablero);
		printf("\n--------------  PC: %d  | Tu: %d  ------------------\n", puntos[0], puntos[1]);

		if(!repite){
			turno = !turno;
		}
	}
	printf("\n\n 		TERMINO EL JUEGO");
	if (puntos[0] > puntos[1]){
		printf("\nHa perdido con %d puntos contra %d puntos de la computadora :(, vuelva a intentar ", puntos[1], puntos[0]);
	}else if(puntos[0] == puntos[1]){
		printf("\nEMPATEEE         Tus puntos:%d        PC:%d\n", puntos[1], puntos[0]);
	}else{
		printf("\nHa ganadooo :)  Tus puntos:%d        PC:%d", puntos[1], puntos[0]);
	}

	return 0;
}
