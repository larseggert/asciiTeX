// symbol.c: layout/dimensioning and drawing routines for symbols (things
// that do not resize)
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

#include <wchar.h>

#include "asciiTeX_struct.h"
#include "parsedef.h"
#include "symbols.h"

/*
 * all non adaptive symbols here
 */
/*
 * integral symbol (it has a constant size)
 */
long dimInt(wchar_t * found __attribute__((unused)),
            wchar_t ** Gpos,
            Tdim * Our,
            struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = INT;
    gpos++;
    *gpos = 0;
    our.x += 4;
    if (our.baseline == 0) {
        our.baseline++;
        our.y++;
    }
    if (our.y - our.baseline < 3)
        our.y = 3 + our.baseline;
    return 3;

#undef gpos
#undef our
}

void drawInt(long * Kid __attribute__((unused)),
             long * Curx,
             long * Cury,
             wchar_t *** screen,
             struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury - 2][curx + 2] = L'_';
    (*screen)[cury - 1][curx + 1] = L'/';
    (*screen)[cury][curx + 1] = L'|';
    (*screen)[cury + 1][curx + 1] = L'/';
    (*screen)[cury + 1][curx] = L'_';
    curx += 4;
}

/*
 * closed path integral
 */
long dimOint(wchar_t * found __attribute__((unused)),
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = OINT;
    gpos++;
    *gpos = 0;
    our.x += 4;
    if (our.baseline == 0) {
        our.baseline++;
        our.y++;
    }
    if (our.y - our.baseline < 3)
        our.y = 3 + our.baseline;
    return 4;

#undef gpos
#undef our
}

void drawOint(long * Kid __attribute__((unused)),
              long * Curx,
              long * Cury,
              wchar_t *** screen,
              struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury - 2][curx + 2] = L'_';
    (*screen)[cury - 1][curx + 1] = L'/';
    (*screen)[cury][curx + 1] = L'O';
    (*screen)[cury + 1][curx + 1] = L'/';
    (*screen)[cury + 1][curx] = L'_';
    curx += 4;
}

/*
 * product sign
 */
long dimProd(wchar_t * found __attribute__((unused)),
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = PROD;
    gpos++;
    *gpos = 0;
    our.x += 4;
    if (our.baseline == 0) {
        our.baseline++;
        our.y++;
    }
    if (our.y - our.baseline < 2)
        our.y++;
    return 4;

#undef gpos
#undef our
}

void drawProd(long * Kid __attribute__((unused)),
              long * Curx,
              long * Cury,
              wchar_t *** screen,
              struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury - 1][curx] = L'_';
    (*screen)[cury - 1][curx + 1] = L'_';
    (*screen)[cury - 1][curx + 2] = L'_';
    (*screen)[cury][curx] = L'|';
    (*screen)[cury][curx + 2] = L'|';
    (*screen)[cury + 1][curx] = L'|';
    (*screen)[cury + 1][curx + 2] = L'|';
    curx += 4;
}

/*
 * sum sign
 */
long dimSum(wchar_t * found __attribute__((unused)),
            wchar_t ** Gpos,
            Tdim * Our,
            struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = SUM;
    gpos++;
    *gpos = 0;
    our.x += 4;
    if (our.baseline == 0) {
        our.baseline++;
        our.y++;
    }
    if (our.y - our.baseline < 2)
        our.y++;
    return 3;

#undef gpos
#undef our
}

void drawSum(long * Kid __attribute__((unused)),
             long * Curx,
             long * Cury,
             wchar_t *** screen,
             struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury - 1][curx] = L' ';
    (*screen)[cury - 1][curx + 1] = L'_';
    (*screen)[cury - 1][curx + 2] = L'_';
    (*screen)[cury][curx] = L'\\';
    (*screen)[cury + 1][curx] = L'/';
    (*screen)[cury + 1][curx + 1] = L'_';
    (*screen)[cury + 1][curx + 2] = L'_';
    curx += 4;
}

/*
 * to sign ->
 */

long dimTo(wchar_t * found __attribute__((unused)),
           wchar_t ** Gpos,
           Tdim * Our,
           struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = TO;
    gpos++;
    *gpos = 0;
    our.x += 2;
    return 2;

#undef gpos
#undef our
}

void drawTo(long * Kid __attribute__((unused)),
            long * Curx,
            long * Cury,
            wchar_t *** screen,
            struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury][curx++] = L'-';
    (*screen)[cury][curx++] = L'>';
}

long dimLeadsto(wchar_t * found __attribute__((unused)),
                wchar_t ** Gpos,
                Tdim * Our,
                struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = LEADSTO;
    gpos++;
    *gpos = 0;
    our.x += 2;
    return 7;

#undef gpos
#undef our
}

void drawLeadsto(long * Kid __attribute__((unused)),
                 long * Curx,
                 long * Cury,
                 wchar_t *** screen,
                 struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury][curx++] = L'~';
    (*screen)[cury][curx++] = L'>';
}

long dimLceil(wchar_t * found __attribute__((unused)),
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = LCEIL;
    gpos++;
    *gpos = 0;
    our.x += 2;
    if (our.baseline == 0) {
        our.baseline++;
        our.y++;
    }
    if (our.y - our.baseline < 2)
        our.y = 2 + our.baseline;
    return 5;

#undef gpos
#undef our
}

void drawLceil(long * Kid __attribute__((unused)),
               long * Curx,
               long * Cury,
               wchar_t *** screen,
               struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury][curx++] = L'|';
    (*screen)[cury - 1][curx++] = L'_';
}

long dimRceil(wchar_t * found __attribute__((unused)),
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = RCEIL;
    gpos++;
    *gpos = 0;
    our.x += 2;
    if (our.baseline == 0) {
        our.baseline++;
        our.y++;
    }
    if (our.y - our.baseline < 2)
        our.y = 2 + our.baseline;
    return 5;

#undef gpos
#undef our
}

void drawRceil(long * Kid __attribute__((unused)),
               long * Curx,
               long * Cury,
               wchar_t *** screen,
               struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury - 1][curx++] = L'_';
    (*screen)[cury][curx++] = L'|';
}

long dimLfloor(wchar_t * found __attribute__((unused)),
               wchar_t ** Gpos,
               Tdim * Our,
               struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = LFLOOR;
    gpos++;
    *gpos = 0;
    our.x += 2;
    return 6;

#undef gpos
#undef our
}

void drawLfloor(long * Kid __attribute__((unused)),
                long * Curx,
                long * Cury,
                wchar_t *** screen,
                struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury][curx++] = L'|';
    (*screen)[cury][curx++] = L'_';
}

long dimRfloor(wchar_t * found __attribute__((unused)),
               wchar_t ** Gpos,
               Tdim * Our,
               struct Tgraph * graph __attribute__((unused)))
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

    *gpos = 1; /* See parsedef.h for the keyword
                * definitions */
    gpos++;
    *gpos = RFLOOR;
    gpos++;
    *gpos = 0;
    our.x += 2;
    return 6;

#undef gpos
#undef our
}

void drawRfloor(long * Kid __attribute__((unused)),
                long * Curx,
                long * Cury,
                wchar_t *** screen,
                struct Tgraph * graph __attribute__((unused)))
/*
The drawXxx routines all have the forllowing arguments:
Kid		--	Ineger index of the current child
Curx		--	Current x position in the 2D character field
Cury		--	Current y position in the 2D character field
screen		--	pointer to the 2D character field
graph		--	The parent
*/
{
#define curx (*Curx)
#define cury (*Cury)
    (*screen)[cury][curx++] = L'_';
    (*screen)[cury][curx++] = L'|';
}
