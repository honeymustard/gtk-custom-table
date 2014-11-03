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
void gtk_custom_table_calc_cols(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    int unlimited = 0;
    int specified = 0;
    int offset = 0;

    /* calculate total fixed width and total */
    for(i = 0; i < priv->x; i++) {

        /* skip hidden columns */
        if(priv->cols[i]->hidden == TRUE) {
            continue;
        }

        /* sum up specified width */
        if(priv->cols[i]->width_orig != -1) {
            specified += priv->cols[i]->width_orig;
        }
        /* sum up cols with unlimited width */
        else {
            unlimited += 1;
        }
    }

    /* calculate width */
    int max_width = gtk_widget_get_allocated_width(table);
    int available = max_width - specified > 0 ? max_width - specified : 0;
    
    /* divvy up remainder of space to cells without widths */
    for(i = 0; i < priv->x; i++) {

        /* skip hidden columns */
        if(priv->cols[i]->hidden == TRUE) {
            continue;
        }

        if(priv->cols[i]->width_orig == -1) {
            priv->cols[i]->width_temp = available / unlimited;
        }
        else {
            priv->cols[i]->width_temp = priv->cols[i]->width_orig;
        }

        /* calculate offset of each column, for alignment */
        priv->col_offset_temp[i] = offset;
        offset += priv->cols[i]->width_temp;
    }

    /* set end of table offset */
    priv->col_offset_temp[i] = offset - 1;
}


/**
 * @brief calculate current table row heights and offsets
 * @param table    current table
 * @return void
 */
void gtk_custom_table_calc_rows(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    int unlimited = 0;
    int specified = 0;
    int offset = 0;

    /* calculate total fixed height and total */
    for(i = 0; i < priv->y; i++) {

        /* skip hidden rows */
        if(priv->rows[i]->hidden == TRUE) {
            continue;
        }

        /* sum up specified height */
        if(priv->rows[i]->height_orig != -1) {
            specified += priv->rows[i]->height_orig;
        }
        /* sum up cols with unlimited width */
        else {
            unlimited += 1;
        }
    }

    /* calculate height */
    int max_height = gtk_widget_get_allocated_height(table);
    int available = max_height - specified > 0 ? max_height - specified : 0;
    
    /* divvy up remainder of space to cells without widths */
    for(i = 0; i < priv->y; i++) {

        /* skip hidden rows */
        if(priv->rows[i]->hidden == TRUE) {
            continue;
        }

        if(priv->rows[i]->height_orig == -1) {
            priv->rows[i]->height_temp = available / unlimited;
        }
        else {
            priv->rows[i]->height_temp = priv->rows[i]->height_orig;
        }

        /* calculate offset of each row, for alignment */
        priv->row_offset_temp[i] = offset;
        offset += priv->rows[i]->height_temp;
    }

    /* set end of table offset */
    priv->row_offset_temp[i] = offset - 1;
}

