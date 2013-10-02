/***********************************************************************
 *
 * Copyright (C) 2011-2013  Adrian Solumsmo
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

#define GTK_TYPE_CUSTOM_TABLE             (gtk_custom_table_get_type ())
#define GTK_CUSTOM_TABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CUSTOM_TABLE, GtkCustomTable))
#define GTK_CUSTOM_TABLE_CLASS(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), GTK_CUSTOM_TABLE,  GtkCustomTableClass))
#define GTK_IS_CUSTOM_TABLE(obj)          (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CUSTOM_TABLE))
#define GTK_IS_CUSTOM_TABLE_CLASS(obj)    (G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_CUSTOM_TABLE))
#define GTK_CUSTOM_TABLE_GET_CLASS        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CUSTOM_TABLE, GtkCustomTableClass))
#define GTK_CUSTOM_TABLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_CUSTOM_TABLE, GtkCustomTablePrivate))

#define GTK_CUSTOM_TABLE_ASC 0
#define GTK_CUSTOM_TABLE_DESC 1
#define GTK_CUSTOM_TABLE_INVERT -1

#define PANGO_DEFAULT_FONT "Monospace 10"
#define PANGO_ALIGN_NONE -1

typedef struct _GtkCustomTable GtkCustomTable;
typedef struct _GtkCustomTableClass GtkCustomTableClass;
typedef struct _GtkCustomTablePrivate GtkCustomTablePrivate;

/* structure for per instance public data */
struct _GtkCustomTable {
    
    GtkDrawingArea parent;
};

/* structure for all widget instances */
struct _GtkCustomTableClass {
    
    GtkDrawingAreaClass parent_class;  
};

/* table cell meta data */
struct table_meta {

    char *font;
    char *bg_image;
    double color[3];
    double graph[3];
    gboolean graphable;
    gboolean has_bg_image;
    gboolean has_bg_color;
    PangoAlignment align;
};

/* table cells */
struct table_cell {

    char *text;
    struct table_meta *meta;
};

/* table cols */
struct table_cols {

    struct table_meta *meta;
    struct table_cell **cell;
};

/* table rows */
struct table_rows {

    GtkCustomTablePrivate *priv;

    int row_current;
    int row_genesis;
    struct table_meta *meta;
    struct table_cell **cell;
};

/* table binary tree */
struct table_tree {

    struct table_rows *data;
    struct table_tree *left;
    struct table_tree *right;
};

/* structure for per instance private data */
struct _GtkCustomTablePrivate {

    gboolean table_is_sortable;

    int table_x;
    int table_y;
    int table_tree_index;
    int table_sort_index;
    int table_sort_order;
    int table_has_header;
    int table_has_footer;
    int table_has_primary;
    int table_col_primary;
    int table_min_width;
    int table_max_width;
    int table_min_height;
    int table_max_height;
    int table_row_height;
    int table_scroll_lock;

    int *table_column_widths;
    int *table_column_index;
    int *table_column_hidden;
    int *table_column_widths_temp;
    int *table_column_offset_temp;

    struct table_tree *table_tree;

    struct table_cols **table_cols;
    struct table_rows **table_rows;
    struct table_cell **table_cell;
    struct table_rows *table_head;
    struct table_rows *table_foot;
};


/* important, required for casting */
GtkType gtk_custom_table_get_type(void);

/* widget private functions */
void gtk_custom_table_resize(GtkWidget *table, int cols, int rows);
void gtk_custom_table_calc(GtkCustomTablePrivate *table);
void gtk_custom_table_paint(GtkWidget *table, GdkEventExpose *event);
void gtk_custom_table_tree_free(struct table_tree *tree);
void gtk_custom_table_tree_get_recurse(GtkCustomTablePrivate *priv, 
    struct table_tree *tree, char *value, int col);
void gtk_custom_table_tree_add(struct table_tree *tree, 
    struct table_rows *data, int primary);
void gtk_custom_table_alloc(GtkCustomTablePrivate *priv, int cols, 
    int rows, int column_widths[]);
int gtk_custom_table_is_integer(char *string);
void gtk_custom_table_free_cells(GtkCustomTablePrivate *priv);

