AC_INIT(src/main.c)
AC_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE(cournol, 0.4)

AC_PROG_CC
AC_C_CONST

CFLAGS="-O2 -Wall -pedantic -W"

AC_HEADER_STDC
AC_CHECK_HEADERS(math.h)

AM_PATH_GTK_2_0

ALL_LINGUAS="de"
AM_GNU_GETTEXT

# Set GETTEXT_PACKAGE in config.h
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "cournol", [Gettext Package])

# Set LOCALEDIR in config.h.
if test "$prefix" = "NONE" ; then
	prefix=$ac_default_prefix
fi
AC_DEFINE_UNQUOTED(LOCALEDIR, "${prefix}/share/locale", [Locale Directory])

AC_PROG_RANLIB

AC_PROG_INSTALL

AC_OUTPUT([
Makefile
lib/Makefile
lib/nxpruler/Makefile
lib/nxpgtkdatabox/Makefile
src/Makefile
intl/Makefile
po/Makefile.in
])
