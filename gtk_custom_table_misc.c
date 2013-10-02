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
 */
int gtk_custom_table_is_integer(char *string) {

    int i = 0;

    for(i = 0; string[i] != '\0'; i++) {
        
        if(isdigit(string[i]) == 0) {
            return 0;
        }
    }

    return 1;
}

