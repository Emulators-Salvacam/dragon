/*
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
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <png.h>

#include "global.h"
#include "psp_sdl.h"
#include "psp_danzeff.h"
#include "psp_dragon.h"

int psp_screenshot_mode = 0;
int dragon_in_menu = 0;

static void
dragon_render_normal()
{
  int x; 
  int y;
  u16 *src_pixel = (u16*)blit_surface->pixels;
  u16 *dst_pixel = (u16*)back_surface->pixels;
  dst_pixel += (320 - DRAGON_SCREEN_W) / 2;
  dst_pixel += ((240 - DRAGON_SCREEN_H) * PSP_LINE_SIZE / 2);
  for (y = 0; y < DRAGON_SCREEN_H; ++y) {
    for (x = 0; x < DRAGON_SCREEN_W; ++x) {
      dst_pixel[x] = src_pixel[x];
    }
    dst_pixel += PSP_LINE_SIZE;
    src_pixel += DRAGON_SCREEN_W;
  }
}

/* 

  LUDO: 16-bit HiColor (565 format) 
  see http://www.compuphase.com/graphic/scale3.htm

 */
static inline u16 loc_coloraverage(u16 a, u16 b)
{
  return (u16)(((a ^ b) & 0xf7deU) >> 1) + (a & b);
}

static inline void 
dragon_X125_pixel(u16 *dist, const u16 *src)
{
  dist[0] = src[0];
  dist[1] = loc_coloraverage(src[0], src[1]);
  dist[2] = src[1];
  dist[3] = src[2];
  dist[4] = src[3];
}

static void
dragon_render_fit()
{
  u16 *src_pixel = (u16*)blit_surface->pixels;
  u16 *dst_pixel = (u16*)back_surface->pixels;

  int src_y;
  int dst_x;
  int dst_y;
  int count;

  src_pixel += DRAGON_SCREEN_W * 24;

  for (dst_y = 0; dst_y < 240; dst_y++) {
    src_y = (dst_y * REAL_DRAGON_H) / 240;
    u16* src_line = &src_pixel[(src_y * DRAGON_SCREEN_W)];
    u16* dst_line = dst_pixel;

    src_line += 32;
    count = 256;
    while (count > 0) {
      dragon_X125_pixel(dst_line, src_line);
      src_line += 4;
      dst_line += 5;
      count -= 4;
    }
    dst_pixel += PSP_LINE_SIZE;
  }
}

static void
dragon_synchronize(void)
{
	static u32 nextclock = 1;

  if (DRAGON.dragon_speed_limiter) {

	  if (nextclock) {
		  u32 curclock;
		  do {
        curclock = SDL_GetTicks();
		  } while (curclock < nextclock);
  
      nextclock = curclock + (u32)( 1000 / DRAGON.dragon_speed_limiter);
    }
  }
}

void
dragon_update_fps()
{
  static u32 next_sec_clock = 0;
  static u32 cur_num_frame = 0;
  cur_num_frame++;
  u32 curclock = SDL_GetTicks();
  if (curclock > next_sec_clock) {
    next_sec_clock = curclock + 1000;
    DRAGON.dragon_current_fps = cur_num_frame;
    cur_num_frame = 0;
  }
}

extern int psp_in_dragon_menu;

void
psp_sdl_render()
{
  if (dragon_in_menu) return;

  if (DRAGON.psp_skip_cur_frame <= 0) {

    DRAGON.psp_skip_cur_frame = DRAGON.psp_skip_max_frame;

    if (DRAGON.dragon_render_mode == DRAGON_RENDER_NORMAL) dragon_render_normal();
    else                          
    if (DRAGON.dragon_render_mode == DRAGON_RENDER_FIT) dragon_render_fit();

    if (psp_kbd_is_danzeff_mode()) {

      danzeff_moveTo(-50, -50);
      danzeff_render();
    }

    if (DRAGON.dragon_view_fps) {
      char buffer[32];
      sprintf(buffer, "%3d", (int)DRAGON.dragon_current_fps);
      psp_sdl_fill_print(0, 0, buffer, 0xffffff, 0 );
    }

    if (DRAGON.psp_display_lr) {
      psp_kbd_display_active_mapping();
    }
    psp_sdl_flip();
  
    if (psp_screenshot_mode) {
      psp_screenshot_mode--;
      if (psp_screenshot_mode <= 0) {
        psp_sdl_save_screenshot();
        psp_screenshot_mode = 0;
      }
    }

  } else if (DRAGON.psp_skip_max_frame) {
    DRAGON.psp_skip_cur_frame--;
  }

  if (DRAGON.dragon_speed_limiter) {
    dragon_synchronize();
  }

  if (DRAGON.dragon_view_fps) {
    dragon_update_fps();
  }
}
