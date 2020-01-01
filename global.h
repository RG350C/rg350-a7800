#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#include "gp2x_psp.h"
#include <time.h>

# ifndef CLK_TCK
# define CLK_TCK  CLOCKS_PER_SEC
# endif

#define CPU_CLOCK_IDLE 60
#define CPU_CLOCK_STD 200

# define ATARI_RENDER_NORMAL     0
# define ATARI_LAST_RENDER       0

# define ATARI_WIDTH  320
# define ATARI_HEIGHT 223

# define SNAP_WIDTH   (ATARI_WIDTH*10/25)
# define SNAP_HEIGHT  (ATARI_HEIGHT*10/25)

# define MAX_PATH   256
# define ATARI_MAX_SAVE_STATE 5

#include <SDL/SDL.h>

  typedef struct Atari_save_t {

    SDL_Surface    *surface;
    char            used;
    char            thumb;
    time_t          date;

  } Atari_save_t;

  typedef struct ATARI_t {
 
    unsigned char keyboard_data[19];
    Atari_save_t atari_save_state[ATARI_MAX_SAVE_STATE];
    char atari_save_name[MAX_PATH];
    char atari_home_dir[MAX_PATH];
    int  psp_screenshot_id;
    int  psp_cpu_clock;
    int  psp_joystick_cursor;
    int  psp_display_lr;
    int  atari_view_fps;
    int  atari_current_fps;
    int  atari_snd_enable;
    int  atari_render_mode;
    int  psp_active_joystick;
    int  psp_skip_max_frame;
    int  psp_skip_cur_frame;
    int  atari_speed_limiter;
    int  atari_auto_fire;
    int  atari_auto_fire_pressed;
    int  atari_auto_fire_period;

  } ATARI_t;

  extern ATARI_t ATARI;


//END_LUDO:
  extern void atari_default_settings(void);
  extern int atari_save_settings(void);
  extern int atari_load_settings();
  extern int atari_load_file_settings(char *FileName);

  extern void atari_update_save_name(char *Name);
  extern void reset_save_name();
  extern void atari_kbd_load(void);
  extern int atari_kbd_save(void);
  extern void emulator_reset(void);
  extern int atari_load_rom(char *FileName, int zip_format);
  extern int atari_load_state(char *FileName);
  extern int atari_snapshot_save_slot(int save_id);
  extern int atari_snapshot_load_slot(int load_id);
  extern int atari_snapshot_del_slot(int save_id);

#ifdef __cplusplus
}
#endif

#endif
