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

int puntos[2];	//puntos[0] contiene los puntos de la PC, puntos[1] los del usuario

struct caja {
	/*Una pared (ARRIBA, ABAJO, IZQ o DER) puede indicar informacion con 4 valores distintos:
	 * 		- 0 : esta 'abierta'
	 * 		- 1 : esta 'cerrada'
	 * 		- 2 : esta abierta pero la caja contigua tiene pCerradas=2
	 * 		- 3 : esta cerrada y la caja contigua tiene pCerradas=2
	 *
	 *
	 *El peso de una caja indica cuantas cajas alrededor suyo tienen pCerradas=2
	 * 		- Cada caja ady que cumpla esa condicion le agrega un peso=2
	 * 		- Por tanto  0 <= peso <= 8
	 */
	unsigned int peso;
    unsigned int pCerradas;  //indica la cantidad de paredes cerradas
    unsigned int ARRIBA :2;
    unsigned int ABAJO  :2;
    unsigned int IZQ    :2;
    unsigned int DER    :2;
};



//Imprime el tablero en el terminal
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


//Agrega pared y retorna la cantidad de cajas cerradas en el proceso
int AgregarPared(struct caja tablero[][N], int x, int y, int p){
	//Se agrega una pared p a la caja en tablero[x][y]
	int cajasCerradas = 0;
	switch(p){
		//Agrega arriba
		case 0:
			tablero[x][y].ARRIBA=TRUE;
			//Se agrega la pared a la cajas adyacente de la caja actual
			if(x-1>=0)
			{
				if(tablero[x-1][y].ABAJO==FALSE){  //condicion de ruptura (de la funcion recursiva)
					cajasCerradas = AgregarPared(tablero, x-1, y, 1);
				}
			}
			break;
		//Agrega abajo
		case 1:
			tablero[x][y].ABAJO=TRUE;

			if(x+1<N)
			{
				if(tablero[x+1][y].ARRIBA==FALSE){
					cajasCerradas = AgregarPared(tablero, x+1, y, 0);
				}
			}
			break;
		//Agrega a la derecha
		case 2:
			tablero[x][y].DER=TRUE;

			if(y+1<N)
			{
				if(tablero[x][y+1].IZQ==FALSE){
					cajasCerradas = AgregarPared(tablero, x, y+1, 3);
				}
			}
			break;
		//Agrega a la izquierda
		case 3:
			tablero[x][y].IZQ=TRUE;

			if(y-1>=0)
			{
				if(tablero[x][y-1].DER==FALSE){
					cajasCerradas = AgregarPared(tablero, x, y-1, 2);
				}
			}
			break;
	}
	//Controla si la jugada realizada cerro la caja actual
	if (tablero[x][y].ARRIBA && tablero[x][y].ABAJO && tablero[x][y].DER && tablero[x][y].IZQ){
		tablero[x][y].abierta = FALSE;
		cajasCerradas += 1;
	}

	return cajasCerradas;
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


//Juega el humano, retorna la cantidad de cajas cerradas en un movimiento
int mov_usuario(struct caja tablero[][N]){
	//Datos a ingresar
    int f = 0;  //fila
    int c = 0;  //columna
    int p = 0;  //pared

	printf("\nIngrese las coordenadas de la caja [fila, columna]: ");
	scanf("%d, %d", &f, &c);

	//Comprobacion fila y columna dentro del rango de la matriz NO COMPRUEBA PARA (f,c)<0
	while( f >= N || c >= N){
		printf("\nError. Coordenadas fuera de rango. Vuelva a ingresar: ");
		scanf("%d, %d", &f, &c);
	}
	//Si la caja que eligio esta cerrada vuelve a pedir NO COMPRUEBA QUE ESTEN DENTRO DE RANGO
	while(!tablero[f][c].abierta){
		printf("\nLa caja seleccionada esta cerrada. Vuelva a elegir: ");
		scanf("%d, %d", &f, &c);
	}

	//Pedir pared a cerrar
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

	//Se hace el movimiento
	int cajasCerradas;
	cajasCerradas = AgregarPared(tablero, f, c, p);

	//suma los puntos acorde a la cant de cajas cerradas
	if (cajasCerradas){
		puntos[1] += 10 * cajasCerradas;
		printf("\nHas cerrado %d caja%s", cajasCerradas, (cajasCerradas==2)?"s":"");
		printf("\n	Se te han sumado %d puntos. Tienes %d puntos.", 10*cajasCerradas, puntos[1]);
		return cajasCerradas;
	}
	return cajasCerradas;

}


//Movimiento de la pc de manera random, retorna la cantidad de cajas cerradas en un movimiento
int mov_pc(struct caja tablero[][N]){
	srand(time(NULL));
	//Seleccion de coordenadas de caja
	int fi= rand()% N;
	int co = rand()% N;
	while(!tablero[fi][co].abierta){		//Elige hasta encontrar una caja abierta
		fi= rand()% N;
		co = rand()% N;
	}
	printf("\nCoordenadas de la caja: %d, %d", fi, co);

	//Elige pared a cerrar
	int pa = rand()% 4;
	while(pared_check(tablero, fi, co, pa)==0){		//Elige hasta encontrar una pared abierta
		pa = rand()% 4;
	}
	printf("\n\n0:Arriba\n1:Abajo\n2:Derecha\n3:Izquierda\nPared a cerrar: %d", pa);

	//Se hace el movimiento
	int cajasCerradas;
	cajasCerradas = AgregarPared(tablero, fi, co, pa);

	//se suman los puntos acorde a la cant de cajas cerradas
	if (cajasCerradas){
		puntos[0] += 10 * cajasCerradas;
		printf("\nLa PC ha cerrado %d caja%s", cajasCerradas, (cajasCerradas==2)?"s":"");
		printf("\n	PC gano %d puntos. Ahora tiene %d puntos.", 10*cajasCerradas, puntos[0]);
		return cajasCerradas;
	}
	return cajasCerradas;
}



int main(int argc, char *argv[]){
	nombre();  //verificar
	int turno = jugador();  //verificar
	color();  //hay que modificar
	N = dim_matriz()-1;  //modificar dim_matriz HACE MAL LAS COMPROBACIONES

	//Inicializa puntajes
	puntos[0] = 0;
	puntos[1] = 0;

	//Creacion de tablero
	struct caja tablero[N][N];
	InitBoxes(tablero);
	PrintBox(tablero);
	printf("\nLa matriz es de: %d x %d \n", N+1, N+1);

	//Ejecucion del juego
	int repite;  //indica si se repite el turno o no
	int cajasAbiertas = N*N;  //cant de cajas abiertas, si llega a 0 termina la partida
	while (cajasAbiertas){

		if (turno == 1){ 	//Juega humano
			printf("\n\n 		Juega usted\n");
			repite = mov_usuario(tablero);

		}else if (turno == 0){  //Juega PC
			printf("\n\n 		Juega la computadora\n");
			repite = mov_pc(tablero);
		}

		PrintBox(tablero);
		printf("\n--------------  PC: %d  | Tu: %d  ------------------\n", puntos[0], puntos[1]);

		if(!repite){	//si la cant de cajas cerradas es cero, cambia el turno
			turno = !turno;
		}

		cajasAbiertas -= repite;  //se le resta la cant de cajas cerradas
	}

	//Mensajes fin de juego
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
