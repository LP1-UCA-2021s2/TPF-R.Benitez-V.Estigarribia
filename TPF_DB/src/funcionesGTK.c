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

char *imagenes[] = {"./IMG/circ.png","./IMG/blanco.png"};


void mostrar_acerca(GtkWidget *widget, gpointer data) {
	gtk_dialog_run(GTK_DIALOG(dialogAcerca) );// mostramos la ventana de diálogo
	gtk_widget_hide(GTK_WIDGET(dialogAcerca) );	// escondemos la ventana
}

void mostrar_ayuda(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new (GTK_WINDOW(win_entrada),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			"Descripción\n"
			"El juego comienza con una cuadrícula vacía de puntos. La cuadrícula puede ser de cualquier tamaño."
			"Los jugadores se turnan para conectar 2 puntos adyacentes horizontal, vertical o diagonalmente separados."
			"Un jugador que completa el cuarto lado de un cuadro 1x1 gana 10 puntos y debe tomar otro turno."
			"El juego termina cuando se dibujan todas las líneas y se reclaman las casillas."
			"El jugador con más puntos gana. Si más de un jugador tiene la misma puntuación alta, el juego es un empate.");
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK){
		gtk_widget_destroy(dialog);
	}
}

void JuegoNuevo(GtkWidget *widget, gpointer data){
	gtk_widget_show_all(win_modo_juego);
	gtk_widget_hide(win_entrada);
}

void ShowStats (GtkWidget *widget, gpointer data) {
	FILE *stats;
	GtkWidget *dialog;
	char *txt=NULL, buff[100];

	stats = fopen ("estadisticas.txt", "r");
	if (stats == NULL)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(win_entrada),
					GTK_DIALOG_MODAL,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,
					"Sin datos que mostrar");
		gtk_window_set_title(GTK_WINDOW(dialog), "ESTADISTICAS");
		if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK){
				gtk_widget_destroy(dialog);
		}
	}
	else
	{
		int txtlen=1;
		txt = (char*)malloc(sizeof(char));
		txt[0]='\0';
		while (fgets (buff, 100, stats)!=NULL){
			printf ("\n==================================\nbuff:\n%s\n", buff);
			txtlen += strlen (buff);
			txt = (char *)realloc(txt, txtlen*sizeof(char));
			strcat (txt, buff);
			printf ("\n-----------------------------------\ntxt:\n%s\n\n", txt);
		}
		fclose (stats);

		GtkWidget *window;
		GtkWidget *view;
		GtkWidget *vbox;
		GtkWidget *scrolledwindow;
		GtkTextBuffer *buffer;
		GtkTextIter iter;

		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
		gtk_window_set_title(GTK_WINDOW(window), "Estadisticas");

		vbox = gtk_box_new(FALSE, 0);
		scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
		view = gtk_text_view_new();
		gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);

		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

		gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

		gtk_text_buffer_insert(buffer, &iter, txt, -1);
		 gtk_container_add(GTK_CONTAINER(scrolledwindow),vbox);
		gtk_container_add(GTK_CONTAINER(window),scrolledwindow);

		gtk_widget_show_all(window);
		free (txt);
	}
}

void Salir(GtkWidget *widget, gpointer data){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(win_salir),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"¿Seguro que desea abandonar el juego?");
	gtk_window_set_title(GTK_WINDOW(dialog), "ATENCION");

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_destroy(win_entrada);
	}else{
		gtk_widget_destroy(dialog);
	}
}


void ModoUSRvsPC(GtkWidget *widget, gpointer data) {

	modoJuego = 0;

	/* Obtencion de nombres. Si fueron ingresados muestra la sgte ventana, si no, muestra un error*/
	nombre1 = gtk_entry_get_text (GTK_ENTRY(name_entry1));
	nombre2 = gtk_entry_get_text (GTK_ENTRY(name_entry2));

	if (nombre1[0] == '\0' || nombre2[0] == '\0') {
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(win_warning),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_WARNING,
						GTK_BUTTONS_OK,
						"Error, complete los campos.");
				gtk_window_set_title(GTK_WINDOW(dialog), "Error");
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
	} else {
		gtk_widget_show_all (win_principal);
		gtk_widget_hide (win_modo_juego);
	}
}


void ModoPCvsPC(GtkWidget *widget, gpointer data) {

	modoJuego = 1;

	/* Obtencion de nombres. Si fueron ingresados muestra la sgte ventana, si no, muestra un error*/
	nombre1 = gtk_entry_get_text (GTK_ENTRY(name_entry1));
	nombre2 = gtk_entry_get_text (GTK_ENTRY(name_entry2));

	if (nombre1[0] == '\0' || nombre2[0] == '\0') {
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(win_warning),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_WARNING,
						GTK_BUTTONS_OK,
						"Error, complete los campos.");
				gtk_window_set_title(GTK_WINDOW(dialog), "Error");
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
	} else {
		gtk_widget_show_all (win_principal);
		gtk_widget_hide (win_modo_juego);
	}
}

