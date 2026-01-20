// CLOCK USING SDL AND C

// GLOBAL DIMENSIONS

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

// Function Definitions

void eventHandler(bool *isRunning, bool *tFormatHover, bool *isButtonPress, struct Box *minutes, AppWindow windowMode);

void drawCircles(SDL_Renderer *pRen, int xc, int yc, int radius, int r, int g, int b);

void renderText(SDL_Renderer *pRen, TTF_Font *font, int value, SDL_Color colour, SDL_Rect textBox);

void renderTextStr(SDL_Renderer *pRen, TTF_Font *font, char *text, SDL_Color colour, SDL_Rect stringBox);

void getCurrentTime(int *h, int *m, int *s, Uint32 *lTick);

void renderUI(SDL_Renderer *pRen, TTF_Font *font, TTF_Font *titleFont,
              TTF_Font *tFormatFont, struct Box *hours,
              struct Box *minutes, struct Box *seconds, struct Box *fillH,
              struct Box *fillM, struct Box *fillS, int cx1,
              int cx2, int top, bool tFormatHover,
              int h, int m, int s, int bottom, AppWindow windowMode);

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

    TTF_Font *font = TTF_OpenFont("Gohu/GohuFont11NerdFontMono-Regular.ttf", TEXT_FONT_SIZE);
    TTF_Font *titleFont = TTF_OpenFont("Gohu/GohuFont11NerdFontMono-Regular.ttf", STR_FONT_SIZE);
    TTF_Font *tFormatFont = TTF_OpenFont("Gohu/GohuFont11NerdFontMono-Regular.ttf", TFORMAT_FONT_SIZE);

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

void eventHandler(bool *isRunning, bool *tFormatHover, bool *isButtonPress, struct Box *minutes, AppWindow windowMode)
{
    SDL_Event event;

    // Switch the text around depending on the window using a dynamic variable
    int Y_Dynamic = (windowMode == WINDOW_CLOCK_24) ? FORMAT_24_TEXT_Y : FORMAT_TEXT_Y;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            *isRunning = false;
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            int mouse_X = event.motion.x;
            int mouse_Y = event.motion.y;

            SDL_Rect tFormatRect =
                {(minutes->xpos + minutes->size / 2 - (TFORMAT_BUTTON_WIDTH / 2) / 2) - TFORMAT_BUTTON_X_OFFSET, Y_Dynamic, TFORMAT_BUTTON_WIDTH, TFORMAT_BUTTON_HEIGHT};

            if (mouse_X >= tFormatRect.x - HOVER_HITBOX_OFFSET_X_LEFT && mouse_X <= tFormatRect.x + tFormatRect.w + HOVER_HITBOX_OFFSET_X_RIGHT && mouse_Y >= tFormatRect.y && mouse_Y <= tFormatRect.y + tFormatRect.h)
            {
                *tFormatHover = true;
            }
            else
            {
                *tFormatHover = false;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {

                SDL_Rect tFormatRect =
                    {(minutes->xpos + minutes->size / 2 - (TFORMAT_BUTTON_WIDTH / 4)) - TFORMAT_BUTTON_X_OFFSET, Y_Dynamic, TFORMAT_BUTTON_WIDTH, TFORMAT_BUTTON_HEIGHT};

                int pressX = event.button.x;
                int pressY = event.button.y;

                if (pressX >= tFormatRect.x - HOVER_HITBOX_OFFSET_X_LEFT && pressX <= tFormatRect.x + tFormatRect.w + HOVER_HITBOX_OFFSET_X_RIGHT && pressY >= tFormatRect.y && pressY <= tFormatRect.y + tFormatRect.h)
                {
                    *isButtonPress = 1;
                }
            }
        }
    }
}

void drawCircles(SDL_Renderer *pRen, int xc, int yc, int radius, int r, int g, int b)
{
    SDL_SetRenderDrawColor(pRen, r, g, b, OPAQUE_ALPHA);

    for (int i = 0; i < radius * 2; i++)
    {
        for (int j = 0; j < radius * 2; j++)
        {
            int offsetCentreX = radius - i;
            int offsetCentreY = radius - j;

            if ((offsetCentreX * offsetCentreX + offsetCentreY * offsetCentreY) <= radius * radius)
            {
                SDL_RenderDrawPoint(pRen, xc + offsetCentreX, yc + offsetCentreY);
            }
        }
    }
}

