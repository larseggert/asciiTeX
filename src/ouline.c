// ouline.c: layout/dimensioning and drawing routines for over and underlines
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
#include "ouline.h"
#include "parsedef.h"
#include "utils.h"


long dimOverl(wchar_t * found,
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
    *gpos = (char)OVERLINE;
    gpos++;
    *gpos = 0;

    start = wcschr(found, '{');
    if (!start) {
        SyntaxError(
            L"Usage: \\overline{X}\n\tdraws a line above expression X\n");
        return 0;
    }
    end = findClosingBrace(start + 1);
    if (end - start < 2) {
        SyntaxError(
            L"Usage: \\overline{X}\n\tdraws a line above expression X\n");
        return 0;
    }

    *end = 0;
    tmp = wcsdup(start + 1);
    *end = '}';
    out = dim(tmp, newChild(graph));
    free(tmp);

    if (start - found - 9 > 0)
        SyntaxWarning(L"Warning: Spurious characters ignored in \\overline\n");

    out.y++; /* add the line on top for the overline */

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
    our.x += out.x;
    return end - (found);
#undef gpos
#undef our
}

void drawOverl(long * Kid,
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
    long i;
    drawInternal(screen, graph->down[kid], curx,
                 cury - (graph->down[kid]->dim.y -
                         (graph->down[kid]->dim.baseline + 1)));
    for (i = 0; i < graph->down[kid]->dim.x; i++)
        (*screen)[cury - (graph->down[kid]->dim.y) +
                  (graph->down[kid]->dim.baseline)][curx++] = '_';
    kid++;
}

long dimUnderl(wchar_t * found,
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
    *gpos = (char)UNDERLINE;
    gpos++;
    *gpos = 0;

    start = wcschr(found, '{');
    if (!start) {
        SyntaxError(
            L"Usage: \\underline{X}\n\tdraws a line under expression X\n");
        return 0;
    }
    end = findClosingBrace(start + 1);
    if (end - start < 2) {
        SyntaxError(
            L"Usage: \\underline{X}\n\tdraws a line under expression X\n");
        return 0;
    }

    *end = 0;
    tmp = wcsdup(start + 1);
    *end = '}';
    out = dim(tmp, newChild(graph));
    free(tmp);

    if (start - found - 10 > 0)
        SyntaxWarning(L"Warning: Spurious characters ignored in \\underline\n");

    out.y++; /* add the line under for the underline */
    out.baseline++;

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
    our.x += out.x;
    return end - (found);
#undef gpos
#undef our
}

void drawUnderl(long * Kid,
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
    long i;
    drawInternal(screen, graph->down[kid], curx,
                 cury - (graph->down[kid]->dim.y -
                         (graph->down[kid]->dim.baseline + 1)));
    for (i = 0; i < graph->down[kid]->dim.x; i++)
        (*screen)[cury + (graph->down[kid]->dim.y) +
                  (graph->down[kid]->dim.baseline)][curx++] = '-';
    kid++;
}
