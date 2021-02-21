/*  XRoar - a Dragon/Tandy Coco emulator
 *  Copyright (C) 2003-2007  Ciaran Anscomb
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_SDL
# include <SDL.h>
#endif

#include "xroar.h"
#include "logging.h"
#include "global.h"

int a_main(int argc, char **argv) 
{
  dragon_global_init();

	atexit(xroar_shutdown);
	if (xroar_init(argc, argv) != 0) {
		exit(1);
  }

  psp_run_load_file();
  psp_sdl_black_screen();

  DRAGON.emu_started = 1;
  dragon_set_video_artifact(DRAGON.video_artifact_mode);

	xroar_mainloop();
	return 0;
}
