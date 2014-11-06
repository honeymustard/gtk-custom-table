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
#include <ctype.h>
#include <stdio.h>


/** @file */


/**
 * @brief test if a string can be represented as an integer..
 * @param string    string to be tested
 * @return int      returns 1 if true else 0
 */
int gtk_custom_table_string_is_integer(char *string) {

    int i = 0;

    for(i = 0; string[i] != '\0'; i++) {
        
        if(isdigit(string[i]) == 0) {
            return 0;
        }
    }

    return 1;
}


/**
 * @brief sanitize a string and convert it to integer
 * @param string            string to be converted
 * @return unsigned long    returns an integer
 */
gulong gtk_custom_table_string_parseint(char *string) {

    int i = 0;
    int b = 0;

    char *temp = malloc(strlen(string) + 1);

    for(i = 0; i < strlen(string); i++) {
    
        if(string[i] >= 48 && string[i] <= 57) {

            temp[b++] = string[i];
        }
    }

    temp[b] = '\0';

    gulong result = strtoul(temp, NULL, 10);

    free(temp);

    return result;
}

