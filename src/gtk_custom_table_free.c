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


/** @file */


/**
 * @brief free all table cells and data from a specific table widget
 * @param GtkWidget *table    table to be freed
 * @return void
 */
void gtk_custom_table_free_cells(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    /* free memory occupied by table cells */
    for(i = 0; i < (priv->table_x * priv->table_y); i++) {

        /* free cell text and image */
        if(priv->table_cell[i]->text != NULL) {
            free(priv->table_cell[i]->text);
        }

        free(priv->table_cell[i]->meta);
        free(priv->table_cell[i]);
    }

    /* free memory occupied by table rows */
    for(i = 0; i < priv->table_y; i++) {

        free(priv->table_rows[i]->meta);
        free(priv->table_rows[i]->cell);
        free(priv->table_rows[i]);
    }

    /* free memory occupied by table cols */
    for(i = 0; i < priv->table_x; i++) {

        free(priv->table_cols[i]->meta);
        free(priv->table_cols[i]->cell);
        free(priv->table_cols[i]);
    }

    free(priv->table_cell);
    free(priv->table_cols);
    free(priv->table_rows);
}


/**
 * @brief free contents and structures from a specific table widget
 * @param GtkWidget *table    table to be freed
 * @return void
 */
void gtk_custom_table_free(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    gtk_custom_table_free_cells(table);

    /* free memory occupied by header/footer */
    for(i = 0; i < priv->table_x; i++) {

        if(priv->table_head->cell[i]->text != NULL) {
            free(priv->table_head->cell[i]->text);
        }

        free(priv->table_head->cell[i]->meta);
        free(priv->table_head->cell[i]);

        if(priv->table_foot->cell[i]->text != NULL) {
            free(priv->table_foot->cell[i]->text);
        }

        free(priv->table_foot->cell[i]->meta);
        free(priv->table_foot->cell[i]);
    }

    free(priv->table_head->cell); 
    free(priv->table_foot->cell); 
    free(priv->table_head->meta); 
    free(priv->table_foot->meta); 
    free(priv->table_head);
    free(priv->table_foot);

    /* free misc memory */
    free(priv->table_column_widths);
    free(priv->table_column_index);
    free(priv->table_column_hidden);
    free(priv->table_column_widths_temp);
    free(priv->table_column_offset_temp);

    /* free memory occupied by primed column */
    gtk_custom_table_tree_free(priv->table_tree);
}

