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

//Macros usados en Play() unicamente
#define LINEA_HORIZONTAL (i%2==0 && j%2==1)
#define LINEA_VERTICAL (i%2==1 && j%2==0)


enum propiedades_pared {ABIERTA=0, CERRADA=1, PESO=2};  //Constantes simbolicas para operar con las cajas


int ultCoords[] = {0, 0};  //contiene las coordenadas de la ultima caja que se eligio (utilizada por la IA)


char *imagenes[] = {"./IMG/circ.png","./IMG/blanco.png"};




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
			if(i==0){
				cajas[i][j].peso += PESO;
				cajas[i][j].ARRIBA += PESO;
			}if(j==0){
				cajas[i][j].peso += PESO;
				cajas[i][j].IZQ += PESO;
			}if(i==N-1){
				cajas[i][j].peso += PESO;
				cajas[i][j].ABAJO += PESO;
			}if(j==N-1){
				cajas[i][j].peso += PESO;
				cajas[i][j].DER += PESO;
			}
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


void PintarCaja(int x, int y){
	int i = 2*x + 1;
	int j = 2*y + 1;

	if(color==0)
	{
		if(turno == 1){
			gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/rojo.png");
		}else{
			gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/verde.png");
		}
	}
	else
	{
		if(turno == 1){
			gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/verde.png");
		}else{
			gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/rojo.png");
		}
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

			if(x+1<N)
			{
				if(tablero[x+1][y].ARRIBA%2 == ABIERTA){
					cajasCerradas = AgregarPared(tablero, x+1, y, 0);
				}
			}
			break;

		case 2: 	//Agrega a la derecha

			tablero[x][y].DER += CERRADA;

			if(y+1<N)
			{
				if(tablero[x][y+1].IZQ%2 == ABIERTA){
					cajasCerradas = AgregarPared(tablero, x, y+1, 3);
				}
			}
			break;

		case 3: 	//Agrega a la izquierda

			tablero[x][y].IZQ += CERRADA;

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
		PintarCaja(x, y);
	}


	return cajasCerradas;
}

void AgregarLinea(int x, int y, int p){
	int i, j;

	if(p==0 || p==1){  //Linea horizontal

		i = 2*x;
		if(p==1){
			i += 2;
		}
		j = 2*y + 1;
		if(color == 0){
			gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineaG.png");
		}else{
			gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/linea.png");
		}

	}
	if(p==2 || p==3){  //Linea vertical

		j = 2*y;
		if(p==2){
			j += 2;
		}
		i = 2*x + 1;

		if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineavG.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineav.png");
			}
	}

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
	if(absRandom)
	{
		while(tablero[fila][columna].pCerradas == 4){  //absolutamente random
			fila = rand()% N;
			columna = rand()% N;
		}
	}
	else  //Mov random con condiciones extra
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
	printf("\nCoordenadas de la caja: %d, %d", fila, columna);


	//Ahora elige pared random a cerrar
	pared = rand()% 4;
	while(pared_check(tablero, fila, columna, pared) == 0){		//Elige hasta encontrar una pared abierta
		pared = rand()% 4;
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

	//Mov random si es la primera jugada o no quedan movimientos 'convenientes'
	if( (tablero[fila][columna].pCerradas==0 || cajas2p==N*N) && !cajas3p )
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


void Salir(GtkWidget *widget, gpointer data){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(win_salir),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"¿Seguro de quiere salir del juego?");
	gtk_window_set_title(GTK_WINDOW(dialog), "ATENCION");

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_destroy(win_entrada);
	}else{
		gtk_widget_destroy(dialog);
	}
}


void VolverAInicio(GtkWidget *widget, gpointer data){

	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(win_yes_no),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"¿Seguro de quiere volver?");
	gtk_window_set_title(GTK_WINDOW(dialog), "ATENCION");

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_show_all(win_entrada);
		gtk_widget_hide(win_principal);
	}else{
		gtk_widget_destroy(dialog);
	}
}

void JuegoNuevo(GtkWidget *widget, gpointer data){
	gtk_widget_show_all(win_principal);
	gtk_widget_hide(win_entrada);
}

//Pide el nombre del usuario
void nombre(GtkWidget *widget, gpointer data){
	//const gchar *nombre = gtk_entry_get_text (GTK_ENTRY(name_entry));
	//luego poner el nombre en algun label o algo
}

