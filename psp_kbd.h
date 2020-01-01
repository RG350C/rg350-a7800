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

# ifndef _KBD_H_
# define _KBD_H_

#ifdef __cplusplus
extern "C" {
#endif

# define PSP_ALL_BUTTON_MASK 0xFFFF

 enum atari_keys_emum {

// | 00       | Joystick 1   | Right
// | 01       | Joystick 1   | Left
// | 02       | Joystick 1   | Down
// | 03       | Joystick 1   | Up
// | 04       | Joystick 1   | Button 1
// | 05       | Joystick 1   | Button 2
// | 06       | Joystick 2   | Right
// | 07       | Joystick 2   | Left
// | 08       | Joystick 2   | Down
// | 09       | Joystick 2   | Up
// | 10       | Joystick 2   | Button 1
// | 11       | Joystick 2   | Button 2
// | 12       | Console      | Reset
// | 13       | Console      | Select
// | 14       | Console      | Pause
// | 15       | Console      | Left Difficulty
// | 16       | Console      | Right Difficulty

    ATARI_JOY1_RIGHT = 0,
    ATARI_JOY1_LEFT, 
    ATARI_JOY1_DOWN, 
    ATARI_JOY1_UP,   
    ATARI_JOY1_FIRE1,
    ATARI_JOY1_FIRE2,

    ATARI_JOY2_RIGHT,
    ATARI_JOY2_LEFT, 
    ATARI_JOY2_DOWN, 
    ATARI_JOY2_UP,   
    ATARI_JOY2_FIRE1,
    ATARI_JOY2_FIRE2,

    ATARI_RESET,    
    ATARI_SELECT,   
    ATARI_PAUSE,    
    ATARI_LDIFF,    
    ATARI_RDIFF,    
    ATARIC_FPS,
    ATARIC_JOY,
    ATARIC_RENDER,
    ATARIC_LOAD,
    ATARIC_SAVE,
    ATARIC_RESET,
    ATARIC_AUTOFIRE,
    ATARIC_INCFIRE,
    ATARIC_DECFIRE,
    ATARIC_SCREEN,

    ATARI_MAX_KEY      
  };

# define KBD_UP           0
# define KBD_RIGHT        1
# define KBD_DOWN         2
# define KBD_LEFT         3
# define KBD_TRIANGLE     4
# define KBD_CIRCLE       5
# define KBD_CROSS        6
# define KBD_SQUARE       7
# define KBD_SELECT       8
# define KBD_START        9
# define KBD_LTRIGGER    10
# define KBD_RTRIGGER    11
# define KBD_FIRE        12

# define KBD_MAX_BUTTONS 13

# define KBD_JOY_UP      13
# define KBD_JOY_RIGHT   14
# define KBD_JOY_DOWN    15
# define KBD_JOY_LEFT    16

# define KBD_ALL_BUTTONS 17

# define KBD_UNASSIGNED         -1

# define KBD_LTRIGGER_MAPPING   -2
# define KBD_RTRIGGER_MAPPING   -3
# define KBD_NORMAL_MAPPING     -1

 struct atari_key_trans {
   int  key;
   char name[10];
 };
  

  extern int psp_screenshot_mode;
  extern int psp_kbd_mapping[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_mapping_L[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_mapping_R[ KBD_ALL_BUTTONS ];
  extern int psp_kbd_presses[ KBD_ALL_BUTTONS ];
  extern int kbd_ltrigger_mapping_active;
  extern int kbd_rtrigger_mapping_active;

  extern struct atari_key_trans psp_atari_key_info[ATARI_MAX_KEY];

  extern int  psp_update_keys(void);
  extern void kbd_wait_start(void);
  extern void psp_init_keyboard(void);
  extern void psp_kbd_wait_no_button(void);
  extern int  psp_kbd_is_danzeff_mode(void);
  extern void psp_kbd_display_active_mapping(void);
  extern int psp_kbd_load_mapping(char *kbd_filename);
  extern int psp_kbd_save_mapping(char *kbd_filename);
  extern void psp_kbd_display_active_mapping(void);

#ifdef __cplusplus
}
#endif
# endif
