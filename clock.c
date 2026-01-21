// CLOCK USING SDL AND C
// Font used: GohuFont11NerdFontMono-Regular.ttf (WTFPL)

// HANDLE MAIN
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"
#include "functions.h"

int main()
{
    // Running Status
    bool isRunning = true;

    // tFormat Button Check
    bool tFormatHover = false;

    // Window change Check
    bool isButtonPress = false;

    // Set Current Window definition
    AppWindow currentWindow = WINDOW_CLOCK_24;

    // Offsets for boxes
    int offset_1 = BOXSPACING;
    int offset_2 = 0;
    int offset_3 = -BOXSPACING;

    // Hours Display Structure
    struct Box box_hours =
        {BOX_R, BOX_G, BOX_B, BOXSIZE, (WIN_WIDTH / 2 - BOXSIZE / 2) - offset_1, (WIN_HEIGHT / 2 - BOXSIZE / 2)};

    // Minutes Display Structure
    struct Box box_minutes =
        {BOX_R, BOX_G, BOX_B, BOXSIZE, (WIN_WIDTH / 2 - BOXSIZE / 2) - offset_2, (WIN_HEIGHT / 2 - BOXSIZE / 2)};

    // Seconds Display Structure
    struct Box box_seconds =
        {BOX_R, BOX_G, BOX_B, BOXSIZE, (WIN_WIDTH / 2 - BOXSIZE / 2) - offset_3, (WIN_HEIGHT / 2 - BOXSIZE / 2)};

    // Fill (Hours)
    struct Box fill_hours =
        {FILL_R, FILL_G, FILL_B, BOXSIZE - 2 * BOUNDARY, (box_hours.xpos + BOUNDARY), (box_hours.ypos + BOUNDARY)};

    // Fill (Minutes)
    struct Box fill_minutes =
        {FILL_R, FILL_G, FILL_B, BOXSIZE - 2 * BOUNDARY, (box_minutes.xpos + BOUNDARY), (box_minutes.ypos + BOUNDARY)};

    // Fill (Seconds)
    struct Box fill_seconds =
        {FILL_R, FILL_G, FILL_B, BOXSIZE - 2 * BOUNDARY, (box_seconds.xpos + BOUNDARY), (box_seconds.ypos + BOUNDARY)};

    // Colon dimension
    int colon_X1 = box_hours.xpos + box_hours.size + COLON_OFFSET_X;
    int colon_X2 = box_minutes.xpos + box_minutes.size + COLON_OFFSET_X;
    int topColon_Y = WIN_HEIGHT / 2 - COLON_OFFSET_Y;
    int bottomColon_Y = WIN_HEIGHT / 2 + COLON_OFFSET_Y;

    // Initialize SDL RENDERER
    SDL_Init(SDL_INIT_VIDEO);

    // Return error if TTF doesnt initialise properly

    if (TTF_Init() == -1)
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return 1;
    }

    // Load Fonts

    TTF_Font *font = TTF_OpenFont("GohuFont11NerdFontMono-Regular.ttf", TEXT_FONT_SIZE);
    TTF_Font *titleFont = TTF_OpenFont("GohuFont11NerdFontMono-Regular.ttf", STR_FONT_SIZE);
    TTF_Font *tFormatFont = TTF_OpenFont("GohuFont11NerdFontMono-Regular.ttf", TFORMAT_FONT_SIZE);

    // Return error if font does not load properly
    if (!font || !titleFont || !tFormatFont)
    {
        printf("Font load error: %s\n", TTF_GetError());
        return 1;
    }

    // Time Calculation using ticks and time.h
    int h, m, s;
    time_t time_now = time(NULL);
    struct tm *realTime = localtime(&time_now);

    h = realTime->tm_hour;
    m = realTime->tm_min;
    s = realTime->tm_sec;

    Uint32 lastTick = SDL_GetTicks();

    // Create Window and Renderer
    SDL_Window *pWin = SDL_CreateWindow("Clock in C!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    SDL_Renderer *pRen = SDL_CreateRenderer(pWin, -1, SDL_RENDERER_ACCELERATED);

    if (!pWin || !pRen)
    {
        printf("Renderer or Window error: %s\n", SDL_GetError());
        return 1;
    }

    while (isRunning)
    {
        getCurrentTime(&h, &m, &s, &lastTick);

        eventHandler(&isRunning, &tFormatHover, &isButtonPress, &box_minutes, currentWindow);

        // Swap between windows
        if (isButtonPress)
        {
            currentWindow = (currentWindow == WINDOW_CLOCK_24) ? WINDOW_CLOCK_12 : WINDOW_CLOCK_24;
            isButtonPress = false;
        }
        if (currentWindow == WINDOW_CLOCK_24)
        {
            SDL_SetRenderDrawColor(pRen, WINDOW_R, WINDOW_G, WINDOW_B, OPAQUE_ALPHA);
            SDL_RenderClear(pRen);

            // Render all UI Elements for first window
            renderUI(pRen, font, titleFont, tFormatFont, &box_hours, &box_minutes, &box_seconds,
                     &fill_hours, &fill_minutes, &fill_seconds,
                     colon_X1, colon_X2, topColon_Y, tFormatHover, h, m, s, bottomColon_Y, currentWindow);

            SDL_Delay(DELAY_60_FPS);
        }
        else if (currentWindow == WINDOW_CLOCK_12)
        {
            SDL_SetRenderDrawColor(pRen, WINDOW_R, WINDOW_G, WINDOW_B, OPAQUE_ALPHA);
            SDL_RenderClear(pRen);

            // Render all UI Elements for second window
            renderUI(pRen, font, titleFont, tFormatFont, &box_hours, &box_minutes, &box_seconds,
                     &fill_hours, &fill_minutes, &fill_seconds,
                     colon_X1, colon_X2, topColon_Y, tFormatHover, h, m, s, bottomColon_Y, currentWindow);

            SDL_Delay(DELAY_60_FPS);
        }
    }
    // EXIT
    SDL_DestroyRenderer(pRen);
    SDL_DestroyWindow(pWin);
    SDL_Quit();

    return 0;

}

