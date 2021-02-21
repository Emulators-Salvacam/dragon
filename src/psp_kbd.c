/*
 *  Copyright (C) 2006 Ludovic Jacomme (ludovic.jacomme@gmail.com)
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
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include <SDL.h>

#include "global.h"
#include "psp_kbd.h"
#include "psp_menu.h"
#include "psp_sdl.h"
#include "psp_danzeff.h"

# define KBD_MIN_ANALOG_TIME  150000
# define KBD_MIN_START_TIME  1000000
# define KBD_MAX_EVENT_TIME   500000
# define KBD_MIN_PENDING_TIME 300000
# define KBD_MIN_DANZEFF_TIME 150000
# define KBD_MIN_COMMAND_TIME 100000
# define KBD_MIN_BATTCHECK_TIME 90000000 
# define KBD_MIN_AUTOFIRE_TIME   1000000

 static gp2xCtrlData    loc_button_data;
 static unsigned int   loc_last_event_time = 0;
 static unsigned int   loc_last_analog_time = 0;
 static long           first_time_stamp = -1;
 static long           first_time_auto_stamp = -1;
 static char           loc_button_press[ KBD_MAX_BUTTONS ]; 
 static char           loc_button_release[ KBD_MAX_BUTTONS ]; 
 static unsigned int   loc_button_mask[ KBD_MAX_BUTTONS ] =
 {
   GP2X_CTRL_UP         , /*  KBD_UP         */
   GP2X_CTRL_RIGHT      , /*  KBD_RIGHT      */
   GP2X_CTRL_DOWN       , /*  KBD_DOWN       */
   GP2X_CTRL_LEFT       , /*  KBD_LEFT       */
   GP2X_CTRL_TRIANGLE   , /*  KBD_TRIANGLE   */
   GP2X_CTRL_CIRCLE     , /*  KBD_CIRCLE     */
   GP2X_CTRL_CROSS      , /*  KBD_CROSS      */
   GP2X_CTRL_SQUARE     , /*  KBD_SQUARE     */
   GP2X_CTRL_SELECT     , /*  KBD_SELECT     */
   GP2X_CTRL_START      , /*  KBD_START      */
   GP2X_CTRL_LTRIGGER   , /*  KBD_LTRIGGER   */
   GP2X_CTRL_RTRIGGER   , /*  KBD_RTRIGGER   */
   GP2X_CTRL_FIRE,        /*  KBD_FIRE       */
 };

 static char loc_button_name[ KBD_ALL_BUTTONS ][20] =
 {
   "UP",
   "RIGHT",
   "DOWN",
   "LEFT",
   "Y",      // Triangle
   "B",      // Circle
   "X",      // Cross
   "A",      // Square
   "SELECT",
   "START",
   "LTRIGGER",
   "RTRIGGER",
   "JOY_FIRE",
   "JOY_UP",
   "JOY_RIGHT",
   "JOY_DOWN",
   "JOY_LEFT"
 };

 static char loc_button_name_L[ KBD_ALL_BUTTONS ][20] =
 {
   "L_UP",
   "L_RIGHT",
   "L_DOWN",
   "L_LEFT",
   "L_Y",      // Triangle
   "L_B",      // Circle
   "L_X",      // Cross
   "L_A",      // Square
   "L_SELECT",
   "L_START",
   "L_LTRIGGER",
   "L_RTRIGGER",
   "L_JOY_FIRE",
   "L_JOY_UP",
   "L_JOY_RIGHT",
   "L_JOY_DOWN",
   "L_JOY_LEFT"
 };
 
  static char loc_button_name_R[ KBD_ALL_BUTTONS ][20] =
 {
   "R_UP",
   "R_RIGHT",
   "R_DOWN",
   "R_LEFT",
   "R_Y",      // Triangle
   "R_B",      // Circle
   "R_X",      // Cross
   "R_A",      // Square
   "R_SELECT",
   "R_START",
   "R_LTRIGGER",
   "R_RTRIGGER",
   "R_JOY_FIRE",
   "R_JOY_UP",
   "R_JOY_RIGHT",
   "R_JOY_DOWN",
   "R_JOY_LEFT"
 };
 
  struct dragon_key_trans psp_dragon_key_info[DRAGONK_MAX_KEY]=
  {
    // DRAGONK            NAME 
    { DRAGONK_0,           "0"          },
    { DRAGONK_1,           "1"          },
    { DRAGONK_2,           "2"          },
    { DRAGONK_3,           "3"          },
    { DRAGONK_4,           "4"          },
    { DRAGONK_5,           "5"          },
    { DRAGONK_6,           "6"          },
    { DRAGONK_7,           "7"          },
    { DRAGONK_8,           "8"          },
    { DRAGONK_9,           "9"          },
    { DRAGONK_A,           "A"          },
    { DRAGONK_B,           "B"          },
    { DRAGONK_C,           "C"          },
    { DRAGONK_D,           "D"          },
    { DRAGONK_E,           "E"          },
    { DRAGONK_F,           "F"          },
    { DRAGONK_G,           "G"          },
    { DRAGONK_H,           "H"          },
    { DRAGONK_I,           "I"          },
    { DRAGONK_J,           "J"          },
    { DRAGONK_K,           "K"          },
    { DRAGONK_L,           "L"          },
    { DRAGONK_M,           "M"          },
    { DRAGONK_N,           "N"          },
    { DRAGONK_O,           "O"          },
    { DRAGONK_P,           "P"          },
    { DRAGONK_Q,           "Q"          },
    { DRAGONK_R,           "R"          },
    { DRAGONK_S,           "S"          },
    { DRAGONK_T,           "T"          },
    { DRAGONK_U,           "U"          },
    { DRAGONK_V,           "V"          },
    { DRAGONK_W,           "W"          },
    { DRAGONK_X,           "X"          },
    { DRAGONK_Y,           "Y"          },
    { DRAGONK_Z,           "Z"          },
    { DRAGONK_a,           "a"          },
    { DRAGONK_b,           "b"          },
    { DRAGONK_c,           "c"          },
    { DRAGONK_d,           "d"          },
    { DRAGONK_e,           "e"          },
    { DRAGONK_f,           "f"          },
    { DRAGONK_g,           "g"          },
    { DRAGONK_h,           "h"          },
    { DRAGONK_i,           "i"          },
    { DRAGONK_j,           "j"          },
    { DRAGONK_k,           "k"          },
    { DRAGONK_l,           "l"          },
    { DRAGONK_m,           "m"          },
    { DRAGONK_n,           "n"          },
    { DRAGONK_o,           "o"          },
    { DRAGONK_p,           "p"          },
    { DRAGONK_q,           "q"          },
    { DRAGONK_r,           "r"          },
    { DRAGONK_s,           "s"          },
    { DRAGONK_t,           "t"          },
    { DRAGONK_u,           "u"          },
    { DRAGONK_v,           "v"          },
    { DRAGONK_w,           "w"          },
    { DRAGONK_x,           "x"          },
    { DRAGONK_y,           "y"          },
    { DRAGONK_z,           "z"          },
    { DRAGONK_QUOTE,       "\'"         },
    { DRAGONK_COMMA,       ","          },
    { DRAGONK_LESS,        "<"          },
    { DRAGONK_PERIOD,      "."          },
    { DRAGONK_GREATER,     ">"          },
    { DRAGONK_SEMICOLON,   ";"          },
    { DRAGONK_COLON,       ":"          },
    { DRAGONK_UNDERSCORE,  "_"          },
    { DRAGONK_PIPE,        "|"          },
    { DRAGONK_EQUAL,       "="          },
    { DRAGONK_PLUS,        "+"          },
    { DRAGONK_TILDA    ,   "~"          },
    { DRAGONK_DBLQUOTE,    "\""         },
    { DRAGONK_QUESTION,    "?"          },
    { DRAGONK_SLASH,       "/"          },
    { DRAGONK_MINUS,       "-"          },
    { DRAGONK_LBRACKET,    "["          },
    { DRAGONK_RBRACKET,    "]"          },
    { DRAGONK_LCBRACE,     "{"          },
    { DRAGONK_RCBRACE,     "}"          },
    { DRAGONK_SPACE,       "SPACE"      },
    { DRAGONK_EXCLAMATN,   "!"          },
    { DRAGONK_AT,          "@"          },
    { DRAGONK_HASH,        "#"          },
    { DRAGONK_DOLLAR,      "$"          },
    { DRAGONK_PERCENT,     "%"          },
    { DRAGONK_POWER,       "^"          },
    { DRAGONK_AMPERSAND,   "&"          },
    { DRAGONK_ASTERISK,    "*"          },
    { DRAGONK_LPAREN,      "("          },
    { DRAGONK_RPAREN,      ")"          },
    { DRAGONK_BACKSLASH,   "\\"         },
    { DRAGONK_BACKQUOTE,   "`"          },
    { DRAGONK_TAB,         "TAB"        },
    { DRAGONK_BACKSPACE,   "BACKSPACE"  },
    { DRAGONK_LEFT,        "LEFT"       },
    { DRAGONK_RIGHT,       "RIGHT"      },
    { DRAGONK_UP,          "UP"         },
    { DRAGONK_DOWN,        "DOWN"       },
    { DRAGONK_DELETE,      "DELETE"     },
    { DRAGONK_RETURN,      "RETURN"     },
    { DRAGONK_SHIFT,       "SHIFT"      },
    { DRAGONK_JOY_UP,      "JOY_UP"     },
    { DRAGONK_JOY_DOWN,    "JOY_DOWN"   },
    { DRAGONK_JOY_LEFT,    "JOY_LEFT"   },
    { DRAGONK_JOY_RIGHT,   "JOY_RIGHT"  },
    { DRAGONK_JOY_FIRE,    "JOY_FIRE"   },
    { CMD_FPS,             "CMD_FPS"    },
    { CMD_JOY,             "CMD_JOY"    },
    { CMD_RENDER,          "CMD_RENDER" },
    { CMD_LOAD,            "CMD_LOAD"   },
    { CMD_SAVE,            "CMD_SAVE"   },
    { CMD_RESET,           "CMD_RESET"  },
    { CMD_AUTOFIRE,        "CMD_AUTOFIRE" },
    { CMD_INCFIRE,         "CMD_INCFIRE" },
    { CMD_DECFIRE,         "CMD_DECFIRE" },
    { CMD_SCREEN,          "CMD_SCREEN" }
  };

  static int loc_default_mapping[ KBD_ALL_BUTTONS ] = {
    DRAGONK_UP            , /*  KBD_UP         */
    DRAGONK_RIGHT         , /*  KBD_RIGHT      */
    DRAGONK_DOWN          , /*  KBD_DOWN       */
    DRAGONK_LEFT          , /*  KBD_LEFT       */
    DRAGONK_RETURN        , /*  KBD_TRIANGLE   */
    DRAGONK_1             , /*  KBD_CIRCLE     */
    DRAGONK_JOY_FIRE      , /*  KBD_CROSS      */
    DRAGONK_2             , /*  KBD_SQUARE     */
    -1                    , /*  KBD_SELECT     */
    -1                    , /*  KBD_START      */
    KBD_LTRIGGER_MAPPING  , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING  , /*  KBD_RTRIGGER   */
    DRAGONK_JOY_FIRE      , /*  KBD_JOY_FIRE   */
    DRAGONK_JOY_UP        , /*  KBD_JOY_UP     */
    DRAGONK_JOY_RIGHT     , /*  KBD_JOY_RIGHT  */
    DRAGONK_JOY_DOWN      , /*  KBD_JOY_DOWN   */
    DRAGONK_JOY_LEFT        /*  KBD_JOY_LEFT   */
  };

  static int loc_default_mapping_L[ KBD_ALL_BUTTONS ] = {
    DRAGONK_UP            , /*  KBD_UP         */
    DRAGONK_RIGHT         , /*  KBD_RIGHT      */
    DRAGONK_DOWN          , /*  KBD_DOWN       */
    DRAGONK_LEFT          , /*  KBD_LEFT       */
    CMD_LOAD              , /*  KBD_TRIANGLE   */
    CMD_RENDER            , /*  KBD_CIRCLE     */
    CMD_SAVE              , /*  KBD_CROSS      */
    CMD_FPS               , /*  KBD_SQUARE     */
    -1                    , /*  KBD_SELECT     */
    -1                    , /*  KBD_START      */
    KBD_LTRIGGER_MAPPING  , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING  , /*  KBD_RTRIGGER   */
    DRAGONK_JOY_FIRE      , /*  KBD_JOY_FIRE   */
    DRAGONK_JOY_UP        , /*  KBD_JOY_UP     */
    DRAGONK_JOY_RIGHT     , /*  KBD_JOY_RIGHT  */
    DRAGONK_JOY_DOWN      , /*  KBD_JOY_DOWN   */
    DRAGONK_JOY_LEFT        /*  KBD_JOY_LEFT   */
  };

  static int loc_default_mapping_R[ KBD_ALL_BUTTONS ] = {
    DRAGONK_UP            , /*  KBD_UP         */
    CMD_INCFIRE           , /*  KBD_RIGHT      */
    DRAGONK_DOWN          , /*  KBD_DOWN       */
    CMD_DECFIRE           , /*  KBD_LEFT       */
    DRAGONK_RETURN        , /*  KBD_TRIANGLE   */
    DRAGONK_4             , /*  KBD_CIRCLE     */
    CMD_AUTOFIRE          , /*  KBD_CROSS      */
    DRAGONK_SPACE         , /*  KBD_SQUARE     */
    -1                    , /*  KBD_SELECT     */
    -1                    , /*  KBD_START      */
    KBD_LTRIGGER_MAPPING  , /*  KBD_LTRIGGER   */
    KBD_RTRIGGER_MAPPING  , /*  KBD_RTRIGGER   */
    DRAGONK_JOY_FIRE      , /*  KBD_JOY_FIRE   */
    DRAGONK_JOY_UP        , /*  KBD_JOY_UP     */
    DRAGONK_JOY_RIGHT     , /*  KBD_JOY_RIGHT  */
    DRAGONK_JOY_DOWN      , /*  KBD_JOY_DOWN   */
    DRAGONK_JOY_LEFT        /*  KBD_JOY_LEFT   */
  };

