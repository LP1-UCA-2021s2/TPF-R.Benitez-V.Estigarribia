/*
 * declaracionesGTK.h
 *
 *  Created on: 19 oct. 2021
 *      Author: lp1-2021
 */

#ifndef DECLARACIONESGTK_H_
#define DECLARACIONESGTK_H_

#include <gtk/gtk.h>

GtkBuilder *builder;

GtkWidget *box_tablero;
GtkWidget *grid_tablero;

/*VENTANAS*/
GtkWidget *win_entrada;
GtkWidget *win_principal;
GtkWidget *win_juego;
GtkWidget *win_warning;
GtkWidget *win_salir;
GtkWidget *win_yes_no;
GtkWidget *win_fin;

/*BOTONES*/
GtkWidget *btn_newgame;
GtkWidget *btn_stats;
GtkWidget *btn_help;
GtkWidget *btn_credits;
GtkWidget *btn_exit;
GtkWidget *btn_iniciar;
GtkWidget *btn_atras;
GObject *dialogAcerca;
GObject *dialogAyuda;
GObject *menu_mostrar_acerca;
GObject *menu_mostrar_ayuda;

/*COMBO_BOXES - ENTRIES*/
GtkWidget *name_entry;
//GtkWidget *modo_juego; NO SE USA AUN
GtkWidget *quien_inicia;
GtkWidget *colour;
GtkWidget *matrix_dim;


/*FUNCIONES*/
void JuegoNuevo(GtkWidget *widget, gpointer data);
void nombre(GtkWidget *widget, gpointer data);
void QuienInicia(GtkWidget *widget, gpointer data);
void Color(GtkWidget *widget, gpointer data);
void DimMatriz(GtkWidget *widget, gpointer data);
void IniciarPartida(GtkWidget *widget, gpointer data);
void VolverAInicio(GtkWidget *widget, gpointer data);
void Salir(GtkWidget *widget, gpointer data);
void mostrar_acerca(GtkWidget *widget, gpointer data);
void mostrar_ayuda(GtkWidget *widget, gpointer data);
GtkWidget *CrearTablero();

#endif /* DECLARACIONESGTK_H_ */
