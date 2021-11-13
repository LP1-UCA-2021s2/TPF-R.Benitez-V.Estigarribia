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

#define CAJA (tablero[fila][columna])

int N = 0;  //Dimension de la matriz de cajas (no del tablero)


//cajasP4: cant de cajas que tienen al menos 4 de peso
//cajasP6: ''  6 de peso
//La idea es que si cajasP4==N*N se busquen solo cajas de peso 4 a no ser que ya no haya
//Si no hay cajas de peso=4 entonces cajasP6-cajasP4 == 0
int cajas2p, cajas3p, cajasP4, cajasP6;

enum propiedades_pared {ABIERTA=0, CERRADA=1, PESO=2};  //Constantes simbolicas para operar con las cajas

int puntos[2];	//puntos[0] contiene los puntos de la PC, puntos[1] los del usuario

int ultCoords[] = {0, 0};  //contiene las coordenadas de la ultima caja que se eligio


struct caja {
	/* Una pared (ARRIBA, ABAJO, IZQ o DER) puede indicar informacion con 4 valores distintos:
	 * 		- 0 : esta 'abierta'
	 * 		- 1 : esta 'cerrada'
	 * 		- 2 : esta abierta pero la caja contigua tiene pCerradas=2
	 * 		- 3 : esta cerrada y la caja contigua tiene pCerradas=2
	 *
	 *
	 * El peso de una caja indica cuantas cajas alrededor suyo tienen pCerradas=2
	 * 		- Cada caja ady que cumpla esa condicion le agrega un peso=2
	 * 		- La pared pegada a dicha caja tambien adquiere PESO
	 * 		- Por tanto  0 <= peso <= 8
	 *
	 *
	 * Como saber si una caja esta 'abierta' o 'cerrada' independientemente de su peso?
	 * 		Con este sistema, la forma de hacer esta comprobacion es
	 *		usando el operador % (modulo) con el valor PESO
	 *		- PARED%PESO dara 1 si PARED esta cerrada
	 *		- PARED%PESO dara 0 si PARED esta abierta
	 *
	 */

	//Declaracion de atributos de caja
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
            printf("%s+",cajas[i][j].ARRIBA%2?"----":"    ");
        }
        puts(" ");
        for(int j=0; j < N; j++){  //IZQ
            printf("%s  %d ", cajas[i][j].IZQ%2?"|":" ", cajas[i][j].peso);
        }
        printf("%s", cajas[i][N-1].DER%2?"|":" "); //DER ultima columna
        puts(" ");
    }
    printf("+");
    for(int j=0; j < N; j++){  //ABAJO ultima fila
        printf("%s+",cajas[N-1][j].ABAJO%2?"----":"    ");
    }
}

