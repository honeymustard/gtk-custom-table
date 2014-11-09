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
    priv->is_sortable = TRUE;
    priv->has_header = FALSE;
    priv->has_footer = FALSE;
    priv->has_primary = FALSE;

    priv->x = cols;
    priv->y = rows;
    priv->tree_index = -1;
    priv->sort_index = 0;
    priv->sort_order = GCT_SORT_ASC;
    priv->col_primary = 0;
    priv->clip_upper = 0;
    priv->clip_lower = 0;

    priv->tree = NULL;
    priv->head = NULL;
    priv->foot = NULL;

    priv->cell = NULL;
    priv->cols = NULL;
    priv->rows = NULL;

    gtk_custom_table_alloc(table);

    return table;
}

