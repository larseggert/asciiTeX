##Process this file with automake to create Makefile.in
if ENABLE_GTK
bin_PROGRAMS = asciiTeX asciiTeX_gui
asciiTeX_SOURCES = array.c brace.c draw.c main.c sqrt.c symbols.c asciiTeX.c dim.c frac.c limit.c ouline.c sscript.c utils.c \
array.h config.h parsedef.h asciiTeX.h dim.h sqrt.h utils.h asciiTeX_struct.h \
 draw.h limit.h sscript.h brace.h frac.h ouline.h symbols.h
if WITH_WIN_ICON
asciiTeX_gui_SOURCES = array.c brace.c draw.c gui.c sqrt.c symbols.c asciiTeX.c dim.c frac.c limit.c ouline.c sscript.c utils.c \
gui.h gui_ico.h manual.h asciitexgui.rc logo.ico
.rc.o:
	i686-pc-mingw32-windres $^ -o $@
%.o : %.rc
	i686-pc-mingw32-windres $^ -o $@
else
asciiTeX_gui_SOURCES = array.c brace.c draw.c gui.c sqrt.c symbols.c asciiTeX.c dim.c frac.c limit.c ouline.c sscript.c utils.c
endif
asciiTeX_gui_LDADD = @GTK2_LIBS@ @GDK_PIXBUF_LIBS@
asciiTeX_gui_LDFLAGS = -mwindows
asciiTeX_gui_CFLAGS = @GTK2_CFLAGS@ @GDK_PIXBUF_CFLAGS@
else
bin_PROGRAMS = asciiTeX
asciiTeX_SOURCES = array.c brace.c draw.c main.c sqrt.c symbols.c asciiTeX.c dim.c frac.c limit.c ouline.c sscript.c utils.c \
 array.h config.h gui.h parsedef.h asciiTeX.h dim.h gui_ico.h sqrt.h utils.h asciiTeX_struct.h \
 draw.h limit.h sscript.h brace.h frac.h ouline.h symbols.h
endif

