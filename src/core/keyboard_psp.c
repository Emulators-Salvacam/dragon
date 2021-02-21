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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "types.h"
#include "logging.h"
#include "cart.h"
#include "events.h"
#include "hexs19.h"
#include "joystick.h"
#include "keyboard.h"
#include "machine.h"
#include "module.h"
#include "pia.h"
#include "snapshot.h"
#include "tape.h"
#include "vdisk.h"
#include "vdrive.h"
#include "xroar.h"

#include "keyboard_psp.h"
#include "global.h"

static int init(int argc, char **argv);
static void shutdown(void);

KeyboardModule keyboard_psp_module = {
  { "psp", "PSP keyboard driver",
    init, 0, shutdown, NULL }
};

static event_t *poll_event;

static void do_poll(void *context);
extern int psp_update_keys(void);

static void 
do_poll(void *context) 
{
  (void)context;
  psp_update_keys();
  poll_event->at_cycle += OSCILLATOR_RATE / 100;
  event_queue(poll_event);
}

static unsigned int loc_shift = 0;

static int dragon_to_keymap[DRAGONK_MAX_KEY][2] = {
  { '0', 0 }, // DRAGONK_0          
  { '1', 0 }, // DRAGONK_1          
  { '2', 0 }, // DRAGONK_2          
  { '3', 0 }, // DRAGONK_3          
  { '4', 0 }, // DRAGONK_4          
  { '5', 0 }, // DRAGONK_5          
  { '6', 0 }, // DRAGONK_6          
  { '7', 0 }, // DRAGONK_7          
  { '8', 0 }, // DRAGONK_8          
  { '9', 0 }, // DRAGONK_9          
  { 'A', 0 }, // DRAGONK_A          
  { 'B', 0 }, // DRAGONK_B          
  { 'C', 0 }, // DRAGONK_C          
  { 'D', 0 }, // DRAGONK_D          
  { 'E', 0 }, // DRAGONK_E          
  { 'F', 0 }, // DRAGONK_F          
  { 'G', 0 }, // DRAGONK_G          
  { 'H', 0 }, // DRAGONK_H          
  { 'I', 0 }, // DRAGONK_I          
  { 'J', 0 }, // DRAGONK_J          
  { 'K', 0 }, // DRAGONK_K          
  { 'L', 0 }, // DRAGONK_L          
  { 'M', 0 }, // DRAGONK_M          
  { 'N', 0 }, // DRAGONK_N          
  { 'O', 0 }, // DRAGONK_O          
  { 'P', 0 }, // DRAGONK_P          
  { 'Q', 0 }, // DRAGONK_Q          
  { 'R', 0 }, // DRAGONK_R          
  { 'S', 0 }, // DRAGONK_S          
  { 'T', 0 }, // DRAGONK_T          
  { 'U', 0 }, // DRAGONK_U          
  { 'V', 0 }, // DRAGONK_V          
  { 'W', 0 }, // DRAGONK_W          
  { 'X', 0 }, // DRAGONK_X          
  { 'Y', 0 }, // DRAGONK_Y          
  { 'Z', 0 }, // DRAGONK_Z          
  { 'a', 0 }, // DRAGONK_a          
  { 'b', 0 }, // DRAGONK_b          
  { 'c', 0 }, // DRAGONK_c          
  { 'd', 0 }, // DRAGONK_d          
  { 'e', 0 }, // DRAGONK_e          
  { 'f', 0 }, // DRAGONK_f          
  { 'g', 0 }, // DRAGONK_g          
  { 'h', 0 }, // DRAGONK_h          
  { 'i', 0 }, // DRAGONK_i          
  { 'j', 0 }, // DRAGONK_j          
  { 'k', 0 }, // DRAGONK_k          
  { 'l', 0 }, // DRAGONK_l          
  { 'm', 0 }, // DRAGONK_m          
  { 'n', 0 }, // DRAGONK_n          
  { 'o', 0 }, // DRAGONK_o          
  { 'p', 0 }, // DRAGONK_p          
  { 'q', 0 }, // DRAGONK_q          
  { 'r', 0 }, // DRAGONK_r          
  { 's', 0 }, // DRAGONK_s          
  { 't', 0 }, // DRAGONK_t          
  { 'u', 0 }, // DRAGONK_u          
  { 'v', 0 }, // DRAGONK_v          
  { 'w', 0 }, // DRAGONK_w          
  { 'x', 0 }, // DRAGONK_x          
  { 'y', 0 }, // DRAGONK_y          
  { 'z', 0 }, // DRAGONK_z          
  { '7', 1 }, // DRAGONK_QUOTE
  { ',', 0 }, // DRAGONK_COMMA
  { ',', 1 }, // DRAGONK_LESS       
  { '.', 0 }, // DRAGONK_PERIOD     
  { '.', 1 }, // DRAGONK_GREATER    
  { ';', 0 }, // DRAGONK_SEMICOLON  
  { ':', 0 }, // DRAGONK_COLON
  { '_', 0 }, // DRAGONK_UNDERSCORE
  { '|', 0 }, // DRAGONK_PIPE
  { '-', 1 }, // DRAGONK_EQUAL
  { ';', 1 }, // DRAGONK_PLUS
  { '~', 0 }, // DRAGONK_TILDA
  { '2', 1 }, // DRAGONK_DBLQUOTE
  { '/' ,1 }, // DRAGONK_QUESTION
  { '/' ,0 }, // DRAGONK_SLASH
  { '-' ,0 }, // DRAGONK_MINUS
  { '[' ,0 }, // DRAGONK_LBRACKET
  { ']' ,0 }, // DRAGONK_RBRACKET
  { '{' ,0 }, // DRAGONK_LCBRACE
  { '}' ,0 }, // DRAGONK_RCBRACE
  { ' ' ,0 }, // DRAGONK_SPACE
  { '1' ,1 }, // DRAGONK_EXCLAMATN
  { '@' ,0 }, // DRAGONK_AT
  { '3' ,1 }, // DRAGONK_HASH
  { '4' ,1 }, // DRAGONK_DOLLAR
  { '5' ,1 }, // DRAGONK_PERCENT
  { '^' ,0 }, // DRAGONK_POWER
  { '6' ,1 }, // DRAGONK_AMPERSAND
  { ':' ,1 }, // DRAGONK_ASTERISK
  { '8' ,1 }, // DRAGONK_LPAREN
  { '9' ,1 }, // DRAGONK_RPAREN
  { '\\',0 }, // DRAGONK_BACKSLASH
  { '`' ,0 }, // DRAGONK_BACKQUOTE
  { 0x9 ,0 }, // DRAGONK_TAB
  { 0x8 ,0 }, // DRAGONK_BACKSPACE
  {   1 ,0 }, // DRAGONK_LEFT
  {   1 ,0 }, // DRAGONK_RIGHT        
  {   1 ,0 }, // DRAGONK_UP
  {   1 ,0 }, // DRAGONK_DOWN
  { 0x8 ,0 }, // DRAGONK_DELETE
  { 0xd ,0 }, // DRAGONK_RETURN
  {   0 ,0 }, // DRAGONK_SHIFT
  {   1 ,0 }, // DRAGONK_JOY_UP
  {   1 ,0 }, // DRAGONK_JOY_DOWN
  {   1 ,0 }, // DRAGONK_JOY_LEFT
  {   1 ,0 }, // DRAGONK_JOY_RIGHT
  {   1 ,0 }, // DRAGONK_JOY_FIRE
  {   1 ,0 }, // CMD_FPS,
  {   1 ,0 }, // CMD_JOY,
  {   1 ,0 }, // CMD_RENDER,
  {   1 ,0 }, // CMD_LOAD,
  {   1 ,0 }, // CMD_SAVE,
  {   1 ,0 }, // CMD_RESET,
  {   1 ,0 }, // CMD_AUTOFIRE,
  {   1 ,0 }, // CMD_INCFIRE,
  {   1 ,0 }, // CMD_DECFIRE,
  {   1 ,0 }, // CMD_RESET,
  {   1 ,0 }  // CMD_SCREEN
 };

