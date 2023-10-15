bin_PROGRAMS = cournol

cournol_SOURCES = main.c options.c input.c get_values.c create_table.c init_values.c init_points.c output.c message.c

noinst_HEADERS = main.h options.h input.h get_values.h create_table.h init_values.h init_points.h output.h message.h

INCLUDES = -I$(top_builddir)/lib/nxpgtkdatabox -I$(top_builddir)/lib/nxpruler

AM_CFLAGS = -DG_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1 @GTK_CFLAGS@
cournol_LDADD = $(top_builddir)/lib/nxpgtkdatabox/libnxpgtkdatabox.a \
		$(top_builddir)/lib/nxpruler/libnxpruler.a \
		@GTK_LIBS@
