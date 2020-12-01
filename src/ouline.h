/* ouline.h: header for layout/dimentioning and drawing routines for over
   and underlines. */

/*  This file is part of asciiTeX.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; see the file COPYING.  If not, write to
      The Free Software Foundation, Inc.
      59 Temple Place, Suite 330
      Boston, MA 02111 USA


    Authors:
    Original program (eqascii): Przemek Borys
    Fork by: Bart Pieters
    Fork by: Lars Eggert (https://github.com/larseggert/asciiTeX)

*************************************************************************/

#ifndef OULINEL_H
#define OULINEL_H

int dimOverl(char * found, char ** Gpos, Tdim * Our, struct Tgraph * graph);
void drawOverl(int * Kid,
               int * Curx,
               int * Cury,
               char *** screen,
               struct Tgraph * graph);

int dimUnderl(char * found, char ** Gpos, Tdim * Our, struct Tgraph * graph);
void drawUnderl(int * Kid,
                int * Curx,
                int * Cury,
                char *** screen,
                struct Tgraph * graph);

#endif