# define KBD_MAX_ENTRIES   104

  int kbd_layout[KBD_MAX_ENTRIES][2] = {
    /* Key            Ascii */
    { DRAGONK_0,          '0' },
    { DRAGONK_1,          '1' },
    { DRAGONK_2,          '2' },
    { DRAGONK_3,          '3' },
    { DRAGONK_4,          '4' },
    { DRAGONK_5,          '5' },
    { DRAGONK_6,          '6' },
    { DRAGONK_7,          '7' },
    { DRAGONK_8,          '8' },
    { DRAGONK_9,          '9' },
    { DRAGONK_A,          'A' },
    { DRAGONK_B,          'B' },
    { DRAGONK_C,          'C' },
    { DRAGONK_D,          'D' },
    { DRAGONK_E,          'E' },
    { DRAGONK_F,          'F' },
    { DRAGONK_G,          'G' },
    { DRAGONK_H,          'H' },
    { DRAGONK_I,          'I' },
    { DRAGONK_J,          'J' },
    { DRAGONK_K,          'K' },
    { DRAGONK_L,          'L' },
    { DRAGONK_M,          'M' },
    { DRAGONK_N,          'N' },
    { DRAGONK_O,          'O' },
    { DRAGONK_P,          'P' },
    { DRAGONK_Q,          'Q' },
    { DRAGONK_R,          'R' },
    { DRAGONK_S,          'S' },
    { DRAGONK_T,          'T' },
    { DRAGONK_U,          'U' },
    { DRAGONK_V,          'V' },
    { DRAGONK_W,          'W' },
    { DRAGONK_X,          'X' },
    { DRAGONK_Y,          'Y' },
    { DRAGONK_Z,          'Z' },
    { DRAGONK_a,          'a' },
    { DRAGONK_b,          'b' },
    { DRAGONK_c,          'c' },
    { DRAGONK_d,          'd' },
    { DRAGONK_e,          'e' },
    { DRAGONK_f,          'f' },
    { DRAGONK_g,          'g' },
    { DRAGONK_h,          'h' },
    { DRAGONK_i,          'i' },
    { DRAGONK_j,          'j' },
    { DRAGONK_k,          'k' },
    { DRAGONK_l,          'l' },
    { DRAGONK_m,          'm' },
    { DRAGONK_n,          'n' },
    { DRAGONK_o,          'o' },
    { DRAGONK_p,          'p' },
    { DRAGONK_q,          'q' },
    { DRAGONK_r,          'r' },
    { DRAGONK_s,          's' },
    { DRAGONK_t,          't' },
    { DRAGONK_u,          'u' },
    { DRAGONK_v,          'v' },
    { DRAGONK_w,          'w' },
    { DRAGONK_x,          'x' },
    { DRAGONK_y,          'y' },
    { DRAGONK_z,          'z' },
    { DRAGONK_QUOTE,      '\'' },
    { DRAGONK_COMMA,      ',' },
    { DRAGONK_LESS,       '<' },
    { DRAGONK_PERIOD,     '.' },
    { DRAGONK_GREATER,    '>' },
    { DRAGONK_SEMICOLON,  ';' },
    { DRAGONK_COLON,      ':' },
    { DRAGONK_UNDERSCORE, '_'  },
    { DRAGONK_PIPE,       '|' },
    { DRAGONK_EQUAL,      '=' },
    { DRAGONK_PLUS,       '+' },
    { DRAGONK_TILDA    ,  '~' },
    { DRAGONK_DBLQUOTE,   '"' },
    { DRAGONK_QUESTION,   '?' },
    { DRAGONK_SLASH,      '/' },
    { DRAGONK_MINUS,      '-' },
    { DRAGONK_LBRACKET,   '[' },
    { DRAGONK_RBRACKET,   ']' },
    { DRAGONK_LCBRACE,    '{' },
    { DRAGONK_RCBRACE,    '}' },
    { DRAGONK_SPACE,      ' ' },
    { DRAGONK_EXCLAMATN,  '!' },
    { DRAGONK_AT,         '@' },
    { DRAGONK_HASH,       '#' },
    { DRAGONK_DOLLAR,     '$' },
    { DRAGONK_PERCENT,    '%' },
    { DRAGONK_POWER,      '^' },
    { DRAGONK_AMPERSAND,  '&' },
    { DRAGONK_ASTERISK,   '*' },
    { DRAGONK_LPAREN,     '(' },
    { DRAGONK_RPAREN,     ')' },
    { DRAGONK_BACKSLASH,  '\\' },
    { DRAGONK_BACKQUOTE,  '`' },
    { DRAGONK_TAB,        DANZEFF_TAB },
    { DRAGONK_BACKSPACE,  DANZEFF_DEL },
    { DRAGONK_LEFT,       -1  },
    { DRAGONK_RIGHT,      -1  },
    { DRAGONK_UP,         -1  },
    { DRAGONK_DOWN,       -1  },
    { DRAGONK_DELETE,     DANZEFF_SUPPR  },
    { DRAGONK_RETURN,     DANZEFF_ENTER  },
    { DRAGONK_SHIFT,      DANZEFF_SHIFT  }
  };

 int psp_kbd_mapping[ KBD_ALL_BUTTONS ];
 int psp_kbd_mapping_L[ KBD_ALL_BUTTONS ];
 int psp_kbd_mapping_R[ KBD_ALL_BUTTONS ];
 int psp_kbd_presses[ KBD_ALL_BUTTONS ];
 int kbd_ltrigger_mapping_active;
 int kbd_rtrigger_mapping_active;

 static int danzeff_dragon_key     = 0;
 static int danzeff_dragon_pending = 0;
 static int danzeff_mode        = 0;


       char command_keys[ 128 ];
 static int command_mode        = 0;
 static int command_index       = 0;
 static int command_size        = 0;
 static int command_dragon_pending = 0;
 static int command_dragon_key     = 0;
  
