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
 * @param GtkWidget *table    current table widget
 * @param int cols            amount of table cols
 * @param int rows            amount of table rows
 */
void gtk_custom_table_alloc(GtkCustomTablePrivate *priv, int cols, int rows, 
        int column_widths[]) {

    priv->table_x = cols;
    priv->table_y = rows;

    int i = 0;
    int j = 0;
    int k = 0;

    priv->table_column_widths = malloc(cols * sizeof(int));
    priv->table_column_index = malloc(cols * sizeof(int));
    priv->table_column_hidden = malloc(cols * sizeof(int));

    priv->table_column_widths_temp = malloc(cols * sizeof(int));
    priv->table_column_offset_temp = malloc((cols + 1) * sizeof(int));

    /* setup cells, rows and columns */
    priv->table_cols = malloc(sizeof(struct table_cols *) * cols); 
    priv->table_rows = malloc(sizeof(struct table_rows *) * rows); 
    priv->table_cell = malloc(sizeof(struct table_cell *) * (rows * cols)); 

    int cell = 0;

    for(i = 0; i < (rows * cols); i++) {

        /* create new table cell */
        priv->table_cell[i] = malloc(sizeof(struct table_cell));
        priv->table_cell[i]->text = NULL;
        priv->table_cell[i]->meta = malloc(sizeof(struct table_meta));
        priv->table_cell[i]->meta->font = NULL;
        priv->table_cell[i]->meta->bg_image = NULL;
        priv->table_cell[i]->meta->align = PANGO_ALIGN_NONE;
        priv->table_cell[i]->meta->graphable = FALSE;
        priv->table_cell[i]->meta->has_bg_color = FALSE;
        priv->table_cell[i]->meta->has_bg_image = FALSE;

        for(k = 0; k < 3; k++) {
            priv->table_cell[i]->meta->graph[k] = rgb_graph[k];
            priv->table_cell[i]->meta->color[k] = rgb_cell[k];
        }
    }

    for(i = 0; i < rows; i++) {

        priv->table_rows[i] = malloc(sizeof(struct table_rows));
        priv->table_rows[i]->meta = malloc(sizeof(struct table_meta));
        priv->table_rows[i]->meta->font = NULL;
        priv->table_rows[i]->meta->bg_image = NULL;
        priv->table_rows[i]->meta->align = PANGO_ALIGN_NONE;
        priv->table_rows[i]->meta->graphable = FALSE;
        priv->table_rows[i]->meta->has_bg_color = FALSE;
        priv->table_rows[i]->meta->has_bg_image = FALSE;

        priv->table_rows[i]->row_genesis = i;

        for(k = 0; k < 3; k++) {
            priv->table_rows[i]->meta->graph[k] = rgb_graph[k];
            priv->table_rows[i]->meta->color[k] = rgb_cell[k];
        }

        priv->table_rows[i]->cell = malloc(sizeof(struct table_cell *) * cols);
        priv->table_rows[i]->priv = priv;

        for(j = 0; j < cols; j++, cell++) {
            priv->table_rows[i]->cell[j] = priv->table_cell[cell];
        }
    }

    cell = 0;

    for(i = 0; i < cols; i++) {

        priv->table_cols[i] = malloc(sizeof(struct table_cols));
        priv->table_cols[i]->meta = malloc(sizeof(struct table_meta));
        priv->table_cols[i]->meta->font = PANGO_DEFAULT_FONT;
        priv->table_cols[i]->meta->bg_image = NULL;
        priv->table_cols[i]->meta->align = PANGO_ALIGN_LEFT;
        priv->table_cols[i]->meta->graphable = FALSE;
        priv->table_cols[i]->meta->has_bg_color = FALSE;
        priv->table_cols[i]->meta->has_bg_image = FALSE;

        for(k = 0; k < 3; k++) {
            priv->table_cols[i]->meta->graph[k] = rgb_graph[k];
            priv->table_cols[i]->meta->color[k] = rgb_cell[k];
        }

        priv->table_cols[i]->cell = malloc(sizeof(struct table_cell *) * rows);

        for(j = 0; j < rows; j++, cell++) {
            priv->table_cols[i]->cell[j] = priv->table_cell[cell % priv->table_x];
        }

        priv->table_column_widths[i] = column_widths[i];
        priv->table_column_index[i] = FALSE;
        priv->table_column_hidden[i] = FALSE;
        priv->table_column_widths_temp[i] = 0;
        priv->table_column_offset_temp[i] = 0;
    }

    priv->table_column_offset_temp[cols] = 0;
}


