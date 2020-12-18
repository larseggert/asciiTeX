// main.c: parse the command line, reading files, and output to stdout
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


#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "asciiTeX.h"


static wchar_t * readfile(char * filename)
{
    FILE * f;
    size_t l_alloc = 1000;
    size_t l = 0, esc = 0;
    wchar_t * results = malloc(l_alloc * sizeof(wchar_t));
    if ((f = fopen(filename, "rbe")) == NULL) {
        fprintf(stderr, "File %s not found\n", filename);
        exit(1);
    }
    while (!feof(f)) {
        if (l == l_alloc) {
            l_alloc += 100;
            results = realloc(results, l_alloc * sizeof(wchar_t));
        }
        results[l++] = fgetwc(f);

        if ((results[l - 1] == L'%') && (!esc)) {
            /* % is the comment sign, ignore rest of the line */
            l--;
            while (!feof(f) && fgetwc(f) != L'\n')
                ;
        }
        if (esc) /* the escape flag is to comment out comment signs, i.e. \% */
            esc = 0;
        else if (l > 0 && results[l - 1] == L'\\')
            esc = 1;
    }
    results[--l] = L'\0';
    fclose(f);
    return results;
}

int main(int argc, char * argv[])
{
    setlocale(LC_ALL, ""); // en_US.UTF-8
    wchar_t ** screen = NULL;
    long i, cols = 0, rows = 0;
    wchar_t * eq = NULL;
    int ll = 80;
    enum { LL, FILE, EQ } opt_parse = EQ;
    char * usage = {
        "Usage: asciiTeX [-v] [-f file] [-ll line-length] [equation]\n"
        "\t-v\t\tShow version info and exit\n"
        "\t-h\t\tShow this help and exit\n"
        "\t-f file\t\tRead equation form input file\n"
        "\t-ll line-length\tSet the line length\n"};
    char * header = {"    ___   _____ ______________   ______    _  __\n"
                     "   /   | / ___// ____/  _/  _/  /_  __/__ | |/ /\n"
                     "  / /| | \\__ \\/ /    / / / /     / / / _ \\|   / \n"
                     " / ___ |___/ / /____/ /_/ /     / / /  __/   |  \n"
                     "/_/  |_/____/\\____/___/___/    /_/  \\___/_/|_|  \n"
                     "The ASCII equation renderer\n"
                     "Version %s - %s\n\n"
                     "asciiTeX is a fork of eqascii by Przemek Borys\n"
                     "Fork by Bart Pieters\n"
                     "Fork by Lars Eggert\n"
                     "Distributed under GPL V2\n"};

    for (i = 1; i < argc; i++) {
        if (opt_parse == FILE) {
            if (eq) {
                fprintf(stderr, "Cannot handle multiple inputs\n");
                goto done;
            }
            eq = readfile(argv[i]);
            opt_parse = EQ;
        } else if (opt_parse == LL) {
            opt_parse = EQ;
            ll = atoi(argv[i]);
            if (ll < 0) {
                fprintf(stderr, "Cannot handle negative line lengths\n");
                goto done;
            }

        } else if (opt_parse == EQ) {
            if (strncmp("-ll", argv[i], 3) == 0) {
                if (i == argc - 1) {
                    fprintf(stderr, "Missing line length value\n");
                    goto done;
                }

                opt_parse = LL;
            } else if (strncmp("-f", argv[i], 2) == 0) {
                if (i == argc - 1) {
                    fprintf(stderr, "Missing file name\n");
                    goto done;
                }

                opt_parse = FILE;
            } else if (strncmp("-v", argv[i], 2) == 0) {
                printf(header, "lars", __DATE__);
                goto done;
            } else if (strncmp("-h", argv[i], 2) == 0) {
                puts(usage);
                goto done;
            } else if (eq) {
                fprintf(stderr, "More than one equation found\n");
                goto done;
            } else {
                if (eq) {
                    fprintf(stderr, "Cannot handle multiple inputs\n");
                    goto done;
                }
                eq = malloc((strlen(argv[i]) + 1) * sizeof(wchar_t));
                swprintf(eq, (strlen(argv[i]) + 1) * sizeof(wchar_t), L"%s",
                         argv[i]);
            }
        }
    }

    if (!eq) {
        fputs(usage, stderr);
        return 1;
    }
    screen = asciiTeX(eq, ll, &cols, &rows);
done:
    free(eq);
    if (screen) {
        for (i = 0; i < rows; i++) {
            // remove trailing whitespace
            long j = cols + 1;
            while (j >= 0 && (iswspace(screen[i][j]) || screen[i][j] == L'\0'))
                screen[i][j--] = L'\0';
            wprintf(L"%ls\n", screen[i]);
            free(screen[i]);
        }
        free(screen);
    } else
        return 1;
    return 0;
}
