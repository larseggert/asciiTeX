#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(asciiTeX, 0.21)
AM_INIT_AUTOMAKE(asciiTeX, 0.21)
AC_CONFIG_SRCDIR([src/config.h.in])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

AC_DEFINE([_GNU_SOURCE], [], [Enable GNU extensions])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strchr strdup strstr])

AC_CHECK_TOOL([WINDRES], [windres], [no])
AM_CONDITIONAL([WITH_WIN_ICON], [test "$WINDRES" != "no"])


AC_ARG_ENABLE(gtk,[  --disable-gtk disable the gtk user interface], enable_gtk=no, enable_gtk=yes)
if test "x$enable_gtk" = "xyes"; then
PKG_CHECK_MODULES(GTK2, [gtk+-2.0 >= 2.16])
AC_SUBST([GTK2_CFLAGS])
AC_SUBST([GTK2_LIBS])

PKG_CHECK_MODULES(GDK_PIXBUF, [gdk-pixbuf-2.0])
AC_SUBST([GDK_PIXBUF_CFLAGS])
AC_SUBST([GDK_PIXBUF_LIBS])

fi

AM_CONDITIONAL(ENABLE_GTK, [test "x$enable_gtk" = "xyes"])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

