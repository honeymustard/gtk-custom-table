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
    priv->is_sortable = FALSE;
    priv->row_height = 25;
    priv->has_primary = 0;
    priv->has_header = 0;
    priv->has_footer = 0;
    priv->tree = NULL;
    priv->sort_index = 0;
    priv->sort_order = 0;
    priv->tree_index = -1;
    priv->col_primary = 2;

    /* create header & footer */
    priv->head = malloc(sizeof(TableRows));
    priv->foot = malloc(sizeof(TableRows));

    priv->head->cell = malloc(sizeof(TableCell *) * cols); 
    priv->foot->cell = malloc(sizeof(TableCell *) * cols); 

    int i = 0;
    int j = 0;

    priv->head->meta = malloc(sizeof(TableMeta));
    priv->head->meta->font = NULL;
    priv->head->meta->bg_image = NULL;
    priv->head->meta->align = PANGO_ALIGN_NONE;
    priv->head->meta->format = FORMAT_NONE;
    priv->head->meta->graphable = FALSE;
    priv->head->meta->has_format = FALSE;
    priv->head->meta->has_bg_color = FALSE;
    priv->head->meta->has_bg_image = FALSE;

    for(i = 0; i < 3; i++) {

        priv->head->meta->graph[i] = rgb_graph[i];
        priv->head->meta->color[i] = rgb_cell[i];
    }

    priv->foot->meta = malloc(sizeof(TableMeta));
    priv->foot->meta->font = NULL;
    priv->foot->meta->bg_image = NULL;
    priv->foot->meta->align = PANGO_ALIGN_NONE;
    priv->foot->meta->format = FORMAT_NONE;
    priv->foot->meta->graphable = FALSE;
    priv->foot->meta->has_format = FALSE;
    priv->foot->meta->has_bg_color = FALSE;
    priv->foot->meta->has_bg_image = FALSE;

    for(i = 0; i < 3; i++) {

        priv->foot->meta->graph[i] = rgb_graph[i];
        priv->foot->meta->color[i] = rgb_cell[i];
    }

    for(i = 0; i < cols; i++) {

        /* create new table cell */
        priv->head->cell[i] = malloc(sizeof(TableCell));
        priv->head->cell[i]->text = NULL;

        priv->head->cell[i]->meta = malloc(sizeof(TableMeta));
        priv->head->cell[i]->meta->font = NULL;
        priv->head->cell[i]->meta->bg_image = NULL;
        priv->head->cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->head->cell[i]->meta->format = FORMAT_NONE;
        priv->head->cell[i]->meta->graphable = FALSE;
        priv->head->cell[i]->meta->has_format = FALSE;
        priv->head->cell[i]->meta->has_bg_color = FALSE;
        priv->head->cell[i]->meta->has_bg_image = FALSE;

        for(j = 0; j < 3; j++) {

            priv->head->cell[i]->meta->graph[j] = rgb_graph[j];
            priv->head->cell[i]->meta->color[j] = rgb_cell[j];
        }

        priv->foot->cell[i] = malloc(sizeof(TableCell));
        priv->foot->cell[i]->text = NULL;

        priv->foot->cell[i]->meta = malloc(sizeof(TableMeta));
        priv->foot->cell[i]->meta->font = NULL;
        priv->foot->cell[i]->meta->bg_image = NULL;
        priv->foot->cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->foot->cell[i]->meta->format = FORMAT_NONE;
        priv->foot->cell[i]->meta->graphable = FALSE;
        priv->foot->cell[i]->meta->format = FALSE;
        priv->foot->cell[i]->meta->has_bg_color = FALSE;
        priv->foot->cell[i]->meta->has_bg_image = FALSE;

        for(j = 0; j < 3; j++) {

            priv->foot->cell[i]->meta->graph[j] = rgb_graph[j];
            priv->foot->cell[i]->meta->color[j] = rgb_cell[j];
        }
    }

    priv->x = cols;
    priv->y = rows;

    gtk_custom_table_alloc(table, col_widths);

    return table;
}

