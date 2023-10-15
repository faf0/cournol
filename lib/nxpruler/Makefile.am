noinst_LIBRARIES = libnxpruler.a

libnxpruler_a_SOURCES = nxpruler.c nxphruler.c nxpvruler.c \
			nxpruleradjustment.c nxpruler-marshalers.c

noinst_HEADERS = nxpruler.h nxphruler.h nxpvruler.h \
		 nxpruleradjustment.h nxpruler-marshalers.h

AM_CFLAGS = -DG_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1 @GTK_CFLAGS@

BUILT_SOURCES = nxpruler-marshalers.c nxpruler-marshalers.h

$(srcdir)/nxpruler-marshalers.c: $(srcdir)/nxpruler-marshalers.list
	glib-genmarshal --prefix=_nxpruler_marshal --body $< > tmp
	echo '#include "nxpruler-marshalers.h"' | cat - tmp > $@
	rm -f tmp
$(srcdir)/nxpruler-marshalers.h: $(srcdir)/nxpruler-marshalers.list
	glib-genmarshal --prefix=_nxpruler_marshal --header $< > $@

EXTRA_DIST = nxpruler-marshalers.list