void fPrintBox(struct caja cajas[][N]){
	FILE *board;
	board = fopen("tablero.txt", "w");
	fprintf(board,"\n");
	for(int i=0; i < N; i++){
		fprintf(board,"+");
		for(int j=0; j < N; j++){  //ARRIBA
			fprintf(board,"%s+",cajas[i][j].ARRIBA%2?"----":"    ");
		}
		fprintf(board," \n");
		for(int j=0; j < N; j++){  //IZQ
			fprintf(board,"%s    ", cajas[i][j].IZQ%2?"|":" ");
		}
		fprintf(board,"%s", cajas[i][N-1].DER%2?"|":" "); //DER ultima columna
		fprintf(board," \n");
	}
	fprintf(board,"+");
	for(int j=0; j < N; j++){  //ABAJO ultima fila
		fprintf(board,"%s+",cajas[N-1][j].ABAJO%2?"----":"    ");
	}
	fclose(board);
}
//Inicializa y cera las paredes
void InitBoxes(struct caja cajas[][N]){
	/*Todas las cajas inician con todas las paredes y pCerradas en cero
	 * PERO el peso no es el mismo para todas, las paredes que esten
	 * en el 'borde' del tablero ya le agregan peso a la caja
	 *
	 * Por tanto las cajas ubicadas en las esquinas tienen peso inicial 4
	 * y las que estan en el borde tienen peso inicial 2
	 */
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			cajas[i][j].peso = 0;
			cajas[i][j].pCerradas = 0;
			cajas[i][j].ARRIBA  = ABIERTA;
			cajas[i][j].ABAJO   = ABIERTA;
			cajas[i][j].DER     = ABIERTA;
			cajas[i][j].IZQ     = ABIERTA;

			//Peso de cajas en el borde
			/* Esto era una opcion para hacer que las cajas del borde puedan llegar a tener el peso maximo
			 * en algun punto del juego y no ocasione problemas. Como no hay ninguna caja adyacente que pueda
			 * agregarle peso a esas paredes del borde, tenemos que hacerlo en codigo. De otro modo el peso de
			 * esas cajas solo estan entre 0 y 4 (no entre 0 y 8 como debe ser)
			 *
			 * Sin embargo al hacer esto se genera un conflicto a la hora de buscar alguna caja conveniente que cerrar.
			 * Esto sucede cuando en una caja se intenta encontrar una pared abierta que ademas tenga
			 * peso=0 (con pared_check y suponiendo que es posible encontrar dicha pared) para evitar agregarle
			 * la tercera pared a una caja adyacente (y por tanto darle ventaja al oponente)
			 *
			 * Por tanto, la opcion que queda es agregarle peso inmediatamente despues de cerrarlas, a las
			 * paredes que sean del borde del tablero, en la funcion AgregarPared()*/
//			if(i==0){
//				cajas[i][j].peso += PESO;
//				cajas[i][j].ARRIBA += PESO;
//			}if(j==0){
//				cajas[i][j].peso += PESO;
//				cajas[i][j].IZQ += PESO;
//			}if(i==N-1){
//				cajas[i][j].peso += PESO;
//				cajas[i][j].ABAJO += PESO;
//			}if(j==N-1){
//				cajas[i][j].peso += PESO;
//				cajas[i][j].DER += PESO;
//			}
		}
	}
}


//Se actualiza el peso de las cajas adyacentes a caja[x][y]
void ActualizarPeso(struct caja caja[][N], int x, int y){
	/* Si la caja en (x,y) tiene pCerradas==2 se llama esta funcion
	 * que a cada caja adyacente le suma un PESO en la pared correspondiente
	 * (la pared 'pegada' a caja[x][y])
	 */

	if(x-1 >= 0){  //caja de arriba
		caja[x-1][y].ABAJO += PESO;
		caja[x-1][y].peso += PESO;
	}
	if(x+1 < N){  //caja de abajo
		caja[x+1][y].ARRIBA += PESO;
		caja[x+1][y].peso += PESO;
	}
	if(y-1 >= 0){  //caja de la izquierda
		caja[x][y-1].DER += PESO;
		caja[x][y-1].peso += PESO;
	}
	if(y+1 < N){  //caja de la derecha
		caja[x][y+1].IZQ += PESO;
		caja[x][y+1].peso += PESO;
	}
}


