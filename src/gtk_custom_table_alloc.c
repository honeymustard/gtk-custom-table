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
 * @brief create a new custom table and setup default variables
 * @param table    current table
 * @return void
 */
void gtk_custom_table_alloc(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int cols = priv->x;
    int rows = priv->y;

    int i = 0;
    int j = 0;
    int k = 0;
 
    int indices = cols * sizeof(int);

    priv->col_offset_temp = malloc(indices + sizeof(int));

    /* setup cells, rows and columns */
    priv->cols = malloc(sizeof(TableCols *) * cols); 
    priv->rows = malloc(sizeof(TableRows *) * rows); 
    priv->cell = malloc(sizeof(TableCell *) * (rows * cols)); 

    /* create a temporary default meta object */
    TableMeta *temp = malloc(sizeof(TableMeta));
    temp->font = NULL;
    temp->bg_image = NULL;
    temp->align = PANGO_ALIGN_NONE;
    temp->format = FORMAT_NONE;
    temp->graphable = FALSE;
    temp->has_format = FALSE;
    temp->has_bg_color = FALSE;
    temp->has_bg_image = FALSE;

    int cell = 0;

    for(i = 0; i < (rows * cols); i++) {

        /* create new table cell */
        priv->cell[i] = malloc(sizeof(TableCell));
        priv->cell[i]->text = NULL;
        priv->cell[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->cell[i]->meta, temp, sizeof(TableMeta));

        for(k = 0; k < 3; k++) {
            priv->cell[i]->meta->graph[k] = rgb_graph[k];
            priv->cell[i]->meta->color[k] = rgb_cell[k];
        }
    }

    for(i = 0; i < rows; i++) {

        priv->rows[i] = malloc(sizeof(TableRows));
        priv->rows[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->rows[i]->meta, temp, sizeof(TableMeta));

        priv->rows[i]->row_genesis = i;
        priv->rows[i]->row_current = 0;

        for(k = 0; k < 3; k++) {
            priv->rows[i]->meta->graph[k] = rgb_graph[k];
            priv->rows[i]->meta->color[k] = rgb_cell[k];
        }

        priv->rows[i]->cell = malloc(sizeof(TableCell *) * cols);
        priv->rows[i]->priv = priv;

        for(j = 0; j < cols; j++, cell++) {
            priv->rows[i]->cell[j] = priv->cell[cell];
        }
    }

    cell = 0;

    for(i = 0; i < cols; i++) {

        priv->cols[i] = malloc(sizeof(TableCols));
        priv->cols[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->cols[i]->meta, temp, sizeof(TableMeta));

        priv->cols[i]->meta->align = PANGO_ALIGN_RIGHT;
        priv->cols[i]->meta->font = PANGO_DEFAULT_FONT;

        for(k = 0; k < 3; k++) {
            priv->cols[i]->meta->graph[k] = rgb_graph[k];
            priv->cols[i]->meta->color[k] = rgb_cell[k];
        }

        priv->cols[i]->cell = malloc(sizeof(TableCell *) * rows);

        for(j = 0; j < rows; j++, cell++) {
            priv->cols[i]->cell[j] = priv->cell[cell % priv->x];
        }

        priv->cols[i]->index = FALSE;
        priv->cols[i]->hidden = FALSE;
        priv->cols[i]->width_orig = 20;
        priv->cols[i]->width_temp = 0;
    }

    priv->col_offset_temp[cols] = 0;

    free(temp);
}

