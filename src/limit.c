// limit.c: layout/dimensioning and drawing routines for limits
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asciiTeX_struct.h"
#include "dim.h"
#include "draw.h"
#include "parsedef.h"
#include "utils.h"


int dimLimit(wchar_t * found,
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph)
/*
The dimXxx routines all have the forllowing arguments:
found		--	Pointer to a sting containing the remaining part of the
equation Gpos		--	Pointer to a string which will contain the part
of the equation relevant to the current parent with flags to indicate which
drawing routines to use. Our		--	dimention of the parent graph
--	The parent The routines returns the number of characters it used of the
found vector.
*/
{
#define gpos (*Gpos)
#define our (*Our)
    wchar_t *start, *end, *tmp;
    Tdim out;

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = (char)LIMIT;
    gpos++;
    *gpos = 0;

    start = wcschr(found, '{');
    if (!start) {
        SyntaxError(L"Usage: \\limit{X}\n\tProduces a limit\n");
        return 0;
    }
    end = findClosingBrace(start + 1);
    if (end - start < 2) {
        SyntaxError(L"Usage: \\limit{X}\n\tProduces a limit\n\te.g \\lim{x \\to "
                    "\\infty}\n");
        return 0;
    }

    *end = 0;
    tmp = wcsdup(start + 1);
    *end = '}';
    out = dim(tmp, newChild(graph));
    free(tmp);

    if (start - found - 6 > 0)
        SyntaxWarning(L"Warning: Spurious characters ignored in \\limit\n");

    out.baseline = out.y; /* expressison under limit */
    out.y++;              /* add the line under for the limit text */

    if (out.x > 3)
        our.x += out.x + 1;
    else
        our.x += 4;

    if (our.baseline < out.baseline) {
        our.y += (out.baseline - our.baseline);
        our.baseline = out.baseline;
    }
    if (our.y - our.baseline < (out.y - out.baseline)) {
        /*
         * our.baseline++;
         */
        our.y = (out.y - out.baseline) + our.baseline;
    }
    return end - (found);
#undef gpos
#undef our
}

void drawLimit(int * Kid,
               int * Curx,
               int * Cury,
               wchar_t *** screen,
               struct Tgraph * graph)
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
    int width = graph->down[kid]->dim.x;
    if (width < 3)
        width = 3;
    drawInternal(screen, graph->down[kid], curx, cury + 1);
    (*screen)[cury][curx + (width - 1) / 2 - 1] = 'l';
    (*screen)[cury][curx + (width - 1) / 2] = 'i';
    (*screen)[cury][curx + (width - 1) / 2 + 1] = 'm';
    curx += width + 1;

    kid++;
}
