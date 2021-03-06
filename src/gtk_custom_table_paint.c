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
 * @brief calculate widget dimensions, paint to cairo context
 * @param table    current table
 * @param cr       cairo context
 * @return void
 */
void gtk_custom_table_paint(GtkWidget *table, cairo_t *cr) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* update table dimensions */
    gtk_custom_table_calc_cols(table);
    gtk_custom_table_calc_rows(table);
    gtk_custom_table_calc_clip(table);

    #ifdef DEBUG

    printf("\n paint rows : (%d -> %d)\n", priv->clip_upper, priv->clip_lower);
    printf("\n paint cols : (%d -> %d)\n", priv->clip_left, priv->clip_right);
    
    #endif

    cairo_set_line_width(cr, 1);

    PangoAlignment alignment = 0;
    char *font = NULL;
    double *graph = NULL;

    int i = 0;
    int j = 0;

    /* DRAW HEADER ROW */

    if(priv->has_header && priv->clip_upper == 0) {

        for(i = priv->clip_left; i < priv->clip_right; i++) {

            if(priv->cols[i]->hidden) {
                continue;
            }

            cairo_set_source_rgb(cr,
                rgb_border[0],
                rgb_border[1],
                rgb_border[2]
            );

            cairo_rectangle(cr,
                priv->col_offset_temp[i] + 0.5,
                1,
                priv->cols[i]->width_temp,
                priv->head->height_temp - 1.5
            );

            cairo_stroke_preserve(cr);

            /* column selected background color */
            if(priv->sort_index == i) {

                cairo_set_source_rgb(cr,
                    rgb_bold[0],
                    rgb_bold[1],
                    rgb_bold[2]
                );
            }
            /* the default background color */
            else {

                cairo_set_source_rgb(cr,
                    rgb_header[0],
                    rgb_header[1],
                    rgb_header[2]
                );
            }

            cairo_fill(cr); 

            cairo_set_source_rgb(cr,
                rgb_text[0],
                rgb_text[1],
                rgb_text[2]
            );

            /* align text */
            cairo_move_to(cr, 
                priv->col_offset_temp[i] + 10, 
                4
            );

            /* determine cell text alignment */
            if(priv->head->cell[i]->meta->has_alignment) {

                alignment = priv->head->cell[i]->meta->alignment;
            }
            else if(priv->head->meta->has_alignment) {

                alignment = priv->head->meta->alignment;
            }
            else {

                alignment = PANGO_ALIGN_RIGHT;
            }

            /* determine cell font */
            if(priv->head->cell[i]->meta->has_font) {

                font = priv->head->cell[i]->meta->font;
            }
            else if(priv->head->meta->has_font) {

                font = priv->head->meta->font;
            }
            else {

                font = GCT_FONT_MONO;
            }
             
            /* BEGIN PANGO RENDERING */

            PangoLayout *layout = NULL;
            PangoFontDescription *description = NULL;

            layout = pango_cairo_create_layout(cr);
            description = pango_font_description_from_string(font);

            /* make sure text is not empty */
            if(priv->head->cell[i]->text != NULL) {

                pango_layout_set_text(layout, 
                    priv->head->cell[i]->text, -1);
            }

            pango_layout_set_font_description(layout, description);

            pango_layout_set_width(layout, 
                (priv->cols[i]->width_temp - 20) * PANGO_SCALE);

            pango_layout_set_height(layout, 
                priv->head->height_temp * PANGO_SCALE);

            pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
            pango_layout_set_alignment(layout, alignment);
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

    /* only draw visible rows */
    for(i = priv->clip_upper; i < priv->clip_lower; i++) {

        /* skip hidden rows */
        if(priv->rows[i]->hidden) {
            continue;
        }

        /* get row meta data */
        meta_rows = priv->rows[i]->meta;

        for(j = priv->clip_left; j < priv->clip_right; j++) {

            /* skip hidden columns */
            if(priv->cols[j]->hidden) {
                continue;
            }

            /* get cell, col meta data */
            meta_cell = priv->rows[i]->cell[j]->meta;
            meta_cols = priv->cols[j]->meta;
            
            /* DRAW CELL BACKGROUND COLOR */

            cairo_rectangle(cr, 
                priv->col_offset_temp[j] + 0.5,
                priv->row_offset_temp[i],
                priv->cols[j]->width_temp,
                priv->rows[i]->height_temp
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

                double *stripe = i % 2 == 0 ? rgb_white : rgb_gray;

                cairo_set_source_rgb(cr,
                    stripe[0],
                    stripe[1],
                    stripe[2]
                );
            }

            cairo_fill(cr);

            /* FIND META WITH HIGHEST PRECEDENCE */

            if(meta_cell->has_graph) {

                graph = meta_cell->graph;
            }
            else if(meta_rows->has_graph) {

                graph = meta_rows->graph;
            }
            else if(meta_cols->has_graph) {

                graph = meta_cols->graph;
            }
            else {

                graph = NULL;
            }

            /* DRAW CELL DATA AS GRAPH */
            if(graph != NULL && gtk_custom_table_string_is_integer(
                priv->rows[i]->cell[j]->text)) {

                /* calculate graph size */
                graph_step = (priv->cols[j]->width_temp - 8) / 100.0;
                graph_amount = atoi(priv->rows[i]->cell[j]->text);
                graph_width = graph_amount * graph_step;

                cairo_rectangle(cr, 
                    priv->col_offset_temp[j] + 4,
                    priv->row_offset_temp[i] + 4,
                    graph_width > 0 ? graph_width : 1,
                    priv->rows[i]->height_temp - 8
                );

                cairo_set_source_rgb(cr, 
                    rgb_border[0],
                    rgb_border[1],
                    rgb_border[2]
                );

                cairo_stroke_preserve(cr);

                cairo_set_source_rgb(cr, 
                    graph[0],
                    graph[1],
                    graph[2]
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
                    priv->col_offset_temp[j] + 10, 
                    priv->row_offset_temp[i] + 4
                );

                char *text_temp = NULL;

                /* column is an index, show numbering */
                if(priv->cols[j]->index) {

                    char temp[10];
                    sprintf(temp, "%d", i + 1);
                    text_temp = temp;
                }
                else {

                    text_temp = priv->rows[i]->cell[j]->text;
                }

                /* determine cell text alignment */
                if(meta_cell->has_alignment) {

                    alignment = meta_cell->alignment;
                }
                else if(meta_rows->has_alignment) {

                    alignment = meta_rows->alignment;
                }
                else if(meta_cols->has_alignment) {

                    alignment = meta_cols->alignment;
                }
                else {

                    alignment = PANGO_ALIGN_RIGHT;
                }

                /* determine cell font */
                if(priv->rows[i]->cell[j]->meta->has_font) {

                    font = priv->rows[i]->cell[j]->meta->font;
                }
                else if(priv->rows[i]->meta->font) {

                    font = priv->rows[i]->meta->font;
                }
                else if(priv->cols[j]->meta->has_font) {

                    font = priv->cols[j]->meta->font;
                }
                else {
                    
                    font = GCT_FONT_MONO;
                }

                /* determine background image */
                if(priv->rows[i]->cell[j]->meta->has_bg_image) {

                    char *bg_image = priv->rows[i]->cell[j]->meta->bg_image;

                    cairo_save(cr);

                    cairo_surface_t *surface;
                    surface = cairo_image_surface_create_from_png(bg_image);

                    if(cairo_surface_status(surface) == CAIRO_STATUS_SUCCESS) {

                        cairo_set_source_surface(cr,
                            surface,
                            priv->col_offset_temp[j] + 10,
                            priv->row_offset_temp[i] + 5
                        );

                        cairo_paint(cr);
                    }

                    cairo_restore(cr);
                }
                     
                /* BEGIN PANGO RENDERING */

                PangoLayout *layout = NULL;
                PangoFontDescription *description = NULL;

                layout = pango_cairo_create_layout(cr);
                description = pango_font_description_from_string(font);
               
                /* make sure text is not empty */
                if(text_temp != NULL) {

                    pango_layout_set_text(layout, text_temp, -1);
                }

                int text_box = priv->cols[j]->width_temp - 20;

                pango_layout_set_font_description(layout, description);

                pango_layout_set_width(layout, 
                    (text_box > 0 ? text_box : 1) * PANGO_SCALE);

                pango_layout_set_height(layout, 
                    priv->rows[i]->height_temp * PANGO_SCALE);

                pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
                pango_layout_set_alignment(layout, alignment);
                pango_cairo_show_layout(cr, layout);
                pango_font_description_free(description);

                g_object_unref(layout);
            }
        }
    }

    /* DRAW FOOTER ROW */ 
    if(priv->has_footer && priv->clip_lower == priv->y) {

        for(i = priv->clip_left; i < priv->clip_right; i++) {

            /* skip hidden columns */
            if(priv->cols[i]->hidden) {
                continue;
            }
                
            cairo_set_source_rgb(cr,
                rgb_border[0],
                rgb_border[1],
                rgb_border[2]
            );

            /* draw footer background color */
            cairo_rectangle(cr,
                priv->col_offset_temp[i] + 0.5,
                priv->row_offset_temp[priv->y],
                priv->cols[i]->width_temp,
                priv->foot->height_temp
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
                priv->col_offset_temp[i] + 10,
                priv->row_offset_temp[priv->y] + 4
            );

            /* determine cell text alignment */
            if(priv->foot->cell[i]->meta->has_alignment) {

                alignment = priv->foot->cell[i]->meta->alignment;
            }
            else if(priv->foot->meta->has_alignment) {

                alignment = priv->foot->meta->alignment;
            }
            else {

                alignment = PANGO_ALIGN_RIGHT;
            }
            
            /* determine cell font */
            if(priv->foot->cell[i]->meta->has_font) {

                font = priv->foot->cell[i]->meta->font;
            }
            else if(priv->foot->meta->has_font) {

                font = priv->foot->meta->font;
            }
            else {

                font = GCT_FONT_MONO;
            }
            
            /* BEGIN PANGO RENDERING */

            PangoLayout *layout = NULL;
            PangoFontDescription *description = NULL;

            layout = pango_cairo_create_layout(cr);
            description = pango_font_description_from_string(font);

            /* make sure text is not empty */
            if(priv->foot->cell[i]->text != NULL) {

                pango_layout_set_text(layout, 
                    priv->foot->cell[i]->text, -1);
            }

            pango_layout_set_font_description(layout, description);

            pango_layout_set_width(layout, 
                (priv->cols[i]->width_temp - 20) * PANGO_SCALE);

            pango_layout_set_height(layout, 
                priv->foot->height_temp * PANGO_SCALE);

            pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
            pango_layout_set_alignment(layout, alignment);
            pango_cairo_show_layout(cr, layout);
            pango_font_description_free(description);

            g_object_unref(layout);
        }
    }
}

