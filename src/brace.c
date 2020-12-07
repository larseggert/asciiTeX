// brace.c: layout/dimensioning and drawing routines for braces
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
#include "brace.h"
#include "dim.h"
#include "draw.h"
#include "parsedef.h"
#include "utils.h"


long dimBrace(wchar_t * found,
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph)
/*
The dimXxx routines all have the forllowing arguments:
found		--	Pointer to a sting containing the remaining part of the
equation Gpos		--	Pointer to a string which will contain the part
of the equation relevant to the current parent with flags to indicate which
drawing routines to use. Our		--	dimention of the parent graph
--	The parent The routines returns the number of wcharacters_t it used of
the found vector.
*/
{
#define gpos (*Gpos)
#define our (*Our)
    wchar_t *start, *end, *tmp, c;
    Tdim out;

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = (wchar_t)BRACES;
    gpos++;
    *gpos = 0;

    start = found + 5;
    while (*start == L'\\')
        start++;

    end = findClosingLRBrace(start);
    c = (*end);
    *end = 0;
    tmp = wcsdup(start + 1);
    *end = c;
    out = dim(tmp, newChild(graph));
    free(tmp);

    int off = 6;
    while (*(end + off) == L'\\')
        off++;

    tmp = malloc(sizeof(wchar_t) * 3);
    tmp[0] = (*start);
    tmp[1] = (*(end + off));
    tmp[2] = L'\0';

    /*
     * Store the brace type in the options string of the child
     */
    /*
     * We will use it in the drawing routine
     */
    graph->down[graph->children - 1]->options = wcsdup(tmp);
    free(tmp);

    if ((graph->down[graph->children - 1]->options[0] == L'[') &&
        (graph->down[graph->children - 1]->options[1] == L']')) {
        if (out.y > 1) {
            out.y++;    /* make room for an underscore at
                         * the top */
            out.x += 2; /* two braces of two chars wide */
        }
    } else if ((graph->down[graph->children - 1]->options[0] == L'[') ||
               (graph->down[graph->children - 1]->options[1] == L']')) {
        if (out.y > 1) {
            out.y++;    /* make room for an underscore at
                         * the top */
            out.x += 1; /* one brace is two chars wide */
        }
    }

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

    if ((graph->down[graph->children - 1]->options[0] == L'<') ||
        (graph->down[graph->children - 1]->options[1] == L'>')) {
        our.y += (!(our.y % 2)); /* ensure y is uneven with
                                  * room at the top */
        out.x += our.y;
        our.x += out.x;
    }

    if ((graph->down[graph->children - 1]->options[0] == L'{') ||
        (graph->down[graph->children - 1]->options[1] == L'}')) {
        our.y += (!(our.y % 2)); /* ensure y is uneven with
                                  * room at the top */
    } else
        our.x += out.x + 2;

    return end + off - (found);
#undef gpos
#undef our
}

