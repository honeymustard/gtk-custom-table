#include <stdio.h>
#include "gtk_custom_table.h"


int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    printf("Gtk Custom Table\n");

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Examples");

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    //gtk_box_pack_start(GTK_BOX(vbox), mwin->menu, FALSE, FALSE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* last steps, show window */
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

