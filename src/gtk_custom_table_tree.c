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
 * @brief free memory occupied by primed column's binary tree..
 * @param tree     the root of the tree..
 * @return void
 */
void gtk_custom_table_tree_free(TableTree *tree) {

    if(tree != NULL) {
        
        /* depth-first traversal. textbook! */
        gtk_custom_table_tree_free(tree->left);
        gtk_custom_table_tree_free(tree->right);

        free(tree);
    }
}


/**
 * @brief recursively get value from tree specified by col, save row index
 * @param table    private table structure
 * @param tree     binary tree with values
 * @param value    value to be found
 * @param col      column in which to look
 * @return void
 */
void gtk_custom_table_tree_get(GtkWidget *table, TableTree *tree, 
    char *value, int col) {

    GtkCustomTablePrivate *priv;
    priv = GTK_CUSTOM_TABLE_GET_PRIVATE(table);

    /* if tree is null we've gone too far */
    if(tree == NULL) {
        return;
    }

    /* have we found the correct cell? */
    if(strcmp(value, (tree->data->cell[col])->text) == 0) {

        priv->tree_index = tree->data->row_temp;
    }
    /* else, go left then right */
    else {

        gtk_custom_table_tree_get(table, tree->left, value, col);
        gtk_custom_table_tree_get(table, tree->right, value, col);
    }
}


/**
 * @brief add a reference to a table row in a binary tree based on primary index..
 * @param tree       widget's index tree..
 * @param data       row of table cells in question..
 * @param primary    primary index column..
 * @return void
 */
void gtk_custom_table_tree_add(TableTree *tree, TableRows *data, int primary) {

    /* found an empty node, add to it and stop */
    if(tree->data == NULL) {

        tree->data = data;
        tree->left = NULL;
        tree->right = NULL;
    }
    /* compare text in primary index to current node text */
    else {

        int compare = strcmp(tree->data->cell[primary]->text, 
            data->cell[primary]->text);

        /* we'll go left in such cases.. */
        if(compare > 0 || compare == 0) {

            /* if this is a null node, create a new one */
            if(tree->left == NULL) {

                tree->left = malloc(sizeof(TableTree));
                (tree->left)->data = NULL;
            }

            gtk_custom_table_tree_add(tree->left, data, primary);
        }
        /* else we'll go right.. */
        else if(compare < 0) {

            /* if this is a null node, create a new one */
            if(tree->right == NULL) {

                tree->right = malloc(sizeof(TableTree));
                (tree->right)->data = NULL;
            }

            gtk_custom_table_tree_add(tree->right, data, primary);
        }
    }
}

