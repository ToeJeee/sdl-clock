// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>

// WINDOW AND STATIC UI DIMENSIONS
#define SDL_MAIN_HANDLED
#define WIN_WIDTH 900
#define WIN_HEIGHT 600
#define BOUNDARY 30
#define BOXSIZE 250
#define BOXSPACING 300
#define COLONRADIUS 15
#define COLON_OFFSET_X 22
#define COLON_OFFSET_Y 30

// TEXT AND STRING DIMENSIONS
#define TEXT_FONT_SIZE 125
#define STR_FONT_SIZE 125
#define TFORMAT_FONT_SIZE 45
#define TOP_TITLE_X 0
#define TOP_TITLE_Y 40
#define TOP_TITLE_HEIGHT 100
#define HOVER_HITBOX_OFFSET_X_LEFT 260
#define HOVER_HITBOX_OFFSET_X_RIGHT 250
#define HOVER_TEXT_X_PADDING 10
#define HOVER_TEXT_HEIGHT 50
#define TEXT_X_OFFSET 5
#define FORMAT_24_TEXT_Y 480
#define FORMAT_TEXT_Y 520

// BUTTON DIMENSIONS
#define TFORMAT_BUTTON_WIDTH 235
#define TFORMAT_BUTTON_HEIGHT 50
#define TFORMAT_BUTTON_X_OFFSET 50
#define AM_OR_PM_Y 440
#define AM_PM_RECT_HEIGHT 50

// TIME CONSTANTS
#define DELAY_60_FPS 16
#define MS_PER_SECONDS 1000
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR 60
#define HOURS_PER_DAY 24

// GLOBAL COLOUR VALUES
#define OPAQUE_ALPHA 255

#define WINDOW_R 155
#define WINDOW_G 100
#define WINDOW_B 155

#define BOX_R 70
#define BOX_G 59
#define BOX_B 120

#define FILL_R 227
#define FILL_G 188
#define FILL_B 227

#define COLON_R 94
#define COLON_G 201
#define COLON_B 209

#define TEXT_R 20
#define TEXT_G 20
#define TEXT_B 20

#define STR_R 138
#define STR_G 43
#define STR_B 81

#define HOVER_R 66
#define HOVER_G 21
#define HOVER_B 36

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// BOX STRUCT
struct Box
{
    int r, g, b;
    int size;
    int xpos;
    int ypos;
};

// WINDOW MODE ENUM
typedef enum
{
    WINDOW_CLOCK_24,
    WINDOW_CLOCK_12
} AppWindow;

#endif