void NuevaPartida(GtkWidget *widget, gpointer data){

	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(win_yes_no),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"¿Seguro que desea abandonar la partida?");
	gtk_window_set_title(GTK_WINDOW(dialog), "ATENCION");

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_show_all(win_modo_juego);
		gtk_widget_destroy(grid_tablero);
		gtk_widget_hide(win_juego);
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
				"¿Seguro que desea volver?");
	gtk_window_set_title(GTK_WINDOW(dialog), "ATENCION");

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_show_all(win_entrada);
		gtk_widget_hide(win_modo_juego);
	}else{
		gtk_widget_destroy(dialog);
	}
}

void VolverAModoJuego (GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(win_yes_no),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"¿Seguro que desea volver?");
	gtk_window_set_title(GTK_WINDOW(dialog), "ATENCION");

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES){
		gtk_widget_destroy(dialog);
		gtk_widget_show_all(win_modo_juego);
		gtk_widget_hide(win_principal);
	}else{
		gtk_widget_destroy(dialog);
	}
}

void QuienInicia(GtkWidget *widget, gpointer data){
	//Por default inicia el usuario (o PC local en modo PC vs PC)
	turno = gtk_combo_box_get_active(GTK_COMBO_BOX(quien_inicia));
	if(turno == -1){  //lectura invalida
		turno = 1;
	}
	if(turno == 2){  //Aleatorio
		srand(time(NULL));
		turno = rand() % 2;
	}
}

void Color(GtkWidget *widget, gpointer data){
	color = gtk_combo_box_get_active(GTK_COMBO_BOX(colour));
	printf("\ncolor %d", color);
	if(color == -1){
		color = 0;  //rojo default
	}
	if(color == 2){
		srand(time(NULL));
		color = rand() % 2;
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

		gtk_widget_destroy(grid_tablero);
		gtk_widget_hide(win_juego);
	}else{
		gtk_widget_destroy(dialog);
		gtk_widget_destroy(win_juego);
	}
}

void PintarCaja(int x, int y){
	int i = 2*x + 1;
	int j = 2*y + 1;

	if(color==0)  //Si color==0, el rojo le corresponde al usuario, o en modo PC vs PC, a la PC local
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

void AgregarLinea(int x, int y, int p){
	int i, j;

	if(p==0 || p==1)  //Linea horizontal
	{
		i = 2*x;
		if(p==1){
			i += 2;
		}
		j = 2*y + 1;

		if (turno == 0) {
			if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineaG.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/linea.png");
			}
		} else {
			if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/linea.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineaG.png");
			}
		}
	}
	else if (p==2 || p==3) //Linea vertical
	{
		j = 2*y;
		if(p==2){
			j += 2;
		}
		i = 2*x + 1;

		if (turno == 0) {
			if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineavG.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineav.png");
			}
		} else {
			if(color == 0){
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineav.png");
			}else{
				gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(grid_tablero),j,i)), "IMG/lineavG.png");
			}
		}
	}
}


void PCvsPC (GtkWidget *object, GdkEventButton *event, gpointer data) {
	struct caja **tablero = data;
	int repiteTurno;
	repiteTurno = JuegaPC (tablero);
	cajasAbiertas -= repiteTurno;
	PrintBox(tablero);

	if (!repiteTurno) {
		repiteTurno = JuegaOponente (tablero);
		cajasAbiertas -= repiteTurno;
		PrintBox(tablero);
		while (repiteTurno && cajasAbiertas) {
			repiteTurno = JuegaOponente (tablero);
			cajasAbiertas -= repiteTurno;
			PrintBox(tablero);
		}
	}


	if (!cajasAbiertas) {
		free(tablero);
		//Mensajes fin de juego
		printf("\n\n 		TERMINO EL JUEGO");
		if (puntos[0] > puntos[1]){
			GuardarEstadisticas(-1);
			FinJuego("\nHa perdido con %d puntos contra %d puntos del contrario.\nDesea volver a jugar?");
		}else if(puntos[0] == puntos[1]){
			GuardarEstadisticas(0);
			FinJuego("\nEmpate. Tus puntos: %d || PC: %d \nDesea volver a jugar?");
		}else{
			GuardarEstadisticas(1);
			FinJuego("\nGanaste. Tus puntos: %d || PC: %d \nDesea volver a jugar?");
		}
	}
}


void MostrarEstado () {
	char puntos1_txt[20];
	char puntos2_txt[20];
	sprintf(puntos1_txt, "%d", puntos[1]);
	sprintf(puntos2_txt, "%d", puntos[0]);
	gtk_label_set_label(GTK_LABEL(lbl_puntos1), puntos1_txt);
	gtk_label_set_label(GTK_LABEL(lbl_puntos2), puntos2_txt);
}


