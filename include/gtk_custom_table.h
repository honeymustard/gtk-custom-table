/***********************************************************************
 *
 * Copyright (C) 2011-2014  Adrian Solumsmo
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/


#ifndef __GTKCUSTOMTABLE_H__
#define __GTKCUSTOMTABLE_H__

#include <pango/pangocairo.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

/* important, required for casting */
GType gtk_custom_table_get_type(void);

#define GTK_TYPE_CUSTOM_TABLE             (gtk_custom_table_get_type ())
#define GTK_CUSTOM_TABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CUSTOM_TABLE, GtkCustomTable))
#define GTK_CUSTOM_TABLE_CLASS(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), GTK_CUSTOM_TABLE,  GtkCustomTableClass))
#define GTK_IS_CUSTOM_TABLE(obj)          (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CUSTOM_TABLE))
#define GTK_IS_CUSTOM_TABLE_CLASS(obj)    (G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_CUSTOM_TABLE))
#define GTK_CUSTOM_TABLE_GET_CLASS        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CUSTOM_TABLE, GtkCustomTableClass))
#define GTK_CUSTOM_TABLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_CUSTOM_TABLE, GtkCustomTablePrivate))

/* table orientation definitions */
#define GTK_CUSTOM_TABLE_ASC 0
#define GTK_CUSTOM_TABLE_DESC 1
#define GTK_CUSTOM_TABLE_INVERT -1

#define PANGO_DEFAULT_FONT "Monospace 10"
#define PANGO_CONTENT_FONT "sans 10"
#define PANGO_ALIGN_NONE -1

/* text format definitions */
typedef enum { 

    FORMAT_NONE, 
    FORMAT_INTEGER_SIGNED, 
    FORMAT_INTEGER_UNSIGNED

} TextFormat;


/** structure for per instance public data */
typedef struct _GtkCustomTable GtkCustomTable;

/** custom table structure */
struct _GtkCustomTable {

    GtkDrawingArea parent; /**< parent widget */
};


/* main functions */
GtkWidget* gtk_custom_table_new(int cols, int rows, int col_widths[]);
void  gtk_custom_table_refresh(GtkWidget *table);
void  gtk_custom_table_sort(GtkWidget *table, int col, int orientation);
void  gtk_custom_table_resize(GtkWidget *table, int cols, int rows);
void  gtk_custom_table_free(GtkWidget *table);

/* string functions */
int   gtk_custom_table_string_is_integer(char *string);
unsigned long gtk_custom_table_string_parseint(char *string);
char* gtk_custom_table_string_thousand_separator(char *string);
char* gtk_custom_table_string_strip_alpha(char *string);

/* misc functions */
int   gtk_custom_table_get_width(GtkWidget *table);
int   gtk_custom_table_get_height(GtkWidget *table);
int   gtk_custom_table_get_indexof(GtkWidget *table, char *value);
void  gtk_custom_table_set_sortable(GtkWidget *table, gboolean truth);
void  gtk_custom_table_set_graph_color_col(GtkWidget *table, int col, double rgb[]);
void  gtk_custom_table_set_graph_color_cell(GtkWidget *table, int col, int row, double rgb[]);

/* cell functions */
void  gtk_custom_table_set_cell_bg_image(GtkWidget *table, int col, int row, char *filename);
void  gtk_custom_table_set_cell_color(GtkWidget *table, int col, int row, double rgb[]);
void  gtk_custom_table_set_cell_color_enable(GtkWidget *table, int col, int row, gboolean value);
void  gtk_custom_table_set_cell_text(GtkWidget *table, int col, int row, char *text);
void  gtk_custom_table_set_cell_alignment(GtkWidget *table, int col, int row, PangoAlignment align);
void  gtk_custom_table_set_cell_bg_image_enable(GtkWidget *table, int col, int row, gboolean value);
void  gtk_custom_table_set_cell_font(GtkWidget *table, int col, int row, char *font);
char* gtk_custom_table_get_cell_text(GtkWidget *table, int col, int row);

/* row functions */
void  gtk_custom_table_set_row_color(GtkWidget *table, int row, double rgb[]);
void  gtk_custom_table_set_row_alignment(GtkWidget *table, int row, PangoAlignment align);
void  gtk_custom_table_set_row_font(GtkWidget *table, int row, char *font);
int   gtk_custom_table_get_rows(GtkWidget *table);

/* col functions */
void  gtk_custom_table_set_col_index(GtkWidget *table, int col, gboolean value);
void  gtk_custom_table_set_col_hide(GtkWidget *table, int col, gboolean value);
void  gtk_custom_table_set_col_alignment(GtkWidget *table, int col, PangoAlignment align);
void  gtk_custom_table_set_col_prime(GtkWidget *table, int col, gboolean value);
void  gtk_custom_table_set_col_graph(GtkWidget *table, int col, gboolean value);
void  gtk_custom_table_set_col_format(GtkWidget *table, int col, TextFormat format);
void  gtk_custom_table_set_col_font(GtkWidget *table, int col, char *font);
int   gtk_custom_table_get_cols(GtkWidget *table);

/* header functions */
void  gtk_custom_table_set_head_cell_alignment(GtkWidget *table, int col, PangoAlignment align);
void  gtk_custom_table_set_head_cell_font(GtkWidget *table, int col, char *font);
void  gtk_custom_table_set_head_text(GtkWidget *table, int col, char *text);
void  gtk_custom_table_set_head_row_alignment(GtkWidget *table, PangoAlignment align);
void  gtk_custom_table_set_head_row_font(GtkWidget *table, char *font);

/* footer functions */
void  gtk_custom_table_set_foot_cell_alignment(GtkWidget *table, int col, PangoAlignment align);
void  gtk_custom_table_set_foot_cell_font(GtkWidget *table, int col, char *font);
void  gtk_custom_table_set_foot_text(GtkWidget *table, int col, char *text);
void  gtk_custom_table_set_foot_row_alignment(GtkWidget *table, PangoAlignment align);
void  gtk_custom_table_set_foot_row_font(GtkWidget *table, char *font);

#endif

