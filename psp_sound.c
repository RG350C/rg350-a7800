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
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "global.h"
#include "psp_sdl.h"
#include "psp_sound.h"

  static u8 *psp_read_buffer  = 0;
  static u8 *psp_write_buffer = 0;
  static u8 *psp_top_buffer   = 0;

         int psp_buffer_size     = 0;
  static int psp_buffer_max_size = 0;

void
psp_sound_send_buffer(unsigned char *sample, int length)
{
  int dst_index = 0;
  for (dst_index = 0; dst_index < psp_buffer_size; dst_index++) {
    int src_index = (dst_index * length) / psp_buffer_size;
    psp_write_buffer[dst_index] = sample[src_index];
  }
}

static void 
psp_sound_callback(void *udata, u8 *stream, int length)
{
  if (ATARI.atari_snd_enable) {
    long volume = (SDL_MIX_MAXVOLUME * gp2xGetSoundVolume()) / 100;
    SDL_MixAudio(stream, (unsigned char *)psp_read_buffer, length, volume);
  } else {
    memset(stream, 0, length);
  }
}

int
psp_sound_init()
{
 SDL_AudioSpec wanted;
 SDL_AudioSpec obtained;

  memset(&wanted, 0, sizeof(wanted));
  memset(&obtained, 0, sizeof(obtained));

  wanted.freq     = 44100;
  wanted.format   = AUDIO_S8;
  wanted.channels = 1;
  wanted.samples  = 1024;
  wanted.callback = psp_sound_callback;
  wanted.userdata = NULL;

  /* Open the sound device, forcing the desired format */
  if ( SDL_OpenAudio(&wanted, &obtained) < 0 ) {
    fprintf(stderr, "Couldn't open sound: %s\n", SDL_GetError());
    return(0);
  }
  /* WARNING : sample 8 bits */
  psp_buffer_size     = obtained.samples;
  psp_buffer_max_size = 2 * obtained.samples;
  psp_top_buffer = (u8 *)malloc(psp_buffer_max_size);
  memset(psp_top_buffer, 0, psp_buffer_max_size);

# if 0
  psp_write_buffer = psp_top_buffer;
  psp_read_buffer  = psp_top_buffer + psp_buffer_size;
# else
  psp_write_buffer = psp_top_buffer;
  psp_read_buffer  = psp_top_buffer;
# endif

  SDL_Delay(1000);        // Give sound some time to init
  SDL_PauseAudio(0);


  return(1);
}

void 
psp_sound_pause(void)
{
  SDL_PauseAudio(1);
}

void 
psp_sound_resume(void)
{
  if (ATARI.atari_snd_enable) {
    SDL_PauseAudio(0);
  }
}

void 
psp_sound_shutdown(void)
{
  SDL_CloseAudio();
}