void Play(GtkWidget *event_box, GdkEventButton *event, gpointer data){
	/* Encargado de lo que pasa en la ventana del juego en si.
	 *
	 * Agrega lineas al tablero en respuesta a los clicks del usuario y
	 * muestra los cambios en pantalla despues de cada movimiento,
	 *  ya sea del usuario o de la PC
	 */
	guint i,j;  //coords tablero grafico
	int x, y, p;  //coords tablero logico (struct caja [][])
	//jugadaExitosa indica si el usuario pudo hacer un movimiento con exito
	int jugadaExitosa = -1;  //tiene que inicializarse con -1 para que la PC no haga un mov en caso de que el usuario presione una posicion invalida
	int repiteOponente = 0;


	i = (GUINT_FROM_LE(event->y) / 25); //las imagenes tienen son 50x50pixeles
	j = (GUINT_FROM_LE(event->x) / 25);


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
			p = 0;
			jugadaExitosa = mov_usuario(tablero, x, y, p);
		}else{
			p = 1;
			jugadaExitosa = mov_usuario(tablero, x-1, y, p);
		}
	}
	else if(LINEA_VERTICAL)
	{
		y = j/2;
		x = (i-1)/2;

		if(y != N){
			p = 3;
			jugadaExitosa = mov_usuario(tablero, x, y, p);
		}else{
			p = 2;
			jugadaExitosa = mov_usuario(tablero, x, y-1, p);
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
	} else {
		cajasAbiertas -= jugadaExitosa;
	}

	MostrarEstado ();
	if(jugadaExitosa == 0)  //solo si no hemos cerrado una caja le damos al rival la oportunidad de jugar en el siguiente turno
	{

		repiteOponente = JuegaPC(tablero);
		cajasAbiertas -= repiteOponente;
		MostrarEstado ();
		while(repiteOponente && cajasAbiertas!=0){
			repiteOponente = JuegaPC(tablero);
			cajasAbiertas -= repiteOponente;
			MostrarEstado ();
		}
	}

	PrintBox(tablero);
	puts("\n\n=================================================================\n\n");


	if(cajasAbiertas==0){
		for (i = 0; i < N; i++) {
			free (tablero[i]);
		}
		free (tablero);
		//Mensajes fin de juego
		printf("\n\n 		TERMINO EL JUEGO");
		if (puntos[0] > puntos[1]){
			GuardarEstadisticas(-1);
			FinJuego("\nHa perdido con %d puntos contra %d puntos de la computadora.\nDesea volver a jugar?");
		}else if(puntos[0] == puntos[1]){
			GuardarEstadisticas(0);
			FinJuego("\nEmpate. Tus puntos: %d || PC: %d \nDesea volver a jugar?");
		}else{
			GuardarEstadisticas(1);
			FinJuego("\nGanaste. Tus puntos: %d || PC: %d \nDesea volver a jugar?");
		}
	}
}

GtkWidget *CrearTablero(struct caja **tablero){
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
	//Inicializa puntajes etc
	puntos[0] = 0;
	puntos[1] = 0;
	cadenas[0]=100;
	cajas2p = cajas3p = 0;
	cajasAbiertas = N*N;  //cant de cajas abiertas, si llega a 0 termina la partida

	if(turno == 0){  //aca turno==0 dice si se eligio que el rival inicie la partida
		if (modoJuego == 0) JuegaPC(tablero);  //Human vs PC, empieza PC
		if (modoJuego == 1) JuegaOponente(tablero); //PC vs PC, empieza rival
	}

	if (modoJuego == 0) {
		g_signal_connect(eventbox, "button-press-event", G_CALLBACK(Play), tablero);  //llamada a Play()
		return eventbox;
	} else {
		g_signal_connect(eventbox, "button-press-event", G_CALLBACK(PCvsPC), tablero);
		return eventbox;
	}


	return eventbox;
}


//Muestra la pantalla de juego
void IniciarPartida(GtkWidget *widget, gpointer data){

	dim_matriz = gtk_entry_get_text (GTK_ENTRY(matrix_dim));

	if (dim_matriz[0] == '\0') {
		N = 2;
	} else {
		N = atoi (dim_matriz);
		if (N < 2 || N > 15) {
			GtkWidget *dialog;
			dialog = gtk_message_dialog_new(GTK_WINDOW(win_warning),
							GTK_DIALOG_DESTROY_WITH_PARENT,
							GTK_MESSAGE_WARNING,
							GTK_BUTTONS_OK,
							"La dimension del tablero debe estar entre 3 y 15 (inclusivo)");
			gtk_window_set_title(GTK_WINDOW(dialog), "DATOS INCORRECTOS");
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
		}
	}


	gtk_label_set_label(GTK_LABEL(lbl_name1), nombre1);
	gtk_label_set_label(GTK_LABEL(lbl_name2), nombre2);

	//Creacion de TABLERO LOGICO
	struct caja **tablero = TableroNuevo(N);
	InitBoxes(tablero);
	PrintBox(tablero);
	puts("\n\n=================================================================\n\n");

	box_tablero = GTK_WIDGET(gtk_builder_get_object(builder, "box_tablero"));
	gtk_box_pack_start(GTK_BOX(box_tablero), CrearTablero(tablero), TRUE, FALSE, 20);  //crea el box del tablero
	gtk_widget_show_all(win_juego);
	gtk_widget_hide(win_principal);

}
