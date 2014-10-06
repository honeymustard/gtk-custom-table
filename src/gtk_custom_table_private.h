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


#ifndef __GTKCUSTOMTABLEPRIVATE_H__
#define __GTKCUSTOMTABLEPRIVATE_H__

#include <pango/pangocairo.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>


typedef struct _GtkCustomTableClass GtkCustomTableClass;
typedef struct _GtkCustomTablePrivate GtkCustomTablePrivate;

/* structure for all widget instances */
struct _GtkCustomTableClass {
    
    GtkDrawingAreaClass parent_class;  
};

/* structure type definitions */
typedef struct table_meta TableMeta;
typedef struct table_cell TableCell;
typedef struct table_cols TableCols;
typedef struct table_rows TableRows;
typedef struct table_tree TableTree;

/* table cell meta data */
struct table_meta {

    char *font;
    char *bg_image;
    double color[3];
    double graph[3];
    TextFormat format;
    PangoAlignment align;
    gboolean graphable;
    gboolean has_format;
    gboolean has_bg_image;
    gboolean has_bg_color;
};

/* table cells */
struct table_cell {

    char *text;
    TableMeta *meta;
};

/* table cols */
struct table_cols {

    TableMeta *meta;
    TableCell **cell;
};

/* table rows */
struct table_rows {

    GtkCustomTablePrivate *priv;

    int row_current;
    int row_genesis;
    TableMeta *meta;
    TableCell **cell;
};

/* table binary tree */
struct table_tree {

    TableRows *data;
    TableTree *left;
    TableTree *right;
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
    int table_max_width;  /* never used.. */
    int table_min_height; /* never used.. */
    int table_max_height;
    int table_row_height;

    int *table_column_widths;
    int *table_column_index;
    int *table_column_hidden;
    int *table_column_widths_temp;
    int *table_column_offset_temp;

    TableTree *table_tree;
    TableRows *table_head;
    TableRows *table_foot;

    TableCell **table_cell;
    TableCols **table_cols;
    TableRows **table_rows;
};


/* internal functions */
void gtk_custom_table_calc(GtkWidget *table);
void gtk_custom_table_paint(GtkWidget *table, cairo_t *cr);
void gtk_custom_table_alloc(GtkWidget *table, int column_widths[]);
void gtk_custom_table_free_cells(GtkWidget *table);

/* internal binary tree functions */
void gtk_custom_table_tree_free(TableTree *tree);
void gtk_custom_table_tree_get(GtkWidget *table, TableTree *tree, char *value, int col);
void gtk_custom_table_tree_add(TableTree *tree, TableRows *data, int primary);

/* internal event functions */
gboolean gtk_custom_table_key_released(GtkWidget *table, GdkEventKey *event);
gboolean gtk_custom_table_clicked(GtkWidget *table, GdkEventMotion *event);
gboolean gtk_custom_table_mouse_released(GtkWidget *table, GdkEventButton *event);
gboolean gtk_custom_table_draw(GtkWidget *table, cairo_t *cr, gpointer data);
gboolean gtk_custom_table_destroy(GtkWidget *table, GdkEvent *event);
gboolean gtk_custom_table_scroll(GtkWidget *table, GdkEventScroll *event);
gboolean gtk_custom_table_config(GtkWidget *table, GdkEvent *event);

/* internal default colors */
double checkers[2][3];
double rgb_header[3];
double rgb_footer[3];
double rgb_header_bold[3];
double rgb_border[3];
double rgb_text[3];
double rgb_graph[3];
double rgb_cell[3];

#endif

