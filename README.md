# SDL 2 Clock in C

A digital clock written in **C using the SDL2 library**, **supports both 24 hour and 12 hour time formats.**
Displays hours, minutes and seconds along with AM/PM (for 12 hour format) with a neat and clear UI.

-------

## Features:

- Real-time clock with hours, minutes and seconds displayed.
- Ability to freely switch between 24 hour and 12 hour time formats.
- Clear AM/PM distinction in 12 hour format.
- Custom font used : **GohuFont11NerdFontMono-Regular.ttf.** (licensed under WTFPL, see `GohuFont-WTFPL.txt` for the full license)
- Colour-coded boxes and neat UI elements.

-------

## Dependencies

- **SDL2**
- **SDL2_ttf**

### Windows (MSYS2/MinGW64)
```
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
gcc clock.c functions.c -o clock -lSDL2 -lSDL2_ttf
./clock
```

### Linux (Debian/Ubuntu)
```
sudo apt install libsdl2-dev libsdl2-ttf-dev
gcc clock.c functions.c -o clock -lSDL2 -lSDL2_ttf
./clock
```