#define JOY_UNLOW(j) if (j < 127) j = 127;
#define JOY_UNHIGH(j) if (j > 128) j = 128;

static void
loc_dragon_key_press(int key_id)
{
  if (! DRAGON.dragon_joystick_anal) {
    if (DRAGON.dragon_joystick == 0) {
      if (key_id == DRAGONK_JOY_UP) { joystick_lefty = 0; return; }
      if (key_id == DRAGONK_JOY_DOWN) { joystick_lefty = 255; return; }
      if (key_id == DRAGONK_JOY_LEFT) { joystick_leftx = 0; return; }
      if (key_id == DRAGONK_JOY_RIGHT) { joystick_leftx = 255; return; }
    }
    if (DRAGON.dragon_joystick == 1) {
      if (key_id == DRAGONK_JOY_UP) { joystick_righty = 0; return; }
      if (key_id == DRAGONK_JOY_DOWN) { joystick_righty = 255; return; }
      if (key_id == DRAGONK_JOY_LEFT) { joystick_rightx = 0; return; }
      if (key_id == DRAGONK_JOY_RIGHT) { joystick_rightx = 255; return; }
    }
  }
  if (DRAGON.dragon_joystick == 0) {
    if (key_id == DRAGONK_JOY_FIRE) { PIA_0A.tied_low &= 0xfd; return; }
  }
  if (DRAGON.dragon_joystick == 1) {
    if (key_id == DRAGONK_JOY_FIRE) { PIA_0A.tied_low &= 0xfe; return; }
  }
  if (key_id == DRAGONK_SHIFT) {
    loc_shift = 1;
    KEYBOARD_PRESS(0);
    return;
  }
  if (key_id == DRAGONK_UP) { KEYBOARD_PRESS(94); return; }
  if (key_id == DRAGONK_DOWN) { KEYBOARD_PRESS(10); return; }
  if (key_id == DRAGONK_LEFT) { KEYBOARD_PRESS(8); return; }
  if (key_id == DRAGONK_RIGHT) { KEYBOARD_PRESS(9); return; }
  //LUDO: TO_BE_DONE ! if (key_id == DRAGONK_HOME) { KEYBOARD_PRESS(12); return; }
# if 0 //TO_BE_DONE ! 
  if (unicode == '\\') {
    /* CoCo and Dragon 64 in 64K mode have a different way
     * of scanning for '\' */
    if (IS_COCO_KEYMAP || (IS_DRAGON64 && !(PIA_1B.port_output & 0x04))) {
      KEYBOARD_PRESS(0);
      KEYBOARD_PRESS(12);
    } else {
      KEYBOARD_PRESS(0);
      KEYBOARD_PRESS(12);
      KEYBOARD_PRESS('/');
    }
    return;
  }
  if (shift && (unicode == 8 || unicode == 127)) {
    KEYBOARD_PRESS(0);
    KEYBOARD_PRESS(8);
    return;
  }
  if (unicode == 163) {
    KEYBOARD_PRESS(0);
    KEYBOARD_PRESS('3');
    return;
  }
  if (unicode < 128) {
    unsigned int code = unicode_to_dragon[unicode];
    if (code & 128)
      KEYBOARD_PRESS(0);
    else
      KEYBOARD_RELEASE(0);
    KEYBOARD_PRESS(code & 0x7f);
  }
  return;
# endif
  if (key_id < DRAGONK_MAX_KEY) {
    unsigned int mapped = dragon_to_keymap[ key_id ][0];
    unsigned int shift  = dragon_to_keymap[ key_id ][1];
    if (shift && !loc_shift) {
      KEYBOARD_PRESS(0);
    }
    if (mapped) {
      KEYBOARD_PRESS(mapped);
    }
  }
}