int
dragon_key_event(int dragon_idx, int press)
{
  if (dragon_idx < CMD_FPS) {
    if ((dragon_idx >=               0) && 
        (dragon_idx < DRAGONK_MAX_KEY )) {
 
      if (press) {
        dragon_key_press(dragon_idx);
      } else {
        dragon_key_release(dragon_idx);
      }
    }
  } else {
    if (press) {
      dragon_treat_command_key(dragon_idx);
    }
  }
  return 0;
}

int 
dragon_kbd_reset()
{
  dragon_reset_keyboard();
  return 0;
}

int
dragon_get_key_from_ascii(int key_ascii)
{
  int index;
  for (index = 0; index < KBD_MAX_ENTRIES; index++) {
   if (kbd_layout[index][1] == key_ascii) return kbd_layout[index][0];
  }
  return -1;
}

void
psp_kbd_run_command(char *Command)
{
  strncpy(command_keys, Command, 128);
  command_size  = strlen(Command);
  command_index = 0;

  command_dragon_key     = 0;
  command_dragon_pending = 0;
  command_mode        = 1;
}

int
psp_kbd_reset_mapping(void)
{
  memcpy(psp_kbd_mapping  , loc_default_mapping, sizeof(loc_default_mapping));
  memcpy(psp_kbd_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_L));
  memcpy(psp_kbd_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));
  return 0;
}

