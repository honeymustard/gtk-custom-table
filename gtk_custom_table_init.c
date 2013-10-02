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


static void gtk_custom_table_class_init(GtkCustomTableClass *class);
static void gtk_custom_table_init(GtkCustomTable *table);


G_DEFINE_TYPE(GtkCustomTable, gtk_custom_table, GTK_TYPE_DRAWING_AREA);


/**
 * set-up custom widget signals, if applicable..
 * @param GtkCustomTableClass *class    
 */
static void gtk_custom_table_class_init(GtkCustomTableClass *class) {
    
    GtkWidgetClass *widget_class;

    widget_class = GTK_WIDGET_CLASS(class);
    widget_class->expose_event = gtk_custom_table_expose;
    
    g_type_class_add_private(class, sizeof(GtkCustomTablePrivate));
}


/**
 * add custom widget event masks and callbacks....
 * @param GtkCustomTable *table    
 */
static void gtk_custom_table_init(GtkCustomTable *table) {

    /* enable table events events */
    gtk_widget_add_events(GTK_WIDGET(table), 
        GDK_BUTTON_PRESS_MASK | 
        GDK_BUTTON_RELEASE_MASK | 
        GDK_POINTER_MOTION_MASK
    );

    /* connect these events to callbacks */
    g_signal_connect(G_OBJECT(table), "button_release_event", 
        G_CALLBACK(gtk_custom_table_mouse_released), NULL);

    g_signal_connect(G_OBJECT(table), "button_press_event", 
        G_CALLBACK(gtk_custom_table_clicked), NULL);
}

