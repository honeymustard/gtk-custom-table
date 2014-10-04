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
#include "strnatcmp/strnatcmp.h"
#include <stdlib.h>


/** @file */


/**
 * @brief comparision callback function for table sorting
 * @param const void *cmp1    pointer to comparable 1
 * @param const void *cmp2    pointer to comparable 2
 * @return                    see: strcmp
 */
int gtk_custom_table_compare(const void *cmp1, const void *cmp2) {

    TableRows *row1 = *(((TableRows **)cmp1));
    TableRows *row2 = *(((TableRows **)cmp2));

    int col = row1->priv->table_sort_index;
    int sort = row1->priv->table_sort_order != GTK_CUSTOM_TABLE_ASC;

    int result = 0;

    /* check to see if column has a sorting format */
    if(row1->priv->table_cols[col]->meta->has_format) {

        int format = row1->priv->table_cols[col]->meta->format;

        /* compare large integers with optional junk */
        if(format == FORMAT_INTEGER_UNSIGNED) {

            unsigned long x = 0;
            unsigned long y = 0;

            x = gtk_custom_table_string_parseint(row1->cell[col]->text);
            y = gtk_custom_table_string_parseint(row2->cell[col]->text);

            result = sort ? (y < x) ? -1 : (y > x) : (x < y) ? -1 : (x > y);
        }
        /* compare large integers with optional +/- signs */
        else if(format == FORMAT_INTEGER_SIGNED) {

            int x = atoll(row1->cell[col]->text);
            int y = atoll(row2->cell[col]->text);

            result = sort ? (y < x) ? -1 : (y > x) : (x < y) ? -1 : (x > y);
        }
    }
    /* use natural compare sort */
    else {

        char *x = row1->cell[col]->text;
        char *y = row2->cell[col]->text;

        result = sort ? strnatcmp(y, x) : strnatcmp(x, y);
    }

    /* use secondary sorting to break ties */
    if(result == 0) {

        int x = row1->row_genesis;
        int y = row2->row_genesis;

        result = sort ? x > y : y > x;
    }

    return result;
}


/**
 * @brief sort an existing table using qsort..
 * @param GtkWidget *table    current table widget
 * @param int col             sort table by contents of table[col]
 * @param int orit            sort orientation, desc, asc or inverse
 */
void gtk_custom_table_sort(GtkWidget *table, int col, int orient) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* only sort if table is sortable */
    if(priv->table_is_sortable == FALSE) {
        return; 
    }

    /* set next table sort order */
    priv->table_sort_index = col;
    priv->table_sort_order = orient == -1 ? !priv->table_sort_order : orient;

    /* perform qsort on table cells array */
    qsort((void *)priv->table_rows, priv->table_y, sizeof(TableRows *), 
        gtk_custom_table_compare);

    int i = 0;

    /* reset row index, so that binary search can find them */
    for(i = 0; i < priv->table_y; i++) {

        priv->table_rows[i]->row_current = i;
    }
}

