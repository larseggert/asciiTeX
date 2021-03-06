// sscript.c: layout/dimentioning and drawing routines for super and sub scripts
//
// This file is part of asciiTeX.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// The Free Software Foundation, Inc.
// 59 Temple Place, Suite 330
// Boston, MA 02111 USA
//
// Authors:
// Original program (eqascii): Przemek Borys
// Fork by: Bart Pieters
// Fork by: Lars Eggert (https://github.com/larseggert/asciiTeX)

#include <stdlib.h>
#include <wchar.h>

#include "asciiTeX_struct.h"
#include "dim.h"
#include "draw.h"
#include "parsedef.h"
#include "sscript.h"
#include "utils.h"


long dimSubscript(wchar_t * found,
                  wchar_t ** Gpos,
                  Tdim * Our,
                  struct Tgraph * graph)
/*
The dimXxx routines all have the forllowing arguments:
found		--	Pointer to a string containing the remaining part of the
equation Gpos		--	Pointer to a string which will contain the part
of the equation relevant to the current parent with flags to indicate which
drawing routines to use. Our		--	dimension of the parent graph
--	The parent The routines returns the number of characters it used of the
found vector.
*/
{
#define gpos (*Gpos)
#define our (*Our)
    wchar_t *start = found + 1, *end, *tmp;
    Tdim out;

    *gpos = 1;
    gpos++;
    *gpos = SUB;
    gpos++;
    *gpos = 0;

    end = findClosingBrace(start + 1);
    *end = 0;
    tmp = wcsdup(start + 1);
    *end = L'}';
    out = dim(tmp, newChild(graph));
    free(tmp);
    if (out.y > our.baseline) {
        our.baseline = out.y;
        our.y += out.y;
    }
    if (gpos - graph->txt >= 4) { /* check if we had a superscript before */
        if ((*(gpos - 4) == 1) && (*(gpos - 3) == SUPER)) { /* yep, we had it */
            long width = graph->down[graph->children - 2]->dim.x;
            if (width < out.x)
                our.x += out.x - width;
        } else
            our.x += out.x;
    } else
        our.x += out.x;
    return end - found;
#undef gpos
#undef our
}

void drawSubscript(long * Kid,
                   long * Curx,
                   long * Cury,
                   wchar_t *** screen,
                   struct Tgraph * graph,
                   wchar_t * txt)
{
#define kid (*Kid)
#define curx (*Curx)
#define cury (*Cury)
    long width = graph->down[kid]->dim.x;
    if (txt - 3 >= graph->txt) {
        if ((*(txt - 3) == 1) && (*(txt - 2) == SUPER)) {
            drawInternal(screen, graph->down[kid],
                         curx - (graph->down[kid - 1]->dim.x), cury + 1);
            if (width > graph->down[kid - 1]->dim.y)
                curx += width - graph->down[kid - 1]->dim.y;
        } else {
            drawInternal(screen, graph->down[kid], curx, cury + 1);
            curx += width;
        }
    } else {
        drawInternal(screen, graph->down[kid], curx, cury + 1);
        curx += width;
    }
    kid++;
}

long dimSuperscript(wchar_t * found,
                    wchar_t ** Gpos,
                    Tdim * Our,
                    struct Tgraph * graph)
/*
The dimXxx routines all have the forllowing arguments:
found		--	Pointer to a string containing the remaining part of the
equation Gpos		--	Pointer to a string which will contain the part
of the equation relevant to the current parent with flags to indicate which
drawing routines to use. Our		--	dimension of the parent graph
--	The parent The routines returns the number of characters it used of the
found vector.
*/
{
#define gpos (*Gpos)
#define our (*Our)
    wchar_t *start = found + 1, *end, *tmp;
    Tdim out;

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = SUPER;
    gpos++;
    *gpos = 0;

    end = findClosingBrace(start + 1);
    *end = 0;
    tmp = wcsdup(start + 1);
    *end = L'}';
    out = dim(tmp, newChild(graph));
    free(tmp);
    if (out.y + 1 > our.y - our.baseline)
        our.y = our.baseline + out.y + 1;

    if (gpos - graph->txt >= 4) { /* check if we had a subscript before */
        if ((*(gpos - 4) == 1) &&
            (*(gpos - 3) == SUB)) { /* yep, we had it--our superscript will
                                     * start at the same pos as the subscript */
            long width = graph->down[graph->children - 2]->dim.x;
            if (width < out.x)
                our.x += out.x - width;
        } else
            our.x += out.x;
    } else
        our.x += out.x;
    return end - found;
}

void drawSuperscript(long * Kid,
                     long * Curx,
                     long * Cury,
                     wchar_t *** screen,
                     struct Tgraph * graph,
                     wchar_t * txt)
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define kid (*Kid)
#define curx (*Curx)
#define cury (*Cury)
    long width = graph->down[kid]->dim.x;
    if (txt - 3 >= graph->txt) {
        if ((*(txt - 3) == 1) && (*(txt - 2) == SUB)) {
            drawInternal(screen, graph->down[kid],
                         curx - (graph->down[kid - 1]->dim.x),
                         cury - (graph->down[kid]->dim.y));
            if (width > graph->down[kid - 1]->dim.x)
                curx += width - graph->down[kid - 1]->dim.x;
        } else {
            drawInternal(screen, graph->down[kid], curx,
                         cury - (graph->down[kid]->dim.y));
            curx += width;
        }
    } else {
        drawInternal(screen, graph->down[kid], curx,
                     cury - (graph->down[kid]->dim.y));
        curx += width;
    }
    kid++;
}
