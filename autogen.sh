#! /bin/sh
 
aclocal \
&& autoheader \
&& automake --add-missing --copy \
&& autoconf \
&& gettextize --copy --force --intl
