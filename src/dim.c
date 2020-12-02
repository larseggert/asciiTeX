// dim.c: main layout/dimensioning routines.
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

#include "array.h"
#include "asciiTeX_struct.h"
#include "brace.h"
#include "frac.h"
#include "limit.h"
#include "ouline.h"
#include "parsedef.h"
#include "sqrt.h"
#include "sscript.h"
#include "symbols.h"
#include "text.h"
#include "utils.h"

const KEYWORD Keys[] = {{L"^{", 2, SUPER},
                        {L"_{", 2, SUB},
                        {L"\\frac", 5, FRAC},
                        {L"\\sqrt", 5, SQRT},
                        {L"\\overline", 9, OVERLINE},
                        {L"\\underline", 10, UNDERLINE},
                        {L"\\limit", 6, LIMIT},
                        {L"\\left", 5, BRACES},
                        {L"\\begin{array}", 13, ARRAY},
                        {L"\\to", 3, TO},
                        {L"\\leadsto", 8, LEADSTO},
                        {L"\\sum", 4, SUM},
                        {L"\\prod", 5, PROD},
                        {L"\\int", 4, INT},
                        {L"\\oint", 5, OINT},
                        {L"\\infty", 6, INFTY},
                        {L"\\lceil", 6, LCEIL},
                        {L"\\rceil", 6, RCEIL},
                        {L"\\lfloor", 7, LFLOOR},
                        {L"\\rfloor", 7, RFLOOR},
                        {L"\\text", 5, TEXT},
                        {L"\\mathrm", 7, TEXT},
                        {L"\\", 1, ESCAPE},
                        {NULL, 0, ERR}};
PRSDEF
LookupKey(wchar_t * txt, const KEYWORD * Keys)
{
    for (; Keys->name; Keys++) {
        if (wcsncmp(txt, Keys->name, Keys->len) == 0)
            break;
    }
    return Keys->Nr;
}

wchar_t * findLineEnd(wchar_t * txt)
{
    int len = wcslen(txt);
    int i;
    for (i = 0; i < len; i++) {
        /* return pointer to the next endline */
        if (wcsncmp(txt + i, L"\\begin", 6) == 0) /* skip nested parts */
            i = 4 + getbegin_endEnd(txt + i + 6) - txt;
        else if (wcsncmp(txt + i, L"\\left", 5) == 0)
            i = 6 + findClosingLRBrace(txt + i + 5) - txt;
        else if (txt[i] == '{')
            i = findClosingBrace(txt + i + 1) - txt;
        else if (txt[i] == '\n')
            return txt + i;
    }
    return txt + i; /* no line end found */
}