void QuienInicia(GtkWidget *widget, gpointer data){
	turno = gtk_combo_box_get_active(GTK_COMBO_BOX(quien_inicia));
	if(turno == -1){
		turno = 1;
	}
	if(turno == 2){
		srand(time(NULL));
		turno = rand() % 2;
	}
}

void Color(GtkWidget *widget, gpointer data){
	color = gtk_combo_box_get_active(GTK_COMBO_BOX(colour));
	if(color == -1){
		color = 0;  //rojo default
	}
}

//Dimension de la matriz, default = 3
void DimMatriz(GtkWidget *widget, gpointer data){
	const gchar *dimension = gtk_entry_get_text (GTK_ENTRY(matrix_dim));
	int dim = atoi(dimension);
	if(dim < 3 || dim > 15){
		N = 2;
	}else{
		N = dim-1;
	}
}


void FinJuego(char *message) {
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(win_fin),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_YES_NO,
			message, puntos[1], puntos[0]);
	gtk_window_set_title(GTK_WINDOW(dialog), "Fin del juego");

	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_show_all(win_principal);
		gtk_widget_hide(win_juego);
	}else{
		gtk_widget_destroy(dialog);
		gtk_widget_destroy(win_juego);
	}
}


void Play(GtkWidget *event_box, GdkEventButton *event, gpointer data){
	/* Encargado de lo que pasa en la ventana del juego en si.
	 *
	 * Agrega lineas al tablero en respuesta a los clicks del usuario y
	 * muestra los cambios en pantalla despues de cada movimiento,
	 *  ya sea del usuario o de la PC
	 */
	guint i,j;  //coords tablero grafico
	int x, y;  //coords tablero logico (struct caja [][])
	//jugadaExitosa indica si el usuario pudo hacer un movimiento con exito
	int jugadaExitosa = -1;  //tiene que inicializarse con -1 para que la PC no haga un mov en caso de que el usuario presione una posicion invalida
	int repitePC = 0;


	i = (GUINT_FROM_LE(event->y) / 50); //las imagenes tienen son 50x50pixeles
	j = (GUINT_FROM_LE(event->x) / 50);

	struct caja **tablero = data;


	/* Funcionamiento de coordenadas de tablero Grafico -> Logico
	 *
	 * El patron que describe las coordenadas de una caja en el tablero logico a partir de las coordenadas
	 * de una cierta posicion en el tablero grafico es el siguiente:
	 *
	 * - [ i par, j impar ] -> CLICK EN LINEA HORIZONTAL
	 * 		Donde si la linea horizontal no es la correspondiente al 'borde' inferior del tablero:
	 * 			[x, y] == [i/2, (j-1)/2]	y  se  agrega  una  pared  ARRIBA  a  Caja(x,y)
	 *
	 * 		De otro modo:
	 * 			[x, y] == [(i/2)-1, (j-1)/2]	y  se  agrega  una  pared  ABAJO  a  Caja(x,y)
	 *
	 * 		Esto es porque i/2 seria igual a N, y N esta fuera del rango del tablero
	 *
	 *
	 * - [ i impar, j par ] -> CLICK EN LINEA VERTICAL
	 * 		Donde si la linea vertical no es la correspondiente al 'borde' derecho del tablero:
	 * 			[x, y] == [(i-1)/2, j/2]	y  se  agrega  una  pared  IZQ  a  Caja(x,y)
	 *
	 * 		De otro modo:
	 * 			[x, y] == [(i-1)/2, (j/2)-1]	y  se  agrega  una  pared  DER  a  Caja(x,y)
	 *
	 * 		Esto es porque i/2 seria igual a N, y N esta fuera del rango del tablero
	 *
	 * *Caja(x,y)=tablero[x][y]
	 *
	 */
	if(LINEA_HORIZONTAL)
	{
		x = i/2;
		y = (j-1)/2;

		if(x != N){
			jugadaExitosa = mov_usuario(tablero, x, y, 0);
		}else{
			jugadaExitosa = mov_usuario(tablero, x-1, y, 1);
		}

		if(jugadaExitosa != -1){
			if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/linea.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineaG.png");
			}
		}

	}
	else if(LINEA_VERTICAL)
	{
		y = j/2;
		x = (i-1)/2;

		if(y != N){
			jugadaExitosa = mov_usuario(tablero, x, y, 3);
		}else{
			jugadaExitosa = mov_usuario(tablero, x, y-1, 2);
		}
		if(jugadaExitosa != -1){
			if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineav.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineavG.png");
			}
		}
	}

	printf("\n\nHUMANO: %d\n\n", jugadaExitosa);

	//Crea la ventana emergente que contiene la advertencia
	if (jugadaExitosa==-1){
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(win_warning),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_WARNING,
					GTK_BUTTONS_OK,
					"Movimiento invalido. Vuelva a intentarlo");
		gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}

	if(jugadaExitosa == 0)
	{
		repitePC = JuegaPC(tablero);
		cajasAbiertas -= repitePC;

		while(repitePC && cajasAbiertas!=0){
			repitePC = JuegaPC(tablero);
			cajasAbiertas -= repitePC;
		}

	}
	else if(jugadaExitosa != -1)
	{
		cajasAbiertas -= jugadaExitosa;
	}

	PrintBox(tablero);
	puts("\n\n=================================================================\n\n");


	if(cajasAbiertas==0){
		//Mensajes fin de juego
		printf("\n\n 		TERMINO EL JUEGO");
		if (puntos[0] > puntos[1]){
			FinJuego("\nHa perdido con %d puntos contra %d puntos de la computadora.\nDesea volver a jugar?");
		}else if(puntos[0] == puntos[1]){
			FinJuego("\nEmpate. Tus puntos: %d || PC: %d \nDesea volver a jugar?");
		}else{
			FinJuego("\nGanaste. Tus puntos: %d || PC: %d \nDesea volver a jugar?");
		}
		gtk_widget_destroy(grid_tablero);
	}

}


