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
	if(gtk_builder_add_from_file(builder,"D&B grafics.glade",&error)==0){
		g_print("Error en la funcion gtk_builder_add_from_file: \n");
		return 1;
	}


	/*VENTANAS*/
	win_entrada = GTK_WIDGET(gtk_builder_get_object(builder,"win_entrada"));
	g_signal_connect(win_entrada,"destroy",gtk_main_quit,NULL);

	win_modo_juego = GTK_WIDGET(gtk_builder_get_object(builder,"win_modo_juego"));
	g_signal_connect(win_modo_juego,"destroy",gtk_main_quit,NULL);

	win_principal = GTK_WIDGET(gtk_builder_get_object(builder,"win_principal"));
	g_signal_connect(win_principal,"destroy",gtk_main_quit,NULL);

	win_juego = GTK_WIDGET(gtk_builder_get_object(builder,"win_juego"));
	g_signal_connect(win_juego,"destroy",gtk_main_quit,NULL);


	/*BOTONES*/
	btn_newgame = GTK_WIDGET(gtk_builder_get_object(builder,"btn_star"));
	g_signal_connect(btn_newgame,"button-press-event", G_CALLBACK(JuegoNuevo),NULL);

	btn_stats = GTK_WIDGET(gtk_builder_get_object(builder,"btn_est"));
	g_signal_connect(btn_stats,"button-press-event", G_CALLBACK(ShowStats),NULL);

	btn_help = gtk_builder_get_object(builder, "btn_ver");
	g_signal_connect(btn_help, "button-press-event", G_CALLBACK (mostrar_ayuda), NULL);

	btn_exit = GTK_WIDGET(gtk_builder_get_object(builder,"btn_exi"));
	g_signal_connect(btn_exit,"button-press-event", G_CALLBACK(Salir),NULL);

	btn_HvsPC = GTK_WIDGET(gtk_builder_get_object(builder,"btn_humano_vs_pc"));
	g_signal_connect(btn_HvsPC,"button-press-event", G_CALLBACK(ModoUSRvsPC), NULL);

	btn_PCvsPC = GTK_WIDGET(gtk_builder_get_object(builder,"btn_pc_vs_pc"));
	g_signal_connect(btn_PCvsPC,"button-press-event", G_CALLBACK(ModoPCvsPC), NULL);

	btn_iniciar = GTK_WIDGET(gtk_builder_get_object(builder,"btn_inicio"));
	g_signal_connect(btn_iniciar,"clicked", G_CALLBACK(IniciarPartida),NULL);

	btn_atras1 = GTK_WIDGET(gtk_builder_get_object(builder,"btn_atras1"));
	g_signal_connect(btn_atras1,"clicked", G_CALLBACK(VolverAInicio),NULL);

	btn_atras2 = GTK_WIDGET(gtk_builder_get_object(builder,"btn_atras2"));
	g_signal_connect(btn_atras2,"clicked", G_CALLBACK(VolverAModoJuego),NULL);

	dialogAcerca = gtk_builder_get_object(builder, "win_acercad");

	menu_mostrar_acerca = gtk_builder_get_object(builder, "imagemenuitem11");
	g_signal_connect(menu_mostrar_acerca, "activate", G_CALLBACK (mostrar_acerca), NULL);

	menu_mostrar_acerca2 = gtk_builder_get_object(builder, "imagemenuitem16");
	g_signal_connect(menu_mostrar_acerca2, "activate", G_CALLBACK (mostrar_acerca), NULL);


	menu_mostrar_ayuda = gtk_builder_get_object(builder, "ver_itm");
	g_signal_connect(menu_mostrar_ayuda, "activate", G_CALLBACK (mostrar_ayuda), NULL);

	menu_mostrar_ayuda2 = gtk_builder_get_object(builder, "ver_itm1");
	g_signal_connect(menu_mostrar_ayuda2, "activate", G_CALLBACK (mostrar_ayuda), NULL);



	name_entry1 = GTK_WIDGET(gtk_builder_get_object(builder,"txt_jugador1"));
	name_entry2 = GTK_WIDGET(gtk_builder_get_object(builder,"txt_jugador2"));

	matrix_dim = GTK_WIDGET(gtk_builder_get_object(builder,"txt_m"));

	/*COMBO_BOX*/
	quien_inicia = GTK_WIDGET(gtk_builder_get_object(builder,"cb_jug_inicial"));
	g_signal_connect(quien_inicia,"changed", G_CALLBACK(QuienInicia),NULL);

	colour = GTK_WIDGET(gtk_builder_get_object(builder,"cb_color"));
	g_signal_connect(colour,"changed", G_CALLBACK(Color),NULL);

	/*LABELS*/
	lbl_puntos = GTK_WIDGET(gtk_builder_get_object(builder,"lbl_estado"));
	lbl_name = GTK_WIDGET(gtk_builder_get_object(builder,"lbl_na"));


	gtk_widget_show_all(win_entrada);
	gtk_main();

	return 0;
}
