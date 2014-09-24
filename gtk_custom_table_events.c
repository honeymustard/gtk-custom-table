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
#include <gdk/gdkkeysyms.h>


/**
 * configure handler for custom table..
 * @param GtkWidget *table    current table..
 * @param GdkEvent *event     configure event
 * @return gboolean           gboolean
 */
gboolean gtk_custom_table_config(GtkWidget *table, GdkEvent *event) {

    return FALSE;
}


/**
 * key-released handler for scrolling with arrow keys..
 * @param GtkWidget *table      current table..
 * @param GdkEventKey *event    key event
 * @return gboolean             gboolean
 */
gboolean gtk_custom_table_key_released(GtkWidget *table, GdkEventKey *event) {

    GtkAdjustment *adj = NULL;
    adj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(gtk_widget_get_parent(table)));

    int step = 80;
    int value = gtk_adjustment_get_value(adj);
    int upper = gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj);

    gboolean retval = FALSE;

    switch(event->keyval) {

        case GDK_KEY_Up:

            value -= step;
 
            if(value > 0) {

                gtk_widget_grab_focus(table);
            }

            gtk_adjustment_set_value(adj, value < 0 ? 0 : value);

            retval = value > 0 ? TRUE : FALSE;

            break;

        case GDK_KEY_Down:

            value += step;

            int cond = value < upper;

            if(cond) {

                gtk_widget_grab_focus(table);
            }

            gtk_adjustment_set_value(adj, !cond ? upper : value);

            retval = cond ? TRUE : FALSE;

            break;

        default:

            return FALSE;
    }

    return retval;
}


/**
 * custom table clicked handler, changes curson when clicking headers
 * @param GtkWidget *table         current table..
 * @param GdkEventMotion *event    gdk motion event
 * @return gboolean                gboolean
 */
gboolean gtk_custom_table_clicked(GtkWidget *table, GdkEventMotion *event) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* make sure table has a header, and is sortable */
    if(!priv->table_has_header || (priv->table_is_sortable == FALSE)) {
        return TRUE;
    }

    int i = 0;

    GdkCursor *cur;

    /* determine if click occured on header */
    if(event->y < priv->table_row_height) {

        for(i = 0; i < priv->table_x; i++) {

            if(event->x < priv->table_column_offset_temp[i+1]) {

                /* make sure column is not an index column */
                if(priv->table_column_index[i] == FALSE) {

                    cur = gdk_cursor_new(GDK_HAND1);
                    gdk_window_set_cursor(gtk_widget_get_window(table), cur);
                    g_object_unref(cur);

                    return FALSE;
                }

                break;
            }
        }
    }
   
    return TRUE;
}


/**
 * custom table mouse-button-released handler.. for clicking headers..
 * @param GtkWidget *table         current table..
 * @param GdkEventButton *event    button event
 * @return gboolean                gboolean
 */
gboolean gtk_custom_table_mouse_released(GtkWidget *table, GdkEventButton *event) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* make sure table has a header, and is sortable */
    if(!priv->table_has_header || (priv->table_is_sortable == FALSE)) {
        return TRUE;
    }

    /* make sure this was indeed a button release */
    if(event->type != GDK_BUTTON_RELEASE) {
        return TRUE;
    }

    GdkCursor *cur_arrow = gdk_cursor_new(GDK_ARROW);
    gdk_window_set_cursor(gtk_widget_get_window(table), cur_arrow);
    g_object_unref(cur_arrow);

    int i = 0;

    /* determine if click occured on header */
    if(event->y < priv->table_row_height) {

        for(i = 0; i < priv->table_x; i++) {

            if(event->x < priv->table_column_offset_temp[i+1]) {

                /* make sure column is not an index column */
                if(priv->table_column_index[i] == FALSE) {

                    gtk_custom_table_sort(table, i, GTK_CUSTOM_TABLE_INVERT);
                    gtk_custom_table_refresh(table);
                }

                return FALSE;
            }
        }
    }

    return TRUE;
}


/**
 * gtk custom table wheel-scroll event handler..
 * @param GtkWidget *table         table that was scrolled
 * @param GdkEventScroll *event    gdk scroll event
 * @return gboolean                gboolean
 */
gboolean gtk_custom_table_scroll(GtkWidget *table, GdkEventScroll *event) {

    gtk_widget_grab_focus(table);

    GtkAdjustment *adj = NULL;
    adj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(gtk_widget_get_parent(table)));

    int step = 200;
    int value = gtk_adjustment_get_value(adj);
    int upper = gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj);

    switch(event->direction) {

        case GDK_SCROLL_DOWN:

            value += step;
 
            gtk_adjustment_set_value(adj, value > upper ? upper : value);

            break;

        case GDK_SCROLL_UP:

            value -= step;
 
            gtk_adjustment_set_value(adj, value < 0 ? 0 : value);

            break;

        default:

            return FALSE;
    }

    return TRUE;
}


/**
 * gtk custom table draw event handler..
 * @param GtkWidget *table    table that was exposed
 * @param cairo_t *cr         cairo context
 * @return gboolean           returns true
 */
gboolean gtk_custom_table_draw(GtkWidget *table, cairo_t *cr, gpointer data) {

    gtk_custom_table_paint(table, cr);

    return TRUE;
}


/**
 * handle widget destroy event by freeing resources..
 * @param GtkWidget *table    current table
 * @param GdkEvent *event     destroy event
 * @return gboolean           returns false
 */
gboolean gtk_custom_table_destroy(GtkWidget *table, GdkEvent *event) {

    gtk_custom_table_free(table);

    return FALSE;
}

