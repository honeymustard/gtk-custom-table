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
 * @brief add text to a table's footer or header, based on type..
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

    /* detect table overflow */
    if(col >= priv->x) {
        g_error("table overflow");
    }

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
    
    gtk_widget_set_size_request(table, -1, gtk_custom_table_get_height(table));

    /* free old text as needed */
    if(table_row->cell[col]->text != NULL) {
        free(table_row->cell[col]->text);
    }

    /* malloc and copy new text to cell */
    table_row->cell[col]->text = malloc(strlen(text) + 1);      
    strcpy(table_row->cell[col]->text, text);
}


/**
 * @brief get the index of a string of text in a table.. 
 * @param table    table widget in which to search..
 * @param text     text to be searched for
 * @return int     returns index on success, else -1
 */
int gtk_custom_table_get_indexof(GtkWidget *table, char *text) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* check if table is primed */
    if(!priv->has_primary) {
        g_error("table does not have a primary index");
    }

    gtk_custom_table_tree_get(table, priv->tree, text, 
        priv->col_primary);

    int index = GTK_CUSTOM_TABLE_GET_PRIVATE(table)->tree_index;
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

    /* we can't allow this.. */
    if(text == NULL) {
        g_error("could not set cell text: text was null");
    }

    /* detect table overflow, crash and burn */
    if((col >= priv->x) || (row >= priv->y)) {
        g_error("could not set cell text: invalid cell");
    }

    /* when adding text to the same cell twice, free memory */
    if(priv->rows[row]->cell[col]->text != NULL) {
        free(priv->rows[row]->cell[col]->text);
    }

    priv->rows[row]->cell[col]->text = malloc(strlen(text) + 1); 
    strcpy(priv->rows[row]->cell[col]->text, text);

    /* add values in primary index column to binary tree */
    if((col == priv->col_primary) && priv->has_primary) {

        /* head node is null, take care of it */
        if(priv->tree == NULL) {

            priv->tree = malloc(sizeof(TableTree));
            priv->tree->data = priv->rows[row];
            priv->tree->left = NULL;
            priv->tree->right = NULL;
        }
        /* add all following nodes as such.. */
        else {

            gtk_custom_table_tree_add(priv->tree, 
                priv->rows[row], col);
        }
    }
}


/**
 * @brief set background-image to specific cell..
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

    priv->rows[row]->cell[col]->meta->bg_image = filename;
    priv->rows[row]->cell[col]->meta->has_bg_image = TRUE;
}


/**
 * @brief mark a column as graphable.. i.e. integers will show up as graph..
 * @param table    current table
 * @param col      column 
 * @param value    turn on or off
 * @return void
 */
void gtk_custom_table_set_col_graph(GtkWidget *table, int col, gboolean value) {
 
    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    priv->cols[col]->meta->graphable = value;
}


/**
 * @brief add foreground-color to a all graphs in a column..
 * @param table    current working table
 * @param col      column 
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_graph_color_col(GtkWidget *table, int col,  
    double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    for(i = 0; i < 3; i++) {
        priv->cols[col]->meta->graph[i] = rgb[i];
    }

    priv->cols[col]->meta->graphable = TRUE;
}


/**
 * @brief add foreground-color to a graph in a specific cell..
 * @param table    current working table
 * @param col      column 
 * @param row      row
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_graph_color_cell(GtkWidget *table, int col,  
    int row, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    for(i = 0; i < 3; i++) {
        priv->rows[row]->cell[col]->meta->graph[i] = rgb[i];
    }

    priv->rows[row]->cell[col]->meta->graphable = TRUE;
}


/**
 * @brief add a background color to a specific table column
 * @param table    current table
 * @param col      column to get a background color
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_col_color(GtkWidget *table, int col, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    for(i = 0; i < 3; i++) {
        priv->cols[col]->meta->color[i] = rgb[i];
    }

    priv->cols[col]->meta->has_bg_color = TRUE;
}


/**
 * @brief add a background color to a specific table row
 * @param table    current table
 * @param row      row to get a background color
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_row_color(GtkWidget *table, int row, double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    int i = 0;

    for(i = 0; i < 3; i++) {
        priv->rows[row]->meta->color[i] = rgb[i];
    }

    priv->rows[row]->meta->has_bg_color = TRUE;
}


/**
 * @brief add a background color to a specific table cell
 * @param table    current table
 * @param col      column
 * @param row      row
 * @param rgb      array of colors for graph
 * @return void
 */
void gtk_custom_table_set_cell_color(GtkWidget *table, int col, int row, 
    double rgb[]) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    
    int i = 0;

    for(i = 0; i < 3; i++) {
        priv->rows[row]->cell[col]->meta->color[i] = rgb[i];
    }

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

    TableMeta *meta = priv->rows[row]->cell[col]->meta;

    meta->has_bg_color = value;
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

    TableMeta *meta = priv->rows[row]->cell[col]->meta;

    meta->has_bg_image = value;
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

    if(row >= priv->y || col >= priv->x) {
        g_error("can't get cell, coordinates were out of bounds");    
    }

    return priv->rows[row]->cell[col]->text;
}


