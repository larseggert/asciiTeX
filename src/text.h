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

#pragma once

#include <wchar.h>

#include "asciiTeX_struct.h"


long dimText(wchar_t * found,
             wchar_t ** Gpos,
             Tdim * Our,
             struct Tgraph * graph);

void drawText(long * Kid,
              long * Curx,
              long * Cury,
              wchar_t *** screen,
              struct Tgraph * graph,
              wchar_t * txt);
