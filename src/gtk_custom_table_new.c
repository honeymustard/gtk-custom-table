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


#include "gtk_custom_table.h"
#include "gtk_custom_table_private.h"


/** @file */


/**
 * @brief create a new custom table
 * @param cols          amount of table columns
 * @param rows          amount of table rows
 * @param col_widths    an array with the column widths
 * @return table        returns newly created table widget
 */
GtkWidget * gtk_custom_table_new(int cols, int rows, int col_widths[]) {
    
    GtkWidget *table = g_object_new(GTK_TYPE_CUSTOM_TABLE, NULL);

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* variable defaults */
    priv->table_is_sortable = FALSE;
    priv->table_min_width = 0;
    priv->table_min_height = 0;
    priv->table_row_height = 25;
    priv->table_has_primary = 0;
    priv->table_has_header = 0;
    priv->table_has_footer = 0;
    priv->table_tree = NULL;
    priv->table_sort_index = 0;
    priv->table_sort_order = 0;
    priv->table_tree_index = -1;
    priv->table_col_primary = 2;

    /* create header & footer */
    priv->table_head = malloc(sizeof(TableRows));
    priv->table_foot = malloc(sizeof(TableRows));

    priv->table_head->cell = malloc(sizeof(TableCell *) * cols); 
    priv->table_foot->cell = malloc(sizeof(TableCell *) * cols); 

    int i = 0;
    int j = 0;

    priv->table_head->meta = malloc(sizeof(TableMeta));
    priv->table_head->meta->font = NULL;
    priv->table_head->meta->bg_image = NULL;
    priv->table_head->meta->align = PANGO_ALIGN_NONE;
    priv->table_head->meta->format = FORMAT_NONE;
    priv->table_head->meta->graphable = FALSE;
    priv->table_head->meta->has_format = FALSE;
    priv->table_head->meta->has_bg_color = FALSE;
    priv->table_head->meta->has_bg_image = FALSE;

    for(i = 0; i < 3; i++) {

        priv->table_head->meta->graph[i] = rgb_graph[i];
        priv->table_head->meta->color[i] = rgb_cell[i];
    }

    priv->table_foot->meta = malloc(sizeof(TableMeta));
    priv->table_foot->meta->font = NULL;
    priv->table_foot->meta->bg_image = NULL;
    priv->table_foot->meta->align = PANGO_ALIGN_NONE;
    priv->table_foot->meta->format = FORMAT_NONE;
    priv->table_foot->meta->graphable = FALSE;
    priv->table_foot->meta->has_format = FALSE;
    priv->table_foot->meta->has_bg_color = FALSE;
    priv->table_foot->meta->has_bg_image = FALSE;

    for(i = 0; i < 3; i++) {

        priv->table_foot->meta->graph[i] = rgb_graph[i];
        priv->table_foot->meta->color[i] = rgb_cell[i];
    }

    for(i = 0; i < cols; i++) {

        /* create new table cell */
        priv->table_head->cell[i] = malloc(sizeof(TableCell));
        priv->table_head->cell[i]->text = NULL;

        priv->table_head->cell[i]->meta = malloc(sizeof(TableMeta));
        priv->table_head->cell[i]->meta->font = NULL;
        priv->table_head->cell[i]->meta->bg_image = NULL;
        priv->table_head->cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->table_head->cell[i]->meta->format = FORMAT_NONE;
        priv->table_head->cell[i]->meta->graphable = FALSE;
        priv->table_head->cell[i]->meta->has_format = FALSE;
        priv->table_head->cell[i]->meta->has_bg_color = FALSE;
        priv->table_head->cell[i]->meta->has_bg_image = FALSE;

        for(j = 0; j < 3; j++) {

            priv->table_head->cell[i]->meta->graph[j] = rgb_graph[j];
            priv->table_head->cell[i]->meta->color[j] = rgb_cell[j];
        }

        priv->table_foot->cell[i] = malloc(sizeof(TableCell));
        priv->table_foot->cell[i]->text = NULL;

        priv->table_foot->cell[i]->meta = malloc(sizeof(TableMeta));
        priv->table_foot->cell[i]->meta->font = NULL;
        priv->table_foot->cell[i]->meta->bg_image = NULL;
        priv->table_foot->cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->table_foot->cell[i]->meta->format = FORMAT_NONE;
        priv->table_foot->cell[i]->meta->graphable = FALSE;
        priv->table_foot->cell[i]->meta->format = FALSE;
        priv->table_foot->cell[i]->meta->has_bg_color = FALSE;
        priv->table_foot->cell[i]->meta->has_bg_image = FALSE;

        for(j = 0; j < 3; j++) {

            priv->table_foot->cell[i]->meta->graph[j] = rgb_graph[j];
            priv->table_foot->cell[i]->meta->color[j] = rgb_cell[j];
        }
    }

    priv->table_x = cols;
    priv->table_y = rows;

    gtk_custom_table_alloc(table, col_widths);

    return table;
}

