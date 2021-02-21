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
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

#include <zlib.h>
#include <dirent.h>
#include "global.h"
#include "psp_dragon.h"
#include "psp_fmgr.h"
#include "psp_sdl.h"
#include "core/keyboard_psp.h"

//LUDO:
  DRAGON_t DRAGON;

void
dragon_set_video_artifact(int mode)
{
  DRAGON.video_artifact_mode = mode;
  if (DRAGON.emu_started) {
    vdg_set_mode();
  }
}

void
dragon_default_settings()
{
  DRAGON.dragon_snd_enable    = 1;
  DRAGON.psp_reverse_analog   = 0;
  DRAGON.dragon_render_mode   = DRAGON_RENDER_FIT;
  DRAGON.dragon_model         = 3;
  DRAGON.dragon_joystick      = 0;
  DRAGON.dragon_joystick_step = 3;
  DRAGON.dragon_joystick_anal = 1;
  DRAGON.psp_cpu_clock        = 220;
  DRAGON.psp_display_lr       = 0;
  DRAGON.dragon_speed_limiter = 50;
  DRAGON.dragon_auto_fire = 0;
  dragon_set_video_artifact(2);
  DRAGON.dragon_auto_fire_period = 6;
  DRAGON.dragon_auto_fire_pressed = 0;
  DRAGON.dragon_view_fps      = 0;
  DRAGON.psp_screenshot_id    = 0;

  gp2xPowerSetClockFrequency(DRAGON.psp_cpu_clock);
}

int
loc_dragon_save_settings(char *chFileName)
{
  FILE* FileDesc;
  int   error = 0;

  FileDesc = fopen(chFileName, "w");
  if (FileDesc != (FILE *)0 ) {

    fprintf(FileDesc, "psp_cpu_clock=%d\n"         , DRAGON.psp_cpu_clock);
    fprintf(FileDesc, "psp_reverse_analog=%d\n"    , DRAGON.psp_reverse_analog);
    fprintf(FileDesc, "psp_skip_max_frame=%d\n"    , DRAGON.psp_skip_max_frame);
    fprintf(FileDesc, "psp_display_lr=%d\n"        , DRAGON.psp_display_lr);
    fprintf(FileDesc, "dragon_snd_enable=%d\n"     , DRAGON.dragon_snd_enable);
    fprintf(FileDesc, "dragon_render_mode=%d\n"    , DRAGON.dragon_render_mode);
    fprintf(FileDesc, "dragon_model=%d\n"          , DRAGON.dragon_model);
    fprintf(FileDesc, "dragon_joystick=%d\n"       , DRAGON.dragon_joystick);
    fprintf(FileDesc, "dragon_joystick_step=%d\n"  , DRAGON.dragon_joystick_step);
    fprintf(FileDesc, "dragon_joystick_anal=%d\n"  , DRAGON.dragon_joystick_anal);
    fprintf(FileDesc, "dragon_speed_limiter=%d\n"  , DRAGON.dragon_speed_limiter);
    fprintf(FileDesc, "dragon_auto_fire_period=%d\n", DRAGON.dragon_auto_fire_period);
    fprintf(FileDesc, "video_artifact_mode=%d\n"    , DRAGON.video_artifact_mode);
    fprintf(FileDesc, "dragon_view_fps=%d\n"       , DRAGON.dragon_view_fps);

    fclose(FileDesc);

  } else {
    error = 1;
  }

  return error;
}

int
dragon_save_settings(void)
{
  char  FileName[MAX_PATH+1];
  int   error;

  error = 1;

  snprintf(FileName, MAX_PATH, "%s/set/%s.set", DRAGON.dragon_home_dir, DRAGON.dragon_save_name);
  error = loc_dragon_save_settings(FileName);

  return error;
}

