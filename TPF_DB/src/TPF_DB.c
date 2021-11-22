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
void PrintBox(struct caja **cajas){
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

void PrintBox2(struct caja **cajas){
	printf("\n");
    for(int i=0; i < N; i++){
        printf("+");
        for(int j=0; j < N; j++){  //ARRIBA
            printf("  %d  +",cajas[i][j].ARRIBA);
        }
        puts(" ");
        for(int j=0; j < N; j++){  //IZQ
            printf("%d     ", cajas[i][j].IZQ);
        }
        printf("%d", cajas[i][N-1].DER); //DER ultima columna
        puts(" ");
    }
    printf("+");
    for(int j=0; j < N; j++){  //ABAJO ultima fila
        printf("  %d  +",cajas[N-1][j].ABAJO);
    }
}

void fPrintBox(struct caja **cajas){
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


int BoardFromTxt(struct caja **tablero, int *cajasAbiertas){
	FILE *board;
	int up, down, der, izq;
	board = fopen("board1.txt", "r");
	if (board==NULL){
		return -1;
	}
	//lee el tablero asignando sus pesos a cada pared
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			fscanf(board, "%d %d %d %d\n", &up, &down, &der, &izq);
			tablero[i][j].ARRIBA = up;
			tablero[i][j].peso += (up>=2)?PESO:0;

			tablero[i][j].ABAJO = down;
			tablero[i][j].peso += (down>=2)?PESO:0;

			tablero[i][j].DER = der;
			tablero[i][j].peso += (der>=2)?PESO:0;

			tablero[i][j].IZQ = izq;
			tablero[i][j].peso += (izq>=2)?PESO:0;
		}
	}

	fclose(board);

	//le damos valores a pCerradas y cajas2p
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			int cont = 0;
			if (tablero[i][j].ARRIBA%2){
				tablero[i][j].pCerradas++;
				cont++;
			}
			if (tablero[i][j].ABAJO%2){
				tablero[i][j].pCerradas++;
				cont++;
			}
			if (tablero[i][j].DER%2){
				tablero[i][j].pCerradas++;
				cont++;
			}
			if (tablero[i][j].IZQ%2){
				tablero[i][j].pCerradas++;
				cont++;
			}

			if (cont >= 2){
				cajas2p++;
			}
			if (cont == 4){
				*cajasAbiertas -= 1;
			}
		}
	}


	return 0;
}


//Inicializa y cera las paredes
void InitBoxes(struct caja **cajas){
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
		}
	}
}


struct caja **
CrearTablero(int size){
	/*
	* Funcion que crea el tablero dependiendo del tamaño que recibe.
	* Parametros:
	* 	size -> tamaño introducido por el usuario.
	* Retorno:
	*  La posicion de memoria donde se encuentra el tablero creado.
	*/
	struct caja **board;
	board=malloc(size*sizeof(struct caja));
	for(int i=0;i<size;i++){
		board[i]=malloc(size*sizeof(struct caja));
	}

	return board;
}


