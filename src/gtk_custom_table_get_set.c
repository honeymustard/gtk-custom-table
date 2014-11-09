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
 * @brief add text to a table's footer or header, based on type
 * @param table    current table
 * @param col      column index
 * @param text     text to be added
 * @param type     type is either 'header' or 'footer'
 * @return void
 */
void gtk_custom_table_set_head_foot_text(GtkWidget *table, int col, 
    char *text, char *type) {
    
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    TableRows *table_row;

    /* type is header */
    if(strcmp(type, "header") == 0) {

        table_row = priv->head;
        priv->has_header = 1;
    }
    /* type is footer */
    else {
        table_row = priv->foot;
        priv->has_footer = 1;
    }

    int size_x = gtk_custom_table_get_width(table);
    int size_y = gtk_custom_table_get_height(table);

    gtk_widget_set_size_request(table, size_x, size_y);

    /* free old text as needed */
    if(table_row->cell[col]->text != NULL) {
        free(table_row->cell[col]->text);
    }

    /* malloc and copy new text to cell */
    table_row->cell[col]->text = malloc(strlen(text) + 1);
    strcpy(table_row->cell[col]->text, text);
}


/**
 * @brief get the index of a string of text in a table
 * @param table    table widget in which to search
 * @param text     text to be searched for
 * @return int     returns index on success, else -1
 */
int gtk_custom_table_get_indexof(GtkWidget *table, char *text) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(text == NULL) {
        g_error("could not get indexof text: text was null");
    }

    if(!priv->has_primary) {
        g_error("could not get indexof text: no primary index has been set");
    }

    gtk_custom_table_tree_get(table, priv->tree, text, priv->col_primary);

    int index = priv->tree_index;
    priv->tree_index = -1;

    return index; 
}


/**
 * @brief write text to cell indicated by row and col
 * @param table    current working table
 * @param col      col to write to
 * @param row      row to write to
 * @param text     text to be written
 * @return void
 */
void gtk_custom_table_set_cell_text(GtkWidget *table, int col, int row, 
    char *text) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(text == NULL) {
        g_error("could not set cell text: text was null");
    }

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell text: coords were out of bounds");
    }

    /* when adding text to the same cell twice, free memory */
    if(priv->rows[row]->cell[col]->text != NULL) {
        free(priv->rows[row]->cell[col]->text);
    }

    priv->rows[row]->cell[col]->text = malloc(strlen(text) + 1); 
    strcpy(priv->rows[row]->cell[col]->text, text);

    /* add values in primary index column to binary tree */
    if((col == priv->col_primary) && priv->has_primary) {

        /* head node is null */
        if(priv->tree == NULL) {

            priv->tree = malloc(sizeof(TableTree));
            priv->tree->data = priv->rows[row];
            priv->tree->left = NULL;
            priv->tree->right = NULL;
        }
        /* add all following nodes as such */
        else {

            gtk_custom_table_tree_add(priv->tree, priv->rows[row], col);
        }
    }
}


/**
 * @brief set background image to a specific cell
 * @param table       current table
 * @param col         column 
 * @param row         row
 * @param filename    image filename
 * @return void
 */
void gtk_custom_table_set_cell_bg_image(GtkWidget *table, int col, int row, 
    char *filename) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell bg image: coords were out of bounds");
    }   

    priv->rows[row]->cell[col]->meta->bg_image = filename;
    priv->rows[row]->cell[col]->meta->has_bg_image = TRUE;
}


/**
 * @brief enable or disable graphing for a column
 * @param table    current table
 * @param col      column for which to enable or disable graph
 * @param value    enable or disable graph
 * @return void
 */
void gtk_custom_table_set_col_graph_enable(GtkWidget *table, int col, gboolean value) {
 
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col graph enable: col was out of bounds");
    }

    priv->cols[col]->meta->has_graph = value;
}


/**
 * @brief enable or disable graphing for a row
 * @param table    current table
 * @param row      row for which to enable or disable graph
 * @param value    enable or disable graph
 * @return void
 */
void gtk_custom_table_set_row_graph_enable(GtkWidget *table, int row, gboolean value) {
 
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || row >= priv->y) {
        g_error("could not set row graph enable: row was out of bounds");
    }

    priv->rows[row]->meta->has_graph = value;
}


