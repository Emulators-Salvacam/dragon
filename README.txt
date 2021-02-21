
    Welcome to GP2X-Dragon

Original Author of XRoar 

  Ciaran Anscomb  xroar(at)6809.org.uk

  Homepage: http://www.6809.org.uk/dragon/xroar.shtml

Author of the PSP and GP2X port version 

  Ludovic Jacomme alias Zx-81 zx81.zx81(at)gmail.com

  Homepage: http://zx81.zx81.free.fr


1. INTRODUCTION
   ------------

  Xroar is the best emulator of Dragon32/64 and Tandy Coco home computer
  running on many systems such as Linux, Unix, Mac OS X, GP32 and Windows32.
  See http://www.6809.org.uk/dragon/xroar.shtml for further informations.

  GP2X-Dragon is a port on GP2X of the PSP version.

  Special thanks to Danzel and Jeff Chen for their virtual keyboard,
  Ciaran Anscomb for this great emulator, Raven's for eboot icons
  and to all GP2X-SDK developpers.

  This package is under GPL Copyright, read COPYING file for
  more information about it.


2. INSTALLATION
   ------------

  Unzip the zip file, and copy the content of the directory game to your
  SD memory.

  For any comments or questions on this version, please visit 
  http://zx81.zx81.free.fr or http://www.gp32x.com/


3. CONTROL
 ------------

In the Dragon emulator window, there are three different mapping (standard,
left trigger, and right Trigger mappings).  
You can toggle between while playing inside the emulator using the two GP2X
trigger keys.

  -------------------------------------
  GP2X       Dragon          (standard)

  A          2
  B          ENTER
  Y          1
  X          Joystick Fire
  Up         Up
  Down       Down
  Left       Left 
  Right      Right

  Joystick   Joystick

  -------------------------------------
  GP2X       Dragon (left trigger)

  A          Fps
  B          Render
  Y          Load state 
  X          Save state
  Up         Up
  Down       Down
  Left       Left 
  Right      Right

  -------------------------------------
  GP2X       Dragon (right trigger)

  A          Space
  B          ENTER
  Y          4
  X          Auto fire on/off
  Up         Up
  Down       Down
  Left       Dec Fire
  Right      Inc Fire

  Analog     Joystick
  
  Joystick   Up, Down, Left, Rigth
    
  Press Select  to enter in emulator main menu.
  Press Start   open/close the On-Screen keyboard

  In the main menu

  RTrigger   Reset the emulator

  Y   Go Up directory
  X   Valid
  B   Valid
  A   Go Back to the emulator window

  The On-Screen Keyboard of "Danzel" and "Jeff Chen"

  Use the stick to choose one of the 9 squares, and
  use A, B, X, Y to choose one of the 4 letters 
  of the highlighted square.

  Use LTrigger and RTrigger to see other 9 squares 
  figures.


4. LOADING DISK FILES (DSK)
 ------------

If you want to load disk image in the virtual disk
drive of your emulator, you have to put your disk
file (with .dsk file extension) on your memory
card in the 'disk' directory. 

Then, while inside emulator, just press SELECT to
enter in the emulator main menu, choose "Load Disc"
and then using the file selector choose one disc
file to load in your emulator.

To list the content of the disk, you have to use
the virtual keyboard (press START key) and type the
command DIR followed by ENTER.

To run the game MYGAME.BIN on your disk, you have
to use the virtual keyboard and type the command
LOADM"MYGAME" followed by ENTER.  Once the game is
loaded, enter the command EXEC followed by ENTER.

To run a basic game (with .BAS extention) use RUN
command instead of LOADM.

If you want to specify the command to run for given
games then you can do it in the run.txt, using the
following syntax :

  tapename=CLOAD"":EXEC
  diskname=LOADM"RunName":EXEC
  diskname=RUN"RunName"


5. LOADING KEY MAPPING FILES
 ------------

For given games, the default keyboard mapping between GP2X Keys and DRAGON
keys, is not suitable, and the game can't be played on DRAGON.

To overcome the issue, you can write your own mapping file. Using notepad for
example you can edit a file with the .kbd extension and put it in the kbd
directory.

For the exact syntax of those mapping files, have a look on sample files
already presents in the kbd directory (default.kbd etc ...).

After writting such keyboard mapping file, you can load them using the main
menu inside the emulator.

If the keyboard filename is the same as the cartridge or disk file then when
you load this file, the corresponding keyboard file is automatically loaded !

You can now use the Keyboard menu and edit, load and save your keyboard
mapping files inside the emulator. 

The Save option save the .kbd file in the kbd directory using the "Game Name"
as filename.  The game name is displayed on the right corner in the emulator
menu.

6. COMPILATION
   ------------

  It has been developped under Linux using gcc with GP2XSDK. 
  To rebuild the homebrew run the Makefile in the src archive.

