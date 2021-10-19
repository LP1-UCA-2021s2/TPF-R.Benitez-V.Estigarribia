/*
 * main.c
 *
 *  Created on: 19 oct. 2021
 *      Author: lp1-2021
 */

#include <stdio.h>
#include "declaracionesGTK.h"
#include "declaraciones.h"


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
	cajas2p = cajas3p = 0;

	while (cajasAbiertas){

		if (turno == 1){ 	//Juega humano
			printf("\n\n 		Juega usted\n");
			repite = mov_usuario(tablero);

		}else if (turno == 0){  //Juega PC
			printf("\n\n 		Juega la computadora\n");
			repite = JuegaPC(tablero);
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
