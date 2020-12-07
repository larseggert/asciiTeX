// array.c: layout/dimensioning and drawing routines for arrays
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

#include "array.h"
#include "asciiTeX_struct.h"
#include "dim.h"
#include "draw.h"
#include "parsedef.h"
#include "utils.h"


wchar_t * findArrayDelimiter(wchar_t * txt)
{
    size_t len = wcslen(txt);
    size_t i;
    for (i = 0; i < len; i++) {
        if (txt[i] == L'\\') {
            if (wcsncmp(txt + i, L"\\begin", 6) == 0) /* skip
                                                       * nested
                                                       * * parts
                                                       */
                i += (size_t)(6 + getbegin_endEnd(txt + i + 1) - (txt + i));
        }
        if ((txt[i] == L'&') || (txt[i] == L'\n'))
            return txt + i;
    }
    return txt + i; /* no delimiter has been found */
}

long dimArray(wchar_t * found,
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph)
/*
The dimXxx routines all have the forllowing arguments:
found		--	Pointer to a sting containing the remaining part of the
equation Gpos		--	Pointer to a string which will contain the part
of the equation relevant to the current parent with flags to indicate which
drawing routines to use. Our		--	dimention of the parent graph
--	The parent The routines returns the number of characters it used of
the found vector.
*/
{
#define gpos (*Gpos)
#define our (*Our)
    wchar_t *start, *end = 0, *tmp = getbegin_endEnd(found + 1), rowal = L'c';
    Tdim out;

    wchar_t ** cells = (wchar_t **)malloc(sizeof(wchar_t *));
    long ncells = 0;
    long rows = 0, cols = 0;
    long curcols = 0;
    long i, j;

    if (tmp)
        *tmp = 0;
    else {
        SyntaxError(L"Could not find matching \\end in array %s\n", found);
        free(cells);
        return 0;
    }

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = (wchar_t)ARRAY;
    gpos++;
    *gpos = 0;

    newChild(graph);
    /* find the column-alignment argument */
    start = wcschr(found + 6 + 7, L'{');
    if (start)
        end = findClosingBrace(start + 1);
    if (!start || !end || (end - start < 2)) {
        SyntaxError(L"Usage: \\begin{array}{alignment} elements "
                    "\\end{array}\n\tProduces an array.\n");
        free(cells);
        return 0;
    }
    if (start - found - 6 - 7 > 0) {
        /* search for row alignment */
        if (wcsstr(found + 6 + 7, L"[t]"))
            rowal = L't';
        else if (wcsstr(found + 6 + 7, L"[b]"))
            rowal = L'b';
        else if (wcsstr(found + 6 + 7, L"[c]"))
            rowal = L'c';
        else
            SyntaxWarning(L"Warning spurious characters ignored in \\array\n");
    }

    *end = 0;
    i = 1;

    graph->down[graph->children - 1]->options =
        malloc((wcslen(start) + 1) * sizeof(wchar_t));
    j = 0;
    while (start[i]) {
        switch (start[i]) {
        case L'l':
        case L'c':
        case L'r':
            /* put wchar_t in options */
            graph->down[graph->children - 1]->options[j] = start[i];
            j++;
            __attribute__((fallthrough));
        case L' ':
            /*ignore*/
            break;
        default:
            SyntaxError(L"Ill formatted alignment string\n");
            free(cells);
            return 0;
        }
        i++;
    }
    graph->down[graph->children - 1]->options[j] = L'\0';
    cols = j;

    *end = L'}';

    start = end + 1;
    while (1) {
        end = findArrayDelimiter(start);
        cells = (wchar_t **)realloc(cells,
                                    (size_t)(ncells + 1) * sizeof(wchar_t *));
        cells[ncells] =
            (wchar_t *)malloc((size_t)(end - start + 1) * sizeof(wchar_t));
        wcsncpy(cells[ncells], start, (size_t)(end - start));
        cells[ncells][end - start] = 0; /* terminate the string */
        ncells++;
        if (*end == L'&') {
            start = end + 1;
            curcols++;
        } else if (*end == L'\n') {
            curcols++;
            start = end + 1;
            if ((cols != 0) && (curcols != cols)) {
                SyntaxError(L"Bad number of columns in array\n");
                free(cells);
                exit(1);
            }
            cols = curcols;
            curcols = 0;
            rows++;
        } else if (*end == 0)
            break;
    }
    if (curcols)
        rows++;
    if (!cols) /*there was only one line without endline */
        cols++;


#define Array (graph->down[graph->children - 1])
    Array->array = malloc(sizeof(Tarray));
    Array->array->rows = rows;
    Array->array->cols = cols;
    Array->array->rowy = calloc((size_t)rows + 1, sizeof(*Array->array->rowy));
    Array->array->colx = calloc((size_t)cols + 1, sizeof(*Array->array->colx));
    for (i = 0; i < ncells; i++) {
        long whichrow = i / cols;
        long whichcol = i - whichrow * cols;
        out = dim(cells[i], newChild(Array));
        if (out.x > Array->array->colx[whichcol])
            Array->array->colx[whichcol] = out.x;
        if (out.y > Array->array->rowy[whichrow])
            Array->array->rowy[whichrow] = out.y;
        free(cells[i]);
    }
    free(cells);
    Array->dim.x = 0;
    for (i = 0; i < cols; i++)
        Array->dim.x += Array->array->colx[i];
    Array->dim.y = 0;
    for (i = 0; i < rows; i++)
        Array->dim.y += Array->array->rowy[i];

    Array->dim.y += Array->array->rows - 1;
    Array->dim.x += Array->array->cols - 1;

    switch (rowal) {
    case L'b':
        Array->dim.baseline = 0;
        break;
    case L't':
        Array->dim.baseline = Array->dim.y - 1;
        break;
    default:
    case L'c':
        Array->dim.baseline = Array->dim.y / 2;
        break;
    }


    our.x += Array->dim.x;
    if (our.baseline < Array->dim.baseline) {
        our.y += Array->dim.baseline - our.baseline;
        our.baseline = Array->dim.baseline;
    }
    if (our.y < Array->dim.y)
        our.y = Array->dim.y;

#undef Array
    *tmp = L'\\'; /* restore original value */
    return (tmp + 3 + 7) - found;
#undef gpos
#undef our
}

void drawArray(long * Kid,
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
#define Array (graph->down[kid])
    long cury = (*Cury) - ((Array->dim.y - 1) - Array->dim.baseline);
    long x = 0, y = 0, curitem = 0, xx = 0, yy;
    long i, j;


    for (i = 0; i < Array->array->rows; i++) {
        for (j = 0; j < Array->array->cols; j++) {

            yy = cury + y +
                 (Array->array->rowy[i] - Array->down[curitem]->dim.y + 1) / 2;

            switch (graph->down[kid]->options[j]) {
            /* compute current c position (column alignment) */
            case L'l':
                /* left */
                xx = curx + x;
                break;
            case L'r':
                /* right */
                xx = curx + x +
                     (Array->array->colx[j] - Array->down[curitem]->dim.x);
                break;
            case L'c':
                /* center */
                xx = curx + x +
                     (Array->array->colx[j] - Array->down[curitem]->dim.x) / 2;
                break;
            default:
                break;
            }
            drawInternal(screen, Array->down[curitem], xx, yy);
            curitem++;
            x += Array->array->colx[j];
            x++;
        }
        y += Array->array->rowy[i];
        y++;
        x = 0;
    }

    curx += Array->dim.x;
    kid++;
#undef Array
}
