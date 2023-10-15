noinst_LIBRARIES = libnxpgtkdatabox.a

libnxpgtkdatabox_a_SOURCES =  nxpgtkdatabox.c gtkdatabox_lines.c gtkdatabox_grid.c \
			      gtkdatabox_graph.c gtkdatabox_xyc_graph.c \
			      gtkdatabox_marker.c gtkdatabox_marshal.c

noinst_HEADERS = nxpgtkdatabox.h gtkdatabox_lines.h gtkdatabox_grid.h \
		 gtkdatabox_graph.h gtkdatabox_xyc_graph.h \
		 gtkdatabox_marker.h gtkdatabox_marshal.h

INCLUDES = -I$(top_builddir)/lib/nxpruler

AM_CFLAGS = -DG_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1 @GTK_CFLAGS@

BUILT_SOURCES = gtkdatabox_marshal.c gtkdatabox_marshal.h

$(srcdir)/gtkdatabox_marshal.c: $(srcdir)/gtkdatabox_marshal.list
	glib-genmarshal --prefix=gtk_databox_marshal --body $< > $@
$(srcdir)/gtkdatabox_marshal.h: $(srcdir)/gtkdatabox_marshal.list
	glib-genmarshal --prefix=gtk_databox_marshal --header $< > $@

EXTRA_DIST = gtkdatabox_marshal.list
