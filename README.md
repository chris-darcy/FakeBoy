# FakeBoy
A repository for the development of a GameBoy emulator in C++ 17 and OpenGL (Audio is WIP).

## Dependencies
- GLFW
- OpenGL

GLFW needs to be downloaded and placed in the *deps* folder and will be built alongside the project by CMake.

## ROMS
.gb extension ROM files should be placed in *ROMS* folder. Any save game information will also be placed here by the emulator as a .sav file.

To load a particular ROM, simply change "Your_ROM_File.gb" in the *GameBoy.cpp* file.

## Controls
UP             -Up Arrow Key  
DOWN           -Down Arrow Key  
LEFT           -Left Arrow Key  
RIGHT          -Right Arrow Key  
A              -Z Key  
B              -X Key  
Start          -Enter Key  
Select         -Backslash Key  
  
Immediate Quit -Escape Key  
  
Palette Select -T Key  
