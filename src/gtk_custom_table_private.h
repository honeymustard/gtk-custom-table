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

    char *font;             /**< current font */
    char *bg_image;         /**< current background image */
    double color[3];        /**< current background color */
    double graph[3];        /**< current graph background color */
    TextFormat format;      /**< current text format */
    PangoAlignment align;   /**< current text alignment */
    gboolean graphable;     /**< is current row, column, cell graphable */
    gboolean has_format;    /**< does current row, column, cell have a text format */
    gboolean has_bg_image;  /**< does current row, column, cell have a background image */
    gboolean has_bg_color;  /**< does current row, column, cell have a background color */
};

/** structure for table cells */
struct table_cell {

    char *text;       /**< table cell text */
    TableMeta *meta;  /**< table cell meta-data */
};

/** structure for table cols */
struct table_cols {

    TableMeta *meta;  /**< table columns meta-data */
    TableCell **cell; /**< cells in this table columns */
};

/** structure for table rows */
struct table_rows {

    GtkCustomTablePrivate *priv; /**< private table hook */

    int row_current;  /**< current row */
    int row_genesis;  /**< original row */
    TableMeta *meta;  /**< table rows meta-data */
    TableCell **cell; /**< table cells in this row */
};

/** structure for table binary tree */
struct table_tree {

    TableRows *data;  /**< table row */
    TableTree *left;  /**< left table sub-tree */
    TableTree *right; /**< right table sub-tree */
};

/** structure for per instance private data */
struct _GtkCustomTablePrivate {

    gboolean table_is_sortable;     /**< is table sortable */

    int table_x;                    /**< table rows */
    int table_y;                    /**< table cols */
    int table_tree_index;           /**< index of table tree */
    int table_sort_index;           /**< table sort index */
    int table_sort_order;           /**< table sort order */
    int table_has_header;           /**< does table have header */
    int table_has_footer;           /**< does table have footer */
    int table_has_primary;          /**< does table have a primary column */
    int table_col_primary;          /**< primary table column */
    int table_min_width;            /**< table minimum width */
    int table_max_width;            /**< table maximum width.. never used.. */
    int table_min_height;           /**< table minimum height.. never used.. */
    int table_max_height;           /**< table maximum height */
    int table_row_height;           /**< table row height */

    int *table_column_widths;       /**< array of table column widths */
    int *table_column_index;        /**< array */
    int *table_column_hidden;       /**< array of hidden table columns */
    int *table_column_widths_temp;  /**< temporary array for column widths */
    int *table_column_offset_temp;  /**< temporary array for column offsets */

    TableTree *table_tree;          /**< current table search tree */
    TableRows *table_head;          /**< current table header */
    TableRows *table_foot;          /**< current table footer */

    TableCell **table_cell;         /**< current table cells */
    TableCols **table_cols;         /**< current table columns */
    TableRows **table_rows;         /**< current table rows */
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

