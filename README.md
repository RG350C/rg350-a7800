Welcome to GP2X-7800 - 1.0.3


# AUTHORS

Original Author of ProSystem : 

  Greg Stanton (see http://home.comcast.net/~gscottstanton/) 

Author of the PSP and GP2X ports version 

  Ludovic.Jacomme also known as Zx-81 (see http://zx81.zx81.free.fr)


# INTRODUCTION

  ProSystem is the best emulator of Atari 7800 game console, running on Windows system.

  GP2X-7800 is a port on GP2X of the version v1.2 of ProSystem.

  **** Big Big thanks to Danno for his help on game compatibility issues ! ***

  This package is under GPL Copyright, read COPYING file for more information about it.


# INSTALLATION

  Unzip the zip file, and copy the content of the directory game to your SD memory.

  Put your roms files on "roms" sub-directory. 

  If you want to use the original 7800 Bios, then put the file called 7800.rom in the game/7800 directory.

  For any comments or questions on this version, please visit http://zx81.zx81.free.fr or http://www.gp32x.com/


# CONTROL

  In the 7800 emulator window, there are three different mapping (standard, left trigger, and right Trigger mappings). 

  You can toggle between while playing inside the emulator using the two trigger keys.

  Normal mapping :
  
  GP2X        Atari 7800               
  
    X          Fire 1
    A          Fire 2
    Y          L diff
    B          R diff

  LTrigger mapping :
  
    GP2X        Atari 7800               
    
    X          Save state
    A          FPS
    Y          Load state
    B          Joystick
  
  RTrigger mapping :
  
  GP2X        Atari 7800               
  
    X          Auto-fire
    A          Fire 2
    Y          L Diff
    B          R Diff
    Left/Right Dec/Inc fire
    
    LTrigger   Toogle with L keyboard mapping
    RTrigger   Toggle with R keyboard mapping
  
  Joystick   Joystick
  
    Press Select  to enter in emulator main menu.
    Press Start   open/close the On-Screen keyboard

  In the main menu

        RTrigger   Reset the emulator

        Y   Go Up directory
        X   Valid
        B   Valid
        A   Go Back to the emulator window

  The On-Screen Keyboard of "Danzel" and "Jeff Chen"

    Use the stick to choose one of the 9 squares, and use A, B, X, Y to choose one of the 4 letters of the highlighted square.

    Use LTrigger and RTrigger to see other 9 squares figures.


# LOADING ROM FILES (.A78)

  If you want to load rom images in the virtual drive of your emulator, you have to put your rom file (with .zip or .a78 file extension) on your GP2X SD memory in the 'roms' directory. 

  Then, while inside Atari 7800 emulator, just press SELECT to enter in the emulator main menu, choose "Load ROM" and then using the file selector choose one game file to load in your emulator. Back to the emulator window, your game should run automatically.


# LOADING KEY MAPPING FILES

  For given games, the default keyboard mapping between GP2X Keys and Atari 7800 keys, is not suitable, and the game can't be played on GP2X7800.

  To overcome the issue, you can write your own mapping file. Using notepad for example you can edit a file with the .kbd extension and put it in the kbd directory.

  For the exact syntax of those mapping files, have a look on sample files already presents in the kbd directory (default.kbd etc ...).

  After writting such keyboard mapping file, you can load them using the main menu inside the emulator.

  If the keyboard filename is the same as the rom file (.a78) then when you load this rom file, the corresponding keyboard file is 
  automatically loaded !

  You can now use the Keyboard menu and edit, load and save your keyboard mapping files inside the emulator. The Save option save the .kbd
  file in the kbd directory using the "Game Name" as filename. The game name is displayed on the right corner in the emulator menu.

  
# COMPILATION

  It has been developped under Linux using gcc with GP2XSDK. 
  
  To rebuild the homebrew run the Makefile in the src archive.

# COMPILATION FOR GWC0/RG350 on LINUX
  Developped under Linux Ubuntu / Visual Studio Code / GCC / GCW0 toolchain
  http://www.gcw-zero.com/develop
  
    Compile:
        ./build_gcw0.sh

    Clean:
        ./clean_gcw0.sh

    Release: 
        ./build_gcw0.sh create release file rg350-write.opk on ./build/ folder

  Enjoy,
  
         Jorge Hontoria