int
psp_kbd_reset_hotkeys(void)
{
  int index;
  int key_id;
  for (index = 0; index < KBD_ALL_BUTTONS; index++) {
    key_id = loc_default_mapping[index];
    if ((key_id >= CMD_FPS) && (key_id <= CMD_SCREEN)) {
      psp_kbd_mapping[index] = key_id;
    }
    key_id = loc_default_mapping_L[index];
    if ((key_id >= CMD_FPS) && (key_id <= CMD_SCREEN)) {
      psp_kbd_mapping_L[index] = key_id;
    }
    key_id = loc_default_mapping_R[index];
    if ((key_id >= CMD_FPS) && (key_id <= CMD_SCREEN)) {
      psp_kbd_mapping_R[index] = key_id;
    }
  }
  return 0;
}

int
psp_kbd_load_mapping_file(FILE *KbdFile)
{
  char     Buffer[512];
  char    *Scan;
  int      tmp_mapping[KBD_ALL_BUTTONS];
  int      tmp_mapping_L[KBD_ALL_BUTTONS];
  int      tmp_mapping_R[KBD_ALL_BUTTONS];
  int      dragon_key_id = 0;
  int      kbd_id = 0;

  memcpy(tmp_mapping  , loc_default_mapping  , sizeof(loc_default_mapping));
  memcpy(tmp_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_R));
  memcpy(tmp_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));

  while (fgets(Buffer,512,KbdFile) != (char *)0) {
      
      Scan = strchr(Buffer,'\n');
      if (Scan) *Scan = '\0';
      /* For this #@$% of windows ! */
      Scan = strchr(Buffer,'\r');
      if (Scan) *Scan = '\0';
      if (Buffer[0] == '#') continue;

      Scan = strchr(Buffer,'=');
      if (! Scan) continue;
    
      *Scan = '\0';
      dragon_key_id = atoi(Scan + 1);

      for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++) {
        if (!strcasecmp(Buffer,loc_button_name[kbd_id])) {
          tmp_mapping[kbd_id] = dragon_key_id;
          //break;
        }
      }
      for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++) {
        if (!strcasecmp(Buffer,loc_button_name_L[kbd_id])) {
          tmp_mapping_L[kbd_id] = dragon_key_id;
          //break;
        }
      }
      for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++) {
        if (!strcasecmp(Buffer,loc_button_name_R[kbd_id])) {
          tmp_mapping_R[kbd_id] = dragon_key_id;
          //break;
        }
      }
  }

  memcpy(psp_kbd_mapping, tmp_mapping, sizeof(psp_kbd_mapping));
  memcpy(psp_kbd_mapping_L, tmp_mapping_L, sizeof(psp_kbd_mapping_L));
  memcpy(psp_kbd_mapping_R, tmp_mapping_R, sizeof(psp_kbd_mapping_R));
  
  return 0;
}

