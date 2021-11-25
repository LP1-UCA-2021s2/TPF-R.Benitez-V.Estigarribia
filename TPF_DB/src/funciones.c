/*
 ===========================================================================
 Name        : TPF_D&B.c
 Author      : R.Benitez || V.Estigarribia
 Version     :
 Copyright   : Your copyright notice
 Description : Definicion de funciones declaradas en "declaraciones.h"
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "declaracionesGTK.h"
#include "declaraciones.h"


enum propiedades_pared {ABIERTA=0, CERRADA=1, PESO=2};  //Constantes simbolicas para operar con las cajas


int ultCoords[] = {0, 0};  //contiene las coordenadas de la ultima caja que se eligio (utilizada por la IA)

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
            printf("%s    ", cajas[i][j].IZQ%2?"|":" ");
        }
        printf("%s", cajas[i][N-1].DER%2?"|":" "); //DER ultima columna
        puts(" ");
    }
    printf("+");
    for(int j=0; j < N; j++){  //ABAJO ultima fila
        printf("%s+",cajas[N-1][j].ABAJO%2?"----":"    ");
    }
}


//Inicializa y cera las paredes
void InitBoxes(struct caja **cajas){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			cajas[i][j].peso = 0;
			cajas[i][j].pCerradas = 0;
			cajas[i][j].id_cadena = 0;
			cajas[i][j].ARRIBA  = ABIERTA;
			cajas[i][j].ABAJO   = ABIERTA;
			cajas[i][j].DER     = ABIERTA;
			cajas[i][j].IZQ     = ABIERTA;
		}
	}
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


//Agrega pared y retorna la cantidad de cajas cerradas en el proceso [0, 1, 2]
int AgregarPared(struct caja **tablero,int x, int y, int p){
	/* Agrega una pared p a la caja en tablero[x][y]
	 *
	 * Ademas, puede cambiar los valores de:
	 * - cajas2p
	 * - cajas3p
	 * - ultCoords[]
	 * - caja->pCerradas
	 * - caja->peso (llamando a ActualiarPeso)
	 */
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

		case 1:		//Agrega abajo

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

		case 2: 	//Agrega a la derecha

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

		case 3: 	//Agrega a la izquierda

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

	/* Tener en cuenta que esta es una funcion recursiva y las operaciones siguientes se ejecutan siempre
	 * en la caja de la primera llamada y en la caja adyacente a esta -en la direccion de la pared agregada- */

	//Suma una pared cerrada
	tablero[x][y].pCerradas += 1;

	//Actualiza el peso de las cajas adyacentes
	if(tablero[x][y].pCerradas == 2){
		ActualizarPeso(tablero, x, y);
	}

	//Si tiene 2 paredes incrementa cajas2p
	if(tablero[x][y].pCerradas==2)
		cajas2p += 1;

	//Si tiene 3 paredes incrementa cajas3p y se guarda sus coords para la sgte jugada de la IA
	if(tablero[x][y].pCerradas==3){
		cajas3p += 1;
		ultCoords[0] = x;
		ultCoords[1] = y;
	}

	//Controlar si la jugada realizada cerro la caja actual y actualiza las variables afectadas
	if (tablero[x][y].pCerradas == 4){
		cajasCerradas += 1;  //local
		cajas3p -= 1;  //global
		if (tablero[x][y].id_cadena != 0) {
			cadenas[tablero[x][y].id_cadena] = 100;
		}
		PintarCaja(x, y);
	}


	return cajasCerradas;
}

//Controla que las paredes no esten cerradas [0, 1]
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


int Concatenar (struct caja **tablero, int x, int y, int id, int longitud) {

	tablero[x][y].id_cadena = id;

	if (tablero[x][y].ARRIBA%2 == ABIERTA) {
		if (x-1 >= 0) {
			if (tablero[x-1][y].id_cadena == 0) {
				longitud = Concatenar (tablero, x-1, y, id, longitud);
			}
		}
	}
	if (tablero[x][y].ABAJO%2 == ABIERTA) {
		if (x+1 < N) {
			if (tablero[x+1][y].id_cadena == 0) {
				longitud = Concatenar (tablero, x+1, y, id, longitud);
			}
		}
	}
	if (tablero[x][y].IZQ%2 == ABIERTA) {
		if (y-1 >= 0) {
			if (tablero[x][y-1].id_cadena == 0) {
				longitud = Concatenar (tablero, x, y-1, id, longitud);
			}
		}
	}
	if (tablero[x][y].DER%2 == ABIERTA) {
		if (y+1 < N) {
			if (tablero[x][y+1].id_cadena == 0) {
				longitud = Concatenar (tablero, x, y+1, id, longitud);
			}
		}
	}

	return ++longitud;
}


