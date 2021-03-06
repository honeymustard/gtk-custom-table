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
#include "strnatcmp/strnatcmp.h"
#include <stdlib.h>


/** @file */


/**
 * @brief comparision callback function for table sorting
 * @param cmp1    pointer to comparable 1
 * @param cmp2    pointer to comparable 2
 * @return int    see: strcmp
 */
int gtk_custom_table_compare(const void *cmp1, const void *cmp2) {

    TableRows *row1 = *(((TableRows **)cmp1));
    TableRows *row2 = *(((TableRows **)cmp2));

    int col = row1->priv->sort_index;
    int sort = row1->priv->sort_order != GCT_SORT_ASC;

    char *text1 = row1->cell[col]->text;
    char *text2 = row2->cell[col]->text;

    if(text1 == NULL || text2 == NULL) {
        g_error("could not sort table: cell text was null");
    }

    int result = 0;

    /* check to see if column has a sorting format */
    if(row1->priv->cols[col]->meta->has_text_format) {

        int format = row1->priv->cols[col]->meta->text_format;

        /* compare large integers with optional junk */
        if(format == GCT_TEXT_UNSIGNED_INT) {

            unsigned long x = 0;
            unsigned long y = 0;

            x = gtk_custom_table_string_parseint(text1);
            y = gtk_custom_table_string_parseint(text2);

            result = sort ? (y < x) ? -1 : (y > x) : (x < y) ? -1 : (x > y);
        }
        /* compare large integers with optional +/- signs */
        else if(format == GCT_TEXT_SIGNED_INT) {

            int x = atoll(text1);
            int y = atoll(text2);

            result = sort ? (y < x) ? -1 : (y > x) : (x < y) ? -1 : (x > y);
        }
    }
    /* use natural compare sort */
    else {

        char *x = text1;
        char *y = text2;

        result = sort ? strnatcmp(y, x) : strnatcmp(x, y);
    }

    /* use secondary sorting to break ties */
    if(result == 0) {

        int x = row1->row_orig;
        int y = row2->row_orig;

        result = sort ? x > y : y > x;
    }

    return result;
}


/**
 * @brief sort an existing table using qsort..
 * @param table    current table widget
 * @param col      sort table by contents of table[col]
 * @param sort     sort orientation. desc, asc or swap
 * @return void
 */
void gtk_custom_table_sort(GtkWidget *table, int col, GctSortOrder sort) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* only sort if table is sortable */
    if(!priv->is_sortable) {
        return; 
    }

    /* set next table sort order */
    priv->sort_index = col;

    if(sort == GCT_SORT_SWAP) {
        priv->sort_order = !priv->sort_order;
    }
    else {
        priv->sort_order = sort;
    }

    /* perform qsort on table cells array */
    qsort((void *)priv->rows, priv->y, sizeof(TableRows *), 
        gtk_custom_table_compare);

    int i = 0;

    /* reset row index, so that binary search can find them */
    for(i = 0; i < priv->y; i++) {

        priv->rows[i]->row_temp = i;
    }
}