/**
 * @brief enable or disable graphing for a cell
 * @param table    current working table
 * @param col      column
 * @param row      row
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_cell_graph_enable(GtkWidget *table, int col, int row) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell graph color: coords were out of bounds");
    }

    priv->rows[row]->cell[col]->meta->has_graph = TRUE;
}


/**
 * @brief add a graph to all cells in a column
 * @param table    current working table
 * @param col      column 
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_col_graph(GtkWidget *table, int col, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col graph: col was out of bounds");
    }

    memcpy(priv->cols[col]->meta->graph, rgb, sizeof(double) * 3);

    priv->cols[col]->meta->has_graph = TRUE;
}


/**
 * @brief add a graph to all cells in a row
 * @param table    current working table
 * @param row      row
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_row_graph(GtkWidget *table, int row, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || row >= priv->y) {
        g_error("could not set row graph: row was out of bounds");
    }

    memcpy(priv->rows[row]->meta->graph, rgb, sizeof(double) * 3);

    priv->rows[row]->meta->has_graph = TRUE;
}


/**
 * @brief add a graph to a specific cell
 * @param table    current working table
 * @param col      column 
 * @param row      row
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_cell_graph(GtkWidget *table, int col, int row, 
    double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell graph: coords were out of bounds");
    }
    
    memcpy(priv->rows[row]->cell[col]->meta->graph, rgb, sizeof(double) * 3);

    priv->rows[row]->cell[col]->meta->has_graph = TRUE;
}


/**
 * @brief add a background color to a specific table column
 * @param table    current table
 * @param col      column to get a background color
 * @param rgb      array of colors for column background
 * @return void
 */
void gtk_custom_table_set_col_color(GtkWidget *table, int col, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col color: col was out of bounds");
    }
    
    memcpy(priv->cols[col]->meta->color, rgb, sizeof(double) * 3);

    priv->cols[col]->meta->has_bg_color = TRUE;
}


/**
 * @brief add a background color to a specific table row
 * @param table    current table
 * @param row      row to get a background color
 * @param rgb      array of colors for row background
 * @return void
 */
void gtk_custom_table_set_row_color(GtkWidget *table, int row, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || row >= priv->y) {
        g_error("could not set row color: row was out of bounds");
    }
    
    memcpy(priv->rows[row]->meta->color, rgb, sizeof(double) * 3);

    priv->rows[row]->meta->has_bg_color = TRUE;
}


/**
 * @brief add a background color to a specific table cell
 * @param table    current table
 * @param col      column
 * @param row      row
 * @param rgb      array of colors for cell background
 * @return void
 */
void gtk_custom_table_set_cell_color(GtkWidget *table, int col, int row, 
    double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell color: coords were out of bounds");
    }
    
    memcpy(priv->rows[row]->cell[col]->meta->color, rgb, sizeof(double) * 3);

    priv->rows[row]->cell[col]->meta->has_bg_color = TRUE;
}


/**
 * @brief enable or disable table cell background color..
 * @param table    current table
 * @param col      cell x index
 * @param row      cell y index
 * @param value    enable or disable cell color
 * @return void
 */
void gtk_custom_table_set_cell_color_enable(GtkWidget *table, int col, int row, 
    gboolean value) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell color enable: coords were out of bounds");
    }

    priv->rows[row]->cell[col]->meta->has_bg_color = value;
}


/**
 * @brief enable or disable table cell background image..
 * @param table    current table
 * @param col      cell x index
 * @param row      cell y index
 * @param value    enable or disable background image
 * @return void
 */
void gtk_custom_table_set_cell_bg_image_enable(GtkWidget *table, int col, int row, 
    gboolean value) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell bg image enable: coords were out of bounds");
    }

    priv->rows[row]->cell[col]->meta->has_bg_image = value;
}


/**
 * @brief get the text value of a specific table cell..
 * @param table      current table
 * @param col        col value
 * @param row        row value
 * @return char *    returns the cell text
 */
char* gtk_custom_table_get_cell_text(GtkWidget *table, int col, int row) {
    
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not get cell text: coords were out of bounds");    
    }

    return priv->rows[row]->cell[col]->text;
}


/**
 * @brief get the row count of a table
 * @param table    current table
 * @return int     returns row count
 */
int gtk_custom_table_get_rows(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    return priv->y;   
}