int
psp_kbd_load_mapping(char *kbd_filename)
{
  FILE    *KbdFile;
  int      error = 0;

  KbdFile = fopen(kbd_filename, "r");
  error   = 1;

  if (KbdFile != (FILE*)0) {
    psp_kbd_load_mapping_file(KbdFile);
    error = 0;
    fclose(KbdFile);
  }

  kbd_ltrigger_mapping_active = 0;
  kbd_rtrigger_mapping_active = 0;
    
  return error;
}

int
psp_kbd_save_mapping(char *kbd_filename)
{
  FILE    *KbdFile;
  int      kbd_id = 0;
  int      error = 0;

  KbdFile = fopen(kbd_filename, "w");
  error   = 1;

  if (KbdFile != (FILE*)0) {

    for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++)
    {
      fprintf(KbdFile, "%s=%d\n", loc_button_name[kbd_id], psp_kbd_mapping[kbd_id]);
    }
    for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++)
    {
      fprintf(KbdFile, "%s=%d\n", loc_button_name_L[kbd_id], psp_kbd_mapping_L[kbd_id]);
    }
    for (kbd_id = 0; kbd_id < KBD_ALL_BUTTONS; kbd_id++)
    {
      fprintf(KbdFile, "%s=%d\n", loc_button_name_R[kbd_id], psp_kbd_mapping_R[kbd_id]);
    }
    error = 0;
    fclose(KbdFile);
  }

  return error;
}