static int
loc_dragon_load_settings(char *chFileName)
{
  char  Buffer[512];
  char *Scan;
  unsigned int Value;
  FILE* FileDesc;

  FileDesc = fopen(chFileName, "r");
  if (FileDesc == (FILE *)0 ) return 0;

  while (fgets(Buffer,512, FileDesc) != (char *)0) {

    Scan = strchr(Buffer,'\n');
    if (Scan) *Scan = '\0';
    /* For this #@$% of windows ! */
    Scan = strchr(Buffer,'\r');
    if (Scan) *Scan = '\0';
    if (Buffer[0] == '#') continue;

    Scan = strchr(Buffer,'=');
    if (! Scan) continue;

    *Scan = '\0';
    Value = atoi(Scan+1);

    if (!strcasecmp(Buffer,"psp_cpu_clock"))      DRAGON.psp_cpu_clock = Value;
    else
    if (!strcasecmp(Buffer,"psp_reverse_analog")) DRAGON.psp_reverse_analog = Value;
    else
    if (!strcasecmp(Buffer,"psp_skip_max_frame")) DRAGON.psp_skip_max_frame = Value;
    else
    if (!strcasecmp(Buffer,"psp_display_lr"))     DRAGON.psp_display_lr = Value;
    else
    if (!strcasecmp(Buffer,"dragon_snd_enable"))  DRAGON.dragon_snd_enable = Value;
    else
    if (!strcasecmp(Buffer,"dragon_render_mode")) DRAGON.dragon_render_mode = Value;
    else
    if (!strcasecmp(Buffer,"dragon_model")) DRAGON.dragon_model = Value;
    else
    if (!strcasecmp(Buffer,"dragon_joystick")) DRAGON.dragon_joystick = Value;
    else
    if (!strcasecmp(Buffer,"dragon_joystick_step")) DRAGON.dragon_joystick_step = Value;
    else
    if (!strcasecmp(Buffer,"dragon_joystick_anal")) DRAGON.dragon_joystick_anal = Value;
    else
    if (!strcasecmp(Buffer,"dragon_speed_limiter"))  DRAGON.dragon_speed_limiter = Value;
    else
    if (!strcasecmp(Buffer,"dragon_auto_fire_period")) DRAGON.dragon_auto_fire_period = Value;
    else
    if (!strcasecmp(Buffer,"video_artifact_mode")) DRAGON.video_artifact_mode = Value;
    else
    if (!strcasecmp(Buffer,"dragon_view_fps"))  DRAGON.dragon_view_fps = Value;
  }

  fclose(FileDesc);

  gp2xPowerSetClockFrequency(DRAGON.psp_cpu_clock);

  return 0;
}

int
dragon_load_settings()
{
  char  FileName[MAX_PATH+1];
  int   error;

  error = 1;

  snprintf(FileName, MAX_PATH, "%s/set/%s.set", DRAGON.dragon_home_dir, DRAGON.dragon_save_name);
  error = loc_dragon_load_settings(FileName);

  return error;
}

int
dragon_load_file_settings(char *FileName)
{
  return loc_dragon_load_settings(FileName);
}

static int 
loc_load_rom(char *TmpName)
{
  int error;
  error = cart_insert(TmpName, 1);
  return error;
}

static int 
loc_load_disk(char *TmpName)
{
  int error;
  vdrive_eject_disk(0);
  error = vdrive_insert_disk(0, vdisk_load( TmpName ));
  return error;
}

static int 
loc_load_tape(char *TmpName)
{
  int error;
  error = tape_open_reading(TmpName);
  return error;
}