/**
 * @brief get the row count of a table
 * @param table    current table
 * @return int     returns row count
 */
int gtk_custom_table_get_rows(GtkWidget *table) {

    return GTK_CUSTOM_TABLE_GET_PRIVATE(table)->y;   
}


/**
 * @brief get the column count of a table
 * @param table    current table
 * @return int     returns columns count
 */
int gtk_custom_table_get_cols(GtkWidget *table) {

    return GTK_CUSTOM_TABLE_GET_PRIVATE(table)->x;
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

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->cols[col]->index = value;
}


/**
 * @brief set a column to be hidden
 * @param table    current table
 * @param col      column to be hidden
 * @param value    enable or disables a hidden column
 * @return void
 */
void gtk_custom_table_set_col_hide(GtkWidget *table, int col, gboolean value) {

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->cols[col]->hidden = value;
}


/**
 * @brief set a row to be hidden
 * @param table    current table
 * @param row      row to be hidden
 * @param value    enable or disables a hidden row
 * @return void
 */
void gtk_custom_table_set_row_hide(GtkWidget *table, int row, gboolean value) {

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->rows[row]->hidden = value;
}


/**
 * @brief set a specific text format to a column
 * @param table     current table
 * @param col       column to be assigned a text format
 * @param format    text format to be used
 * @return void
 */
void gtk_custom_table_set_col_format(GtkWidget *table, int col, 
    TextFormat format) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->cols[col]->meta->has_format = TRUE; 
    priv->cols[col]->meta->format = format;
}


/**
 * @brief set the text alignment for a column
 * @param table    current table
 * @param col      column to get an alignment
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_col_alignment(GtkWidget *table, int col, 
    PangoAlignment align) {

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->cols[col]->meta->align = align;
}


/**
 * @brief set the text alignment for a row
 * @param table    current table
 * @param row      row to get an alignment
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_row_alignment(GtkWidget *table, int row, 
    PangoAlignment align) {

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->rows[row]->meta->align = align;
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
    
    priv->rows[row]->height_orig = height;
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
    
    return priv->rows[row]->height_temp;
}


/**
 * @brief set the text alignment for header row
 * @param table    current table
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_head_row_alignment(GtkWidget *table, 
    PangoAlignment align) {

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->head->meta->align = align;
}


/**
 * @brief set the text alignment for footer row
 * @param table    current table
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_foot_row_alignment(GtkWidget *table, 
    PangoAlignment align) {

    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->foot->meta->align = align;
}


/**
 * @brief set the text alignment for a specific cell
 * @param table    current table
 * @param col      cell column
 * @param row      cell row
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_cell_alignment(GtkWidget *table, int col, 
    int row, PangoAlignment align) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->rows[row]->cell[col]->meta->align = align;
}


/**
 * @brief set the text alignment for a specific header cell
 * @param table    current table
 * @param col      cell column
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_head_cell_alignment(GtkWidget *table, int col, 
    PangoAlignment align) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->head->cell[col]->meta->align = align;
}


/**
 * @brief set the text alignment for a specific footer cell
 * @param table    current table
 * @param col      cell column
 * @param align    alignment to use
 * @return void
 */
void gtk_custom_table_set_foot_cell_alignment(GtkWidget *table, int col, 
    PangoAlignment align) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->foot->cell[col]->meta->align = align;
}


/**
 * @brief set the sort index for a table column
 * @param table    current table
 * @param col      column to use
 * @return void
 */
void gtk_custom_table_set_sort_index(GtkWidget *table, int col) {
    
    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->sort_index = col;
}


/**
 * @brief enable or disable sorting for a table
 * @param table    current table
 * @param value    either true or false
 * @return void
 */
void gtk_custom_table_set_sortable(GtkWidget *table, gboolean value) {
 
    GTK_CUSTOM_TABLE_GET_PRIVATE(table)->is_sortable = value;
}


/**
 * @brief set the text for a header cell
 * @param table    current table
 * @param col      column for which to set the text
 * @param text     text to apply to cell
 * @return void
 */
void gtk_custom_table_set_head_text(GtkWidget *table, int col, char *text) {

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

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
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

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->foot->cell[col]->meta->font = font;
}


/**
 * @brief set the font for a header row
 * @param table    current table
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_head_row_font(GtkWidget *table, char *font) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->head->meta->font = font;
}


/**
 * @brief set the font for a footer row
 * @param table    current table
 * @param font     font to use
 * @return void
 */
void gtk_custom_table_set_foot_row_font(GtkWidget *table, char *font) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
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
void gtk_custom_table_set_cell_font(GtkWidget *table, int col, 
    int row, char *font) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
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

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
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

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
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

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
    priv->cols[col]->width_orig = width;
}


/**
 * @brief get the width of a column
 * @param table    current table
 * @param col      column for which to get the width
 * @return int     returns column width in pixels
 */
int gtk_custom_table_get_col_width(GtkWidget *table, int col) {

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);
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

