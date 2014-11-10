/***********************************************************************
 *
 * Copyright (C) 2011-2014  Adrian Solumsmo
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
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


G_BEGIN_DECLS

#define GCT_COL_WIDTH 60 
#define GCT_ROW_HEIGHT 25

typedef struct _GtkCustomTableClass GtkCustomTableClass;
typedef struct _GtkCustomTablePrivate GtkCustomTablePrivate;

/** structure for all widget instances */
struct _GtkCustomTableClass {
    
    /** widget parent class */
    GtkDrawingAreaClass parent_class;  
};

/* structure type definitions */
typedef struct table_meta TableMeta;
typedef struct table_cell TableCell;
typedef struct table_cols TableCols;
typedef struct table_rows TableRows;
typedef struct table_tree TableTree;

/** table row, column and cell meta data */
struct table_meta {

    char *font;                  /**< current font */
    char *bg_image;              /**< current background image */
    double color[3];             /**< current background color */
    double graph[3];             /**< current graph background color */
    GctTextFormat text_format;   /**< current text format */
    PangoAlignment alignment;    /**< current text alignment */
    gboolean has_graph;          /**< does meta have a graph */
    gboolean has_font;           /**< does meta have a font */
    gboolean has_alignment;      /**< does meta have an alignment */
    gboolean has_text_format;    /**< does meta have a text format */
    gboolean has_bg_image;       /**< does meta have a background image */
    gboolean has_bg_color;       /**< does meta have a background color */
};

/** structure for table cells */
struct table_cell {

    char *text;                  /**< table cell text */
    TableMeta *meta;             /**< table cell meta-data */
};

/** structure for table cols */
struct table_cols {

    gboolean index;              /**< is table column an index */
    gboolean hidden;             /**< is table column hidden */
    int width_orig;              /**< original column width */
    int width_temp;              /**< current calculated column width */

    TableMeta *meta;             /**< table columns meta data */
    TableCell **cell;            /**< cells in this table columns */
};

/** structure for table rows */
struct table_rows {

    GtkCustomTablePrivate *priv; /**< private table hook */

    gboolean hidden;             /**< is table row hidden */
    int height_orig;             /**< original row height */
    int height_temp;             /**< current calculated row height */
    int row_orig;                /**< original row number in sequence */
    int row_temp;                /**< current row depending on sort order */

    TableMeta *meta;             /**< table rows meta-data */
    TableCell **cell;            /**< table cells in this row */
};

/** structure for table binary tree */
struct table_tree {

    TableRows *data;             /**< table row */
    TableTree *left;             /**< left table sub-tree */
    TableTree *right;            /**< right table sub-tree */
};

/** structure for per instance private data */
struct _GtkCustomTablePrivate {

    gboolean is_sortable;        /**< is table sortable */
    gboolean has_header;         /**< does table have header */
    gboolean has_footer;         /**< does table have footer */
    gboolean has_primary;        /**< does table have a primary column */

    int x;                       /**< table rows */
    int y;                       /**< table cols */
    int tree_index;              /**< index of table tree */
    int sort_index;              /**< table sort index */
    int sort_order;              /**< table sort order */
    int col_primary;             /**< primary table column */
    int clip_upper;              /**< first visible table row */
    int clip_lower;              /**< last visible table row */
    int hidden_cols;             /**< number of hidden columns */
    int hidden_rows;             /**< number of hidden rows */

    int *col_offset_temp;        /**< temporary array for column offsets */
    int *row_offset_temp;        /**< temporary array for row offsets */

    TableTree *tree;             /**< current table search tree */
    TableRows *head;             /**< current table header */
    TableRows *foot;             /**< current table footer */

    TableCell **cell;            /**< current table cells */
    TableCols **cols;            /**< current table columns */
    TableRows **rows;            /**< current table rows */
};


/* main functions */
void gtk_custom_table_calc_size(GtkWidget *table);
void gtk_custom_table_calc_cols(GtkWidget *table);
void gtk_custom_table_calc_rows(GtkWidget *table);
void gtk_custom_table_calc_clip(GtkWidget *table);
void gtk_custom_table_paint(GtkWidget *table, cairo_t *cr);
void gtk_custom_table_alloc(GtkWidget *table);

/* string functions */
int    gtk_custom_table_string_is_integer(char *string);
gulong gtk_custom_table_string_parseint(char *string);

/* binary tree functions */
void gtk_custom_table_tree_free(TableTree *tree);
void gtk_custom_table_tree_get(GtkWidget *table, TableTree *tree, char *value, int col);
void gtk_custom_table_tree_add(TableTree *tree, TableRows *data, int primary);

/* event functions */
gboolean gtk_custom_table_key_released(GtkWidget *table, GdkEventKey *event);
gboolean gtk_custom_table_clicked(GtkWidget *table, GdkEventMotion *event);
gboolean gtk_custom_table_mouse_released(GtkWidget *table, GdkEventButton *event);
gboolean gtk_custom_table_draw(GtkWidget *table, cairo_t *cr, gpointer data);
gboolean gtk_custom_table_realize(GtkWidget *table, gpointer data);
gboolean gtk_custom_table_destroy(GtkWidget *table, GdkEvent *event);
gboolean gtk_custom_table_scroll(GtkWidget *table, GdkEventScroll *event);
gboolean gtk_custom_table_config(GtkWidget *table, GdkEvent *event);

/* default colors */
double rgb_white[3];
double rgb_gray[3];
double rgb_header[3];
double rgb_footer[3];
double rgb_bold[3];
double rgb_border[3];
double rgb_text[3];
double rgb_graph[3];
double rgb_cell[3];

G_END_DECLS

#endif

