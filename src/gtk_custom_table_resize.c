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
 * @param GtkWidget *table    table to be resized
 * @param int cols            amount of cols
 * @param int rows            amount of rows
 * @return void
 */
void gtk_custom_table_resize(GtkWidget *table, int cols, int rows) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int t_cols = cols >= 0 ? cols : priv->table_x;
    int t_rows = rows >= 0 ? rows : priv->table_y;

    int t_copy_cols = t_cols > priv->table_x ? priv->table_x : t_cols;
    int t_copy_rows = t_rows > priv->table_y ? priv->table_y : t_rows;
    int t_copy_cell = t_copy_cols * t_copy_rows;

    /* copy table meta-data */
    TableMeta **table_cols = malloc(sizeof(TableMeta *) * t_copy_cols); 
    TableMeta **table_rows = malloc(sizeof(TableMeta *) * t_copy_rows); 
    TableMeta **table_cell = malloc(sizeof(TableMeta *) * t_copy_cell); 

    int i = 0;
    int j = 0;

    for(i = 0; i < t_copy_rows; i++) {
        
        table_rows[i] = malloc(sizeof(TableMeta));
        
        memcpy(table_rows[i], priv->table_rows[i]->meta, sizeof(TableMeta));
    }

    for(i = 0; i < t_copy_cols; i++) {

        table_cols[i] = malloc(sizeof(TableMeta));

        memcpy(table_cols[i], priv->table_cols[i]->meta, sizeof(TableMeta));
    }

    int cell = 0;

    for(i = 0; i < t_copy_rows; i++) {
        
        for(j = 0; j < priv->table_x; j++) {

            /* table might've shrunk */
            if(j >= t_cols) {
                continue;
            }

            table_cell[cell] = malloc(sizeof(TableMeta));

            memcpy(table_cell[cell], priv->table_rows[i]->cell[j]->meta, 
                sizeof(TableMeta));

            cell++;
        }
    }

    gtk_custom_table_free_cells(table);

    int col_widths[t_cols];

    for(i = 0; i < t_cols; i++) {
        col_widths[i] = priv->table_column_widths[i];
    }
    
    /* free misc memory */
    free(priv->table_column_widths);
    free(priv->table_column_index);
    free(priv->table_column_hidden);
    free(priv->table_column_widths_temp);
    free(priv->table_column_offset_temp);

    /* free memory occupied by primed column */
    gtk_custom_table_tree_free(priv->table_tree);
    priv->table_tree = NULL;

    priv->table_x = t_cols;
    priv->table_y = t_rows;

    gtk_custom_table_alloc(table, col_widths);

    /* put saved meta-data back into table */
    for(i = 0; i < t_copy_rows; i++) {
        
        memcpy(priv->table_rows[i]->meta, table_rows[i], sizeof(TableMeta));

        free(table_rows[i]);
    }

    for(i = 0; i < t_copy_cols; i++) {

        memcpy(priv->table_cols[i]->meta, table_cols[i], sizeof(TableMeta));

        free(table_cols[i]);
    }

    for(i = 0; i < t_copy_cell; i++) {

        memcpy(priv->table_cell[i]->meta, table_cell[i], sizeof(TableMeta));

        free(table_cell[i]);
    }

    free(table_rows);
    free(table_cols);
    free(table_cell);
}