//Agrega pared y retorna la cantidad de cajas cerradas en el proceso
int AgregarPared(struct caja tablero[][N], int x, int y, int p){
	//Se agrega una pared p a la caja en tablero[x][y]
	int cajasCerradas = 0;
	switch(p){
		//Agrega arriba
		case 0:
			tablero[x][y].ARRIBA += CERRADA;

			if (x == 0){
				tablero[x][y].ARRIBA += PESO;
				tablero[x][y].peso += PESO;
			}
			//Se agrega tambien la pared a la caja adyacente
			if(x-1>=0)
			{
				if(tablero[x-1][y].ABAJO%2 == ABIERTA){  //condicion de ruptura (de la funcion recursiva)
					cajasCerradas = AgregarPared(tablero, x-1, y, 1);
				}
			}
			break;
		//Agrega abajo
		case 1:
			tablero[x][y].ABAJO += CERRADA;

			if (x == N-1){
				tablero[x][y].ABAJO += PESO;
				tablero[x][y].peso += PESO;
			}

			if(x+1<N)
			{
				if(tablero[x+1][y].ARRIBA%2 == ABIERTA){
					cajasCerradas = AgregarPared(tablero, x+1, y, 0);
				}
			}
			break;
		//Agrega a la derecha
		case 2:
			tablero[x][y].DER += CERRADA;

			if (y == N-1){
				tablero[x][y].DER += PESO;
				tablero[x][y].peso += PESO;
			}

			if(y+1<N)
			{
				if(tablero[x][y+1].IZQ%2 == ABIERTA){
					cajasCerradas = AgregarPared(tablero, x, y+1, 3);
				}
			}
			break;
		//Agrega a la izquierda
		case 3:
			tablero[x][y].IZQ += CERRADA;

			if (y == 0){
				tablero[x][y].IZQ += PESO;
				tablero[x][y].peso += PESO;
			}

			if(y-1>=0)
			{
				if(tablero[x][y-1].DER%2 == ABIERTA){
					cajasCerradas = AgregarPared(tablero, x, y-1, 2);
				}
			}
			break;
	}

	//suma una pared cerrada
	tablero[x][y].pCerradas += 1;

	//Actualiza el peso de las cajas adyacentes
	if(tablero[x][y].pCerradas == 2){
		ActualizarPeso(tablero, x, y);
	}

	//Controla si la jugada realizada cerro la caja actual
	if (tablero[x][y].pCerradas == 4){
		cajasCerradas += 1;
		cajas3p -= 1;
	}


	//Actualizar cant de cajas convenientes restantes
	if(tablero[x][y].pCerradas==2)
		cajas2p += 1;
	if(tablero[x][y].pCerradas==3){
		cajas3p += 1;
		//Guardamos la posicion de la caja que acaba de seleccionarse? sirve para tests PCvsPC
		ultCoords[0] = x;
		ultCoords[1] = y;
	}


	return cajasCerradas;
}


//Controla que las paredes no esten cerradas
//devuelve 1 si p esta abierta y no tiene peso, 2 si tiene, 0 si esta cerrada
int pared_check(struct caja tablero[][N], int fila, int columna, int p, int controlFlag){
	// controlFlag indica si se quiere que pared_check ademas controle o no si esa pared tiene peso
	switch(p){
		case 0:
			if (CAJA.ARRIBA%2 == CERRADA){
				return 0;
			}
			break;
		case 1:
			if (CAJA.ABAJO%2 == CERRADA){
				return 0;
			}
			break;
		case 2:
			if (CAJA.DER%2 == CERRADA){
				return 0;
			}
			break;
		case 3:
			if (CAJA.IZQ%2 == CERRADA){
				return 0;
			}
			break;
		case -1:  //controla si hay alguna pared abierta de CAJA que no tenga peso
			printf("\n\t\t%d %d %d %d\n", CAJA.ARRIBA, CAJA.ABAJO, CAJA.DER, CAJA.IZQ);
			if (CAJA.ARRIBA == 0 || CAJA.ABAJO == 0 || CAJA.DER == 0 || CAJA.IZQ == 0){
				return 1;
			}else{
				return 0;
			}
			break;
	}
	if (controlFlag){
		switch(p){
			case 0:
				if (CAJA.ARRIBA == PESO)
					return 2;
				break;

			case 1:
				if (CAJA.ABAJO == PESO)
					return 2;
				break;

			case 2:
				if (CAJA.DER == PESO)
					return 2;
				break;

			case 3:
				if (CAJA.IZQ == PESO)
					return 2;
				break;
		}
	}

	return 1;
}


