// asciiTeX.c: The equation formatting routine
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

#include "asciiTeX.h"
#include "asciiTeX_struct.h"
#include "dim.h"
#include "draw.h"
#include "utils.h"


wchar_t ** messages;
long Nmes;
long Nall;

STAT SYNTAX_ERR_FLAG;


wchar_t ** asciiTeX(wchar_t * eq, long ll, long * cols, long * rows)
{
    struct Tgraph * graph = malloc(sizeof(struct Tgraph));
    wchar_t ** screen;
    wchar_t * txt;
    SYNTAX_ERR_FLAG = S_NOERR;
    Nmes = 0;
    Nall = 10;
    messages = malloc((size_t)Nall * sizeof(wchar_t *));

    InitGraph(graph);
    eqdim(txt = preparse(eq), graph, ll);
    if (SYNTAX_ERR_FLAG != S_ERR) {
        free(txt);
        screen = draw(graph);
        *rows = graph->dim.y;
        *cols = graph->dim.x;
    } else {
        (*cols) = -1;
        (*rows) = Nmes;
        return messages;
    }
    dealloc(graph);
    free(graph);
    return screen;
}