void BuscarCadenas (struct caja **tablero) {
	int long_cadena, id_cadena = 1;
	for (int i=0; i<N; i++) {
		for (int j=0; j<N; j++) {
			if (tablero[i][j].id_cadena == 0 && tablero[i][j].pCerradas != 4){
				long_cadena = Concatenar (tablero, i, j, id_cadena, 0);
				cadenas[id_cadena++] = long_cadena;
			}
		}
	}
	printf("\n\n ------------ Cadenas formadas: %d -----------\n", id_cadena-1);
	cadenas[0] = -1;  //indica al programa que las cadenas ya estan creadas
}


//Juega el humano, retorna la cantidad de cajas cerradas en un movimiento o -1 si no logro realizarse una jugada valida
int mov_usuario(struct caja **tablero, int i, int j, int p){

	turno = 1;

	//Pedir pared a cerrar
	if(pared_check(tablero, i, j, p)==0){		//La pared seleccionada esta cerrada
		return -1;
	}


	//Guardamos la posicion de la caja que acaba de seleccionarse (para la IA hehe)
	ultCoords[0] = i;
	ultCoords[1] = j;


	//Se hace el movimiento
	int cajasCerradas;
	cajasCerradas = AgregarPared(tablero, i, j, p);

	//Suma los puntos acorde a la cant de cajas cerradas
	if (cajasCerradas){
		puntos[1] += 10 * cajasCerradas;
		printf("\nHas cerrado %d caja%s", cajasCerradas, (cajasCerradas==2)?"s":"");
		printf("\n	Se te han sumado %d puntos. Tienes %d puntos.", 10*cajasCerradas, puntos[1]);
	}


	return cajasCerradas;

}


//Movimiento de la pc (random o con condiciones extra), retorna la cantidad de cajas cerradas en un movimiento
int mov_pc(struct caja **tablero, int fila, int columna, int absRandom){
	/* Hace un movimiento en una caja random o en la fila y columna indicadas.
	 *
	 * Mov random:
	 * - Si absRandom==True, hace un movimiento absolutamente random.
	 * - De lo contrario elige una caja random que ademas sea una caja conveniente (tenga pCerradas!=2) o
	 * busca una con pCerradas==3 si cajas3p!=0
	 */

	srand(time(NULL));
	int cajasCerradas, pared;

	//Si en (fila, columna) no hay una caja valida, se buscan coordenadas random hasta encontrar una.
	if(absRandom == 1)
	{
		while(tablero[fila][columna].pCerradas == 4){  //absolutamente random
			fila = rand()% N;
			columna = rand()% N;
		}
	}
	else if (absRandom == 0) //Mov random con condiciones extra
	{
		if(cajas3p){
			while( tablero[fila][columna].pCerradas==4 || tablero[fila][columna].pCerradas!=3 ){
				fila= rand()% N;
				columna = rand()% N;
			}
		}else{
			while( tablero[fila][columna].pCerradas==4 || tablero[fila][columna].pCerradas==2 ){
				fila= rand()% N;
				columna = rand()% N;
			}
		}
	}
	else if (absRandom == 2)
	{
		puts("\n\n\tELIGIENDO CAJA DE CADENA");
		int j, aux = 100;
		for (int i=1; i<100; i++) {
			if (cadenas[i] < aux) {
				aux = cadenas[i];
				j = i;
			}
		}
		printf("\n\n\t\tLISTA DE CADENAS: [%d, %d, %d, %d]\n\t\tCADENA ELEGIDA: %d\n", cadenas[1], cadenas[2], cadenas[3], cadenas[4], j);
		cadenas[j] = 100;
		while (tablero[fila][columna].id_cadena != j) {
			fila = rand()%N;
			columna = rand()%N;
		}
	}
	printf("\nCoordenadas de la caja: %d, %d", fila, columna);


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

	printf("\n\n0:Arriba\n1:Abajo\n2:Derecha\n3:Izquierda\nPared a cerrar: %d", pared);


	//Se hace el movimiento
	cajasCerradas = AgregarPared(tablero, fila, columna, pared);

	//Se suman los puntos acorde a la cant de cajas cerradas
	if (cajasCerradas){
		puntos[0] += 10 * cajasCerradas;
		printf("\nLa PC ha cerrado %d caja%s", cajasCerradas, (cajasCerradas==2)?"s":"");
		printf("\n	PC gano %d puntos. Ahora tiene %d puntos.", 10*cajasCerradas, puntos[0]);
	}

	AgregarLinea(fila, columna, pared);


	return cajasCerradas;
}


