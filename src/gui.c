/* gui.c: the asciiTeX gui version */

/*  This gui for asciiTeX was based on the excellent:
    "Example running a command into a GtkTextView" by Micah Carrick 
    written for www.gtkforums.com
    
*************************************************************************/
    
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
      
    
    Author: Bart Pieters
    
*************************************************************************/

#ifdef WIN
#include <windows.h>
#endif
#include <gtk/gtk.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include "asciiTeX.h"

/* The following two headers are resources we include in the executable */
/* gui.h contains the xml file with the gui description. This file can  *
 * be generated from gui.glade by:                                      *
 * > gtk-builder-convert gui.glade gui.xml                              *
 * Then run:                                                            * 
 * > XML_to_header.sh  > gui.h                                          */ 
#include "gui.h"

/* gui_ico.h contains the icon for decorating the window. Generate by:  *
 * > gdk-pixbuf-csource --struct --name=icon logo.png > gui_ico.h       */
#include "gui_ico.h"
#include "manual.h"

int ll=80;
int fl=1;

/* structure to hold references to our widgets */
typedef struct
{
        GtkWidget               *textview_in;
        GtkWidget               *textview_out;
        GtkWidget               *toggle;
        GtkWidget               *spin;
} MyWidgets;


GdkPixbuf *create_pixbuf()
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf =  gdk_pixbuf_from_pixdata ( &icon, TRUE, &error);
   /* pixbuf = gdk_pixbuf_new_from_file(filename, &error);*/
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}


char * process_command(char *command)
{
	int l=strlen(command);
	int i, j, esc=0;
	char * results=malloc((l+1)*sizeof(char));
	j=0;
	i=0;
	while(i<l)
	{
		results[j++]=command[i++];
		
		if((results[j-1]=='%')&&(!esc))
		{
			/* % is the comment sign, ignore rest of the line */
			j--;
			while((i<l) && (command[i++] != '\n'));
			i--;
		}
		if (esc) /* the escape flag is to comment out comment signs, i.e. \% */
			esc=0;
		else if(results[j-1]=='\\')
			esc=1;
	
	}
	results[j]='\0';
 	return results;
}

/* call back function for when the button is pressed */
static void
on_execute_button_clicked (GtkWidget* widget, gpointer user_data)
{
        MyWidgets               *widgets;
        char                    *output;
        char              	*command;
        char              	*proc;
        GtkTextBuffer           *buffer;
	char          **screen;
	int             i,
	                j, cols, rows, k;
  	GtkTextIter start,end;
			
        widgets = (MyWidgets *)user_data;
	buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW (widgets->textview_in));
  	gtk_text_buffer_get_start_iter(buffer,&start);
  	gtk_text_buffer_get_end_iter(buffer,&end);
	command = (char *)gtk_text_buffer_get_text (GTK_TEXT_BUFFER (buffer), &start, &end, TRUE);
	proc=process_command(command);
        /* command = (char *)gtk_text_view_get_text (GTK_TEXT_VIEW (widgets->textview_in));*/
	
	screen = asciiTeX(proc, ll*fl, &cols, &rows);
	free(command);
	free(proc);
	if(cols>0)
	{
		output=malloc(((rows+1)*(cols+1)+1)*sizeof(char));
		k=0;
		if (screen)
		{
			for (i = 0; i < rows; i++)
			{
				for (j = 0; j < cols; j++)
					output[k++]=screen[i][j];
				free(screen[i]);
				output[k++]='\n';
			}
			free(screen);
		}
		output[k]='\0';
	}
	else
	{
		cols=0;
		for (i = 0; i < rows; i++)
			cols+=strlen(screen[i]);
		output=malloc((cols+1)*sizeof(char));
		k=0;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < strlen(screen[i]); j++)
				output[k++]=screen[i][j];
			free(screen[i]);
		}
		free(screen);
		output[k]='\0';
	}
	

        /* get the text buffer */
        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (widgets->textview_out));

        /* set the text in the buffer to the output */
        gtk_text_buffer_set_text (buffer, output, -1);

        /* free the output string */
        free (output);
}
static void
on_checkbutton_ll_toggled (GtkWidget* widget, gpointer user_data)
{
        MyWidgets               *widgets;
        widgets = (MyWidgets *)user_data;
	if (fl)
		fl=0;
	else
		fl=1;
}
 
static void
on_spinbutton_ll_value_changed (GtkWidget* widget, gpointer user_data) 
{
        MyWidgets               *widgets;
        widgets = (MyWidgets *)user_data;
	
        ll = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (widgets->spin));
}

static void
on_help_button_clicked (GtkWidget* widget, gpointer user_data)  
{ 
        MyWidgets               *widgets;
        GtkTextBuffer           *buffer; 
        widgets = (MyWidgets *)user_data;
        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (widgets->textview_out));
        gtk_text_buffer_set_text (buffer, syntax, -1);
}     

int
main (int argc, char *argv[])
{

        GtkWidget               *window;
        GtkBuilder              *gxml;
        MyWidgets               *widgets;
        PangoFontDescription    *font_desc;
	int i;
        widgets = g_slice_new (MyWidgets);

        /* initialize the GTK+ library */
        gtk_init (&argc, &argv);

        /* build GladeXML object from the glade XML file which we compiled in through gui.h */
        gxml = gtk_builder_new ();
        gtk_builder_add_from_string (gxml, GUI, -1, NULL);
  

        /* get widgets from GladeXML object */
        window = GTK_WIDGET (gtk_builder_get_object (gxml, "window"));
        widgets->textview_in = GTK_WIDGET (gtk_builder_get_object (gxml, "eq_in"));
        widgets->textview_out = GTK_WIDGET (gtk_builder_get_object (gxml, "eq_out"));
        widgets->toggle = GTK_WIDGET (gtk_builder_get_object (gxml, "checkbutton_ll"));
        widgets->spin = GTK_WIDGET (gtk_builder_get_object (gxml, "spinbutton_ll"));

        /* call gtk_main_quit() when the window's "x" is clicked */
        g_signal_connect (G_OBJECT (window), "delete-event", gtk_main_quit,
                          NULL);

        /*
        Call on_button_clicked() when execute_button is clicked, passing the
        MyWidget struct to the function as the user_data
        */
        g_signal_connect (gtk_builder_get_object (gxml, "execute_button"),
                          "clicked", G_CALLBACK (on_execute_button_clicked),
                          widgets);
        g_signal_connect (gtk_builder_get_object (gxml, "help_button"),
                          "clicked", G_CALLBACK (on_help_button_clicked),
                          widgets);
        g_signal_connect (gtk_builder_get_object (gxml, "checkbutton_ll"),
                          "toggled", G_CALLBACK (on_checkbutton_ll_toggled),
                          widgets);
        g_signal_connect (gtk_builder_get_object (gxml, "spinbutton_ll"),
                          "value_changed", G_CALLBACK (on_spinbutton_ll_value_changed),
                          widgets);

        /* free GladeXML object */
        g_object_unref (G_OBJECT (gxml));

        /* use a fixed width font on the text view so the output looks proper */
        font_desc = pango_font_description_from_string ("monospace 10");
        gtk_widget_modify_font (widgets->textview_out, font_desc);
        gtk_widget_modify_font (widgets->textview_in, font_desc);
        pango_font_description_free (font_desc);

  	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf());
        /* show the main window */
        gtk_widget_show (window);
	
        gtk_main ();

        return 0;
} 