void
update_save_name(char *Name)
{
  char        TmpFileName[MAX_PATH];
  struct stat aStat;
  int         index;
  char       *SaveName;
  char       *Scan1;
  char       *Scan2;

  SaveName = strrchr(Name,'/');
  if (SaveName != (char *)0) SaveName++;
  else                       SaveName = Name;

  if (!strncasecmp(SaveName, "sav_", 4)) {
    Scan1 = SaveName + 4;
    Scan2 = strrchr(Scan1, '_');
    if (Scan2 && (Scan2[1] >= '0') && (Scan2[1] <= '5')) {
      strncpy(DRAGON.dragon_save_name, Scan1, MAX_PATH);
      DRAGON.dragon_save_name[Scan2 - Scan1] = '\0';
    } else {
      strncpy(DRAGON.dragon_save_name, SaveName, MAX_PATH);
    }
  } else {
    strncpy(DRAGON.dragon_save_name, SaveName, MAX_PATH);
  }

  if (DRAGON.dragon_save_name[0] == '\0') {
    strcpy(DRAGON.dragon_save_name,"default");
  }

  for (index = 0; index < DRAGON_MAX_SAVE_STATE; index++) {
    DRAGON.dragon_save_state[index].used  = 0;
    memset(&DRAGON.dragon_save_state[index].date, 0, sizeof(time_t));
    DRAGON.dragon_save_state[index].thumb = 0;

    snprintf(TmpFileName, MAX_PATH, "%s/save/sav_%s_%d.sna", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, index);
    if (! stat(TmpFileName, &aStat)) {
      DRAGON.dragon_save_state[index].used = 1;
      DRAGON.dragon_save_state[index].date = aStat.st_mtime;
      snprintf(TmpFileName, MAX_PATH, "%s/save/sav_%s_%d.png", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, index);
      if (! stat(TmpFileName, &aStat)) {
        if (psp_sdl_load_thumb_png(DRAGON.dragon_save_state[index].surface, TmpFileName)) {
          DRAGON.dragon_save_state[index].thumb = 1;
        }
      }
    }
  }
}

typedef struct thumb_list {
  struct thumb_list *next;
  char              *name;
  char              *thumb;
} thumb_list;

static thumb_list* loc_head_thumb = 0;

static void
loc_del_thumb_list()
{
  while (loc_head_thumb != 0) {
    thumb_list *del_elem = loc_head_thumb;
    loc_head_thumb = loc_head_thumb->next;
    if (del_elem->name) free( del_elem->name );
    if (del_elem->thumb) free( del_elem->thumb );
    free(del_elem);
  }
}

static void
loc_add_thumb_list(char* filename)
{
  thumb_list *new_elem;
  char tmp_filename[MAX_PATH];

  strcpy(tmp_filename, filename);
  char* save_name = tmp_filename;

  /* .png extention */
  char* Scan = strrchr(save_name, '.');
  if ((! Scan) || (strcasecmp(Scan, ".png"))) return;
  *Scan = 0;

  if (strncasecmp(save_name, "sav_", 4)) return;
  save_name += 4;

  Scan = strrchr(save_name, '_');
  if (! Scan) return;
  *Scan = 0;

  /* only one png for a give save name */
  new_elem = loc_head_thumb;
  while (new_elem != 0) {
    if (! strcasecmp(new_elem->name, save_name)) return;
    new_elem = new_elem->next;
  }

  new_elem = (thumb_list *)malloc( sizeof( thumb_list ) );
  new_elem->next = loc_head_thumb;
  loc_head_thumb = new_elem;
  new_elem->name  = strdup( save_name );
  new_elem->thumb = strdup( filename );
}

void
load_thumb_list()
{
  char SaveDirName[MAX_PATH];
  DIR* fd = 0;

  loc_del_thumb_list();

  snprintf(SaveDirName, MAX_PATH, "%s/save", DRAGON.dragon_home_dir);

  fd = opendir(SaveDirName);
  if (!fd) return;

  struct dirent *a_dirent;
  while ((a_dirent = readdir(fd)) != 0) {
    if(a_dirent->d_name[0] == '.') continue;
    if (a_dirent->d_type != DT_DIR) 
    {
      loc_add_thumb_list( a_dirent->d_name );
    }
  }
  closedir(fd);
}

