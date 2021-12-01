
#ifndef DECLARACIONES_H_
#define DECLARACIONES_H_

#define PCvsPC (1)
#define USRvsPC (0)

int N;  //Dimension de la matriz de cajas (no del tablero, que seria de dim <N+1>))

/*cadenas: el index del array corresponde al identificador de la cadena, y el valor contenido es la long de la cadena, cadena[0] siempre se ignora
 porque el 0 se usa para indicar que una caja no pertenece a una cadena.
 Todas las posiciones se inicializan en 100 para poder buscar el menor valor
 */
int cadenas[100];

// Si el turno==0, juega el oponente (IA o PC rival). Si modoJuego==0, se juega Human vs PC
int turno, color, cajasAbiertas, modoJuego;

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
	 * 		- Por tanto  0 <= peso <= 8 para cada caja
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
	unsigned int id_cadena;  //indica si la caja pertenece a una 'cadena' de cajas, 0 si es que no, cualquier entero si es que si
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

void PrintBox(struct caja **cajas);
void InitBoxes(struct caja **cajas);
void ActualizarPeso(struct caja **caja, int x, int y);
int AgregarPared(struct caja **tablero, int x, int y, int p);
void AgregarLinea(int x, int y, int p);
int pared_check(struct caja **tablero, int x, int y, int p);
int mov_usuario(struct caja **tablero, int i, int j, int p);
int mov_pc(struct caja **tablero, int fila, int columna, int absRandom);
int JuegaPC(struct caja **tablero);
int JuegaOponente(struct caja **tablero);
void EnviarJugada (int i, int j, int p);
void GuardarEstadisticas (int result);
struct caja **TableroNuevo(int size);


#endif /* DECLARACIONES_H_ */