void renderText(SDL_Renderer *pRen, TTF_Font *font, int value, SDL_Color colour, SDL_Rect box)
{
    char holdTime[10];
    snprintf(holdTime, sizeof(holdTime), "%02d", value);

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, holdTime, colour);
    SDL_Texture *textText = SDL_CreateTextureFromSurface(pRen, textSurface);

    SDL_Rect textBox;
    textBox.w = textSurface->w;
    textBox.h = textSurface->h;
    textBox.x = (box.x + (box.w - textBox.w) / 2) + TEXT_X_OFFSET;
    textBox.y = (box.y + (box.h - textBox.h) / 2);

    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(pRen, textText, NULL, &textBox);
    SDL_DestroyTexture(textText);
}

void renderTextStr(SDL_Renderer *pRen, TTF_Font *font, char *text, SDL_Color colour, SDL_Rect stringBox)
{
    SDL_Surface *printSurface = TTF_RenderText_Blended(font, text, colour);
    SDL_Texture *printTexture = SDL_CreateTextureFromSurface(pRen, printSurface);

    SDL_Rect string;
    string.w = printSurface->w;
    string.h = printSurface->h;
    string.x = stringBox.x + (stringBox.w - string.w) / 2;
    string.y = stringBox.y + (stringBox.h - string.h) / 2;

    SDL_FreeSurface(printSurface);
    SDL_RenderCopy(pRen, printTexture, NULL, &string);
    SDL_DestroyTexture(printTexture);
}

void getCurrentTime(int *h, int *m, int *s, Uint32 *lTick)
{
    Uint32 currentTick = SDL_GetTicks();

    while (currentTick - *lTick >= MS_PER_SECONDS) // 1000ms = 1s
    {
        (*s)++;
        *lTick += MS_PER_SECONDS;

        if (*s == SECONDS_PER_MINUTE)
        {
            *s = 0;
            (*m)++;
        }
        if (*m == MINUTES_PER_HOUR)
        {
            *m = 0;
            (*h)++;
        }
        if (*h == HOURS_PER_DAY)
        {
            *h = 0;
        }
    }
}

