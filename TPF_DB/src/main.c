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

	GError *error=NULL;
	gtk_init(&argc,&argv);
	builder=gtk_builder_new();

	//Se carga el builder
	if(gtk_builder_add_from_file(builder,"DnB 1.glade",&error)==0){
		g_print("Error en la funcion gtk_builder_add_from_file: \n");
		return 1;
	}

	/*VENTANAS*/
	win_entrada = GTK_WIDGET(gtk_builder_get_object(builder,"win_entrada"));
	g_signal_connect(win_entrada,"destroy",gtk_main_quit,NULL);

	win_principal = GTK_WIDGET(gtk_builder_get_object(builder,"win_principal"));
	g_signal_connect(win_principal,"destroy",gtk_main_quit,NULL);

	win_juego = GTK_WIDGET(gtk_builder_get_object(builder,"win_juego"));
	g_signal_connect(win_juego,"destroy",gtk_main_quit,NULL);


	/*BOTONES*/
	btn_newgame = GTK_WIDGET(gtk_builder_get_object(builder,"btn_star"));
	g_signal_connect(btn_newgame,"clicked", G_CALLBACK(JuegoNuevo),NULL);

	btn_iniciar = GTK_WIDGET(gtk_builder_get_object(builder,"btn_iniciar"));
	g_signal_connect(btn_iniciar,"clicked", G_CALLBACK(IniciarPartida),NULL);


	//name_entry = GTK_WIDGET(gtk_builder_get_object(builder,"txt_jugador"));
	//g_signal_connect(name_entry,"activate", G_CALLBACK(nombre),NULL);

	matrix_dim = GTK_WIDGET(gtk_builder_get_object(builder,"txt_m"));
	g_signal_connect(matrix_dim,"activate", G_CALLBACK(DimMatriz),NULL);

	/*COMBO_BOX*/
	quien_inicia = GTK_WIDGET(gtk_builder_get_object(builder,"cb_modo_juego"));
	g_signal_connect(quien_inicia,"changed", G_CALLBACK(QuienInicia),NULL);

	colour = GTK_WIDGET(gtk_builder_get_object(builder,"cb_color"));
	g_signal_connect(colour,"changed", G_CALLBACK(Color),NULL);


	/*

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

	*/
	gtk_widget_show_all(win_entrada);
	gtk_main();

	return 0;
}