//Movimiento de la pc de manera random, retorna la cantidad de cajas cerradas en un movimiento
int mov_pc(struct caja tablero[][N], int fila, int columna, int absRandom, int print, int turno){
	/* Hace un movimiento en una caja random o en la fila y columna indicadas.
	 *
	 * Mov random:
	 * - Si absRandom==True, hace un movimiento absolutamente random.
	 * - De lo contrario elige una caja random que ademas sea una caja conveniente (pCerradas!=2).
	 */

	int check, cajasCerradas, pared, pass = 0;

	//Si en (fila, columna) no hay una caja valida, se elige coordenadas random hasta encontrar una.
	if(absRandom)
	{
		while(CAJA.pCerradas == 4){  //absolutamente random. Cuando se hace el mov inicial o bien ya no se puede evitar cajas con 2 paredes
			fila = rand()% N;
			columna = rand()% N;
		}
	}
	else
	{
		if(cajas3p){  //encuentra cajas con 3 paredes si las hay
			while(CAJA.pCerradas!=3 ){
				fila= rand()% N;
				columna = rand()% N;
			}
		}else{  //encuentra cajas con 0 o 1 paredes
			while (!pass){
				if (CAJA.pCerradas!=4 && CAJA.pCerradas!=2){
					pass = 1;
				}else{
					fila = rand()% N;
					columna = rand()% N;
					continue;
				}


				if (turno == 0)
				{
					puts("\nEligiendo caja...");
					printf("\tcoord: (%d, %d) de peso: %d", fila, columna, CAJA.peso);
					//control de peso para ver si es realmente conveniente la caja elegida
					if(CAJA.pCerradas == 0){
						if (CAJA.peso==8 && cajas2p != N*N-1){
							fila= rand()% N;
							columna = rand()% N;
							pass = 0;
							continue;
						}
					}
					else if (CAJA.pCerradas == 1)
					{
						if (CAJA.peso==8 && cajas2p != N*N-1){
							fila= rand()% N;
							columna = rand()% N;
							pass = 0;
							continue;
						}
						if (CAJA.peso==6){
							check = pared_check(tablero, fila, columna, -1, 0);
							if (check){
								pass = 1;
							}else{
								fila= rand()% N;
								columna = rand()% N;
								pass = 0;
							}

						}

					}
				}

			}
		}
	}
	if(print)
		printf("\nCoordenadas de la caja: %d, %d", fila, columna);


	//Ahora elige pared random a cerrar

	pared = rand()% 4;

	//&& cajas2p != N*N-1
	if (turno == 0 && CAJA.pCerradas != 3 && cajas2p != N*N)
	{
		puts("\nENTRA");
		while(pared_check(tablero, fila, columna, pared, 1) != 1){		//Elige hasta encontrar una pared abierta que ademas no tenga peso
			pared = rand()% 4;
		}
		puts("SALE");
	}
	else
	{
		while(pared_check(tablero, fila, columna, pared, 0) == 0){		//Elige hasta encontrar una pared abierta
			pared = rand()% 4;
		}
	}


	//Guardamos la posicion de la caja que acaba de seleccionarse? sirve para tests PCvsPC
	ultCoords[0] = fila;
	ultCoords[1] = columna;


	//Se hace el movimiento
	cajasCerradas = AgregarPared(tablero, fila, columna, pared);
	printf("\nPeso: %d\n", CAJA.peso);

	//Se suman los puntos acorde a la cant de cajas cerradas
	if (cajasCerradas){
		puntos[turno] += 10 * cajasCerradas;
		if(print){
			printf("\nLa PC ha cerrado %d caja%s", cajasCerradas, (cajasCerradas==2)?"s":"");
			printf("\n	PC gano %d puntos. Ahora tiene %d puntos.", 10*cajasCerradas, puntos[turno]);
		}
	}


	return cajasCerradas;
}