void
dragon_key_press(int key_id)
{
  loc_dragon_key_press(key_id);
  keyboard_column_update();
  keyboard_row_update();
}

void
dragon_joystick_event(int key_id)
{
  if (! DRAGON.dragon_joystick_anal) return;

  if (DRAGON.dragon_joystick == 0) {
    if (key_id == DRAGONK_JOY_UP) { joystick_lefty -= DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_DOWN) { joystick_lefty += DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_LEFT) { joystick_leftx -= DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_RIGHT) { joystick_leftx += DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_FIRE) { PIA_0A.tied_low |= 0x02; return; }

    if (joystick_leftx < 1) joystick_leftx = 1;
    if (joystick_lefty < 1) joystick_lefty = 1;
    if (joystick_leftx > 255) joystick_leftx = 255;
    if (joystick_lefty > 255) joystick_lefty = 255;
  }
  if (DRAGON.dragon_joystick == 1) {
    if (key_id == DRAGONK_JOY_UP) { joystick_righty -= DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_DOWN) { joystick_righty += DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_LEFT) { joystick_rightx -= DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_RIGHT) { joystick_rightx += DRAGON.dragon_joystick_step; }
    else
    if (key_id == DRAGONK_JOY_FIRE) { PIA_0A.tied_low |= 0x01; return; }

    if (joystick_rightx < 1) joystick_rightx = 1;
    if (joystick_righty < 1) joystick_righty = 1;
    if (joystick_rightx > 255) joystick_rightx = 255;
    if (joystick_righty > 255) joystick_righty = 255;
  }
}

