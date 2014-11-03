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
 * @return table        returns newly created table widget
 */
GtkWidget * gtk_custom_table_new(int cols, int rows) {
    
    GtkWidget *table = g_object_new(GTK_TYPE_CUSTOM_TABLE, NULL);

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* variable defaults */
    priv->is_sortable = FALSE;
    priv->has_primary = FALSE;
    priv->has_header = FALSE;
    priv->has_footer = FALSE;
    priv->tree = NULL;
    priv->sort_index = 0;
    priv->sort_order = 0;
    priv->tree_index = -1;
    priv->col_primary = 0;

    /* create header and footer */
    priv->head = malloc(sizeof(TableRows));
    priv->foot = malloc(sizeof(TableRows));
    priv->head->meta = malloc(sizeof(TableMeta));
    priv->foot->meta = malloc(sizeof(TableMeta));
    priv->head->cell = malloc(sizeof(TableCell *) * cols); 
    priv->foot->cell = malloc(sizeof(TableCell *) * cols); 

    /* make a default meta object */
    TableMeta meta;
    meta.font = NULL;
    meta.bg_image = NULL;
    meta.align = PANGO_ALIGN_NONE;
    meta.format = FORMAT_NONE;
    meta.graphable = FALSE;
    meta.has_format = FALSE;
    meta.has_bg_color = FALSE;
    meta.has_bg_image = FALSE;

    memcpy(&meta.graph, rgb_graph, sizeof(rgb_graph));
    memcpy(&meta.color, rgb_cell, sizeof(rgb_cell));

    /* set default row height */
    priv->head->height_orig = GCT_ROW_HEIGHT;
    priv->head->height_temp = GCT_ROW_HEIGHT;
    priv->foot->height_orig = GCT_ROW_HEIGHT;
    priv->foot->height_temp = GCT_ROW_HEIGHT;

    memcpy(priv->head->meta, &meta, sizeof(TableMeta));
    memcpy(priv->foot->meta, &meta, sizeof(TableMeta));

    int i = 0;

    /* create header and footer cells cells */
    for(i = 0; i < cols; i++) {

        priv->head->cell[i] = malloc(sizeof(TableCell));
        priv->head->cell[i]->text = NULL;
        priv->head->cell[i]->meta = malloc(sizeof(TableMeta));

        priv->foot->cell[i] = malloc(sizeof(TableCell));
        priv->foot->cell[i]->text = NULL;
        priv->foot->cell[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->head->cell[i]->meta, &meta, sizeof(TableMeta));
        memcpy(priv->foot->cell[i]->meta, &meta, sizeof(TableMeta));
    }

    priv->x = cols;
    priv->y = rows;

    gtk_custom_table_alloc(table);

    return table;
}

