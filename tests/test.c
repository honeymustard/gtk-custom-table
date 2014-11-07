#include <stdio.h>
#include "gtk_custom_table.h"


double rgb_red[3] = {
    1.0,
    0.7,
    0.7
};

double rgb_green[3] = {
    0.7,
    1.0,
    0.7
};

double rgb_blue[3] = {
    0.7,
    0.7,
    1.0
};


/**
 * @brief all tests which will occur after an event go here
 * @param data         a gpointer to a gtk_custom_table widget
 * @return gboolean    returns true
 */
gboolean console_debug(gpointer data) {

    GtkWidget *table = (GtkWidget *)data;

    char *code[] = {"NB!", "OK!"};

    int cols = gtk_custom_table_get_cols(table);
    int rows = gtk_custom_table_get_rows(table);
    int col0 = gtk_custom_table_get_col_width(table, 0);
    int col1 = gtk_custom_table_get_col_width(table, 1);
    int col3 = gtk_custom_table_get_col_width(table, 3);

    printf("\n GtkCustomTable debugging:\n");

    printf(" cols        : %4d %s\n", cols, code[cols == 4]);
    printf(" rows        : %4d %s\n", rows, code[rows == 60]);
    printf(" col0 width  : %4d %s\n", col0, code[col0 == 93]);
    printf(" col1 width  : %4d %s\n", col1, code[col1 == 101]);
    printf(" col3 width  : %4d %s\n", col3, code[col3 == 89]);

    return TRUE;
}


int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    /* initiate widgets */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *table = gtk_custom_table_new(4, 80);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *viewport = gtk_viewport_new(NULL, NULL);

    /* set column widths */
    gtk_custom_table_set_col_width(table, 0, 93);
    gtk_custom_table_set_col_width(table, 1, 101);
    gtk_custom_table_set_col_width(table, 2, -1);
    gtk_custom_table_set_col_width(table, 3, 89);

    gtk_custom_table_set_row_height(table, 1, 31);
    gtk_custom_table_set_row_height(table, 2, 60);
    gtk_custom_table_set_row_height(table, 7, 29);
    gtk_custom_table_set_row_height(table, 9, 30);

    gtk_custom_table_set_row_hide(table, 2, TRUE);

    gtk_custom_table_resize(table, 4, 60);
    gtk_custom_table_resize(table, 6, 90);
    gtk_custom_table_resize(table, 3, 92);
    gtk_custom_table_resize(table, 4, 60);

    gtk_custom_table_set_col_alignment(table, 0, PANGO_ALIGN_RIGHT);
    gtk_custom_table_set_col_alignment(table, 1, PANGO_ALIGN_LEFT);
    gtk_custom_table_set_col_alignment(table, 2, PANGO_ALIGN_CENTER);

    gtk_custom_table_set_col_color(table, 2, rgb_red);
    gtk_custom_table_set_row_color(table, 3, rgb_green);
    gtk_custom_table_set_cell_color(table, 2, 3, rgb_blue);

    /* set cell text */
    char temp[10];

    int i = 0;
    int j = 0;

    for(i = 0; i < gtk_custom_table_get_cols(table); i++) {

        sprintf(temp, "(0,%d)", i);
        gtk_custom_table_set_head_text(table, i, temp);
    }

    for(i = 0; i < gtk_custom_table_get_rows(table); i++) {

        for(j = 0; j < gtk_custom_table_get_cols(table); j++) {

            sprintf(temp, "(%d,%d)", i, j);
            gtk_custom_table_set_cell_text(table, j, i, temp);
        }
    }

    /* window meta */
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);

    /* set menubar */
    GtkWidget *menu = gtk_menu_bar_new();
    GtkWidget *menu_file = gtk_menu_new();
    GtkWidget *menu_file_item = gtk_menu_item_new_with_mnemonic("_File");
    GtkWidget *menu_file_item_debug = gtk_menu_item_new_with_mnemonic("_Debug");

    /* accelerator keys */
    GtkAccelGroup *group_file = gtk_accel_group_new();
    gtk_menu_set_accel_group(GTK_MENU(menu_file), group_file);
    gtk_window_add_accel_group(GTK_WINDOW(window), group_file);
    gtk_widget_add_accelerator(menu_file_item_debug, "activate", 
        group_file, GDK_KEY_D, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    /* append items and menus */
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_file_item_debug);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_file_item), menu_file);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_file_item);

    /* add containers */
    gtk_container_add(GTK_CONTAINER(viewport), table);
    gtk_container_add(GTK_CONTAINER(scroll), viewport);
    gtk_box_pack_start(GTK_BOX(vbox), menu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* add signals */
    g_signal_connect_swapped(G_OBJECT(menu_file_item_debug), 
        "activate", G_CALLBACK(console_debug), (gpointer)table);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* show window */
    gtk_window_set_title(GTK_WINDOW(window), "Test");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

