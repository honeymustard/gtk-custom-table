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
 * @brief free contents and structures from a specific table widget
 * @param table    table to be freed
 * @return void
 */
void gtk_custom_table_free(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    /* free memory occupied by header/footer */
    for(i = 0; i < priv->x; i++) {

        if(priv->head->cell[i]->text != NULL) {
            free(priv->head->cell[i]->text);
        }

        free(priv->head->cell[i]->meta);
        free(priv->head->cell[i]);

        if(priv->foot->cell[i]->text != NULL) {
            free(priv->foot->cell[i]->text);
        }

        free(priv->foot->cell[i]->meta);
        free(priv->foot->cell[i]);
    }

    free(priv->head->cell); 
    free(priv->foot->cell); 
    free(priv->head->meta); 
    free(priv->foot->meta); 
    free(priv->head);
    free(priv->foot);

    /* free misc memory */
    free(priv->col_offset_temp);
    free(priv->row_offset_temp);

    /* free memory occupied by table cells */
    for(i = 0; i < (priv->x * priv->y); i++) {

        /* free cell text and image */
        if(priv->cell[i]->text != NULL) {
            free(priv->cell[i]->text);
        }

        free(priv->cell[i]->meta);
        free(priv->cell[i]);
    }

    /* free memory occupied by table rows */
    for(i = 0; i < priv->y; i++) {

        free(priv->rows[i]->meta);
        free(priv->rows[i]->cell);
        free(priv->rows[i]);
    }

    /* free memory occupied by table cols */
    for(i = 0; i < priv->x; i++) {

        free(priv->cols[i]->meta);
        free(priv->cols[i]->cell);
        free(priv->cols[i]);
    }

    free(priv->cell);
    free(priv->cols);
    free(priv->rows);

    /* free memory occupied by primed column */
    gtk_custom_table_tree_free(priv->tree);
    priv->tree = NULL;
}

