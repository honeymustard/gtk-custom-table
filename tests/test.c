#include <stdio.h>
#include "gtk_custom_table.h"


int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    /* initiate widgets */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *table = gtk_custom_table_new(4, 80);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *viewport = gtk_viewport_new(NULL, NULL);

    /* add containers */
    gtk_container_add(GTK_CONTAINER(viewport), table);
    gtk_container_add(GTK_CONTAINER(scroll), viewport);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    int i = 0;

    /* set column widths */
    int widths[] = {
        100, 100, -1, 100
    };

    for(i = 0; i < 4; i++) {
        gtk_custom_table_set_col_width(table, i, widths[i]);
    }

    gtk_custom_table_set_row_height(table, 2, 60);
    gtk_custom_table_resize(table, 4, 60);

    /* set cell text */
    char temp[10];

    for(i = 0; i < gtk_custom_table_get_rows(table); i++) {

        sprintf(temp, "%d", i + 1);
        gtk_custom_table_set_cell_text(table, 0, i, temp);
        sprintf(temp, "%d", (i + 1) * 25);
        gtk_custom_table_set_cell_text(table, 1, i, temp);
    }

    /* table meta */
    gtk_custom_table_set_col_alignment(table, 1, PANGO_ALIGN_LEFT);

    /* window meta */
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);

    /* add signals */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* show window */
    gtk_window_set_title(GTK_WINDOW(window), "Examples");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