/**
 * @brief get the column count of a table
 * @param table    current table
 * @return int     returns columns count
 */
int gtk_custom_table_get_cols(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    return priv->x;
}


/**
 * @brief get the number of hidden columns
 * @param table    current table
 * @return int     returns number of hidden columns
 */
int gtk_custom_table_get_hidden_cols(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    return priv->hidden_cols;
}


/**
 * @brief get the number of hidden rows
 * @param table    current table
 * @return int     returns number of hidden rows
 */
int gtk_custom_table_get_hidden_rows(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    return priv->hidden_rows;
}


/**
 * @brief set the primary column of a table
 * @param table    current table
 * @param col      column to set as prime
 * @param value    enable or disables prime
 * @return void
 */
void gtk_custom_table_set_col_primary(GtkWidget *table, int col, gboolean value) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set primary col: col was out of bounds");
    }

    priv->has_primary = value;
    priv->col_primary = col;
}


/**
 * @brief set the column index of a table
 * @param table    current table
 * @param col      column to set as index
 * @param value    enable or disables index
 * @return void
 */
void gtk_custom_table_set_col_index(GtkWidget *table, int col, gboolean value) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col index: col was out of bounds");
    }

    priv->cols[col]->index = value;
}


/**
 * @brief set a column to be hidden
 * @param table    current table
 * @param col      column to be hidden
 * @param value    enable or disables a hidden column
 * @return void
 */
void gtk_custom_table_set_col_hide(GtkWidget *table, int col, gboolean value) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col as hidden: col was out of bounds");
    }

    priv->hidden_cols = value ? priv->hidden_cols + 1: priv->hidden_cols - 1;

    priv->cols[col]->hidden = value;

    int size_x = gtk_custom_table_get_width(table);
    int size_y = gtk_custom_table_get_height(table);

    gtk_widget_set_size_request(table, size_x, size_y);
}


/**
 * @brief set a row to be hidden
 * @param table    current table
 * @param row      row to be hidden
 * @param value    enable or disables a hidden row
 * @return void
 */
void gtk_custom_table_set_row_hide(GtkWidget *table, int row, gboolean value) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || row >= priv->y) {
        g_error("could not set row as hidden: row was out of bounds");
    }

    priv->hidden_rows = value ? priv->hidden_rows + 1: priv->hidden_rows - 1;

    priv->rows[row]->hidden = value;

    int size_x = gtk_custom_table_get_width(table);
    int size_y = gtk_custom_table_get_height(table);

    gtk_widget_set_size_request(table, size_x, size_y);
}


/**
 * @brief set a specific text format to a column
 * @param table     current table
 * @param col       column to be assigned a text format
 * @param format    text format to be used
 * @return void
 */
void gtk_custom_table_set_col_format(GtkWidget *table, int col, 
    GctTextFormat format) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col format: col was out of bounds");
    }

    priv->cols[col]->meta->has_text_format = TRUE; 
    priv->cols[col]->meta->text_format = format;
}


/**
 * @brief set the text alignment for a column
 * @param table        current table
 * @param col          column to get an alignment
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_col_alignment(GtkWidget *table, int col, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set col alignment: col was out of bounds");
    }

    priv->cols[col]->meta->has_alignment = TRUE;
    priv->cols[col]->meta->alignment = alignment;
}


/**
 * @brief set the text alignment for a row
 * @param table        current table
 * @param row          row to get an alignment
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_row_alignment(GtkWidget *table, int row, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || row >= priv->y) {
        g_error("could not set row alignment: row was out of bounds");
    }

    priv->rows[row]->meta->has_alignment = TRUE;
    priv->rows[row]->meta->alignment = alignment;
}


/**
 * @brief set the row height for a specific row
 * @param table     current table
 * @param row       row for which to set height
 * @param height    height in pixels
 * @return void
 */
void gtk_custom_table_set_row_height(GtkWidget *table, int row, int height) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(height < -1) {
        g_error("could not set row height: height was less than -1");
    }

    if(row < 0 || row >= priv->y) {
        g_error("could not set row height: row was out of bounds");
    }

    priv->rows[row]->height_orig = height;
}


/**
 * @brief set the row height for the header row
 * @param table     current table
 * @param height    height in pixels
 * @return void
 */
void gtk_custom_table_set_head_row_height(GtkWidget *table, int height) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(height < -1) {
        g_error("could not set head row height: height was less than -1");
    }

    priv->head->height_orig = height;
}


