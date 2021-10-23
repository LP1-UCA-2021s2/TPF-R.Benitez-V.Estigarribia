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
	quien_inicia = GTK_WIDGET(gtk_builder_get_object(builder,"cb_jug_inicial"));
	g_signal_connect(quien_inicia,"changed", G_CALLBACK(QuienInicia),NULL);

	colour = GTK_WIDGET(gtk_builder_get_object(builder,"cb_color"));
	g_signal_connect(colour,"changed", G_CALLBACK(Color),NULL);


	gtk_widget_show_all(win_entrada);
	gtk_main();

	return 0;
}
