dnl configure.ac

AC_INIT
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER(config.h)

dnl program name and version
AM_INIT_AUTOMAKE(cournol, 0.5)

dnl C compiler
AC_PROG_CC
AC_C_CONST

dnl Standard headers
AC_HEADER_STDC
AC_CHECK_HEADER([math.h],,[AC_MSG_ERROR([math.h not found])])

dnl Configure GTK+
PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.10)

dnl Optimize and print errors
CFLAGS="-O2 -Wall -pedantic -W"

LIBS="$LIBS $GTK_LIBS"
CFLAGS="$CFLAGS $GTK_CFLAGS"

dnl Look for GtkDatabox library
AS_MESSAGE([checking for GtkDatabox Library and Header files...])
AC_CHECK_LIB([gtkdatabox], [gtk_databox_graph_add], [LIBS="-lgtkdatabox $LIBS"],
             [AC_MSG_ERROR([GtkDatabox library not found. Install libgtkdatabox first.])])

dnl Look for GtkDatabox headers
AC_CHECK_HEADERS([gtkdatabox.h gtkdatabox_lines.h gtkdatabox_marker.h],,
                 [AC_MSG_ERROR([GtkDatabox headers not found. Install libgtkdatabox-dev.])],
                 [#if HAVE_GTKDATABOX_H
                  # include <gtkdatabox.h>
                  #endif])

ALL_LINGUAS="de"
AM_GNU_GETTEXT

dnl Set GETTEXT_PACKAGE in config.h
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "cournol", [Gettext Package])

dnl Set LOCALEDIR in config.h.
if test "$prefix" = "NONE" ; then
	prefix=$ac_default_prefix
fi
AC_DEFINE_UNQUOTED(LOCALEDIR, "${prefix}/share/locale", [Locale Directory])

AC_PROG_INSTALL

AC_CONFIG_FILES([
Makefile
src/Makefile
intl/Makefile
po/Makefile.in
])
AC_OUTPUT