/* widget event functions */
gboolean gtk_custom_table_clicked(GtkWidget *table, 
    GdkEventMotion *event);
gboolean gtk_custom_table_mouse_released(GtkWidget *table, 
    GdkEventButton *event);
gboolean gtk_custom_table_expose(GtkWidget *table, 
    GdkEventExpose *event);

/* widget public functions */
GtkWidget * gtk_custom_table_new(int rows, int cols, int min_width, 
    int min_height, int column_widths[]);
void gtk_custom_table_refresh(GtkWidget *table);
void gtk_custom_table_sort(GtkWidget *table, int col, 
    int orientation);
void gtk_custom_table_resize(GtkWidget *table, int cols, 
    int rows);
void gtk_custom_table_free(GtkWidget *table);

/* public getters and setters */
void gtk_custom_table_set_cell_bg_image(GtkWidget *table, int col, int row, 
    char *filename);
void gtk_custom_table_set_cell_color(GtkWidget *table, int col, 
    int row, double rgb[]);
void gtk_custom_table_set_row_color(GtkWidget *table, int row, 
    double rgb[]);
void gtk_custom_table_set_graph_color_col(GtkWidget *table, int col, 
    double rgb[]);
void gtk_custom_table_set_graph_color_cell(GtkWidget *table, int col,  
        int row, double rgb[]);
void gtk_custom_table_set_column_index(GtkWidget *table, int col, 
    gboolean value);
void gtk_custom_table_set_column_shade(GtkWidget *table, int col, 
    gboolean value);
void gtk_custom_table_set_column_alignment(GtkWidget *table, int col, 
    PangoAlignment align);
void gtk_custom_table_set_column_prime(GtkWidget *table, int col, 
    gboolean value);
void gtk_custom_table_set_column_graph(GtkWidget *table, int col, 
    gboolean value);
void gtk_custom_table_set_sortable(GtkWidget *table, gboolean 
    truth);
void gtk_custom_table_set_cell_text(GtkWidget *table, int col, 
    int row, char *text);
void gtk_custom_table_set_cell_alignment(GtkWidget *table, int col, 
    int row, PangoAlignment align);
void gtk_custom_table_set_head_cell_alignment(GtkWidget *table, int col, 
    PangoAlignment align);
void gtk_custom_table_set_foot_cell_alignment(GtkWidget *table, int col, 
    PangoAlignment align);
void gtk_custom_table_set_head_text(GtkWidget *table, int col, 
    char *text);
void gtk_custom_table_set_foot_text(GtkWidget *table, int col, 
    char *text);
void gtk_custom_table_get_row(GtkWidget *table, int index, char ***container);
void gtk_custom_table_set_row_alignment(GtkWidget *table, int row, 
    PangoAlignment align);
void gtk_custom_table_set_head_row_alignment(GtkWidget *table, 
    PangoAlignment align);
void gtk_custom_table_set_foot_row_alignment(GtkWidget *table, 
    PangoAlignment align);
void gtk_custom_table_set_cell_color_enable(GtkWidget *table, int col, 
    int row, gboolean value);
void gtk_custom_table_set_head_cell_font(GtkWidget *table, int col, 
    char *font);
void gtk_custom_table_set_foot_cell_font(GtkWidget *table, int col, 
    char *font);
void gtk_custom_table_set_head_row_font(GtkWidget *table, char *font);
void gtk_custom_table_set_foot_row_font(GtkWidget *table, char *font);
void gtk_custom_table_set_cell_font(GtkWidget *table, int col, 
    int row, char *font);
void gtk_custom_table_set_row_font(GtkWidget *table, int row, 
    char *font);
void gtk_custom_table_set_column_font(GtkWidget *table, int col, 
    char *font);

int gtk_custom_table_get_rows(GtkWidget *table);
int gtk_custom_table_get_cols(GtkWidget *table);
int gtk_custom_table_get_indexof(GtkWidget *table, char *value);

char* gtk_custom_table_get_cell_text(GtkWidget *table, int col, 
    int row);

double checkers[2][3];
double rgb_header[3];
double rgb_footer[3];
double rgb_header_bold[3];
double rgb_border[3];
double rgb_text[3];
double rgb_graph[3];
double rgb_cell[3];

#endif

