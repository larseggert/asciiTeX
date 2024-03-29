// binom.c: layout/dimensioning and drawing routines for binomials
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
// Fork by: Lars Eggert (https://github.com/larseggert/asciiTeX)

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "asciiTeX_struct.h"
#include "binom.h"
#include "dim.h"
#include "draw.h"
#include "parsedef.h"
#include "utils.h"


long dimBinom(wchar_t * found,
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph)
/*
The dimXxx routines all have the following arguments:
found		--	Pointer to a string containing the remaining part of the
equation Gpos		--	Pointer to a string which will contain the part
of the equation relevant to the current parent with flags to indicate which
drawing routines to use. Our		--	dimension of the parent graph
--	The parent The routines returns the number of characters it used of the
found vector.
*/
{
    Tdim out;
    wchar_t *start, *end = 0, *tmp;
#define our (*Our)
#define gpos (*Gpos)
    long height = 0, width = 0;

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = BINOM;
    gpos++;
    *gpos = 0;

    start = wcschr(found, L'{');
    if (start)
        end = findClosingBrace(start + 1);
    if (!start || !end || (end - start < 2)) {
        SyntaxError(L"Usage: \\binom{n}{k}\n\tProduces the binomial n "
                    "over k.\n");
        return 0;
    }
    if (start - found - 6 > 0)
        fprintf(stderr, "Warning spurious characters ignores in \\binom\n");

    *end = 0;
    tmp = wcsdup(start + 1);
    *end = L'}';

    out = dim(tmp, newChild(graph));
    free(tmp);
    height += out.y;
    width = out.x;

    start = wcschr(end, L'{');
    if (start - end - 1 > 0)
        SyntaxWarning(L"Warning spurious characters ignored in \\binom\n");
    if (start)
        end = findClosingBrace(start + 1);

    if (!start || !end || (end - start < 2)) {
        SyntaxError(L"Usage: \\binom{n}{k}\n\tProduces the binomial n "
                    "over k.\n");
        return 0;
    }

    *end = 0;
    tmp = wcsdup(start + 1);
    *end = L'}';
    out = dim(tmp, newChild(graph));
    free(tmp);

    if (out.y > our.baseline) {
        our.y += out.y - our.baseline;
        our.baseline = out.y; /* baseline+(out.y-baseline) */
    }
    if (height > our.y - our.baseline - 1) {
        our.y += height - (our.y - our.baseline - 1);
    }
    if (out.x > width)
        our.x += out.x;
    else
        our.x += width;

    our.x += 2;

    if (our.baseline < out.y)
        our.baseline = out.y;

    return end - (found); /* skip parsed text */
#undef our
#undef gpos
}

void drawBinom(long * Kid,
               long * Curx,
               long * Cury,
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
    long low = cury + graph->dim.baseline;

    if (graph->dim.y > 2) {
        (*screen)[low][curx] = L'\\';
        for (long i = 1; i < graph->dim.y - 1; i++)
            (*screen)[low - i][curx] = L'|';
        (*screen)[low - graph->dim.y + 1][curx] = L'/';
        curx++;
    } else
        (*screen)[cury][curx++] = L'(';

    long width = graph->down[kid]->dim.x;
    if (width < graph->down[kid + 1]->dim.x)
        width = graph->down[kid + 1]->dim.x;
    drawInternal(screen, graph->down[kid],
                 curx + width / 2 - (graph->down[kid]->dim.x) / 2,
                 cury - (graph->down[kid]->dim.y));
    drawInternal(screen, graph->down[kid + 1],
                 curx + width / 2 - (graph->down[kid + 1]->dim.x) / 2,
                 cury + 1);

    curx += width;

    low = cury + graph->dim.baseline;
    if (graph->dim.y > 2) {
        (*screen)[low][curx] = L'/';
        for (long i = 1; i < graph->dim.y - 1; i++)
            (*screen)[low - i][curx] = L'|';
        (*screen)[low - graph->dim.y + 1][curx] = L'\\';
    } else
        (*screen)[cury][curx] = L')';

    kid += 2;
    curx++;
}
