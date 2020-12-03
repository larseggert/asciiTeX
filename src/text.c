// text.h: header for layout/dimensioning and drawing routines for plain text
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
// Lars Eggert (https://github.com/larseggert/asciiTeX)

#include <stdlib.h>
#include <wchar.h>

#include "asciiTeX_struct.h"
#include "dim.h"
#include "draw.h"
#include "parsedef.h"
#include "text.h"
#include "utils.h"


/// { function_description }
///
/// @param      found  Pointer to a string containing the remaining part of the
///                    equation.
/// @param      Gpos   Pointer to a string which will contain the part of the
///                    equation relevant to the current parent with flags to
///                    indicate which drawing routines to use.
/// @param      Our    Dimension of the parent.
/// @param      graph  The parent.
///
/// @return     Number of characters it used of the found vector.
///
long dimText(wchar_t * found,
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph)
{
#define gpos (*Gpos)
#define our (*Our)
    wchar_t *start, *end, *tmp;
    Tdim out;
    *gpos = 1;
    gpos++;
    *gpos = (char)TEXT;
    gpos++;
    *gpos = 0;

    start = wcschr(found, '{');

    end = findClosingBrace(start + 1);
    *end = 0;
    tmp = wcsdup(start + 1);
    *end = '}';

    out = dim(tmp, newChild(graph));
    free(tmp);
    our.baseline = out.y;
    our.y += out.y;
    our.x += out.x;
    return end - found;
#undef gpos
#undef our
}

void drawText(long * Kid,
              long * Curx,
              long * Cury,
              wchar_t *** screen,
              struct Tgraph * graph,
              wchar_t * txt __attribute__((unused)))
{
#define kid (*Kid)
#define curx (*Curx)
#define cury (*Cury)
    long width = graph->down[kid]->dim.x;
    drawInternal(screen, graph->down[kid], curx, cury);
    curx += width;
    kid++;
}
