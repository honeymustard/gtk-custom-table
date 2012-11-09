/****************************************************************************
*
* Copyright (C) 2011-2012  Adrian Solumsmo
* 
* imdb-plus program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* imdb-plus program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************/


#include "gtk_custom_table.h"


/**
 * free memory occupied by primed column's binary tree..
 * @param struct table_tree *tree    the root of the tree..
 */
void gtk_custom_table_tree_free(struct table_tree *tree) {

    if(tree != NULL) {
        
        /* depth-first traversal. textbook! */
        gtk_custom_table_tree_free(tree->left);
        gtk_custom_table_tree_free(tree->right);

        free(tree);
    }
}


/**
 * recursively look for value in tree specified by col, save row index
 * @param GtkCustomTablePrivate *priv    private table structure
 * @param struct table_tree *tree        binary tree with values
 * @param char *value                    value to be found
 * @param int col                        column in which to look
 */
void gtk_custom_table_tree_get_recurse(GtkCustomTablePrivate *priv, 
        struct table_tree *tree, char *value, int col) {

    /* if tree is null we've gone too far */
    if(tree == NULL) {
        return;
    }

    /* have we found the correct cell? */
    if(strcmp(value, (tree->data->cell[col])->text) == 0) {

        priv->table_tree_index = tree->data->row_current;
    }
    /* else, go left then right */
    else {

        gtk_custom_table_tree_get_recurse(
            priv, tree->left, value, col);

        gtk_custom_table_tree_get_recurse(
            priv, tree->right, value, col);
    }
}


/**
 * add a reference to a table row in a binary tree based on primary index..
 * @param struct table_tree *tree     widget's index tree..
 * @param struct table_cell **data    row of table cells in question..
 * @param int primary                 primary index column..
 */
void gtk_custom_table_tree_add(struct table_tree *tree, struct table_rows *data, 
        int primary) {

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

                tree->left = malloc(sizeof (struct table_tree));
                (tree->left)->data = NULL;
            }

            gtk_custom_table_tree_add(tree->left, data, primary);
        }
        /* else we'll go right.. */
        else if(compare < 0) {

            /* if this is a null node, create a new one */
            if(tree->right == NULL) {

                tree->right = malloc(sizeof (struct table_tree));
                (tree->right)->data = NULL;
            }

            gtk_custom_table_tree_add(tree->right, data, primary);
        }
    }
}

