
#ifndef DECLARACIONES_H_
#define DECLARACIONES_H_


int N;  //Dimension de la matriz de cajas (no del tablero, que seria de dim <N+1>))
int turno, color;

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

//Variables que usa la IA para saber que cajas buscar en una jugada.
int cajas2p;  /* Cantidad de cajas que tienen o HAN TENIDO 2 paredes cerradas. Util para saber si aun es posible evitar cajas con pCerradas==2
				   *[ 0 <= cajas2p <= N*N ] */

int cajas3p;  /*Cantidad de cajas que TIENEN 3 paredes cerradas. Si hay alguna caja con pCerradas==3 la IA lo busca hasta encontrarlo.
 	 	 	 	   *[ 0 <= cajas3p <= 2 ] */

int puntos[2];	//puntos[0] contiene los puntos de la PC, puntos[1] los del usuario

void PrintBox(struct caja cajas[][N]);
void InitBoxes(struct caja cajas[][N]);
void ActualizarPeso(struct caja caja[][N], int x, int y);
int AgregarPared(struct caja tablero[][N], int x, int y, int p);
int pared_check(struct caja tablero[][N], int x, int y, int p);
int mov_usuario(struct caja tablero[][N]);
int mov_pc(struct caja tablero[][N], int fila, int columna, int absRandom);
int JuegaPC(struct caja tablero[][N]);


#endif /* DECLARACIONES_H_ */
