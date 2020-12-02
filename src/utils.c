// utils.c: utillities for asciiTeX
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asciiTeX_struct.h"
#include "utils.h"


void SyntaxError(wchar_t * format_str, ...)
{
    va_list ap;
    va_start(ap, format_str);
    messages[Nmes] = malloc(200 * sizeof(wchar_t));
    swprintf(messages[Nmes++], 200, format_str, ap);
    if (Nmes == Nall) {
        Nall += 10;
        messages = realloc(messages, Nall * sizeof(wchar_t *));
    }
    SYNTAX_ERR_FLAG = S_ERR;
}

void SyntaxWarning(wchar_t * format_str, ...)
{
    va_list ap;
    va_start(ap, format_str);
    messages[Nmes] = malloc(200 * sizeof(wchar_t));
    swprintf(messages[Nmes++], 200, format_str, ap);
    if (Nmes == Nall) {
        Nall += 10;
        messages = realloc(messages, Nall * sizeof(wchar_t *));
    }
    SYNTAX_ERR_FLAG = S_WARN;
}

wchar_t * getbegin_endEnd(wchar_t * txt)
{
    wchar_t * tmp1 = wcsstr(txt, L"\\begin");
    wchar_t * tmp2 = wcsstr(txt, L"\\end");
    while ((tmp1 < tmp2) && (tmp1 != NULL)) {
        tmp2 = wcsstr(tmp2 + 4, L"\\end");
        tmp1 = wcsstr(tmp1 + 6, L"\\begin");
    }
    if (tmp2)
        return tmp2; /* return a pointer to the `\' letter of final \end */
    else {
        SyntaxError(L"Missing \\end in getbegin_endEnd\n");
        exit(1);
    }
}

wchar_t * preparse(wchar_t * txt)
{
    wchar_t * result = malloc((wcslen(txt) * 10) * sizeof(wchar_t));
    wchar_t * ptr = txt;
    wchar_t * rptr = result;
    while (*ptr) {
        while (*ptr == L' ' || *ptr == L'\t' || *ptr == L'\n')
            ptr++;

        if (!*ptr)
            break;

        if (*ptr == L'\\' && *(ptr + 1) != L'\\' && *(ptr + 1) != L'\0') {
            *rptr = *ptr;
            rptr++;
            ptr++;
            *rptr = *ptr;
            rptr++;
            ptr++;
        }

        if (*ptr != L'+' && *ptr != L'-' && *ptr != L'/' && *ptr != L'<' &&
            *ptr != L'>' && *ptr != L'*' && *ptr != L'=' && *ptr != L',' &&
            *ptr != L'!') {
            *rptr = *ptr;
            rptr++;
            ptr++;
        } else {
            if (*ptr != L',' &&
                (*ptr != L'=' || (*(rptr - 1) != '<' && *(rptr - 1) != '>' &&
                                  *(rptr - 1) != '!'))) {
                *rptr = L' ';
                rptr++;
            }
            *rptr = *ptr;
            rptr++;
            ptr++;

            if (*ptr != L'=' || (*(rptr - 1) != L'<' && *(rptr - 1) != L'>' &&
                                 *(rptr - 1) != L'!')) {
                *rptr = L' ';
                rptr++;
            }
        }

        if (*(ptr - 1) == L'\\' && *ptr == L'\\') {
            /* internally we replace \\ with endline characters as a single
             * endline character is more convenient */
            *(rptr - 1) = L'\n';
            ptr++;
        }

        if (*(ptr - 1) == L'^' || (*(ptr - 1) == L'_' && *ptr != L'{')) {
            if (!*ptr && *(ptr - 2) != L'\\') {
                SyntaxError(L"Premature end of input\n");
                return result;
            }

            if (*ptr == L'^' || *ptr == L'_') {
                SyntaxError(L"Ill formatter super- of subscript\n");
                return result;
            }

            if ((ptr - 2 < txt) || *(ptr - 2) != L'\\') {
                *rptr = L'{';
                rptr++;
                *rptr = *ptr;
                ptr++;
                rptr++;

                if (*(ptr - 1) == L'\\') {
                    while ((*ptr >= 0x41 && *ptr <= 0x5a) ||
                           (*ptr >= 0x61 && *ptr <= 0x7a)) {
                        /* while not whitespace or end */
                        *rptr = *ptr;
                        rptr++;
                        ptr++;
                    }
                }

                *rptr = L'}';
                rptr++;
            }
        }
    }
    *rptr = L'\0';
    return result;
}

