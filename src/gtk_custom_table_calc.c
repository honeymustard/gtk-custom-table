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
 * @brief calculate current table column widths and offsets
 * @param GtkWidget *table    current table..
 */
void gtk_custom_table_calc(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    int unlimited = 0;
    int specified = 0;
    int offset = 0;

    /* calculate total fixed width and total */
    for(i = 0; i < priv->table_x; i++) {

        /* skip hidden columns */
        if(priv->table_column_hidden[i] == TRUE) {
            continue;
        }

        /* sum up specified width */
        if(priv->table_column_widths[i] != -1) {
            specified += priv->table_column_widths[i];
        }
        /* sum up cols with unlimited width */
        else {
            unlimited += 1;
        }
    }
    
    /* divvy up remainder of space to cells without widths */
    for(i = 0; i < priv->table_x; i++) {

        /* skip hidden columns */
        if(priv->table_column_hidden[i] == TRUE) {
            continue;
        }

        if(priv->table_column_widths[i] == -1) {

            int width = gtk_widget_get_allocated_width(table);
            priv->table_column_widths_temp[i] = (width - specified) / unlimited; 
        }
        else {
            priv->table_column_widths_temp[i] = priv->table_column_widths[i];
        }

        /* calculate offset of each column, for alignment */
        priv->table_column_offset_temp[i] = offset;
        offset += priv->table_column_widths_temp[i];
    }

    /* set end of table offset */
    priv->table_column_offset_temp[i] = offset - 1;
}

