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
 
    priv->col_offset_temp = malloc((cols * sizeof(int)) + sizeof(int));
    priv->row_offset_temp = malloc((rows * sizeof(int)) + sizeof(int));

    /* setup cells, rows and columns */
    priv->cols = malloc(sizeof(TableCols *) * cols); 
    priv->rows = malloc(sizeof(TableRows *) * rows); 
    priv->cell = malloc(sizeof(TableCell *) * (rows * cols)); 

    /* create a temporary meta object */
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

    /* create header and footer */
    priv->head = malloc(sizeof(TableRows));
    priv->foot = malloc(sizeof(TableRows));
    priv->head->meta = malloc(sizeof(TableMeta));
    priv->foot->meta = malloc(sizeof(TableMeta));
    priv->head->cell = malloc(sizeof(TableCell *) * cols);
    priv->foot->cell = malloc(sizeof(TableCell *) * cols);

    /* set default row height */
    priv->head->height_orig = GCT_ROW_HEIGHT;
    priv->head->height_temp = 0;
    priv->foot->height_orig = GCT_ROW_HEIGHT;
    priv->foot->height_temp = 0;

    memcpy(priv->head->meta, &meta, sizeof(TableMeta));
    memcpy(priv->foot->meta, &meta, sizeof(TableMeta));

    int i = 0;
    int j = 0;

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

    int cell = 0;

    /* allocate all new cells */
    for(i = 0; i < (rows * cols); i++) {

        priv->cell[i] = malloc(sizeof(TableCell));
        priv->cell[i]->text = NULL;
        priv->cell[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->cell[i]->meta, &meta, sizeof(TableMeta));
    }

    /* allocate all new rows */
    for(i = 0; i < rows; i++) {

        priv->rows[i] = malloc(sizeof(TableRows));
        priv->rows[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->rows[i]->meta, &meta, sizeof(TableMeta));

        priv->rows[i]->cell = malloc(sizeof(TableCell *) * cols);
        priv->rows[i]->priv = priv;

        for(j = 0; j < cols; j++, cell++) {
            priv->rows[i]->cell[j] = priv->cell[cell];
        }

        priv->rows[i]->hidden = FALSE;
        priv->rows[i]->height_orig = GCT_ROW_HEIGHT;
        priv->rows[i]->height_temp = 0;
        priv->rows[i]->row_orig = i;
        priv->rows[i]->row_temp = 0;
    }

    cell = 0;

    /* allocate all new columns */
    for(i = 0; i < cols; i++) {

        priv->cols[i] = malloc(sizeof(TableCols));
        priv->cols[i]->meta = malloc(sizeof(TableMeta));

        memcpy(priv->cols[i]->meta, &meta, sizeof(TableMeta));

        priv->cols[i]->meta->align = PANGO_ALIGN_RIGHT;
        priv->cols[i]->meta->font = PANGO_DEFAULT_FONT;

        priv->cols[i]->cell = malloc(sizeof(TableCell *) * rows);

        for(j = 0; j < rows; j++, cell++) {
            priv->cols[i]->cell[j] = priv->cell[cell % priv->x];
        }

        priv->cols[i]->index = FALSE;
        priv->cols[i]->hidden = FALSE;
        priv->cols[i]->width_orig = GCT_COL_WIDTH;
        priv->cols[i]->width_temp = 0;
    }

    priv->col_offset_temp[cols] = 0;
}

