/* cournol
 * Copyright (C) (2006) (Fabian Förg) <fabian.foerg@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
#ifdef WIN32
# include "../config.h.w32"
# include <windows.h>
#endif
#endif

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "main.h"
#include "options.h"

int main(int argc, char *argv[])
{
	appl_t appl;

#ifdef ENABLE_NLS
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif

	gtk_init(&argc, &argv);
	options(&appl);
	gtk_main();

	return 0;
}

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
		     HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}
#endif
