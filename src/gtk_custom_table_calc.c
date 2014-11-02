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
 * @brief calculate current table column widths and offsets
 * @param table    current table
 * @return void
 */
void gtk_custom_table_calc_widths(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    int unlimited = 0;
    int specified = 0;
    int offset = 0;

    /* calculate total fixed width and total */
    for(i = 0; i < priv->x; i++) {

        /* skip hidden columns */
        if(priv->col_hidden[i] == TRUE) {
            continue;
        }

        /* sum up specified width */
        if(priv->col_widths[i] != -1) {
            specified += priv->col_widths[i];
        }
        /* sum up cols with unlimited width */
        else {
            unlimited += 1;
        }
    }

    /* calculate widths */
    int max_width = gtk_widget_get_allocated_width(table);
    int available = max_width - specified > 0 ? max_width - specified : 0;
    
    /* divvy up remainder of space to cells without widths */
    for(i = 0; i < priv->x; i++) {

        /* skip hidden columns */
        if(priv->col_hidden[i] == TRUE) {
            continue;
        }

        if(priv->col_widths[i] == -1) {
            priv->col_widths_temp[i] = available / unlimited;
        }
        else {
            priv->col_widths_temp[i] = priv->col_widths[i];
        }

        /* calculate offset of each column, for alignment */
        priv->col_offset_temp[i] = offset;
        offset += priv->col_widths_temp[i];
    }

    /* set end of table offset */
    priv->col_offset_temp[i] = offset - 1;
}

