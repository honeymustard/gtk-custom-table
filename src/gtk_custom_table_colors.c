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


/** zebra stripes for rows */
double checkers[2][3] = {
    {1.00, 1.00, 1.00}, 
    {0.90, 0.90, 0.95}, 
};

/** default header row color */
double rgb_header[3] = {
    0.85, 
    0.85, 
    0.85, 
};

/** default footer row color */
double rgb_footer[3] = {
    0.8, 
    0.8, 
    0.8, 
};

/** default bold header row color */
double rgb_header_bold[3] = {
    0.90, 
    0.90, 
    0.90, 
};

/** default border color */
double rgb_border[3] = {
    0.2, 
    0.2, 
    0.2, 
};

/** default text color */
double rgb_text[3] = {
    0.1, 
    0.1, 
    0.1, 
};

/** default graph color */
double rgb_graph[] = {
    0.00, 
    0.90, 
    0.00,
};

/** default cell color */
double rgb_cell[] = {
    1.00, 
    1.00, 
    1.00,
};