int
psp_kbd_enter_command()
{
  gp2xCtrlData  c;

  unsigned int command_key = 0;
  int          dragon_key     = 0;
  int          key_event   = 0;

  gp2xCtrlPeekBufferPositive(&c, 1);

  if (command_dragon_pending) 
  {
    if ((c.TimeStamp - loc_last_event_time) > KBD_MIN_COMMAND_TIME) {
      loc_last_event_time = c.TimeStamp;
      command_dragon_pending = 0;
      dragon_key_event(command_dragon_key, 0);
    }

    return 0;
  }

  if ((c.TimeStamp - loc_last_event_time) > KBD_MIN_COMMAND_TIME) {
    loc_last_event_time = c.TimeStamp;

    if (command_index >= command_size) {

      command_mode  = 0;
      command_index = 0;
      command_size  = 0;

      command_dragon_pending = 0;
      command_dragon_key     = 0;

      return 0;
    }
  
    command_key = command_keys[command_index++];
    dragon_key = dragon_get_key_from_ascii(command_key);

    if (dragon_key != -1) {
      command_dragon_key     = dragon_key;
      command_dragon_pending = 1;
      dragon_key_event(command_dragon_key, 1);
    }

    return 1;
  }

  return 0;
}

int 
psp_kbd_is_danzeff_mode()
{
  return danzeff_mode;
}

int
psp_kbd_enter_danzeff()
{
  unsigned int danzeff_key = 0;
  int          dragon_key     = 0;
  int          key_event   = 0;
  gp2xCtrlData  c;

  if (! danzeff_mode) {
    psp_init_keyboard();
    danzeff_mode = 1;
  }

  gp2xCtrlPeekBufferPositive(&c, 1);

  if (danzeff_dragon_pending) 
  {
    if ((c.TimeStamp - loc_last_event_time) > KBD_MIN_PENDING_TIME) {
      loc_last_event_time = c.TimeStamp;
      danzeff_dragon_pending = 0;
      dragon_key_event(danzeff_dragon_key, 0);
    }

    return 0;
  }

  if ((c.TimeStamp - loc_last_event_time) > KBD_MIN_DANZEFF_TIME) {
    loc_last_event_time = c.TimeStamp;
  
    gp2xCtrlPeekBufferPositive(&c, 1);
    danzeff_key = danzeff_readInput(c);
  }

  if (danzeff_key > DANZEFF_START) {
    dragon_key = dragon_get_key_from_ascii(danzeff_key);

    if (dragon_key != -1) {
      danzeff_dragon_key     = dragon_key;
      danzeff_dragon_pending = 1;
      dragon_key_event(danzeff_dragon_key, 1);
    }

    return 1;

  } else if (danzeff_key == DANZEFF_START) {
    danzeff_mode       = 0;
    danzeff_dragon_pending = 0;
    danzeff_dragon_key     = 0;

    psp_kbd_wait_no_button();

  } else if (danzeff_key == DANZEFF_SELECT) {
    danzeff_mode       = 0;
    danzeff_dragon_pending = 0;
    danzeff_dragon_key     = 0;
    psp_main_menu();
    psp_init_keyboard();

    psp_kbd_wait_no_button();
  }

  return 0;
}

