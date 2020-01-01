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
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <SDL/SDL.h>

#include "global.h"
#include "psp_sdl.h"
#include "psp_menu.h"

extern SDL_Surface *back_surface;

#include "psp_kbd.h"
#include "psp_sdl.h"
#include "psp_fmgr.h"

# define PSP_FMGR_MIN_TIME         150000

static struct dirent files[PSP_FMGR_MAX_ENTRY];
static struct dirent *sortfiles[PSP_FMGR_MAX_ENTRY];
static int nfiles;
static int user_file_format = 0;

static void 
SJISCopy(struct dirent *a, char *file)
{
  unsigned char ca;
  int i;
  int len = strlen(a->d_name);

  for(i=0;i<=len;i++){
    ca = a->d_name[i];
    if (((0x81 <= ca)&&(ca <= 0x9f))
    || ((0xe0 <= ca)&&(ca <= 0xef))){
      file[i++] = ca;
      file[i] = a->d_name[i];
    }
    else{
      if(ca>='a' && ca<='z') ca-=0x20;
      file[i] = ca;
    }
  }
}

static int 
cmpFile(struct dirent *a, struct dirent *b)
{
    char file1[0x108];
    char file2[0x108];
  char ca, cb;
  int i, n, ret;

  if(a->d_type == b->d_type){
    SJISCopy(a, file1);
    SJISCopy(b, file2);
    n=strlen(file1);
    for(i=0; i<=n; i++){
      ca=file1[i]; cb=file2[i];
      ret = ca-cb;
      if(ret!=0) return ret;
    }
    return 0;
  }

  if(a->d_type == DT_DIR)  return -1;
  else                     return 1;
}

static void 
sort(struct dirent **a, int left, int right) 
{
  struct dirent *tmp, *pivot;
  int i, p;

  if (left < right) {
    pivot = a[left];
    p = left;
    for (i=left+1; i<=right; i++) {
      if (cmpFile(a[i],pivot)<0){
        p=p+1;
        tmp=a[p];
        a[p]=a[i];
        a[i]=tmp;
      }
    }
    a[left] = a[p];
    a[p] = pivot;
    sort(a, left, p-1);
    sort(a, p+1, right);
  }
}

static void 
my_sort(struct dirent **a, int left, int right) 
{
  struct dirent* swap_elem;
  int length = right - left;
  int index;
  /* LUDO: Better like this ...  
     quicksort is o(n2) when the list is already sorted !!
  */
  for (index = 0; index < length; index++) {
    int perm = rand() % length;
    swap_elem = a[perm];
    a[perm] = a[index];
    a[index] = swap_elem;
  }
  sort(a, left, right);
}

int 
psp_fmgr_getExtId(const char *szFilePath) 
{
  char *pszExt;
  if((pszExt = strrchr(szFilePath, '.'))) {
    if ((!strcasecmp(pszExt, ".rom")) ||
        (!strcasecmp(pszExt, ".a78")) ||
        (!strcasecmp(pszExt, ".bin"))) {
      return FMGR_FORMAT_ROM;
    } else if (!strcasecmp(pszExt, ".sav")) {
      return FMGR_FORMAT_STATE;
    } else if (!strcasecmp(pszExt, ".kbd")) {
      return FMGR_FORMAT_KBD;
    } else if (!strcasecmp(pszExt, ".set")) {
      return FMGR_FORMAT_SET;
    } else if (!strcasecmp(pszExt, ".zip")) {
      return FMGR_FORMAT_ZIP;
    }
  }
  return 0;
}


static void 
getDir(const char *path) 
{
  DIR *fd;

  int b=0;
  int format = 0;

  nfiles = 0;

  if(strcmp(path,"./")){
    strcpy(files[nfiles].d_name,"..");
    files[nfiles].d_type = DT_DIR;
    sortfiles[nfiles] = files + nfiles;
    nfiles++;
    b=1;
  }
  fd = opendir(path);
  if (! fd) return;
  while(nfiles<PSP_FMGR_MAX_ENTRY){
    memset(&files[nfiles], 0x00, sizeof(struct dirent));
    struct dirent *file_entry = readdir(fd);
    if (! file_entry) break;
    memcpy(&files[nfiles], file_entry, sizeof(struct dirent));
    if(files[nfiles].d_name[0] == '.') continue;

    if(files[nfiles].d_type == DT_DIR) {
      strcat(files[nfiles].d_name, "/");
      sortfiles[nfiles] = files + nfiles;
      nfiles++;
      continue;
    }
    sortfiles[nfiles] = files + nfiles;
    format = psp_fmgr_getExtId(files[nfiles].d_name);
    if ( (format == user_file_format            ) || 
         ((format == FMGR_FORMAT_ZIP          ) && 
          (user_file_format != FMGR_FORMAT_KBD) ) ) 
    {
      nfiles++;
    }
  }
  closedir(fd);
  if(b)
    my_sort(sortfiles+1, 0, nfiles-2);
  else
    my_sort(sortfiles, 0, nfiles-1);
}