static void
loc_dragon_key_release(int key_id)
{
  if (! DRAGON.dragon_joystick_anal) {
    if (DRAGON.dragon_joystick == 0) {
      if (key_id == DRAGONK_JOY_UP) { JOY_UNLOW(joystick_lefty); return; }
      if (key_id == DRAGONK_JOY_DOWN) { JOY_UNHIGH(joystick_lefty); return; }
      if (key_id == DRAGONK_JOY_LEFT) { JOY_UNLOW(joystick_leftx); return; }
      if (key_id == DRAGONK_JOY_RIGHT) { JOY_UNHIGH(joystick_leftx); return; }
    }
    if (DRAGON.dragon_joystick == 1) {
      if (key_id == DRAGONK_JOY_UP) { JOY_UNLOW(joystick_righty); return; }
      if (key_id == DRAGONK_JOY_DOWN) { JOY_UNHIGH(joystick_righty); return; }
      if (key_id == DRAGONK_JOY_LEFT) { JOY_UNLOW(joystick_rightx); return; }
      if (key_id == DRAGONK_JOY_RIGHT) { JOY_UNHIGH(joystick_rightx); return; }
    }
  }
  if (DRAGON.dragon_joystick == 0) {
    if (key_id == DRAGONK_JOY_FIRE) { PIA_0A.tied_low |= 0x02; return; }
  }
  if (DRAGON.dragon_joystick == 1) {
    if (key_id == DRAGONK_JOY_FIRE) { PIA_0A.tied_low |= 0x01; return; }
  }
  if (key_id == DRAGONK_SHIFT) {
    loc_shift = 0;
    KEYBOARD_RELEASE(0);
    return;
  }
  if (key_id == DRAGONK_UP) { KEYBOARD_RELEASE(94); return; }
  if (key_id == DRAGONK_DOWN) { KEYBOARD_RELEASE(10); return; }
  if (key_id == DRAGONK_LEFT) { KEYBOARD_RELEASE(8); return; }
  if (key_id == DRAGONK_RIGHT) { KEYBOARD_RELEASE(9); return; }
  //LUDO: TO_BE_DONE ! if (key_id == DRAGONK_HOME) { KEYBOARD_RELEASE(12); return; }
# if 0 //TO_BE_DONE ! 
  if (unicode == '\\') {
    /* CoCo and Dragon 64 in 64K mode have a different way
     * of scanning for '\' */
    if (IS_COCO_KEYMAP || (IS_DRAGON64 && !(PIA_1B.port_output & 0x04))) {
      KEYBOARD_RELEASE(0);
      KEYBOARD_RELEASE(12);
    } else {
      KEYBOARD_RELEASE(0);
      KEYBOARD_RELEASE(12);
      KEYBOARD_RELEASE('/');
    }
    return;
  }
  if (shift && (unicode == 8 || unicode == 127)) {
    KEYBOARD_RELEASE(0);
    KEYBOARD_RELEASE(8);
    return;
  }
  if (unicode == 163) {
    KEYBOARD_RELEASE(0);
    KEYBOARD_RELEASE('3');
    return;
  }
  if (unicode < 128) {
    unsigned int code = unicode_to_dragon[unicode];
    if (code & 128)
      KEYBOARD_RELEASE(0);
    else
      KEYBOARD_RELEASE(0);
    KEYBOARD_RELEASE(code & 0x7f);
  }
  return;
# endif
  if (key_id < DRAGONK_MAX_KEY) {
    unsigned int mapped = dragon_to_keymap[ key_id ][0];
    unsigned int shift  = dragon_to_keymap[ key_id ][1];
    if (mapped) {
      KEYBOARD_RELEASE(mapped);
    }
    if (shift && !loc_shift) {
      KEYBOARD_RELEASE(0);
    }
  }
}

void
dragon_reset_keyboard()
{
  keyboard_reset();
}

void
dragon_key_release(int key_id)
{
  loc_dragon_key_release(key_id);
  keyboard_column_update();
  keyboard_row_update();
}

static int 
init(int argc, char **argv) 
{
  poll_event = event_new();
  poll_event->dispatch = do_poll;
  poll_event->at_cycle = current_cycle + (OSCILLATOR_RATE / 100);
  event_queue(poll_event);
  return 0;
}

static void 
shutdown(void) 
{
  event_free(poll_event);
}