void
psp_kbd_display_active_mapping()
{
  if (kbd_ltrigger_mapping_active) {
    psp_sdl_fill_rectangle(0, 0, 10, 3, psp_sdl_rgb(0x0, 0x0, 0xff), 0);
  } else {
    psp_sdl_fill_rectangle(0, 0, 10, 3, 0x0, 0);
  }

  if (kbd_rtrigger_mapping_active) {
    psp_sdl_fill_rectangle(309, 0, 10, 3, psp_sdl_rgb(0x0, 0x0, 0xff), 0);
  } else {
    psp_sdl_fill_rectangle(309, 0, 10, 3, 0x0, 0);
  }
}

int
dragon_decode_key(int psp_b, int button_pressed)
{
  int wake = 0;
  int reverse_analog = ! DRAGON.psp_reverse_analog;

  if (reverse_analog) {
    if ((psp_b >= KBD_JOY_UP  ) &&
        (psp_b <= KBD_JOY_LEFT)) {
      psp_b = psp_b - KBD_JOY_UP + KBD_UP;
    } else
    if ((psp_b >= KBD_UP  ) &&
        (psp_b <= KBD_LEFT)) {
      psp_b = psp_b - KBD_UP + KBD_JOY_UP;
    }
  }

  if (psp_b == KBD_START) {
     if (button_pressed) psp_kbd_enter_danzeff();
  } else
  if (psp_b == KBD_SELECT) {
    if (button_pressed) {
      psp_main_menu();
      psp_init_keyboard();
    }
  } else {
#ifndef MIYOO_MODE
  if (psp_b == KBD_FIRE) {
    kbd_home_button_released = !button_pressed;
  } else {
#endif
 
    if (psp_kbd_mapping[psp_b] >= 0) {
      wake = 1;
      if (button_pressed) {
        // Determine which buton to press first (ie which mapping is currently active)
        if (kbd_ltrigger_mapping_active) {
          // Use ltrigger mapping
          psp_kbd_presses[psp_b] = psp_kbd_mapping_L[psp_b];
          dragon_key_event(psp_kbd_presses[psp_b], button_pressed);
        } else
        if (kbd_rtrigger_mapping_active) {
          // Use rtrigger mapping
          psp_kbd_presses[psp_b] = psp_kbd_mapping_R[psp_b];
          dragon_key_event(psp_kbd_presses[psp_b], button_pressed);
        } else {
          // Use standard mapping
          psp_kbd_presses[psp_b] = psp_kbd_mapping[psp_b];
          dragon_key_event(psp_kbd_presses[psp_b], button_pressed);
        }
      } else {
          // Determine which button to release (ie what was pressed before)
          dragon_key_event(psp_kbd_presses[psp_b], button_pressed);
      }

    } else {
      if (psp_kbd_mapping[psp_b] == KBD_LTRIGGER_MAPPING) {
        kbd_ltrigger_mapping_active = button_pressed;
        kbd_rtrigger_mapping_active = 0;
      } else
      if (psp_kbd_mapping[psp_b] == KBD_RTRIGGER_MAPPING) {
        kbd_rtrigger_mapping_active = button_pressed;
        kbd_ltrigger_mapping_active = 0;
      }
    }
#ifndef MIYOO_MODE
  }
#endif
  }
  return 0;
}

int
dragon_decode_joystick(int psp_b)
{
  int reverse_analog = DRAGON.psp_reverse_analog;

  if (reverse_analog) {
    if ((psp_b >= KBD_JOY_UP  ) &&
        (psp_b <= KBD_JOY_LEFT)) {
      psp_b = psp_b - KBD_JOY_UP + KBD_UP;
    } else
    if ((psp_b >= KBD_UP  ) &&
        (psp_b <= KBD_LEFT)) {
      psp_b = psp_b - KBD_UP + KBD_JOY_UP;
    }
  }

  if (psp_kbd_mapping[psp_b] != -1) {
    dragon_joystick_event(psp_kbd_mapping[psp_b]);
  }
  return 0;
}


void
kbd_change_auto_fire(int auto_fire)
{
  DRAGON.dragon_auto_fire = auto_fire;
  if (DRAGON.dragon_auto_fire_pressed) {
    dragon_key_event(DRAGONK_JOY_FIRE, 0);
    DRAGON.dragon_auto_fire_pressed = 0;
  }
}


static int 
kbd_reset_button_status(void)
{
  int b = 0;
  /* Reset Button status */
  for (b = 0; b < KBD_MAX_BUTTONS; b++) {
    loc_button_press[b]   = 0;
    loc_button_release[b] = 0;
  }
  psp_init_keyboard();
  return 0;
}