void drawBrace(long * Kid,
               long * Curx,
               long * Cury,
               wchar_t *** screen,
               struct Tgraph * graph)
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D wcharacter_t field
Cury		--	Current y position in the 2D wcharacter_t field
screen		--	pointer to the 2D wcharacter_t field
graph		--	The parent
*/
{
#define kid (*Kid)
#define curx (*Curx)
#define cury (*Cury)
    long low = cury + graph->down[kid]->dim.baseline;
    long i;
    /*
     * the options of our child contains the brace type
     */
    switch (graph->down[kid]->options[0]) {
    case '(':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low][curx] = L'\\';
            for (i = 1; i < graph->down[kid]->dim.y - 1; i++)
                (*screen)[low - i][curx] = L'|';
            (*screen)[low - graph->down[kid]->dim.y + 1][curx] = L'/';
            curx++;
        } else
            (*screen)[cury][curx++] = L'(';
        break;
    case '|':
        if (graph->down[kid]->dim.y > 2) {
            for (i = 0; i < graph->down[kid]->dim.y; i++)
                (*screen)[low - i][curx] = L'|';
            curx++;
        } else
            (*screen)[cury][curx++] = L'|';
        break;
    case '[':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low][curx + 1] = L'_';
            for (i = 0; i < graph->down[kid]->dim.y; i++)
                (*screen)[low - i][curx] = L'|';
            (*screen)[low - graph->down[kid]->dim.y][curx + 1] = L'_';
            curx += 2;
        } else
            (*screen)[cury][curx++] = L'[';
        break;
    case '{':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low][curx] = L'\\';
            (*screen)[low - graph->down[kid]->dim.y / 2][curx] = L'<';
            for (i = 1;
                 i < graph->down[kid]->dim.y - (graph->down[kid]->dim.y % 2);
                 i++)
                if (!(i == graph->down[kid]->dim.y / 2))
                    (*screen)[low - i][curx] = L'|';

            (*screen)[low - graph->down[kid]->dim.y +
                      graph->down[kid]->dim.y % 2][curx] = L'/';
            curx++;
        } else
            (*screen)[cury][curx++] = L'{';
        break;
    case '<':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low - graph->down[kid]->dim.y / 2][curx] = L'<';
            curx += graph->down[kid]->dim.y / 2 - 1;
            for (i = 1; i < graph->down[kid]->dim.y; i++)
                if (i < graph->down[kid]->dim.y / 2)
                    (*screen)[low - i][curx--] = L'\\';
                else if (i == graph->down[kid]->dim.y / 2)
                    curx++;
                else if (i > graph->down[kid]->dim.y / 2)
                    (*screen)[low - i][curx++] = L'/';
        } else
            (*screen)[cury][curx++] = L'<';
        break;
    case '.': /* dummy brace to open or close any type *
               * of brace */
        break;
    default:
        if (graph->down[kid]->dim.y > 2) {
            for (i = 0; i < graph->down[kid]->dim.y; i++)
                (*screen)[low - i][curx] = graph->down[kid]->options[0];
            curx++;
        } else
            (*screen)[cury][curx++] = *(graph->down[kid]->options);
        break;
    }

    /*
     * drawInternal (screen, graph->down[kid], curx, cury - (graph->dim.y
     * - (graph->dim.baseline + 1)));
     */
    drawInternal(screen, graph->down[kid], curx,
                 low - graph->down[kid]->dim.y + 1);
    curx += graph->down[kid]->dim.x;

    switch (graph->down[kid]->options[1]) {
    case ')':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low][curx] = L'/';
            for (i = 1; i < graph->down[kid]->dim.y - 1; i++)
                (*screen)[low - i][curx] = L'|';
            (*screen)[low - graph->down[kid]->dim.y + 1][curx] = L'\\';
        } else
            (*screen)[cury][curx] = L')';
        break;
    case '|':
        if (graph->down[kid]->dim.y > 2) {
            for (i = 0; i < graph->down[kid]->dim.y; i++)
                (*screen)[low - i][curx] = L'|';
        } else
            (*screen)[cury][curx] = L'|';
        break;
    case ']':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low][curx] = L'_';
            for (i = 0; i < graph->down[kid]->dim.y; i++)
                (*screen)[low - i][curx + 1] = L'|';
            (*screen)[low - graph->down[kid]->dim.y][curx] = L'_';
            curx++;
        } else
            (*screen)[cury][curx] = L']';
        break;
    case '}':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low][curx] = L'/';
            (*screen)[low - graph->down[kid]->dim.y / 2][curx] = L'>';
            for (i = 1;
                 i < graph->down[kid]->dim.y - (graph->down[kid]->dim.y % 2);
                 i++)
                if (!(i == graph->down[kid]->dim.y / 2))
                    (*screen)[low - i][curx] = L'|';

            (*screen)[low - graph->down[kid]->dim.y +
                      (graph->down[kid]->dim.y % 2)][curx] = L'\\';
        } else
            (*screen)[cury][curx] = L'}';
        break;
    case '>':
        if (graph->down[kid]->dim.y > 2) {
            (*screen)[low - graph->down[kid]->dim.y / 2]
                     [curx + graph->down[kid]->dim.y / 2 - 1] = L'>';
            for (i = 1; i < graph->down[kid]->dim.y; i++)
                if (i < graph->down[kid]->dim.y / 2)
                    (*screen)[low - i][curx++] = L'/';
                else if (i == graph->down[kid]->dim.y / 2)
                    curx--;
                else if (i > graph->down[kid]->dim.y / 2)
                    (*screen)[low - i][curx--] = L'\\';
        } else
            (*screen)[cury][curx++] = L'>';
        break;
    case '.': /* dummy brace to open or close any type *
               * of brace */
        break;
    default:
        if (graph->down[kid]->dim.y > 2) {
            for (i = 0; i < graph->down[kid]->dim.y; i++)
                (*screen)[low - i][curx] = graph->down[kid]->options[1];
        } else
            (*screen)[cury][curx] = graph->down[kid]->options[1];
        break;
    }
    curx++;

    kid++;
}