/**
 * @brief set the row height for the footer row
 * @param table     current table
 * @param height    height in pixels
 * @return void
 */
void gtk_custom_table_set_foot_row_height(GtkWidget *table, int height) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(height < -1) {
        g_error("could not set foot row height: height was less than -1");
    }

    priv->foot->height_orig = height;
}


/**
 * @brief get the row height for a specific row
 * @param table    current table
 * @param row      row for which to get height
 * @return int     returns height of row in pixels
 */
int gtk_custom_table_get_row_height(GtkWidget *table, int row) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || row >= priv->y) {
        g_error("could not get row height: row was out of bounds");
    }
    
    return priv->rows[row]->height_temp;
}


/**
 * @brief get the row height for the header row
 * @param table    current table
 * @return int     returns height of header in pixels
 */
int gtk_custom_table_get_head_row_height(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    return priv->head->height_temp;
}


/**
 * @brief get the row height for the footer row
 * @param table    current table
 * @return int     returns height of footer in pixels
 */
int gtk_custom_table_get_foot_row_height(GtkWidget *table) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    return priv->foot->height_temp;
}


/**
 * @brief set the text alignment for header row
 * @param table        current table
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_head_row_alignment(GtkWidget *table, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    priv->head->meta->has_alignment = TRUE;
    priv->head->meta->alignment = alignment;
}


/**
 * @brief set the text alignment for footer row
 * @param table        current table
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_foot_row_alignment(GtkWidget *table, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    priv->foot->meta->has_alignment = TRUE;
    priv->foot->meta->alignment = alignment;
}


/**
 * @brief set the text alignment for a specific cell
 * @param table        current table
 * @param col          cell column
 * @param row          cell row
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_cell_alignment(GtkWidget *table, int col, int row, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell alignment: coords were out of bounds");
    }

    priv->rows[row]->cell[col]->meta->has_alignment = TRUE;
    priv->rows[row]->cell[col]->meta->alignment = alignment;
}


/**
 * @brief set the text alignment for a specific header cell
 * @param table        current table
 * @param col          cell column
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_head_cell_alignment(GtkWidget *table, int col, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set head cell alignment: col was out of bounds");
    }

    priv->head->cell[col]->meta->has_alignment = TRUE;
    priv->head->cell[col]->meta->alignment = alignment;
}


/**
 * @brief set the text alignment for a specific footer cell
 * @param table        current table
 * @param col          cell column
 * @param alignment    alignment to use
 * @return void
 */
void gtk_custom_table_set_foot_cell_alignment(GtkWidget *table, int col, 
    PangoAlignment alignment) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set foot cell alignment: col was out of bounds");
    }

    priv->foot->cell[col]->meta->has_alignment = TRUE;
    priv->foot->cell[col]->meta->alignment = alignment;
}


/**
 * @brief set the sort index for a table column
 * @param table    current table
 * @param col      column to use
 * @return void
 */
void gtk_custom_table_set_sort_index(GtkWidget *table, int col) {
 
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not set sort index: col was out of bounds");
    }

    priv->sort_index = col;
}


/**
 * @brief enable or disable sorting for a table
 * @param table    current table
 * @param value    either true or false
 * @return void
 */
void gtk_custom_table_set_sortable(GtkWidget *table, gboolean value) {
 
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    priv->is_sortable = value;
}


/**
 * @brief set the text for a header cell
 * @param table    current table
 * @param col      column for which to set the text
 * @param text     text to apply to cell
 * @return void
 */
void gtk_custom_table_set_head_text(GtkWidget *table, int col, char *text) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(text == NULL) {
        g_error("could not set head cell text: text was null");
    }

    if(col < 0 || col >= priv->x) {
        g_error("could not set head cell text: col was out of bounds");
    }

    gtk_custom_table_set_head_foot_text(table, col, text, "header");
}


/**
 * @brief set the text for a footer cell
 * @param table    current table
 * @param col      column for which to set the text
 * @param text     text to apply to cell
 * @return void
 */
void gtk_custom_table_set_foot_text(GtkWidget *table, int col, char *text) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(text == NULL) {
        g_error("could not set foot cell text: text was null");
    }

    if(col < 0 || col >= priv->x) {
        g_error("could not set foot cell text: col was out of bounds");
    }

    gtk_custom_table_set_head_foot_text(table, col, text, "footer");
}


