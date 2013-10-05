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
 * custom table clicked handler, changes curson when clicking headers
 * @param GtkWidget *table         current table..
 * @param GdkEventMotion *event    gdk motion event
 * @return                         gboolean
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
                    gdk_window_set_cursor(table->window, cur);
                    gdk_cursor_unref(cur);

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
 * @return                         gboolean
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
    gdk_window_set_cursor(table->window, cur_arrow);
    gdk_cursor_unref(cur_arrow);

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
 * gtk custom table expose event handler..
 * @param GtkWidget *table         table that was exposed
 * @param GdkEventExpose *event    gdk expose-event object
 * @return gboolean                returns false
 */
gboolean gtk_custom_table_expose(GtkWidget *table, GdkEventExpose *event) {

#ifdef GDK_WINDOWING_WIN32

    GtkCustomTablePrivate *priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* use invalidate_rect here to combat smooth-scroll lag on windows */
    if(priv->table_scroll_lock == 0) {

        gdk_window_invalidate_rect(GDK_WINDOW(table->window), 
            &table->allocation, TRUE);
        priv->table_scroll_lock = 1;

        return FALSE;
    }

    gtk_custom_table_paint(table, event);
    priv->table_scroll_lock = 0;

#endif
#ifdef GDK_WINDOWING_X11

    gtk_custom_table_paint(table, event);

#endif

    return TRUE;
}

