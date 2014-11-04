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
 * @brief resize existing table to an arbitrary size
 * @param table    table to be resized
 * @param cols     amount of cols
 * @param rows     amount of rows
 * @return void
 */
void gtk_custom_table_resize(GtkWidget *table, int cols, int rows) {

    if(cols <= 0 || rows <= 0) {
        g_error("could not resize table: sizes must be greater than zero");
    }

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int t_cols = cols >= 0 ? cols : priv->x;
    int t_rows = rows >= 0 ? rows : priv->y;

    int t_copy_cols = t_cols > priv->x ? priv->x : t_cols;
    int t_copy_rows = t_rows > priv->y ? priv->y : t_rows;
    int t_copy_cell = t_copy_cols * t_copy_rows;

    /* copy all table data */
    TableCell **table_cell = malloc(sizeof(TableCell *) * t_copy_cell); 
    TableCols **table_cols = malloc(sizeof(TableCols *) * t_copy_cols);
    TableRows **table_rows = malloc(sizeof(TableRows *) * t_copy_rows);

    int i = 0;
    int j = 0;
    int k = 0;

    for(i = 0; i < t_copy_rows; i++) {
        
        for(j = 0; j < t_copy_cols; j++) {

            table_cell[k] = malloc(sizeof(TableCell));
            memcpy(table_cell[k], priv->rows[i]->cell[j], sizeof(TableCell));

            table_cell[k]->text = NULL;

            if(priv->rows[i]->cell[j]->text != NULL) {

                int length = strlen(priv->rows[i]->cell[j]->text) + 1;
                table_cell[k]->text = malloc(length);
                strncpy(table_cell[k]->text, 
                    priv->rows[i]->cell[j]->text, length);
            }

            table_cell[k]->meta = malloc(sizeof(TableMeta));
            memcpy(table_cell[k]->meta, priv->rows[i]->cell[j]->meta, 
                sizeof(TableMeta));

            k++;
        }
    }

    for(i = 0; i < t_copy_rows; i++) {
        
        table_rows[i] = malloc(sizeof(TableRows));
        memcpy(table_rows[i], priv->rows[i], sizeof(TableRows));

        table_rows[i]->meta = malloc(sizeof(TableMeta));
        memcpy(table_rows[i]->meta, priv->rows[i]->meta, sizeof(TableMeta));
    }

    for(i = 0; i < t_copy_cols; i++) {

        table_cols[i] = malloc(sizeof(TableCols));
        memcpy(table_cols[i], priv->cols[i], sizeof(TableCols));

        table_cols[i]->meta = malloc(sizeof(TableMeta));
        memcpy(table_cols[i]->meta, priv->cols[i]->meta, sizeof(TableMeta));
    }

    /* free and allocate a new table */
    gtk_custom_table_free(table);

    priv->x = t_cols;
    priv->y = t_rows;

    gtk_custom_table_alloc(table);

    /* put saved data back into table */
    for(i = 0; i < t_copy_cell; i++) {

        if(table_cell[i]->text != NULL) {

            int length = strlen(table_cell[i]->text) + 1;
            strncpy(priv->cell[i]->text, table_cell[i]->text, length);
        }

        memcpy(priv->cell[i]->meta, table_cell[i]->meta, sizeof(TableMeta));

        free(table_cell[i]->meta);
        free(table_cell[i]);
    }

    for(i = 0; i < t_copy_rows; i++) {
        
        priv->rows[i]->priv = table_rows[i]->priv;
        priv->rows[i]->hidden = table_rows[i]->hidden;
        priv->rows[i]->height_orig = table_rows[i]->height_orig;
        priv->rows[i]->height_temp = table_rows[i]->height_temp;
        priv->rows[i]->row_orig = table_rows[i]->row_orig;
        priv->rows[i]->row_temp = table_rows[i]->row_temp;

        memcpy(priv->rows[i]->meta, table_rows[i]->meta, sizeof(TableMeta));

        free(table_rows[i]->meta);
        free(table_rows[i]);
    }

    for(i = 0; i < t_copy_cols; i++) {

        priv->cols[i]->index = table_cols[i]->index;
        priv->cols[i]->hidden = table_cols[i]->hidden;
        priv->cols[i]->width_orig = table_cols[i]->width_orig;
        priv->cols[i]->width_temp = table_cols[i]->width_temp;

        memcpy(priv->cols[i]->meta, table_cols[i]->meta, sizeof(TableMeta));

        free(table_cols[i]->meta);
        free(table_cols[i]);
    }

    free(table_cell);
    free(table_rows);
    free(table_cols);
}

