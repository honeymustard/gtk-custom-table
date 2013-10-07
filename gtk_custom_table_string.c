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
#include <ctype.h>
#include <stdio.h>


/**
 * test if a string can be represented as an integer..
 * @param char *string    string to be tested
 * @return int            returns 1 if true else 0
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
 * sanitize a string and convert it to integer..
 * @param char *string    string to be converted
 * @return char *         returns an integer
 */
unsigned long gtk_custom_table_string_parseint(char *string) {

    int i = 0;
    int b = 0;

    char *temp = malloc(strlen(string) + 1);

    for(i = 0; i < strlen(string); i++) {
    
        if(string[i] >= 48 && string[i] <= 57) {

            temp[b++] = string[i];
        }
    }

    temp[b] = '\0';

    unsigned long result = strtoul(temp, NULL, 10);

    free(temp);

    return result;
}


/**
 * strip alphanumeric chars from string..
 * @param char *string    string to be stripped
 * @return char *         returns the stripped string
 */
char * gtk_custom_table_string_strip_alpha(char *string) {

    int i = 0;
    int b = 0;

    char *temp = malloc(strlen(string) + 1);

    for(i = 0; i < strlen(string); i++) {

        if(string[i] >= 48 && string[i] <= 57) {

            temp[b++] = string[i];
        }
    }

    temp[b] = '\0';

    return temp;
}


/**
 * add thousand separators to integer string..
 * @param char *string    string to be converted
 * @return char *         returns the converted string
 */
char * gtk_custom_table_string_thousand_separator(char *string) {

    int i = 0;
    int j = 0;
    int b = 0;
    int c = 0;

    char *temp = malloc(strlen(string) * 2);

    for(i = strlen(string) -1; i >= 0; i--, c++) {

        if(c % 3 == 0 && b > 0) {

            temp[b++] = '.';
        }
        
        temp[b++] = string[i];
    }

    temp[b] = '\0';

    int length = strlen(temp) - 1;

    for(i = 0, j = length; i < j; i++, j--) {

         c = temp[i];
         temp[i] = temp[j];
         temp[j] = c;
    }

    return temp;
}

