// utils.h: header for .c: utillities for asciiTeX
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

struct Tgraph;

void SyntaxError(wchar_t * format_str, ...);

void SyntaxWarning(wchar_t * format_str, ...);

wchar_t * preparse(wchar_t * txt); /* make the text formatting nice,
                                    * and create a dynamic array */

wchar_t * findClosingBrace(wchar_t * txt); /* find the closing brace
                                            * of cmds like frac{},
                                            * sqrt{}, etc. */

struct Tgraph * newChild(struct Tgraph * graph); /* add new child to the
                                                  * tree, and return a
                                                  * pointer to this child */

void dealloc(struct Tgraph * graph); /* frees the space used by
                                      * tree */

void InitGraph(struct Tgraph * graph);

wchar_t * getbegin_endEnd(wchar_t * txt); /* similar to
                                           * findClosingBrace, but
                                           * works with \begin{}
                                           * \end{} pairs. */

wchar_t * findClosingLRBrace(wchar_t * txt); /* find the closing brace
                                              * of cmds like \left( */

extern wchar_t ** messages;

extern long Nmes;

extern long Nall;