int
load_thumb_if_exists(char *Name)
{
  char        FileName[MAX_PATH];
  char        ThumbFileName[MAX_PATH];
  struct stat aStat;
  char       *SaveName;
  char       *Scan;

  strcpy(FileName, Name);
  SaveName = strrchr(FileName,'/');
  if (SaveName != (char *)0) SaveName++;
  else                       SaveName = FileName;

  Scan = strrchr(SaveName,'.');
  if (Scan) *Scan = '\0';

  if (!SaveName[0]) return 0;

  thumb_list *scan_list = loc_head_thumb;
  while (scan_list != 0) {
    if (! strcasecmp( SaveName, scan_list->name)) {
      snprintf(ThumbFileName, MAX_PATH, "%s/save/%s", DRAGON.dragon_home_dir, scan_list->thumb);
      if (! stat(ThumbFileName, &aStat)) 
      {
        if (psp_sdl_load_thumb_png(save_surface, ThumbFileName)) {
          return 1;
        }
      }
    }
    scan_list = scan_list->next;
  }
  return 0;
}




void
reset_save_name()
{
  update_save_name("");
}

void
dragon_reset_computer()
{
  machine_reset(1);
}

void
dragon_kbd_load(void)
{
  char        TmpFileName[MAX_PATH + 1];
  struct stat aStat;

  snprintf(TmpFileName, MAX_PATH, "%s/kbd/%s.kbd", DRAGON.dragon_home_dir, DRAGON.dragon_save_name );
  if (! stat(TmpFileName, &aStat)) {
    psp_kbd_load_mapping(TmpFileName);
  }
}

int
dragon_kbd_save(void)
{
  char TmpFileName[MAX_PATH + 1];
  snprintf(TmpFileName, MAX_PATH, "%s/kbd/%s.kbd", DRAGON.dragon_home_dir, DRAGON.dragon_save_name );
  return( psp_kbd_save_mapping(TmpFileName) );
}


int
dragon_load_rom(char *FileName)
{
  char *scan;
  char  SaveName[MAX_PATH+1];
  int   error;

  strncpy(SaveName,FileName,MAX_PATH);
  scan = strrchr(SaveName,'.');
  if (scan) *scan = '\0';
  update_save_name(SaveName);
  error = loc_load_rom(FileName);

  if (! error ) {
    dragon_reset_computer();
    dragon_kbd_load();
    dragon_load_settings();
  }

  return error;
}

int
dragon_load_tape(char *FileName)
{
  char *scan;
  char  SaveName[MAX_PATH+1];
  int   error;

  error = 1;

  strncpy(SaveName,FileName,MAX_PATH);
  scan = strrchr(SaveName,'.');
  if (scan) *scan = '\0';
  update_save_name(SaveName);
  error = loc_load_tape(FileName);

  if (! error ) {
    dragon_reset_computer();
    dragon_kbd_load();
    dragon_load_settings();
  }

  return error;
}

void
dragon_eject_rom()
{
  cart_remove();
  machine_reset(1);
  reset_save_name();
}

int
dragon_load_disk(char *FileName)
{
  char *scan;
  char  SaveName[MAX_PATH+1];
  int   error;

  error = 1;

  strncpy(SaveName,FileName,MAX_PATH);
  scan = strrchr(SaveName,'.');
  if (scan) *scan = '\0';
  update_save_name(SaveName);
  error = loc_load_disk(FileName);

  if (! error ) {
    dragon_kbd_load();
    dragon_load_settings();
  }

  return error;
}

static int
loc_load_state(char *filename)
{
  int error;
  error = read_snapshot(filename);
  return error;
}

int
dragon_load_state(char *FileName)
{
  char *scan;
  char  SaveName[MAX_PATH+1];
  int   error;

  error = 1;

  strncpy(SaveName,FileName,MAX_PATH);
  scan = strrchr(SaveName,'.');
  if (scan) *scan = '\0';
  update_save_name(SaveName);
  error = read_snapshot(FileName);

  if (! error ) {
    dragon_kbd_load();
    dragon_load_settings();
  }

  return error;
}