wchar_t * findClosingBrace(wchar_t * txt)
{
    int opened = 1;
    int len = wcslen(txt);
    int i;
    for (i = 0; i < len; i++) {
        if (txt[i] == L'{')
            opened++;
        if (txt[i] == L'}')
            opened--;
        if (opened == 0)
            return txt + i;
    }
    SyntaxError(L"Couldn't find matching brace\n");
    return txt;
}

wchar_t * findClosingLRBrace(wchar_t * txt)
{
    /* txt should point to the brace after \left */
    int opened = 1;
    int len = wcslen(txt);
    int i;
    wchar_t *lb, *rb, c = (*txt);
    wchar_t * inv = L"()[]{}||<>";

    for (i = 0; i < 9; i += 2)
        if (inv[i] == c)
            c = inv[i + 1];

    lb = malloc(7 * sizeof(wchar_t));
    rb = malloc(8 * sizeof(wchar_t));

    wcsncpy(lb, L"\\left", 6);
    wcsncpy(rb, L"\\right", 7);

    wcsncat(lb, txt, 1);
    wcsncat(rb, &c, 1);

    for (i = 0; i < len; i++) {
        if (opened == 1) {
            /* any left opens */
            /* only the right \right closes */
            if (wcsncmp(txt + i, lb, 5) == 0)
                opened++;
            else if ((c == L'.') && (wcsncmp(txt + i, L"\\right", 6) == 0))
                opened--;
            else if ((wcsncmp(txt + i, L"\\right.", 7) == 0) ||
                     (wcsncmp(txt + i, L"\\right\\.", 8) == 0) ||
                     (wcsncmp(txt + i, rb, 7) == 0))
                opened--;
            if (opened == 0) {
                free(lb);
                free(rb);
                return txt + i;
            }
        } else {
            /* any left opens */
            /* any right closes */
            if (wcsncmp(txt + i, lb, 5) == 0)
                opened++;
            else if ((wcsncmp(txt + i, L"\\right", 6) == 0))
                opened--;
        }
    }
    free(lb);
    free(rb);
    SyntaxError(L"Couldn't find matching right brace\n");
    return txt;
}

void InitGraph(struct Tgraph * graph)
{
    graph->up = NULL;
    graph->down = NULL;
    graph->children = 0;
    graph->options = NULL;
    graph->txt = NULL;
    graph->array = NULL;
}

struct Tgraph * newChild(struct Tgraph * graph)
{
    if (graph->children == 0)
        graph->down = (struct Tgraph **)malloc(sizeof(struct Tgraph *));
    else
        graph->down = (struct Tgraph **)realloc(
            graph->down, sizeof(struct Tgraph **) * (graph->children + 1));
    graph->down[graph->children] =
        (struct Tgraph *)malloc(sizeof(struct Tgraph));
    graph->down[graph->children]->up = graph;
    graph->down[graph->children]->options = NULL;
    graph->down[graph->children]->array = NULL;
    graph->down[graph->children]->txt = NULL;
    graph->down[graph->children]->children = 0;
    graph->children++;
    return graph->down[graph->children - 1];
}

void dealloc_c(struct Tgraph * graph)
{
    int i;
    for (i = 0; i < graph->children; i++) {
        dealloc_c(graph->down[i]);
    }
    if (graph->children)
        free(graph->down);
    if (graph->options)
        free(graph->options);
    if (graph->txt)
        free(graph->txt);
    if (graph->array) {
        free(graph->array->rowy);
        free(graph->array->colx);
        free(graph->array);
    }
    free(graph);
}

void dealloc(struct Tgraph * graph)
{
    int i;
    for (i = 0; i < graph->children; i++) {
        dealloc(graph->down[i]);
        free(graph->down[i]);
    }
    if (graph->children)
        free(graph->down);
    if (graph->options)
        free(graph->options);
    if (graph->txt)
        free(graph->txt);
    if (graph->array) {
        free(graph->array->rowy);
        free(graph->array->colx);
        free(graph->array);
    }
}