/**
 * @brief set the font for a header cell
 * @param table    current table
 * @param col      column for which to set the font
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_head_cell_font(GtkWidget *table, int col, 
    char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set head cell font: font was null");
    }

    if(col < 0 || col >= priv->x) {
        g_error("could not set head cell font: col was out of bounds");
    }

    priv->head->cell[col]->meta->has_font = TRUE;
    priv->head->cell[col]->meta->font = font;
}


/**
 * @brief set the font for a footer cell
 * @param table    current table
 * @param col      column for which to set the font
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_foot_cell_font(GtkWidget *table, int col, 
    char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set foot cell font: font was null");
    }

    if(col < 0 || col >= priv->x) {
        g_error("could not set foot cell font: col was out of bounds");
    }

    priv->foot->cell[col]->meta->has_font = TRUE;
    priv->foot->cell[col]->meta->font = font;
}


/**
 * @brief set the font for a header row
 * @param table    current table
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_head_row_font(GtkWidget *table, char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set head row font: font was null");
    }

    priv->head->meta->has_font = TRUE;
    priv->head->meta->font = font;
}


/**
 * @brief set the font for a footer row
 * @param table    current table
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_foot_row_font(GtkWidget *table, char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set foot row font: font was null");
    }

    priv->foot->meta->has_font = TRUE;
    priv->foot->meta->font = font;
}


/**
 * @brief set the font for a specific cell
 * @param table    current table
 * @param col      cell column
 * @param row      cell row
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_cell_font(GtkWidget *table, int col, int row, 
    char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set cell font: font was null");
    }

    if(row < 0 || col < 0 || row >= priv->y || col >= priv->x) {
        g_error("could not set cell font: coords were out of bounds");
    }

    priv->rows[row]->cell[col]->meta->has_font = TRUE;
    priv->rows[row]->cell[col]->meta->font = font;
}


/**
 * @brief set the font for a row
 * @param table    current table
 * @param row      row for which to set the font
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_row_font(GtkWidget *table, int row, char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set row font: font was null");
    }

    if(row < 0 || row >= priv->y) {
        g_error("could not set row font: row was out of bounds");
    }

    priv->rows[row]->meta->has_font = TRUE;
    priv->rows[row]->meta->font = font;
}


/**
 * @brief set the font for a column
 * @param table    current table
 * @param col      column for which to set the font
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_col_font(GtkWidget *table, int col, char *font) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(font == NULL) {
        g_error("could not set col font: font was null");
    }

    if(col < 0 || col >= priv->x) {
        g_error("could not set col font: col was out of bounds");
    }

    priv->cols[col]->meta->has_font = TRUE;
    priv->cols[col]->meta->font = font;
}


/**
 * @brief set the width of a column
 * @param table    current table
 * @param col      column for which to set the width
 * @param width    width in pixels
 * @return void
 */
void gtk_custom_table_set_col_width(GtkWidget *table, int col, int width) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(width < -1) {
        g_error("could not set col width: width was less than -1");
    }

    if(col < 0 || col >= priv->x) {
        g_error("could not set col width: col was out of bounds");
    }

    priv->cols[col]->width_orig = width;
}


/**
 * @brief get the width of a column
 * @param table    current table
 * @param col      column for which to get the width
 * @return int     returns column width in pixels
 */
int gtk_custom_table_get_col_width(GtkWidget *table, int col) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    if(col < 0 || col >= priv->x) {
        g_error("could not get col width: col was out of bounds");
    }

    return priv->cols[col]->width_temp;
}


/**
 * @brief get the current width of a table in pixels
 * @param table    current table
 * @return int     current width
 */
int gtk_custom_table_get_width(GtkWidget *table) {
    
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    gtk_custom_table_calc_cols(table);

    return priv->col_offset_temp[priv->x];
}


/**
 * @brief get the current height of a table in pixels
 * @param table    current table
 * @return int     current height
 */
int gtk_custom_table_get_height(GtkWidget *table) {
    
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    gtk_custom_table_calc_rows(table);

    int head = priv->has_header ? priv->head->height_temp : 0;
    int foot = priv->has_footer ? priv->foot->height_temp : 0;

    return head + priv->row_offset_temp[priv->y] + foot;
}