int 
psp_fmgr_get_dir_list(char *basedir, int dirmax, char **dirname)
{
  int index = 0;
  DIR* fd = 0;
  nfiles = 0;
  fd = opendir(basedir);
  if (! fd) return 0;
  while ((nfiles<PSP_FMGR_MAX_ENTRY) && (nfiles < dirmax)) {
    memset(&files[nfiles], 0x00, sizeof(struct dirent));
    struct dirent *file_entry = readdir(fd);
    if (! file_entry) break;
    memcpy(&files[nfiles], file_entry, sizeof(struct dirent));
    if(files[nfiles].d_name[0] == '.') continue;

    if(files[nfiles].d_type == DT_DIR) {
      strcat(files[nfiles].d_name, "/");
      sortfiles[nfiles] = files + nfiles;
      nfiles++;
      continue;
    }
  }
  closedir(fd);
  my_sort(sortfiles, 0, nfiles-1);
  for (index = 0; index < nfiles; index++) {
    dirname[index] = strdup(sortfiles[index]->d_name);
  }
  return nfiles;
}

static void 
psp_display_screen_fmgr(void)
{
  psp_sdl_blit_background();
}


static int
psp_fmgr_ask_confirm(void)
{
  gp2xCtrlData c;
  int confirm = 0;

  psp_sdl_back2_print(190,  70, "Delete a file", 
                     PSP_MENU_WARNING_COLOR);
  psp_sdl_back2_print(170,  80, "press X to confirm !", 
                     PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();

  psp_kbd_wait_no_button();

  do
  {
    gp2xCtrlReadBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

    if (c.Buttons & GP2X_CTRL_CROSS) { confirm = 1; break; }

  } while (c.Buttons == 0);

  psp_kbd_wait_no_button();

  return confirm;
}

int 
psp_file_request(char *out, char *pszStartPath)
{
static  int sel=0;

  gp2xCtrlData c;
  int  display_screen;
  int  last_time;
  int  tmp;

  long color;
  int top, rows=20, x, y, i, up=0;
  char path[PSP_FMGR_MAX_PATH];
  char oldDir[PSP_FMGR_MAX_NAME];
  char buffer[PSP_FMGR_MAX_NAME];
  char *p;
  long new_pad;
  long old_pad;
  int  file_selected;

  memset(files, 0x00, sizeof(struct dirent) * PSP_FMGR_MAX_ENTRY);
  memset(sortfiles, 0x00, sizeof(struct dirent *) * PSP_FMGR_MAX_ENTRY);
  nfiles = 0;

  strcpy(path, pszStartPath);
  getDir(path);

  display_screen = 1;
  last_time = 0;
  old_pad = 0;
  top = 0;
  file_selected = 0;

  if (sel >= nfiles) sel = 0;

  for(;;) 
  {
    x = 0; y = 15;

    if (display_screen) {
      psp_display_screen_fmgr();
      psp_sdl_back2_rectangle(x, y, 190, rows * 10);
      //display_screen = 0;
    }

    for(i=0; i<rows; i++){
      if(top+i >= nfiles) break;
      if(top+i == sel) color = PSP_MENU_SEL_COLOR;
      else             color = PSP_MENU_TEXT_COLOR;
      strncpy(buffer, sortfiles[top+i]->d_name, 30);
      string_fill_with_space(buffer, 30);
      psp_sdl_back2_print(x, y, buffer, color);
      y += 10;
    }

    psp_sdl_flip();

    gp2xCtrlReadBufferPositive(&c, 1);
    c.Buttons &= PSP_ALL_BUTTON_MASK;

    new_pad = c.Buttons;

    if ((old_pad != new_pad) || ((c.TimeStamp - last_time) > PSP_FMGR_MIN_TIME)) {
      last_time = c.TimeStamp;
      old_pad = new_pad;

    } else continue;

    if ((new_pad & GP2X_CTRL_CROSS) || (new_pad & GP2X_CTRL_CIRCLE)) {
      if (sortfiles[sel]->d_type == DT_DIR) {
        if(!strcmp(sortfiles[sel]->d_name,"..")){
          up=1;
        } else {
          strcat(path,sortfiles[sel]->d_name);
          display_screen = 1;
          getDir(path);
          sel=0;
        }
      }else{
        strcpy(out, path);
        strcat(out, sortfiles[sel]->d_name);
        strcpy(pszStartPath,path);
        file_selected = 1;
        break;
      }
    } else if(new_pad & GP2X_CTRL_TRIANGLE){
      up=1;
    } else if((new_pad & GP2X_CTRL_SQUARE) || (new_pad & GP2X_CTRL_SELECT)) {
      /* Cancel */
      file_selected = 0;
      break;
    } else if(new_pad & GP2X_CTRL_UP){
      sel--;
    } else if(new_pad & GP2X_CTRL_DOWN){
      sel++;
    } else if(new_pad & GP2X_CTRL_LEFT){
      sel-=10;
    } else if(new_pad & GP2X_CTRL_RIGHT){
      sel+=10;
    } else if(new_pad & GP2X_CTRL_RTRIGGER){
      if (sortfiles[sel]->d_type != DT_DIR) {
        display_screen = 1;
        strcpy(out, path);
        strcat(out, sortfiles[sel]->d_name);
        strcpy(pszStartPath,path);
        if (psp_fmgr_ask_confirm()) {
          for (tmp = sel; tmp < (nfiles - 1); tmp++) {
            sortfiles[tmp] = sortfiles[tmp + 1];
          }
          nfiles--;
          remove(out);
        }
      }
    }

    if(up) {
      display_screen = 1;
      if(strcmp(path,"./")){
        p=strrchr(path,'/');
        *p=0;
        p=strrchr(path,'/');
        p++;
        strcpy(oldDir,p);
        strcat(oldDir,"/");
        *p=0;
        getDir(path);
        sel=0;
        for(i=0; i<nfiles; i++) {
          if(!strcmp(oldDir, sortfiles[i]->d_name)) {
            sel=i;
            top=sel-3;
            break;
          }
        }
      }
      up=0;
    }

    if(top > nfiles-rows) top=nfiles-rows;
    if(top < 0)           top=0;
    if(sel >= nfiles)     sel=nfiles-1;
    if(sel < 0)           sel=0;
    if(sel >= top+rows)   top=sel-rows+1;
    if(sel < top)         top=sel;
  }

  return file_selected;
}

static char user_filedir_kbd[PSP_FMGR_MAX_NAME];
static char user_filedir_set[PSP_FMGR_MAX_NAME];
static char user_filedir_rom[PSP_FMGR_MAX_NAME];

int 
psp_fmgr_menu(int format)
{
  static int  first = 1;

  char *user_filedir;
  char user_filename[PSP_FMGR_MAX_NAME];
  struct stat       aStat;
  int               file_format;
  int               error;
  int               ret;

  user_file_format = format;
  ret = 0;

  if (first) {
    first = 0;
    strcpy(user_filedir_kbd, ".");
    strcpy(user_filedir_set, user_filedir_kbd);
    strcpy(user_filedir_rom, user_filedir_kbd);
    strcat(user_filedir_kbd, "/kbd/");
    strcat(user_filedir_set, "/set/");
    strcat(user_filedir_rom, "/roms/");
  }

  if (format == FMGR_FORMAT_KBD) user_filedir = user_filedir_kbd;
  else
  if (format == FMGR_FORMAT_SET) user_filedir = user_filedir_set;
  else                           user_filedir = user_filedir_rom;

  psp_kbd_wait_no_button();

  if (psp_file_request(user_filename, user_filedir)) {
    error = 0;
    if (stat(user_filename, &aStat)) error = 1;
    else 
    {
      file_format = psp_fmgr_getExtId(user_filename);

      if (file_format == FMGR_FORMAT_ZIP) {

        if (user_file_format == FMGR_FORMAT_ROM) /* Rom */ error = atari_load_rom(user_filename, 1);
      }
      else 
      {
        if (file_format == FMGR_FORMAT_STATE) /* State */ error = atari_load_state(user_filename);
        else
        if (file_format == FMGR_FORMAT_ROM) /* Rom */ error = atari_load_rom(user_filename, 0);
        else
        if (file_format == FMGR_FORMAT_KBD) /* Kbd */  error = psp_kbd_load_mapping(user_filename);
        else
        if (file_format == FMGR_FORMAT_SET) /* Settings */  error = atari_load_file_settings(user_filename);
      }
    }

    if (error) ret = -1;
    else       ret =  1;
  }

  psp_kbd_wait_no_button();

  return ret;
}