int
dragon_snapshot_save_slot(int save_id)
{
  char  FileName[MAX_PATH+1];
  struct stat aStat;
  int   error;

  error = 1;

  if (save_id < DRAGON_MAX_SAVE_STATE) {
    snprintf(FileName, MAX_PATH, "%s/save/sav_%s_%d.sna", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, save_id);
    error = write_snapshot(FileName);
    if (! error) {
      if (! stat(FileName, &aStat)) {
        DRAGON.dragon_save_state[save_id].used  = 1;
        DRAGON.dragon_save_state[save_id].thumb = 0;
        DRAGON.dragon_save_state[save_id].date  = aStat.st_mtime;
        snprintf(FileName, MAX_PATH, "%s/save/sav_%s_%d.png", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, save_id);
        if (psp_sdl_save_thumb_png(DRAGON.dragon_save_state[save_id].surface, FileName)) {
          DRAGON.dragon_save_state[save_id].thumb = 1;
        }
      }
    }
  }

  return error;
}

int
dragon_snapshot_load_slot(int load_id)
{
  char  FileName[MAX_PATH+1];
  int   error;

  error = 1;

  if (load_id < DRAGON_MAX_SAVE_STATE) {
    snprintf(FileName, MAX_PATH, "%s/save/sav_%s_%d.sna", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, load_id);
    error = loc_load_state(FileName);
  }
  return error;
}

int
dragon_snapshot_del_slot(int save_id)
{
  char  FileName[MAX_PATH+1];
  struct stat aStat;
  int   error;

  error = 1;

  if (save_id < DRAGON_MAX_SAVE_STATE) {
    snprintf(FileName, MAX_PATH, "%s/save/sav_%s_%d.sna", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, save_id);
    error = remove(FileName);
    if (! error) {
      DRAGON.dragon_save_state[save_id].used  = 0;
      DRAGON.dragon_save_state[save_id].thumb = 0;
      memset(&DRAGON.dragon_save_state[save_id].date, 0, sizeof(time_t));

      /* We keep always thumbnail with id 0, to have something to display in the file requester */ 
      if (save_id != 0) {
        snprintf(FileName, MAX_PATH, "%s/save/sav_%s_%d.png", DRAGON.dragon_home_dir, DRAGON.dragon_save_name, save_id);
        if (! stat(FileName, &aStat)) {
          remove(FileName);
        }
      }
    }
  }

  return error;
}

void
dragon_audio_pause()
{
  SDL_PauseAudio(1);
}

void
dragon_audio_resume()
{
  SDL_PauseAudio(0);
}

void
dragon_global_init()
{
  memset(&DRAGON, 0, sizeof(DRAGON_t));
  getcwd(DRAGON.dragon_home_dir,MAX_PATH);

  psp_sdl_init();

  dragon_default_settings();
  update_save_name("");
  dragon_load_settings();

  gp2xPowerSetClockFrequency(DRAGON.psp_cpu_clock);
}

void
dragon_treat_command_key(int dragon_idx)
{
  int new_render;

  switch (dragon_idx) 
  {
    case CMD_FPS: DRAGON.dragon_view_fps = ! DRAGON.dragon_view_fps;
    break;
    case CMD_JOY: DRAGON.psp_reverse_analog = ! DRAGON.psp_reverse_analog;
    break;
    case CMD_RENDER: 
      psp_sdl_black_screen();
      new_render = DRAGON.dragon_render_mode + 1;
      if (new_render > DRAGON_LAST_RENDER) new_render = 0;
      DRAGON.dragon_render_mode = new_render;
    break;
    case CMD_LOAD: psp_main_menu_load_current();
    break;
    case CMD_SAVE: psp_main_menu_save_current(); 
    break;
    case CMD_RESET: 
       psp_sdl_black_screen();
       dragon_reset_computer(0);
       reset_save_name();
    break;
    case CMD_AUTOFIRE: 
       kbd_change_auto_fire(! DRAGON.dragon_auto_fire);
    break;
    case CMD_DECFIRE: 
      if (DRAGON.dragon_auto_fire_period > 0) DRAGON.dragon_auto_fire_period--;
    break;
    case CMD_INCFIRE: 
      if (DRAGON.dragon_auto_fire_period < 19) DRAGON.dragon_auto_fire_period++;
    break;
    case CMD_SCREEN: psp_screenshot_mode = 10;
    break;
  }
}