int JuegaPC(struct caja tablero[][N], int print, int turno, int seed){
	/* Hace un movimiento evitando las cajas que ya tengan dos paredes << pCerradas==2 >>
	 * (ya que al agregarle la tercera le daria ventaja al oponente)
	 *
	 * Tambien, intenta realizar el movimiento en la ultima caja seleccionada por el oponente,
	 * ya que es altamente probable realizar un movimiento conveniente en esa caja
	 *
	 *  Algunas variables utilizadas:
	 * 		cajasAbiertas: cantidad total de cajas abiertas restantes.
	 * 		cajasConvenientes: cantidad de cajas con << pCerradas != 2 >>.
	 * 		cajasCerradas: cantidad de cajas cerradas en una jugada (0, 1 o 2).
	 * */

	srand(time(NULL)+seed);
	int fila, columna, cajasCerradas;
	fila = ultCoords[0];
	columna = ultCoords[1];

	//Mov random si es la primera jugada o no quedan movimientos 'convenientes'
	if((tablero[fila][columna].pCerradas==0 || cajas2p==N*N) && !cajas3p)
	{
		fila = rand()%N;
		columna = rand()%N;
		cajasCerradas = mov_pc(tablero, fila, columna, 1, print, turno);
	}
	else if( cajas2p!=N*N || cajas3p!=0 )  //esto es importante para evitar un loop infinito si llamamos mov_pc con absRandom==0
	{
		//Realiza el movimiento en la ultima caja elegida, o un en una caja random
		cajasCerradas = mov_pc(tablero, fila, columna, 0, print, turno);
	}

	return cajasCerradas;
}


int DotsNBoxes(int print, int dim, int iter){

	N = dim;

	int turno = 0;

	//Inicializa puntajes
	puntos[0] = 0;
	puntos[1] = 0;

	//Creacion de tablero
	struct caja tablero[N][N];
	InitBoxes(tablero);
	if(print)
		printf("\n\n=================== NEW GAME ==================\nLa matriz es de: %d x %d \n", N+1, N+1);


	//Ejecucion del juego
	int repite;  //indica si se repite el turno o no
	int cajasAbiertas = N*N;  //cant de cajas abiertas, si llega a 0 termina la partida
	cajas2p = cajas3p = 0;

	static int iteracion=0;  //usada para generar numeros random
	while (cajasAbiertas)
	{
			iteracion++;

			if (turno == 1)
			{
				if(print)
					printf("\n\n 		Juega PC 1\n");
				repite = JuegaPC(tablero, print, 1, iteracion);

			}
			else if (turno == 0)
			{
				if(print)
					printf("\n\n 		Juega PC 2\n");
				repite = JuegaPC(tablero, print, 0, iteracion);
			}

			if(print){
				PrintBox(tablero);
				printf("\n--------------  PC 1: %d  | PC 2: %d  ------------------ %d - %d \n", puntos[1], puntos[0], cajas2p, iter);
			}

			fPrintBox(tablero);  //guarda el tablero en un archivo txt

			if(!repite){	turno = !turno;		}

			cajasAbiertas -= repite;  //se le resta la cant de cajas cerradas
	}

	if(print)
	{
		//Mensajes fin de juego
		printf("\n\n 		TERMINO EL JUEGO");
		if (puntos[0] > puntos[1]){
			printf("\nPC 2 ganadooo :)  PC 1:%d        PC 2:%d\n\n", puntos[1], puntos[0]);
		}else if(puntos[0] == puntos[1]){
			printf("\nEMPATEEE         PC 1:%d        PC 2:%d\n\n", puntos[1], puntos[0]);
		}else{
			printf("\nPC 1 ganadooo :)  PC 1:%d        PC 2:%d\n\n", puntos[1], puntos[0]);
		}
	}
	if (puntos[0] > puntos[1]){
		return 0;
	}else if(puntos[0] == puntos[1]){
		return -1;
	}else{
		return 1;
	}

}



int main(){
	int partGanadas[] = {0, 0, 0};
	int times, i, ganador;
	printf("Times: ");
	scanf("%d", &times);

	for(i=1; i <= times; i++){
		ganador = DotsNBoxes(1, 3, i);
		if(ganador == 1)
			partGanadas[1] += 1;
		else if(ganador == 0)
			partGanadas[0] += 1;
		else
			partGanadas[2] += 1;
	}
	printf("\nRESULTADOS:\n\tPC 1: %d\n\tPC 2: %d\n\tEmpates: %d", partGanadas[1], partGanadas[0], partGanadas[2]);
	return 0;
}
