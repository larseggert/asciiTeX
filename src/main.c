/* main.c: parse the commandline, reading files, and output to stdout. */

/*  This file is part of asciiTeX.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; see the file COPYING.  If not, write to
      The Free Software Foundation, Inc.
      59 Temple Place, Suite 330
      Boston, MA 02111 USA
      
    
    Authors:
    Original program (eqascii): Przemek Borys
    Fork by: Bart Pieters
       
*************************************************************************/

/*
 * #define DEBUG 
 */
#ifdef DEBUG
#include <mcheck.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asciiTeX.h"
#include "utils.h"
#include "config.h"

char * readfile(char *filename)
{
	FILE *f;
	int l_alloc=100;
	int l=0, esc=0;
	char * results=malloc(l_alloc*sizeof(char));
    	if ((f = fopen(filename, "r")) == NULL)
    	{
    		fprintf(stderr,"File %s not found\n", filename);
		exit(1);
    	}
	while (!feof(f))
	{
		if (l==l_alloc)
		{
			l_alloc+=100;
			results=realloc(results,l_alloc*sizeof(char));
		}
		results[l++]=(char)getc(f);
		
		if((results[l-1]=='%')&&(!esc))
		{
			/* % is the comment sign, ignore rest of the line */
			l--;
			while(!feof(f) && ((char)getc(f) != '\n'));
		}
		if (esc) /* the escape flag is to comment out comment signs, i.e. \% */
			esc=0;
		else if((results[l-1]=='\\'))
			esc=1;
	
	}
	results[--l]='\0';
	fclose(f);
 	return results;
}

int
main(int argc, char *argv[])
{
	char          **screen;
	int             i, cols, rows;
	char * eq=NULL;
	int ll=80, f=0;
	enum {LL, FILE, EQ} opt_parse=EQ;
	char *usage = {
		"Usage: asciiTeX [-v] [-f file] [-ll line-length] [equation]\n"
		"\t-v\t\tShow version info and exit\n"
		"\t-h\t\tShow this help and exit\n"
		"\t-f file\t\tRead equation form input file\n"
		"\t-ll line-length\tSet the line length\n"
		};
	char *header = {
		"    ___   _____ ______________   ______    _  __\n"
		"   /   | / ___// ____/  _/  _/  /_  __/__ | |/ /\n"
		"  / /| | \\__ \\/ /    / / / /     / / / _ \\|   / \n"
		" / ___ |___/ / /____/ /_/ /     / / /  __/   |  \n"
		"/_/  |_/____/\\____/___/___/    /_/  \\___/_/|_|  \n"
		"The ASCII equation renderer\n"
		"Version %s - %s\n\n"
		"asciiTeX is a fork of eqascii by Przemek Borys\n"
		"Fork by Bart Pieters\n"
		"Distributed under GPL V2\n"
		};
	
	for (i=1;i<argc;i++)
	{
		if (opt_parse==FILE)
		{
			eq=readfile(argv[i]);
			f=1;
			opt_parse=EQ;
		}
		else if (opt_parse==LL)
		{
			opt_parse=EQ;
			ll=atoi(argv[i]);
			if(ll<0)
			{
				fprintf(stderr,
					"Cannot handle negative line lengths\n");
				exit(1);				
			}
				
		}
		else if (opt_parse==EQ)
		{
			if(strncmp("-ll", argv[i], 3)==0)
			{
				if (i==argc-1)
				{
					fprintf(stderr,
						"Missing line length value\n");
					exit(1);				
				}
				
				opt_parse=LL;
			}
			else if(strncmp("-f", argv[i], 2)==0)
			{
				if (i==argc-1)
				{
					fprintf(stderr,
						"Missing file name\n");
					exit(1);				
				}
				
				opt_parse=FILE;
			}
			else if(strncmp("-v", argv[i], 2)==0)
			{
				printf(header, PACKAGE_VERSION, __DATE__);
				return 0;
			}
			else if(strncmp("-h", argv[i], 2)==0)
			{
				printf(usage);
				return 0;
			}
			else if (eq)
			{
				fprintf(stderr,
					"More than one equation found.\n");
				exit(1);				
			}
			else
				eq=argv[i];			
		}
	}

	if (!eq)
	{
		fprintf(stderr, usage);
		return 1;
	}
	screen = asciiTeX(eq, ll, &cols, &rows);
	if (f)
		free(eq);
	if (screen)
	{
		for (i = 0; i < rows; i++)
		{
			if (cols<0)
				fprintf(stderr,"%s", screen[i]);
			else
				printf("%s", screen[i]);
			free(screen[i]);
			if (cols<0)
				fprintf(stderr,"\n");
			else
			printf("\n");
		}
		free(screen);
		if (cols<0)
			fprintf(stderr,"\n");
		else
			printf("\n");
	}
	else
		return 1;
	return 0;
}