int
kbd_scan_keyboard(void)
{
  gp2xCtrlData c;
  long        delta_stamp;
  int         event;
  int         b;

  int new_Lx;
  int new_Ly;
  int old_Lx;
  int old_Ly;

  event = 0;
  gp2xCtrlPeekBufferPositive( &c, 1 );

  if ((c.Buttons & (GP2X_CTRL_LTRIGGER|GP2X_CTRL_RTRIGGER|GP2X_CTRL_START)) ==
      (GP2X_CTRL_LTRIGGER|GP2X_CTRL_RTRIGGER|GP2X_CTRL_START)) {
    /* Exit ! */
    psp_sdl_exit(0);
  }

  if (DRAGON.dragon_auto_fire) {
    delta_stamp = c.TimeStamp - first_time_auto_stamp;
    if ((delta_stamp < 0) || 
        (delta_stamp > (KBD_MIN_AUTOFIRE_TIME / (1 + DRAGON.dragon_auto_fire_period)))) {
      first_time_auto_stamp = c.TimeStamp;
      dragon_key_event(DRAGONK_JOY_FIRE, DRAGON.dragon_auto_fire_pressed);
      DRAGON.dragon_auto_fire_pressed = ! DRAGON.dragon_auto_fire_pressed;
    }
  }

  for (b = 0; b < KBD_MAX_BUTTONS; b++) 
  {
    if (c.Buttons & loc_button_mask[b]) {

      dragon_decode_joystick(b);

      if (!(loc_button_data.Buttons & loc_button_mask[b])) {
        loc_button_press[b] = 1;
        event = 1;
      }
    } else {
      if (loc_button_data.Buttons & loc_button_mask[b]) {
        loc_button_release[b] = 1;
        loc_button_press[b] = 0;
        event = 1;
      }
    }
  }
  memcpy(&loc_button_data,&c,sizeof(gp2xCtrlData));

  return event;
}

void
psp_kbd_wait_start(void)
{
  while (1)
  {
    gp2xCtrlData c;
    gp2xCtrlReadBufferPositive(&c, 1);
    if (c.Buttons & GP2X_CTRL_START) break;
  }
  psp_kbd_wait_no_button();
}

void
psp_init_keyboard(void)
{
  dragon_kbd_reset();
  kbd_ltrigger_mapping_active = 0;
  kbd_rtrigger_mapping_active = 0;
}

void
psp_kbd_wait_no_button(void)
{
  gp2xCtrlData c;

  do {
   gp2xCtrlPeekBufferPositive(&c, 1);
  } while (c.Buttons != 0);
} 

void
psp_kbd_wait_button(void)
{
  gp2xCtrlData c;

  do {
   gp2xCtrlReadBufferPositive(&c, 1);
  } while (c.Buttons == 0);
} 

int
psp_update_keys(void)
{
  int         b;

  static char first_time = 1;
  static int release_pending = 0;

  if (first_time) {

    memcpy(psp_kbd_mapping, loc_default_mapping, sizeof(loc_default_mapping));
    memcpy(psp_kbd_mapping_L, loc_default_mapping_L, sizeof(loc_default_mapping_L));
    memcpy(psp_kbd_mapping_R, loc_default_mapping_R, sizeof(loc_default_mapping_R));

    gp2xCtrlData c;
    gp2xCtrlPeekBufferPositive(&c, 1);

    if (first_time_stamp == -1) first_time_stamp = c.TimeStamp;
    if ((! c.Buttons) && ((c.TimeStamp - first_time_stamp) < KBD_MIN_START_TIME)) return 0;

    dragon_kbd_load();

    first_time      = 0;
    release_pending = 0;

    for (b = 0; b < KBD_MAX_BUTTONS; b++) {
      loc_button_release[b] = 0;
      loc_button_press[b] = 0;
    }
    gp2xCtrlPeekBufferPositive(&loc_button_data, 1);

    psp_main_menu();
    psp_init_keyboard();

    return 0;
  }

  if (command_mode) {
    return psp_kbd_enter_command();
  }

  if (danzeff_mode) {
    return psp_kbd_enter_danzeff();
  }

  if (release_pending)
  {
    release_pending = 0;
    for (b = 0; b < KBD_MAX_BUTTONS; b++) {
      if (loc_button_release[b]) {
        loc_button_release[b] = 0;
        loc_button_press[b] = 0;
        dragon_decode_key(b, 0);
      }
    }
  }

  kbd_scan_keyboard();

  /* check press event */
  for (b = 0; b < KBD_MAX_BUTTONS; b++) {
    if (loc_button_press[b]) {
      loc_button_press[b] = 0;
      release_pending     = 0;
      dragon_decode_key(b, 1);
    }
  }
  /* check release event */
  for (b = 0; b < KBD_MAX_BUTTONS; b++) {
    if (loc_button_release[b]) {
      release_pending = 1;
      break;
    } 
  }

  return 0;
}
