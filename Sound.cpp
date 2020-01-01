// Copyright 2005 Greg Stanton
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "global.h"
#include "ProSystem.h"
#include "Sound.h"
#define SOUND_SOURCE "Sound.cpp"

#include "psp_sound.h"

byte sound_latency = SOUND_LATENCY_VERY_LOW;

/* LUDO: */
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef struct { 
    WORD  wFormatTag; 
    WORD  nChannels; 
    DWORD nSamplesPerSec; 
    DWORD nAvgBytesPerSec; 
    WORD  nBlockAlign; 
    WORD  wBitsPerSample; 
    WORD  cbSize; 
} WAVEFORMATEX; 

# define WAVE_FORMAT_PCM 0

static const WAVEFORMATEX SOUND_DEFAULT_FORMAT = {WAVE_FORMAT_PCM, 1, 44100, 44100, 1, 8, 0};
static WAVEFORMATEX sound_format = SOUND_DEFAULT_FORMAT;

// ----------------------------------------------------------------------------
// GetSampleLength
// ----------------------------------------------------------------------------
static uint 
sound_GetSampleLength(uint length, uint unit, uint unitMax) 
{
  uint sampleLength = length / unitMax;
  uint sampleRemain = length % unitMax;
  if(sampleRemain != 0 && sampleRemain >= unit) {
    sampleLength++;
  }
  return sampleLength;
}

// ----------------------------------------------------------------------------
// Resample
// ----------------------------------------------------------------------------
static void 
sound_Resample(const byte* source, byte* target, int length) 
{
  int measurement = sound_format.nSamplesPerSec;
  int sourceIndex = 0;
  int targetIndex = 0;
  
  while(targetIndex < length) {
    if(measurement >= 31440) {
      target[targetIndex++] = source[sourceIndex];
      measurement -= 31440;
    }
    else {
      sourceIndex++;
      measurement += sound_format.nSamplesPerSec;
    }
  }
}

//# define MAX_SAMPLE_SIZE 1920
bool 
sound_Store() 
{
  uint length = 735;
  byte sample[length];
  sound_Resample(tia_buffer, sample, length);

  if(cartridge_pokey) {
    byte pokeySample[length];
    sound_Resample(pokey_buffer, pokeySample, length);
    for(int index = 0; index < length; index++) {
      sample[index] = (sample[index] + pokeySample[index]) / 2;
    }
  }
  psp_sound_send_buffer(sample, length);

  return true;
}