//Se actualiza el peso de las cajas adyacentes a caja[x][y]
void ActualizarPeso(struct caja **caja, int x, int y){
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
int AgregarPared(struct caja **tablero, int x, int y, int p){
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
int pared_check(struct caja **tablero, int x, int y, int p){
	switch(p){
		case 0:
			if (tablero[x][y].ARRIBA%2 == CERRADA){
				return 0;
			}
			break;
		case 1:
			if (tablero[x][y].ABAJO%2 == CERRADA){
				return 0;
			}
			break;
		case 2:
			if (tablero[x][y].DER%2 == CERRADA){
				return 0;
			}
			break;
		case 3:
			if (tablero[x][y].IZQ%2 == CERRADA){
				return 0;
			}
			break;
	}
	return 1;
}


int Esquinas(struct caja **tablero) {
	if (tablero[0][0].pCerradas==2){
		if (tablero[0][0].ARRIBA==0 && tablero[0][0].IZQ==0){
			ultCoords[0] = 0;
			ultCoords[1] = 0;
			return 1;
		}
	}
	if (tablero[0][N-1].pCerradas==2) {
		if (tablero[0][N-1].ARRIBA==0 && tablero[0][N-1].DER==0) {
			ultCoords[0] = 0;
			ultCoords[1] = N-1;
			return 1;
		}
	}
	if (tablero[N-1][0].pCerradas==2) {
		if (tablero[N-1][0].ABAJO==0 && tablero[N-1][0].IZQ==0) {
			ultCoords[0] = N-1;
			ultCoords[1] = 0;
			return 1;
		}
	}
	if (tablero[N-1][N-1].pCerradas==2) {
		if (tablero[N-1][N-1].ABAJO==0 && tablero[N-1][N-1].DER==0) {
			ultCoords[0] = N-1;
			ultCoords[1] = N-1;
			return 1;
		}
	}


	return 0;
}


//Movimiento de la pc de manera random, retorna la cantidad de cajas cerradas en un movimiento
int mov_pc(struct caja **tablero, int fila, int columna, int absRandom, int print, int turno, int seed){
	/* Hace un movimiento en una caja random o en la fila y columna indicadas.
	 *
	 * Mov random:
	 * - Si absRandom==True, hace un movimiento absolutamente random.
	 * - De lo contrario elige una caja random que ademas sea una caja conveniente (pCerradas!=2).
	 */

	srand(seed+time(NULL));
	int cajasCerradas, pared;

	//Si en (fila, columna) no hay una caja valida, se elige coordenadas random hasta encontrar una.
	if(absRandom==1)
	{
		while(tablero[fila][columna].pCerradas == 4){  //absolutamente random. Cuando se hace el mov inicial o bien ya no se puede evitar cajas con 2 paredes
			fila = rand()% N;
			columna = rand()% N;
		}
	}
	else if (absRandom==0)
	{
		if(cajas3p){  //encuentra cajas con 3 paredes si las hay
			while(tablero[fila][columna].pCerradas!=3 ){
				fila= rand()% N;
				columna = rand()% N;
			}
		}else{
			while(tablero[fila][columna].pCerradas==4 || tablero[fila][columna].pCerradas==2 ){  //con un criterio adicional
				fila= rand()% N;
				columna = rand()% N;
			}
		}
	}
	else if (absRandom == 2) // v1** si es el turno de PC2 y cajas2p==N*N, en lugar de hacer un mov totalmente random entra aca
	{
		puts("\n\t======= Buscando cajas de peso 4 ========");  //v1** no tiene nada que ver con pesos 4 aun
		static int flag = 1;
		static int cont;	//cont es un contador de cajas de peso 4 y pCerradas<2

		if (cajas2p == N*N){
			flag = 0;
		}
		if (flag) {
			cont = 0;
			for (int i=0; i < N; i++) {
				for (int j=0; j < N; j++) {
					if (tablero[i][j].peso==4 && tablero[i][j].pCerradas<2){
						cont++;
					}
				}
			}

			flag = cont;
		}

		if (flag) {  //v1** jamas entra aca si es que ya no hay cajas con pCerradas<2
			puts("ENTRA1");
			while (!(tablero[fila][columna].peso==4 && tablero[fila][columna].pCerradas<2)){
				fila = rand()% N;
				columna = rand()% N;
			}
			puts("SALE1");
		} else {
			if (Esquinas(tablero)) {
				fila = ultCoords[0];
				columna = ultCoords[1];
			} else {
				puts("ENTRA2");
				/*
				while (!(tablero[fila][columna].peso==4 && tablero[fila][columna].pCerradas!=4)) {  //ojo, solo si aun hay cajas de peso 4 (hacer comprobacion cajasP4-cajasP6)
					fila = rand()% N;
					columna = rand()% N;
				}*/
				while(tablero[fila][columna].pCerradas == 4){  //v1** absolutamente random (por mientras)
							fila = rand()% N;
							columna = rand()% N;
						}

				puts("SALE2");
			}
		}
	}
	if(print)
		printf("\nCoordenadas de la caja: %d, %d ", fila, columna);


	//elige la pared de menor peso
	if (turno == 0){  //caso: ARRIBA tiene peso 1 y hay paredes abiertas pero todos tienen peso, no entra a ningun if y pared=0
		pared = rand()% 4;  //si no es con random va a elegir un solo valor para la mayoria de las jugadas
		int pesoPared;
		if (pared==0){
			pesoPared = tablero[fila][columna].ARRIBA;
		}else if(pared==1){
			pesoPared = tablero[fila][columna].ABAJO;
		}else if(pared==2){
			pesoPared = tablero[fila][columna].DER;
		}else if(pared==3){
			pesoPared = tablero[fila][columna].IZQ;
		}

		if (pesoPared == 1){
			pesoPared = 4;  //si la pared elegida aleatoriamente esta cerrada (valor 1) se le asigna un valor alto a pesoPared para que si o si entre en algun if abajo
		}

		if (tablero[fila][columna].ARRIBA<pesoPared && tablero[fila][columna].ARRIBA!=1){
			pared = 0;
			pesoPared = tablero[fila][columna].ARRIBA;
		}
		if (tablero[fila][columna].ABAJO<pesoPared && tablero[fila][columna].ABAJO!=1){
			pared = 1;
			pesoPared = tablero[fila][columna].ABAJO;
		}
		if (tablero[fila][columna].DER<pesoPared && tablero[fila][columna].DER!=1){
			pared = 2;
			pesoPared = tablero[fila][columna].DER;
		}
		if (tablero[fila][columna].IZQ<pesoPared && tablero[fila][columna].IZQ!=1){
			pared = 3;
			pesoPared = tablero[fila][columna].IZQ;
		}
	}else{//Ahora elige pared random a cerrar
		pared = rand()% 4;
		while(pared_check(tablero, fila, columna, pared) == 0){		//Elige hasta encontrar una pared abierta
			pared = rand()% 4;
		}
	}


	//Guardamos la posicion de la caja que acaba de seleccionarse? sirve para tests PCvsPC
	ultCoords[0] = fila;
	ultCoords[1] = columna;


	//Se hace el movimiento
	cajasCerradas = AgregarPared(tablero, fila, columna, pared);

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


int JuegaPC(struct caja **tablero, int print, int turno, int seed){
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
	if (cajas2p==N*N && turno==0 && !cajas3p){
		cajasCerradas = mov_pc(tablero, fila, columna, 2, print, turno, seed);
	}
	else if((tablero[fila][columna].pCerradas==0 || cajas2p==N*N) && !cajas3p)
	{
		fila = rand()%N;
		columna = rand()%N;
		cajasCerradas = mov_pc(tablero, fila, columna, 1, print, turno, seed);
	}
	else if( cajas2p!=N*N || cajas3p!=0 )  //esto es importante para evitar un loop infinito si llamamos mov_pc con absRandom==0
	{
		//Realiza el movimiento en la ultima caja elegida, o un en una caja random
		cajasCerradas = mov_pc(tablero, fila, columna, 0, print, turno, seed);
	}

	return cajasCerradas;
}


int DotsNBoxes(int print, int dim, int from_txt, int iter){

	N = dim;

	int turno = 0;

	//Inicializa puntajes
	puntos[0] = 0;
	puntos[1] = 0;

	//Creacion de tablero
	struct caja **tablero = CrearTablero(N);
	InitBoxes(tablero);
	if(print)
		printf("\n\n\n//////////////////////////////////////////////// NEW GAME ////////////////////////////////////////////////\n\nLa matriz es de: %d x %d \n", N+1, N+1);


	//Ejecucion del juego
	int repite;  //indica si se repite el turno o no
	int cajasAbiertas = N*N;  //cant de cajas abiertas, si llega a 0 termina la partida
	cajas2p = cajas3p = 0;

	//inicia el juego con un tablero leido desde un archivo de texto
	if (from_txt){
		puts("LEYENDO TABLERO DESDE ARCHIVO...");
		if (BoardFromTxt(tablero, &cajasAbiertas)==-1){
			puts("ERROR AL ABRIR EL ARCHIVO");
			return -1;
		}
		puts("\t\t\t\nInicia la partida con el siguiente tablero:");
		PrintBox(tablero);
	}

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
				//printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
				//PrintBox2(tablero);
				printf("\n--------------  PC 1: %d  | PC 2: %d  ------------------ %d\n", puntos[1], puntos[0], iter);
			}else{
				printf("\n--------------  PC 1: %d  | PC 2: %d  ------------------ %d\n", puntos[1], puntos[0], iter);
			}


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
		ganador = DotsNBoxes(1, 3, 1, i);
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