//Movimiento de la pc con IA, retorna la cantidad de cajas cerradas en un movimiento
int JuegaPC(struct caja **tablero){
	/* Hace un movimiento evitando las cajas que ya tengan dos paredes << pCerradas==2 >>
	 * (ya que al agregarle la tercera le daria ventaja al oponente)
	 *
	 * Tambien, intenta realizar el movimiento en la ultima caja seleccionada por el oponente,
	 * ya que es altamente probable realizar un movimiento conveniente en esa caja
	 *
	 *  Algunas variables utilizadas:
	 * 		cajas2p: cantidad de cajas con << pCerradas != 2 >>.
	 * 		cajasCerradas: cantidad de cajas cerradas en una jugada
	 */

	turno = 0;

	srand(time(NULL));
	int fila, columna, cajasCerradas;
	fila = ultCoords[0];
	columna = ultCoords[1];

	if (cajas2p == N*N && cadenas[0] != -1) {
		for (int i=0; i<100; i++) {
			cadenas[i] = 100;
		}
		BuscarCadenas(tablero);
	}

	if (cajas2p == N*N && !cajas3p) {
		cajasCerradas = mov_pc(tablero, fila, columna, 2);
	}

	//Mov random si es la primera jugada
	else if(tablero[fila][columna].pCerradas==0)
	{
		fila = rand()%N;
		columna = rand()%N;
		cajasCerradas = mov_pc(tablero, fila, columna, 1);
	}
	else if( cajas2p != N*N || cajas3p )  //esto es importante para evitar un loop infinito si llamamos mov_pc con absRandom==0
	{
		//Realiza el movimiento en la ultima caja elegida, o un en una caja random
		cajasCerradas = mov_pc(tablero, fila, columna, 0);
	}
	printf("\n- cajas2p: %d", cajas2p);

	return cajasCerradas;
}


void EnviarJugada (int i, j, p) {
	int x1, y1, x2, y2;
	char nombreArchLocal[30];
	FILE *archLocal;

	sprintf (nombreArchLocal, "%s.txt", nombre1);

	if (p == 0) {
		x1 = x2 = i +1;
		y1 = j +1;
		y2 = j +2;
	}
	else if (p == 1) {
		x1 = x2 = i +2;
		y1 = j +1;
		y2 = j +2;
	}
	else if (p == 2) {
		y1 = y2 = j +2;
		x1 = i +1;
		x2 = i +2;
	}
	else if (p == 3) {
		y1 = y2 = j +1;
		x1 = i +1;
		x2 = i +2;
	}

	archLocal = fopen (nombreArchLocal, "w");

	fprintf (archLocal, "%d,%d\n", x1, y1);
	fprintf (archLocal, "%d,%d", x2, y2);

	fclose (archLocal);

}


//PCvsPC
int JuegaOponente(struct caja **tablero){

	int x1, y1, x2, y2, i, j, p, cajasCerradas;

	char nombreArchOponente[30];
	sprintf (nombreArchOponente, "%s.txt", nombre2);

	FILE *archOponente;
	archOponente = fopen (nombreArchOponente, "r");
	while (archOponente == NULL) {  //busca el archivo hasta poder leerlo
		archOponente = fopen (nombreArchOponente, "r");
	}

	fscanf (archOponente, "%d,%d\n", &x1, &y1);
	fscanf (archOponente, "%d,%d", &x2, &y2);

	fclose (archOponente);
	remove (nombreArchOponente);

	/* Pasar de coordenadas de dos puntos a coordenadas de una caja y la pared correspondiente
	 * - Se asume que el movimiento es valido*/
	if (y1 == y2) {
		if (y1 != N+1) {
			j = y1 -1;
			p = 3;
		} else {
			j = y1 -2;
			p = 2;
		}
		if (x2 < x1) {
			i = x2 -1;
		} else {
			i = x1 -1;
		}
	} else if (x1 == x2) {
		if (x1 != N+1) {
			i = x1 -1;
			p = 0;
		} else {
			i = x1 -2;
			p = 1;
		}
		if (y2 < y1) {
			j = y2 -1;
		} else {
			j = y1 -1;
		}
	}

	cajasCerradas = AgregarPared (tablero, i, j, p);
	if (cajasCerradas){
		puntos[1] += 10 * cajasCerradas;
		printf("\nLa PC rival ha cerrado %d caja%s", cajasCerradas, (cajasCerradas==2)?"s":"");
		printf("\n	PC rival gano %d puntos. Ahora tiene %d puntos.", 10*cajasCerradas, puntos[1]);
	}

	AgregarLinea(i, j, p);

	return cajasCerradas;
}


//Crea el tablero logico
struct caja **
TableroNuevo(int size){
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

