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
 * @brief calculate and update table size
 * @return void
 */
void gtk_custom_table_calc_size(GtkWidget *table) {

    int size_x = gtk_custom_table_get_width(table);
    int size_y = gtk_custom_table_get_height(table);

    printf("size request: %d x %d\n", size_x, size_y);
    gtk_widget_set_size_request(table, size_x, size_y);
}


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
        if(priv->cols[i]->hidden) {
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

        int orig = priv->cols[i]->width_orig;

        /* skip hidden columns */
        if(priv->cols[i]->hidden) {

            priv->cols[i]->width_temp = orig;
            priv->col_offset_temp[i] = offset;

            continue;
        }

        if(orig == -1) {

            int width = available / unlimited;

            priv->cols[i]->width_temp = width <= 0 ? GCT_COL_WIDTH : width;
        }
        else {
            priv->cols[i]->width_temp = orig;
        }

        /* calculate offset of each column, for alignment */
        priv->col_offset_temp[i] = offset;
        offset += priv->cols[i]->width_temp;
    }

    /* set end of table offset */
    priv->col_offset_temp[i] = offset;
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

    int header = priv->head->height_orig;
    int footer = priv->foot->height_orig;

    if(priv->has_header) {

        offset = header;

        specified += header > 0 ? header : 0;
        priv->head->height_temp = priv->head->height_orig;
    }

    if(priv->has_footer) {

        specified += footer > 0 ? footer : 0;
        priv->foot->height_temp = priv->foot->height_orig;
    }

    /* calculate total fixed height and total */
    for(i = 0; i < priv->y; i++) {

        /* skip hidden rows */
        if(priv->rows[i]->hidden) {
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

        int orig = priv->rows[i]->height_orig;

        /* skip hidden rows */
        if(priv->rows[i]->hidden) {

            priv->rows[i]->height_temp = orig;
            priv->row_offset_temp[i] = offset;

            continue;
        }

        if(orig == -1) {

            int height = available / unlimited;

            priv->rows[i]->height_temp = height <= 0 ? GCT_ROW_HEIGHT : height;
        }
        else {

            priv->rows[i]->height_temp = orig;
        }

        /* calculate offset of each row, for alignment */
        priv->row_offset_temp[i] = offset;
        offset += priv->rows[i]->height_temp;
    }

    /* set end of table offset */
    priv->row_offset_temp[i] = offset;
}


/**
 * @brief calculate first and last visible row of a table
 * @param table    current table
 * @return void
 */
void gtk_custom_table_calc_clip(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    GtkScrollable *parent = GTK_SCROLLABLE(gtk_widget_get_parent(table));

    GtkAdjustment *v_adjust = gtk_scrollable_get_vadjustment(parent);
    GtkAdjustment *h_adjust = gtk_scrollable_get_hadjustment(parent);

    int v_beg = gtk_adjustment_get_value(v_adjust);
    int v_end = v_beg + (int)gtk_adjustment_get_page_size(v_adjust);

    int h_beg = gtk_adjustment_get_value(h_adjust);
    int h_end = h_beg + (int)gtk_adjustment_get_page_size(h_adjust);

    priv->clip_upper = 0;
    priv->clip_lower = priv->y;
    priv->clip_left = 0;
    priv->clip_right = priv->x;

    int i = 0;

    /* find first visible row */
    for(i = 0; i < priv->y; i++) {

        if(priv->rows[i]->hidden) {
            continue;
        }

        if(priv->row_offset_temp[i+1] > v_beg) {

            priv->clip_upper = i;
            break;
        }
    }

    /* find last visible row */
    for(i = 0; i < priv->y; i++) {

        if(priv->rows[i]->hidden) {
            continue;
        }

        if(priv->row_offset_temp[i+1] >= v_end) {

            priv->clip_lower = i + 1;
            break;
        }
    }

    /* find first visible column */
    for(i = 0; i < priv->x; i++) {

        if(priv->cols[i]->hidden) {
            continue;
        }

        if(priv->col_offset_temp[i+1] > h_beg) {

            priv->clip_left = i;
            break;
        }
    }

    /* find last visible column */
    for(i = 0; i < priv->x; i++) {

        if(priv->cols[i]->hidden) {
            continue;
        }

        if(priv->col_offset_temp[i+1] >= h_end) {

            priv->clip_right = i + 1;
            break;
        }
    }
}