Tdim dim(wchar_t * txt, struct Tgraph * graph)
{
    /* a linewidth mechanism were cool, i.e. automatic braking of the line */
    /* baceline should jump current y down, x should be the maximum x of all
     * lines */
    /* a flag for linebreak should be placed, containing the y jump size */
    /* so that the draw routines know when to add to y and reset x zo 0 */
    int i;
    int len = wcslen(txt); /* length of text passed
                            * to parse */
    Tdim our;              /* the dimensions of our current object */
    wchar_t * gpos;        /* points to the tree node's text */
    wchar_t * end;
    PRSDEF K; /* keynumber, result from the
               * keywordlookup */
    our.x = 0;
    our.y = 1;
    our.baseline = 0;
    graph->children = 0; /* at the beginning the tree doesn't have
                          * children. We must first find them */
    graph->txt =
        (wchar_t *)malloc((len + 1) * sizeof(wchar_t)); /* allocating the same
                                                         * length is OK. Special
                                                         * wcharacters_t in
                                                         * output are 2 wchars_t
                                                         * long--shorter than in
                                                         * the input */
    gpos = graph->txt; /* we setup now this pointer */
    *gpos = 0;
    if (*(end = findLineEnd(txt)) != '\0') {
        /* the current level contains one or more line ends */
        /* the current level will become aan array of lines */
        int nlines = 0;
        wchar_t * start = txt;
        wchar_t ** lines = (wchar_t **)malloc(sizeof(wchar_t *));
        Tdim out = {0};
        if (SYNTAX_ERR_FLAG == S_ERR)
            return out;
        *gpos = 1; /* See parsedef.h for the keyword
                    * definitions */
        gpos++;
        *gpos = (wchar_t)ARRAY;
        gpos++;
        *gpos = 0;
        newChild(graph);
        graph->down[graph->children - 1]->options =
            malloc((2) * sizeof(wchar_t));
        graph->down[graph->children - 1]->options[0] =
            'c'; /* default col alignment */
        graph->down[graph->children - 1]->options[1] =
            '\0'; /* default col alignment */
        /* count how many lines we have */
        while (1) {
            lines =
                (wchar_t **)realloc(lines, (nlines + 1) * (sizeof(wchar_t *)));
            lines[nlines] =
                (wchar_t *)malloc((end - start + 1) * sizeof(wchar_t));
            wcsncpy(lines[nlines], start, end - start);
            lines[nlines][end - start] = '\0'; /* terminate the string */
            nlines++;
            if (*end == '\0')
                break;
            start = end + 1;
            end = findLineEnd(start);
        }
        /* fill the array with the lines */

#define Array (graph->down[graph->children - 1])
        Array->array = malloc(sizeof(Tarray));
        Array->array->rows = nlines;
        Array->array->cols = 1;
        Array->array->rowy = (int *)calloc(nlines, sizeof(int));
        Array->array->colx = (int *)calloc(1, sizeof(int));
        for (i = 0; i < nlines; i++) {
            out = dim(lines[i], newChild(Array));
            if (out.x > Array->array->colx[0])
                Array->array->colx[0] = out.x;
            if (out.y > Array->array->rowy[i])
                Array->array->rowy[i] = out.y;
            free(lines[i]);
        }
        free(lines);
        Array->dim.x = 0;
        Array->dim.x += Array->array->colx[0];
        Array->dim.y = 0;
        for (i = 0; i < nlines; i++)
            Array->dim.y += Array->array->rowy[i];

        Array->dim.y += Array->array->rows - 1;
        Array->dim.x += Array->array->cols - 1;

        Array->dim.baseline = Array->dim.y / 2;

        our.x += Array->dim.x;
        if (our.baseline < Array->dim.baseline) {
            our.y += Array->dim.baseline - our.baseline;
            our.baseline = Array->dim.baseline;
        }
        if (our.y < Array->dim.y)
            our.y = Array->dim.y;
#undef Array
        graph->dim = our;
        return our;
    }
    for (i = 0; i < len; i++) {
        if (SYNTAX_ERR_FLAG == S_ERR)
            return our;
        if ((txt[i] != '\\') && (txt[i] != '_') && (txt[i] != '^')) {
            our.x++;
            *gpos = txt[i];
            gpos++;
            *gpos = 0;
        } else {
            K = LookupKey(txt + i, Keys);
            switch (K) {
            case SUPER:
                i += dimSuperscript(txt + i, &gpos, &our, graph);
                break;
            case SUB:
                i += dimSubscript(txt + i, &gpos, &our, graph);
                break;
            case FRAC:
                i += dimFrac(txt + i, &gpos, &our, graph);
                break;
            case SQRT:
                i += dimSqrt(txt + i, &gpos, &our, graph);
                break;
            case OVERLINE:
                i += dimOverl(txt + i, &gpos, &our, graph);
                break;
            case UNDERLINE:
                i += dimUnderl(txt + i, &gpos, &our, graph);
                break;
            case LIMIT:
                i += dimLimit(txt + i, &gpos, &our, graph);
                break;
            case BRACES:
                i += dimBrace(txt + i, &gpos, &our, graph);
                break;
            case ARRAY:
                i += dimArray(txt + i, &gpos, &our, graph);
                break;
            case TO:
                i += dimTo(txt + i, &gpos, &our, graph);
                break;
            case LEADSTO:
                i += dimLeadsto(txt + i, &gpos, &our, graph);
                break;
            case SUM:
                i += dimSum(txt + i, &gpos, &our, graph);
                break;
            case PROD:
                i += dimProd(txt + i, &gpos, &our, graph);
                break;
            case INT:
                i += dimInt(txt + i, &gpos, &our, graph);
                break;
            case OINT:
                i += dimOint(txt + i, &gpos, &our, graph);
                break;
            case INFTY:
                wcscat(gpos, L"oo");
                gpos += 2;
                our.x += 2;
                i += 5;
                break;
            case RCEIL:
                i += dimRceil(txt + i, &gpos, &our, graph);
                break;
            case LCEIL:
                i += dimLceil(txt + i, &gpos, &our, graph);
                break;
            case RFLOOR:
                i += dimRfloor(txt + i, &gpos, &our, graph);
                break;
            case LFLOOR:
                i += dimLfloor(txt + i, &gpos, &our, graph);
                break;
            case TEXT:
            case MATHRM:
                i += dimText(txt + i, &gpos, &our, graph);
                break;
            case ESCAPE:
                i++;
                our.x++;
                *gpos = txt[i];
                gpos++;
                *gpos = 0;
                break;
            case ERR:
            default:
                fprintf(stderr,
                        "I screwed up in dim, this should never happen!\n");
                exit(1);
                break;
            }
        }
    }
    graph->dim = our;
    return our;
}