void renderUI(SDL_Renderer *pRen, TTF_Font *font, TTF_Font *titleFont,
              TTF_Font *tFormatFont, struct Box *hours,
              struct Box *minutes, struct Box *seconds, struct Box *fillH,
              struct Box *fillM, struct Box *fillS, int cx1,
              int cx2, int top, bool tFormatHover,
              int h, int m, int s, int bottom, AppWindow windowMode)
{
    // Display text for 24 hour to 12 hour switch
    int formatHour = h;
    char *displayText = "View in 12 hour format instead!";

    // Convert 24 hour value to 12 hour value
    if (windowMode == WINDOW_CLOCK_12)
    {
        formatHour = h % 12;
        if (formatHour == 0)
        {
            formatHour = 12;
        }
        // Display text for 12 hour to 24 hour switch
        displayText = "Back to 24 Hour Format?";
    }

    // Object Rectangles (Boxes and Fills)
    // Hours
    SDL_Rect rectBox_hours =
        {hours->xpos, hours->ypos, hours->size, hours->size};
    SDL_Rect rectFill_hours =
        {fillH->xpos, fillH->ypos, fillH->size, fillH->size};

    // Minutes
    SDL_Rect rectBox_minutes =
        {minutes->xpos, minutes->ypos, minutes->size, minutes->size};
    SDL_Rect rectFill_minutes =
        {fillM->xpos, fillM->ypos, fillM->size, fillM->size};

    // Seconds
    SDL_Rect rectBox_seconds =
        {seconds->xpos, seconds->ypos, seconds->size, seconds->size};
    SDL_Rect rectFill_seconds =
        {fillS->xpos, fillS->ypos, fillS->size, fillS->size};

    // Render Hours Box
    SDL_SetRenderDrawColor(pRen, hours->r, hours->g, hours->b, OPAQUE_ALPHA);
    SDL_RenderFillRect(pRen, &rectBox_hours);
    SDL_SetRenderDrawColor(pRen, fillH->r, fillH->g, fillH->b, OPAQUE_ALPHA);
    SDL_RenderFillRect(pRen, &rectFill_hours);

    // Render Minutes Box
    SDL_SetRenderDrawColor(pRen, minutes->r, minutes->g, minutes->b, OPAQUE_ALPHA);
    SDL_RenderFillRect(pRen, &rectBox_minutes);
    SDL_SetRenderDrawColor(pRen, fillM->r, fillM->g, fillM->b, OPAQUE_ALPHA);
    SDL_RenderFillRect(pRen, &rectFill_minutes);

    // Render Seconds Box
    SDL_SetRenderDrawColor(pRen, seconds->r, seconds->g, seconds->b, OPAQUE_ALPHA);
    SDL_RenderFillRect(pRen, &rectBox_seconds);
    SDL_SetRenderDrawColor(pRen, fillS->r, fillS->g, fillS->b, OPAQUE_ALPHA);
    SDL_RenderFillRect(pRen, &rectFill_seconds);

    // Render Colon circles using function
    drawCircles(pRen, cx1, top, COLONRADIUS, COLON_R, COLON_G, COLON_B);
    drawCircles(pRen, cx1, bottom, COLONRADIUS, COLON_R, COLON_G, COLON_B);
    drawCircles(pRen, cx2, top, COLONRADIUS, COLON_R, COLON_G, COLON_B);
    drawCircles(pRen, cx2, bottom, COLONRADIUS, COLON_R, COLON_G, COLON_B);

    // Render Time as text in the boxes
    SDL_Color textColour = {TEXT_R, TEXT_G, TEXT_B, OPAQUE_ALPHA};
    renderText(pRen, font, formatHour, textColour, rectFill_hours);
    renderText(pRen, font, m, textColour, rectFill_minutes);
    renderText(pRen, font, s, textColour, rectFill_seconds);

    // Render some cute text
    SDL_Rect stringBox =
        {TOP_TITLE_X, TOP_TITLE_Y, WIN_WIDTH, TOP_TITLE_HEIGHT};
    SDL_Color stringColour = {STR_R, STR_G, STR_B, OPAQUE_ALPHA};
    renderTextStr(pRen, titleFont, "CLOCK :3", stringColour, stringBox);

    // Render tFormat Text

    int Y_Dynamic = (windowMode == WINDOW_CLOCK_24) ? FORMAT_24_TEXT_Y : FORMAT_TEXT_Y;
    SDL_Rect tFormatTxt =
        {(rectBox_minutes.x + rectBox_minutes.w / 2 - (WIN_WIDTH / 2) / 2) + HOVER_TEXT_X_PADDING, Y_Dynamic, WIN_WIDTH / 2, HOVER_TEXT_HEIGHT};
    SDL_Color tFormatColour = tFormatHover ? (SDL_Color){HOVER_R, HOVER_G, HOVER_B, OPAQUE_ALPHA} : (SDL_Color){STR_R, STR_G, STR_B, OPAQUE_ALPHA};
    renderTextStr(pRen, tFormatFont, displayText, tFormatColour, tFormatTxt);

    // Render AM or PM for ONLY 12 hour format
    if (windowMode == WINDOW_CLOCK_12)
    {
        char *amorpm = (h >= 12) ? "Time : PM" : "Time : AM";

        SDL_Rect AMPM_Box =
            {(rectBox_minutes.x + rectBox_minutes.w / 2 - (WIN_WIDTH / 2) / 2), AM_OR_PM_Y, WIN_WIDTH / 2, AM_PM_RECT_HEIGHT};

        SDL_Color AMPM_Box_Colour = {TEXT_R, TEXT_G, TEXT_B, OPAQUE_ALPHA};
        renderTextStr(pRen, tFormatFont, amorpm, AMPM_Box_Colour, AMPM_Box);
    }

    // Present Final Render
    SDL_RenderPresent(pRen);
}