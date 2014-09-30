/***********************************************************************
 *
 * Copyright (C) 2011-2013  Adrian Solumsmo
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


/**
 * calculate widget dimensions, paint to cairo context
 * @param GtkCustomTablePrivate *table    current table
 * @param GdkEventExpose *event           expose event
 */
void gtk_custom_table_paint(GtkWidget *table, cairo_t *cr) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* update table dimensions for redrawing table */
    if(gtk_widget_get_allocated_width(table) > priv->table_min_width) {
        gtk_custom_table_calc(table);
    }

    cairo_rectangle_list_t *rl = cairo_copy_clip_rectangle_list(cr);
    cairo_rectangle_t *r = &rl->rectangles[0];

    int scroll_beg_row = (r->y / priv->table_row_height) - 1;
    int scroll_end_row = ((r->y + r->height) / priv->table_row_height) + 1;
    
    scroll_beg_row = scroll_beg_row < 0 ? 0 : scroll_beg_row;

    if(scroll_end_row > priv->table_y) {
        scroll_end_row = priv->table_y;
    }

    #ifdef DEBUG

    printf("\n");
    printf("paint coords : (%d,%d)\n", (int)r->x, (int)r->y);
    printf("paint size   : (%d,%d)\n", (int)r->width, (int)r->height);
    printf("paint rows   : (%d -> %d)\n", scroll_beg_row, scroll_end_row);
    
    #endif

    cairo_rectangle_list_destroy(rl);

    int i = 0;
    int j = 0;

    cairo_set_line_width(cr, 1);

    TableMeta *meta_temp = NULL;

    /* DRAW HEADER ROW */    
    if(priv->table_has_header && (scroll_beg_row == 0)) {

        for(i = 0; i < priv->table_x; i++) {

            if(priv->table_column_hidden[i] == TRUE) {
                continue;
            }

            cairo_set_source_rgb(cr, 
                rgb_border[0], 
                rgb_border[1], 
                rgb_border[2]
            );

            cairo_rectangle(cr, 
                priv->table_column_offset_temp[i] + 0.5, 
                1, 
                priv->table_column_widths_temp[i], 
                priv->table_row_height - 1.5
            );

            cairo_stroke_preserve(cr);

            /* column selected background color */
            if(priv->table_sort_index == i) {

                cairo_set_source_rgb(cr, 
                    rgb_header_bold[0], 
                    rgb_header_bold[1], 
                    rgb_header_bold[2]);
            }
            /* the default background color */
            else {

                cairo_set_source_rgb(cr, 
                    rgb_header[0], 
                    rgb_header[1], 
                    rgb_header[2]);
            }

            cairo_fill(cr); 

            cairo_set_source_rgb(cr, 
                rgb_text[0], 
                rgb_text[1], 
                rgb_text[2]
            );

            /* align text */
            cairo_move_to(cr, 
                priv->table_column_offset_temp[i] + 10, 
                4
            );

            /* determine cell text alignment */
            if(priv->table_head->cell[i]->meta->align != PANGO_ALIGN_NONE) {

                meta_temp = priv->table_head->cell[i]->meta;
            }
            else if(priv->table_head->meta->align != PANGO_ALIGN_NONE) {

                meta_temp = priv->table_head->meta;
            }
            else {

                meta_temp = priv->table_cols[i]->meta;
            }

            char *font_temp = NULL;

            /* determine cell font */
            if(priv->table_head->cell[i]->meta->font != NULL) {

                font_temp = priv->table_head->cell[i]->meta->font;
            }
            else if(priv->table_head->meta->font != NULL) {

                font_temp = priv->table_head->meta->font;
            }
            else {

                font_temp = priv->table_cols[i]->meta->font;
            }
             
            /* BEGIN PANGO RENDERING */

            PangoLayout *layout = NULL;
            PangoFontDescription *description = NULL;

            layout = pango_cairo_create_layout(cr);
            description = pango_font_description_from_string(font_temp);

            pango_layout_set_text(layout, 
                    priv->table_head->cell[i]->text, -1);

            pango_layout_set_font_description(layout, 
                description);

            pango_layout_set_width(layout, 
                (priv->table_column_widths_temp[i] - 20) * PANGO_SCALE);

            pango_layout_set_height(layout, 
                priv->table_row_height * PANGO_SCALE);

            pango_layout_set_ellipsize(layout, 
                PANGO_ELLIPSIZE_END);

            pango_layout_set_alignment(layout, 
                meta_temp->align);

            pango_cairo_show_layout(cr, layout);

            pango_font_description_free(description);
            g_object_unref(layout);
        }
    }

    /* DRAW MAIN ROWS */

    /* for graph calculations */
    double graph_step = 0;
    double graph_amount = 0;
    double graph_width = 0;

    /* for temporary meta cells */
    TableMeta *meta_cell = NULL;
    TableMeta *meta_rows = NULL;
    TableMeta *meta_cols = NULL;

    /* only draw those columns which will be visible on window surface */
    for(i = scroll_beg_row; i < scroll_end_row; i++) {

        /* get row meta data */
        meta_rows = priv->table_rows[i]->meta;

        for(j = 0; j < priv->table_x; j++) {

            /* skip hidden columns */
            if(priv->table_column_hidden[j] == TRUE) {
                continue;
            }

            /* get cell, col meta data */
            meta_cell = priv->table_rows[i]->cell[j]->meta;
            meta_cols = priv->table_cols[j]->meta;
            
            /* DRAW CELL BACKGROUND COLOR */

            int offset = ((i + priv->table_has_header) * 
                priv->table_row_height);

            cairo_rectangle(cr, 
                priv->table_column_offset_temp[j] + 0.5, 
                offset > 0 ? offset : 1, 
                priv->table_column_widths_temp[j], 
                priv->table_row_height
            );

            cairo_set_source_rgb(cr, 
                rgb_border[0], 
                rgb_border[1], 
                rgb_border[2]
            );

            cairo_stroke_preserve(cr);

            /* draw cell background-color */
            if(meta_cell != NULL && meta_cell->has_bg_color) {

                cairo_set_source_rgb(cr, 
                    meta_cell->color[0], 
                    meta_cell->color[1], 
                    meta_cell->color[2]
                );
            }
            /* draw row background-color */
            else if(meta_rows != NULL && meta_rows->has_bg_color) {

                cairo_set_source_rgb(cr, 
                    meta_rows->color[0], 
                    meta_rows->color[1], 
                    meta_rows->color[2]
                );
            }
            /* draw col background-color */
            else if(meta_cols != NULL && meta_cols->has_bg_color) {

                cairo_set_source_rgb(cr, 
                    meta_cols->color[0], 
                    meta_cols->color[1], 
                    meta_cols->color[2]
                );
            }
            /* draw default background-color */
            else {

                int modulus = i % 2;

                cairo_set_source_rgb(cr, 
                    checkers[modulus][0], 
                    checkers[modulus][modulus], 
                    checkers[modulus][2]
                );
            }

            cairo_fill(cr);

            /* FIND META WITH HIGHEST PRECEDENCE */

            meta_temp = NULL;

            if(meta_cell->graphable) {

                meta_temp = meta_cell;
            }
            else if(meta_rows->graphable) {

                meta_temp = meta_rows;
            }
            else if(meta_cols->graphable) {

                meta_temp = meta_cols;
            }

            /* DRAW CELL DATA AS GRAPH */
            if(meta_temp != NULL && gtk_custom_table_string_is_integer(
                    priv->table_rows[i]->cell[j]->text)) {

                /* calculate graph size */
                graph_step = (priv->table_column_widths_temp[j] - 8) / 100.0;
                graph_amount = atoi(priv->table_rows[i]->cell[j]->text);
                graph_width = graph_amount * graph_step;

                cairo_rectangle(cr, 
                    priv->table_column_offset_temp[j] + 4, 
                    offset + 4, 
                    graph_width > 0 ? graph_width : 1,
                    priv->table_row_height - 8
                );

                cairo_set_source_rgb(cr, 
                    rgb_border[0], 
                    rgb_border[1], 
                    rgb_border[2]
                );

                cairo_stroke_preserve(cr);

                cairo_set_source_rgb(cr, 
                    meta_temp->graph[0], 
                    meta_temp->graph[1], 
                    meta_temp->graph[2]
                );

                cairo_fill(cr);
            }
            /* DRAW CELL DATA AS TEXT */
            else {

                cairo_set_source_rgb(cr, 
                    rgb_text[0], 
                    rgb_text[1], 
                    rgb_text[2]
                );

                /* align text */
                cairo_move_to(cr, 
                    priv->table_column_offset_temp[j] + 10, 
                    offset + 4
                );

                char *text_temp = NULL;

                /* column is an index, show numbering */
                if(priv->table_column_index[j]) {

                    char temp[10];
                    sprintf(temp, "%d", i + 1);
                    text_temp = temp;
                }
                else {

                    text_temp = priv->table_rows[i]->cell[j]->text;
                }

                /* determine cell text alignment */
                if(meta_cell->align != PANGO_ALIGN_NONE) {

                    meta_temp = meta_cell;
                }
                else if(meta_rows->align != PANGO_ALIGN_NONE) {

                    meta_temp = meta_rows;
                }
                else if(meta_cols->align != PANGO_ALIGN_NONE) {

                    meta_temp = meta_cols;
                }

                char *font_temp = NULL;

                /* determine cell font */
                if(priv->table_rows[i]->cell[j]->meta->font != NULL) {

                    font_temp = priv->table_rows[i]->cell[j]->meta->font;
                }
                else if(priv->table_rows[i]->meta->font != NULL) {

                    font_temp = priv->table_rows[i]->meta->font;
                }
                else {

                    font_temp = priv->table_cols[j]->meta->font;
                }

                /* determine background image */
                if(priv->table_rows[i]->cell[j]->meta->has_bg_image == TRUE) {

                    char *bg_image = priv->table_rows[i]->cell[j]->meta->bg_image;

                    cairo_save(cr);
                    cairo_surface_t *sur;
                    sur = cairo_image_surface_create_from_png(bg_image);

                    if(cairo_surface_status(sur) == CAIRO_STATUS_SUCCESS) {

                        cairo_set_source_surface(cr, sur, 
                            priv->table_column_offset_temp[j] + 10, 
                            offset + 5);
                        cairo_paint(cr);
                    }
                    cairo_restore(cr);
                }
                     
                /* BEGIN PANGO RENDERING */

                PangoLayout *layout = NULL;
                PangoFontDescription *description = NULL;

                layout = pango_cairo_create_layout(cr);
                description = pango_font_description_from_string(font_temp);
               
                pango_layout_set_text(layout, text_temp, -1);

                pango_layout_set_font_description(layout, 
                    description);

                pango_layout_set_width(layout, 
                    (priv->table_column_widths_temp[j] - 20) * PANGO_SCALE);

                pango_layout_set_height(layout, 
                    priv->table_row_height * PANGO_SCALE);

                pango_layout_set_ellipsize(layout, 
                    PANGO_ELLIPSIZE_END);

                pango_layout_set_alignment(layout, 
                    meta_temp->align);

                pango_cairo_show_layout(cr, layout);

                pango_font_description_free(description);
                g_object_unref(layout);
            }
        }
    }


    /* DRAW FOOTER ROW */ 
    if(priv->table_has_footer && (scroll_end_row >= priv->table_y)) {

        int height = priv->table_row_height * (priv->table_y + priv->table_has_header);

        for(i = 0; i < priv->table_x; i++) {

            /* skip hidden columns */
            if(priv->table_column_hidden[i] == TRUE) {
                continue;
            }
                
            cairo_set_source_rgb(cr, 
                rgb_border[0], 
                rgb_border[1], 
                rgb_border[2]
            );

            /* draw footer background color */
            cairo_rectangle(cr, 
                priv->table_column_offset_temp[i] + 0.5, 
                height, 
                priv->table_column_widths_temp[i], 
                priv->table_row_height
            );

            cairo_stroke_preserve(cr);

            cairo_set_source_rgb(cr, 
                rgb_footer[0], 
                rgb_footer[1], 
                rgb_footer[2]
            );

            cairo_fill(cr); 

            cairo_set_source_rgb(cr, 
                rgb_text[0], 
                rgb_text[1], 
                rgb_text[2]
            );

            cairo_move_to(cr, 
                priv->table_column_offset_temp[i] + 10, 
                height + 4
            );

            /* determine cell text alignment */
            if(priv->table_foot->cell[i]->meta->align != PANGO_ALIGN_NONE) {

                meta_temp = priv->table_foot->cell[i]->meta;
            }
            else if(priv->table_foot->meta->align != PANGO_ALIGN_NONE) {

                meta_temp = priv->table_foot->meta;
            }
            else {

                meta_temp = priv->table_cols[i]->meta;
            }
            
            char *font_temp = NULL;

            /* determine cell font */
            if(priv->table_foot->cell[i]->meta->font != NULL) {

                font_temp = priv->table_foot->cell[i]->meta->font;
            }
            else if(priv->table_foot->meta->font != NULL) {

                font_temp = priv->table_foot->meta->font;
            }
            else {

                font_temp = priv->table_cols[i]->meta->font;
            }
            
            /* BEGIN PANGO RENDERING */

            PangoLayout *layout = NULL;
            PangoFontDescription *description = NULL;

            layout = pango_cairo_create_layout(cr);
            description = pango_font_description_from_string(font_temp);

            pango_layout_set_text(layout, 
                priv->table_foot->cell[i]->text, -1);

            pango_layout_set_font_description(layout, 
                description);

            pango_layout_set_width(layout, 
                (priv->table_column_widths_temp[i] - 20) * PANGO_SCALE);

            pango_layout_set_height(layout, 
                priv->table_row_height * PANGO_SCALE);

            pango_layout_set_ellipsize(layout, 
                PANGO_ELLIPSIZE_END);

            pango_layout_set_alignment(layout, 
                meta_temp->align);

            pango_cairo_show_layout(cr, layout);

            pango_font_description_free(description);
            g_object_unref(layout);
        }
    }
}

