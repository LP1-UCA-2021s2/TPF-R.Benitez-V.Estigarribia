/*
 ===========================================================================
 Name        : TPF_D&B.c
 Author      : R.Benitez || V.Estigarribia
 Version     :
 Copyright   : Your copyright notice
 Description : Definicion de funciones declaradas en "declaraciones.h"
 ============================================================================
 */

#ifndef DECLARACIONESGTK_H_
#define DECLARACIONESGTK_H_

#include <gtk/gtk.h>

const gchar *nombre1, *nombre2, *dim_matriz;
GtkBuilder *builder;

GtkWidget *box_tablero;
GtkWidget *grid_tablero;

/*VENTANAS*/
GtkWidget *win_entrada;
GtkWidget *win_modo_juego;
GtkWidget *win_principal;
GtkWidget *win_juego;
GtkWidget *win_warning;
GtkWidget *win_salir;
GtkWidget *win_yes_no;
GtkWidget *win_fin;

GtkWidget *lbl_estado;

/*BOTONES*/
GtkWidget *btn_newgame;
GtkWidget *btn_stats;
GObject *btn_help;
GtkWidget *btn_credits;
GtkWidget *btn_exit;
GtkWidget *btn_HvsPC;
GtkWidget *btn_PCvsPC;
GtkWidget *btn_iniciar;
GtkWidget *btn_atras1;
GtkWidget *btn_atras2;
GtkWidget *menu_juego_nuevo;
GtkWidget *menu_salir;
GObject *dialogAcerca;
GObject *menu_mostrar_acerca;
GObject *menu_mostrar_acerca2;
GObject *menu_mostrar_ayuda;
GObject *menu_mostrar_ayuda2;

/*COMBO_BOXES - ENTRIES*/
GtkWidget *name_entry1;
GtkWidget *name_entry2;
GtkWidget *quien_inicia;
GtkWidget *colour;
GtkWidget *matrix_dim;

/*LABELS*/
GtkWidget *lbl_puntos1;
GtkWidget *lbl_name1;
GtkWidget *lbl_puntos2;
GtkWidget *lbl_name2;


/*FUNCIONES*/
void JuegoNuevo(GtkWidget *widget, gpointer data);
void NuevaPartida(GtkWidget *widget, gpointer data);
void ModoUSRvsPC(GtkWidget *widget, gpointer data);
void ModoPCvsPC(GtkWidget *widget, gpointer data);
void ShowStats (GtkWidget *widget, gpointer data);
void VolverAModoJuego (GtkWidget *widget, gpointer data);
void QuienInicia(GtkWidget *widget, gpointer data);
void Color(GtkWidget *widget, gpointer data);
void IniciarPartida(GtkWidget *widget, gpointer data);
void VolverAInicio(GtkWidget *widget, gpointer data);
void Salir(GtkWidget *widget, gpointer data);
void mostrar_acerca(GtkWidget *widget, gpointer data);
void mostrar_ayuda(GtkWidget *widget, gpointer data);
void PintarCaja(int x, int y);
void Play(GtkWidget *event_box, GdkEventButton *event, gpointer data);
GtkWidget *CrearTablero();

#endif /* DECLARACIONESGTK_H_ */
