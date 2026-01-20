// functions.h

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "config.h"

// Function Definitions

// event and input handling
void eventHandler(bool *isRunning, bool *tFormatHover, bool *isButtonPress, struct Box *minutes, AppWindow windowMode);

// circle drawing function
void drawCircles(SDL_Renderer *pRen, int xc, int yc, int radius, int r, int g, int b);

// text rendering
void renderText(SDL_Renderer *pRen, TTF_Font *font, int value, SDL_Color colour, SDL_Rect textBox);
void renderTextStr(SDL_Renderer *pRen, TTF_Font *font, char *text, SDL_Color colour, SDL_Rect stringBox);

// time calculation
void getCurrentTime(int *h, int *m, int *s, Uint32 *lTick);

// render UI
void renderUI(SDL_Renderer *pRen, TTF_Font *font, TTF_Font *titleFont,
              TTF_Font *tFormatFont, struct Box *hours,
              struct Box *minutes, struct Box *seconds, struct Box *fillH,
              struct Box *fillM, struct Box *fillS, int cx1,
              int cx2, int top, bool tFormatHover,
              int h, int m, int s, int bottom, AppWindow windowMode);


#endif