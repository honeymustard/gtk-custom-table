/****************************************************************************
*
* Copyright (C) 2011-2012  Adrian Solumsmo
* 
* imdb-plus program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* imdb-plus program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************/


#include "gtk_custom_table.h"


/**
 * create a new custom table and setup default variables
 * @param int cols               amount of table columns
 * @param int rows               amount of table rows
 * @param int min_width          minimun width of the table
 * @param int min_height         minimum height of the table
 * @return GtkWidget *table      returns newly created table widget
 */
GtkWidget * gtk_custom_table_new(int cols, int rows, int min_width, int min_height, 
    int column_widths[]) {
    
    GtkWidget *table = g_object_new(GTK_TYPE_CUSTOM_TABLE, NULL);

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* variable defaults */
    priv->table_min_width = min_width;
    priv->table_row_height = 25;
    priv->table_has_primary = 0;
    priv->table_has_header = 0;
    priv->table_has_footer = 0;
    priv->table_scroll_lock = 0;
    priv->table_tree = NULL;
    priv->table_sort_index = 0;
    priv->table_sort_order = 0;
    priv->table_tree_index = -1;
    priv->table_is_sortable = FALSE;
    priv->table_col_primary = 2;

    /* create header & footer */
    priv->table_head = malloc(sizeof(struct table_rows));
    priv->table_foot = malloc(sizeof(struct table_rows));

    priv->table_head->cell = malloc(sizeof(struct table_cell *) * cols); 
    priv->table_foot->cell = malloc(sizeof(struct table_cell *) * cols); 

    int i = 0;
    int j = 0;

    priv->table_head->meta = malloc(sizeof(struct table_meta));
    priv->table_head->meta->font = NULL;
    priv->table_head->meta->align = PANGO_ALIGN_NONE;
    priv->table_head->meta->graphable = FALSE;
    priv->table_head->meta->has_bg_color = FALSE;

    for(i = 0; i < 3; i++) {

        priv->table_head->meta->graph[i] = rgb_graph[i];
        priv->table_head->meta->color[i] = rgb_cell[i];
    }

    priv->table_foot->meta = malloc(sizeof(struct table_meta));
    priv->table_foot->meta->font = NULL;
    priv->table_foot->meta->align = PANGO_ALIGN_NONE;
    priv->table_foot->meta->graphable = FALSE;
    priv->table_foot->meta->has_bg_color = FALSE;

    for(i = 0; i < 3; i++) {

        priv->table_foot->meta->graph[i] = rgb_graph[i];
        priv->table_foot->meta->color[i] = rgb_cell[i];
    }

    for(i = 0; i < cols; i++) {

        /* create new table cell */
        priv->table_head->cell[i] = malloc(sizeof (struct table_cell));
        priv->table_head->cell[i]->text = NULL;

        priv->table_head->cell[i]->meta = malloc(sizeof(struct table_meta));
        priv->table_head->cell[i]->meta->font = NULL;
        priv->table_head->cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->table_head->cell[i]->meta->graphable = FALSE;
        priv->table_head->cell[i]->meta->has_bg_color = FALSE;

        for(j = 0; j < 3; j++) {

            priv->table_head->cell[i]->meta->graph[j] = rgb_graph[j];
            priv->table_head->cell[i]->meta->color[j] = rgb_cell[j];
        }

        priv->table_foot->cell[i] = malloc(sizeof (struct table_cell));
        priv->table_foot->cell[i]->text = NULL;

        priv->table_foot->cell[i]->meta = malloc(sizeof(struct table_meta));
        priv->table_foot->cell[i]->meta->font = NULL;
        priv->table_foot->cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->table_foot->cell[i]->meta->graphable = FALSE;
        priv->table_foot->cell[i]->meta->has_bg_color = FALSE;

        for(j = 0; j < 3; j++) {

            priv->table_foot->cell[i]->meta->graph[j] = rgb_graph[j];
            priv->table_foot->cell[i]->meta->color[j] = rgb_cell[j];
        }
    }

    gtk_custom_table_alloc(priv, cols, rows, column_widths);

    return table;
}

