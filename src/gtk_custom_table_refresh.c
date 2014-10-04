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


/** @file */


/**
 * @brief minor convenience function to force repaint of widget..
 * @param GtkWidget *table    table widget to refresh
 * @return void
 */
void gtk_custom_table_refresh(GtkWidget *widget) {

    gdk_window_invalidate_rect(GDK_WINDOW(gtk_widget_get_window(widget)), NULL, TRUE);
}