wchar_t * PotLineEnd(wchar_t * txt)
{
    int len = wcslen(txt);
    int i, j;
    wchar_t * plbp = L"+-*/=~";
    for (i = 0; i < len; i++) {
        /* return pointer to the next potential endline position */
        if (wcsncmp(txt + i, L"\\begin", 6) == 0) /* skip nested parts */
            i = 4 + getbegin_endEnd(txt + i + 6) - txt;
        else if (wcsncmp(txt + i, L"\\left", 5) == 0)
            i = 6 + findClosingLRBrace(txt + i + 5) - txt;
        else if (txt[i] == '{')
            i = findClosingBrace(txt + i + 1) - txt;
        else if (txt[i] == '\\')
            i++;
        else if (txt[i] == '\n')
            return txt + i;
        else
            for (j = 0; j < 6; j++)
                if (plbp[j] == txt[i])
                    return txt + i + 1;
    }
    return txt + i; /* no potential line end found */
}

Tdim eqdim(wchar_t * txt, struct Tgraph * graph, int ll)
{
    /* if the linelength (ll) is zero we do not break, otherwise we try to fit
    the eq within ll columns */
    if (ll) {
        /* position linebreaks at + - / * = in the top level */
        wchar_t * END = txt + wcslen(txt);
        struct Tgraph * dummy = malloc(sizeof(struct Tgraph));
        Tdim dumdim;
        wchar_t * start = txt;
        wchar_t *end, c;
        wchar_t * prevplb = NULL;
        int x = 0;
        while (start < END) {
            end = PotLineEnd(start);
            if (SYNTAX_ERR_FLAG == S_ERR)
                return dumdim;
            c = *end;
            *end = '\0';
            InitGraph(dummy);
            dumdim = dim(start, dummy);
            if (SYNTAX_ERR_FLAG == S_ERR)
                return dumdim;
            *end = c;
            dealloc(dummy);
            x += dumdim.x;
            if (dumdim.x > ll)
                SyntaxWarning(L"Warning: overfull line\n");

            if ((x > ll) && (prevplb)) {
                /* at the previous potential line end we have to place one */
                /* in order to make it easy we create a single wchar_t line end,
                 * \n
                 */
                /* note that enters are removed in preparse to allow multi line
                 * editing
                 */
                *(prevplb) = '\n';
                x = dumdim.x;
            }
            if (c == '\n') {
                /* the current end is already a break */
                /* reset x, prevent a new linebreak */
                prevplb = NULL;
                x = 0;
                end++;
            } else
                prevplb = end;

            start = end;
        }
        free(dummy);
    }
    return dim(txt, graph);
}
