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

#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "types.h"
#include "logging.h"
#include "module.h"
#include "sam.h"
#include "ui_sdl.h"
#include "vdg.h"
#include "xroar.h"
#ifdef WINDOWS32
#include "common_windows32.h"
#endif
#include "global.h"
#include "psp_sdl.h"

static int init(int argc, char **argv);
static void shutdown(void);
static int set_fullscreen(int fullscreen);
static void vsync(void);
static void set_mode(unsigned int mode);
static void render_sg4(void);
static void render_sg6(void);
static void render_cg1(void);
static void render_rg1(void);
static void render_cg2(void);
static void render_rg6(void);
static void render_border(void);
static void alloc_colours(void);

VideoModule video_sdl_module = {
	{ "sdl", "Standard SDL surface",
	  init, 0, shutdown, NULL },
	NULL, set_fullscreen, 0,
	vsync, set_mode,
	render_sg4, render_sg6, render_cg1,
	render_rg1, render_cg2, render_rg6,
	render_border
};

# if 0 //LUDO:
typedef Uint8 Pixel;
# else
typedef Uint16 Pixel;
# endif
#define MAPCOLOUR(r,g,b) SDL_MapRGB(screen->format, r, g, b)
#define VIDEO_SCREENBASE ((Pixel *)screen->pixels)
#define XSTEP 1
#define NEXTLINE 0
#define VIDEO_TOPLEFT (VIDEO_SCREENBASE)
#define VIDEO_VIEWPORT_YOFFSET (0)
#define LOCK_SURFACE SDL_LockSurface(screen)
#define UNLOCK_SURFACE SDL_UnlockSurface(screen)

static SDL_Surface *screen;

#include "video_generic_ops.c"

static int init(int argc, char **argv) 
{
# if 0 //LUDO:
	(void)argc;
	(void)argv;
	LOG_DEBUG(2,"Initialising SDL video driver\n");
#ifdef WINDOWS32
	if (!getenv("SDL_VIDEODRIVER"))
		putenv("SDL_VIDEODRIVER=windib");
#endif
	if (!SDL_WasInit(SDL_INIT_NOPARACHUTE)) {
		if (SDL_Init(SDL_INIT_NOPARACHUTE) < 0) {
			LOG_ERROR("Failed to initialiase SDL: %s\n", SDL_GetError());
			return 1;
		}
	}
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		LOG_ERROR("Failed to initialiase SDL video driver: %s\n", SDL_GetError());
		return 1;
	}
	if (set_fullscreen(sdl_video_want_fullscreen))
		return 1;
	alloc_colours();
#ifdef WINDOWS32
	{
		SDL_version sdlver;
		SDL_SysWMinfo sdlinfo;
		SDL_VERSION(&sdlver);
		sdlinfo.version = sdlver;
		SDL_GetWMInfo(&sdlinfo);
		windows32_main_hwnd = sdlinfo.window;
	}
#endif
# else
  screen = blit_surface;
	alloc_colours();
# endif
	return 0;
}

static void shutdown(void) {
	LOG_DEBUG(2,"Shutting down SDL video driver\n");
	set_fullscreen(0);
	/* Should not be freed by caller: SDL_FreeSurface(screen); */
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

SDL_Surface* back_surface = 0;

static int set_fullscreen(int fullscreen) 
{
# if 0 //LUDO: DISABLE
# ifdef PSP
	back_surface = SDL_SetVideoMode(480, 272, 16, SDL_HWSURFACE|SDL_ANYFORMAT|SDL_DOUBLEBUF);
  screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 
     320, 240, 8, 1 << 3, 1 << 2, 1 << 3, 0);
# else
	screen = SDL_SetVideoMode(320, 240, 8, SDL_SWSURFACE|(fullscreen?SDL_FULLSCREEN:0));
fprintf(stdout, "%x %x %x\n", screen->format->Rmask, screen->format->Gmask, screen->format->Bmask);
# endif
	if (screen == NULL) {
		LOG_ERROR("Failed to allocate SDL surface for display\n");
		return 1;
	}
	if (fullscreen)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
	video_sdl_module.is_fullscreen = fullscreen;
# endif
	return 0;
}

static void 
vsync(void) 
{
  psp_sdl_render();

	pixel = VIDEO_TOPLEFT + VIDEO_VIEWPORT_YOFFSET;
	subline = 0;
}