GtkWidget *CrearTablero(){
	/* Crea el tablero grafico y da comienzo a la partida.
	 *
	 * - Retorna el EventBox de la pantalla de juego a IniciarPartida()
	 * - Llama a Play() en respuesta a <button-press-event> en dicho EventBox
	 */

	int i, j;
	GtkWidget *imagen; //auxiliar para cargar la imagen
	GtkWidget *eventbox;  //EventBox del tablero
	eventbox = gtk_event_box_new();
	grid_tablero = gtk_grid_new();  //Grilla que permite colocar en el EventBox las imagenes dependiendo de las coordenadas del click

	//En estos <for> se 'inicializa' el TABLERO GRAFICO, llendando el EventBox de puntos y espacios en blanco
	for (i = 0; i < N*2+1; i++) {
		for (j = 0; j < N*2+1; j++) {
			if(i%2==0&&j%2==0){
				imagen = gtk_image_new_from_file(imagenes[0]);
				gtk_grid_attach(GTK_GRID(grid_tablero), GTK_WIDGET(imagen), j, i, 1, 1);
			}
			else{
				imagen = gtk_image_new_from_file(imagenes[1]);
				gtk_grid_attach(GTK_GRID(grid_tablero), GTK_WIDGET(imagen), j, i, 1, 1);
			}

		}
	}

	gtk_container_add(GTK_CONTAINER(eventbox), grid_tablero);  //vincula la grilla al EventBox


	//INICIALIAZAR VARIABLES USADAS EN LA LOGICA DEL JUEGO
	//Inicializa puntajes
	puntos[0] = 0;
	puntos[1] = 0;

	//Creacion de TABLERO LOGICO
	struct caja **tablero = TableroNuevo(N);
	InitBoxes(tablero);
	PrintBox(tablero);
	puts("\n\n=================================================================\n\n");

	cajas2p = cajas3p = 0;
	cajasAbiertas = N*N;  //cant de cajas abiertas, si llega a 0 termina la partida

	if(!turno){
		JuegaPC(tablero);
	}


	g_signal_connect(eventbox, "button-press-event", G_CALLBACK(Play), tablero);  //llamada a Play()

	return eventbox;
}


//Muestra la pantalla de juego
void IniciarPartida(GtkWidget *widget, gpointer data){
	box_tablero = GTK_WIDGET(gtk_builder_get_object(builder, "box_tablero_j"));
	gtk_box_pack_start(GTK_BOX(box_tablero), CrearTablero(), TRUE, FALSE, 20);  //crea el box del tablero

	gtk_widget_show_all(win_juego);
	gtk_widget_hide(win_principal);
}
