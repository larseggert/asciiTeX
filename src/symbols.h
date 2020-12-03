// symbol.h: header for layout/dimensioning and drawing routines for symbols
// (things that do not resize)
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

#pragma once

#include <wchar.h>

#include "asciiTeX_struct.h"


long dimInt(wchar_t * found,
            wchar_t ** Gpos,
            Tdim * Our,
            struct Tgraph * graph);

void drawInt(long * Kid,
             long * Curx,
             long * Cury,
             wchar_t *** screen,
             struct Tgraph * graph);

long dimOint(wchar_t * found,
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph);

void drawOint(long * Kid,
              long * Curx,
              long * Cury,
              wchar_t *** screen,
              struct Tgraph * graph);

long dimProd(wchar_t * found,
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph);

void drawProd(long * Kid,
              long * Curx,
              long * Cury,
              wchar_t *** screen,
              struct Tgraph * graph);

long dimSum(wchar_t * found,
            wchar_t ** Gpos,
            Tdim * Our,
            struct Tgraph * graph);

void drawSum(long * Kid,
             long * Curx,
             long * Cury,
             wchar_t *** screen,
             struct Tgraph * graph);

long dimTo(wchar_t * found, wchar_t ** Gpos, Tdim * Our, struct Tgraph * graph);

void drawTo(long * Kid,
            long * Curx,
            long * Cury,
            wchar_t *** screen,
            struct Tgraph * graph);

long dimLeadsto(wchar_t * found,
                wchar_t ** Gpos,
                Tdim * Our,
                struct Tgraph * graph);

void drawLeadsto(long * Kid,
                 long * Curx,
                 long * Cury,
                 wchar_t *** screen,
                 struct Tgraph * graph);

long dimLceil(wchar_t * found,
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph);

void drawLceil(long * Kid,
               long * Curx,
               long * Cury,
               wchar_t *** screen,
               struct Tgraph * graph);

long dimRceil(wchar_t * found,
              wchar_t ** Gpos,
              Tdim * Our,
              struct Tgraph * graph);

void drawRceil(long * Kid,
               long * Curx,
               long * Cury,
               wchar_t *** screen,
               struct Tgraph * graph);

long dimLfloor(wchar_t * found,
               wchar_t ** Gpos,
               Tdim * Our,
               struct Tgraph * graph);

void drawLfloor(long * Kid,
                long * Curx,
                long * Cury,
                wchar_t *** screen,
                struct Tgraph * graph);

long dimRfloor(wchar_t * found,
               wchar_t ** Gpos,
               Tdim * Our,
               struct Tgraph * graph);

void drawRfloor(long * Kid,
                long * Curx,
                long * Cury,
                wchar_t *** screen,
                struct Tgraph * graph);
