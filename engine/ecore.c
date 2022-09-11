#include "engine.h"

//======================================================
// Initialization
//======================================================

// Initialize Engine
void initEngine() {
    // ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    // clock
    CORE.prev_clock_time = clock();
}

// Deinitialize Engine
void deinitEngine() {
    endwin();
}


//======================================================
// Viewport
//======================================================

/**
 * Set viewport parameters
 * @param width   Width of viewport
 * @param height  Height of viewport
 * @param fc      Fill character
 */
void setViewport(int width, int height, char fc) {
    CORE.width = width;
    CORE.height = height;

    // Viewport layout = [char | int][char | int][char | int] ···
    // One pixel       = "##"
    CORE.viewport = (Viewport*)malloc((CORE.width * 2) * CORE.height * (sizeof(char) + sizeof(int)));
    for (int i=0; i < (CORE.width * 2) * CORE.height; i++) {
        CORE.viewport[i].ch = 0;
        CORE.viewport[i].color = -1;
    }
}

// Render viewport to terminal
void renderViewport() {
    static int tick_count = 0; // ! For debugging

    // Check if window is bigger than rendering viewport
    int w_width, w_height; 
    getmaxyx(stdscr, w_height, w_width);
    if ((w_height <= CORE.height) || (w_width <= CORE.width * 2)) {
        deinitEngine();
        printf("Exited: Window is smaller than viewport size!");
        exit(0);
    }

    // Render viewport
    for (int i=0; i < (CORE.width * 2) * CORE.height; i++) {
        if (CORE.viewport[i].ch != 0) {
            mvaddch((int)i / (CORE.width * 2), i % (CORE.width * 2), CORE.viewport[i].ch);
        }
    }

    mvaddstr(CORE.height, 0, "tick: "); // ! For debugging
    printw("%d", tick_count++);      // ! For debugging
    CORE.prev_clock_time = clock();
    refresh();

    // Delay to match fps
    while (clock() - CORE.prev_clock_time < CLOCKS_PER_SEC / CORE.fps);
}

// Clear viewport
void clearViewport() {
    clear();
    for (int i=0; i < (CORE.width * 2) * CORE.height; i++) {
        CORE.viewport[i].ch = 0;
        CORE.viewport[i].color = -1;
    }
}


//======================================================
// Time
//======================================================

/**
 * Set target refresh rate
 * @param fps FPS
 */
void setTargetFPS(int fps) {
    CORE.fps = fps;
}

// Return clock time (milliseconds)
ulong_t getClocktime() {
    return clock() / 1000;
}


//======================================================
//                         Draw
//======================================================

/**
 * (USE CAREFULLY)
 * Draw pixel "#"
 * @param px        Precise x position
 * @param py        Presice y poisiton
 * @param ch        Character
 * @param color     Foreground color
 */
void drawPixel(int px, int py, char ch, int color) {
    if ((px >= 0) && (px < (CORE.width * 2)) && (py >= 0) && (py < CORE.height)) {
        CORE.viewport[py * (CORE.width * 2) + px].ch = ch;
        CORE.viewport[py * (CORE.width * 2) + px].color = color;
    }
}

/**
 * Draw point "##"
 * @param x     X position
 * @param y     Y poisiton
 * @param ch    Character
 * @param color Foreground color
 */
void drawPoint(int x, int y, char ch, int color) {
    if ((x >= 0) && (x < CORE.width) && (y >= 0) && (y < CORE.height)) {
        CORE.viewport[y * (CORE.width * 2) + (x * 2)].ch = ch;
        CORE.viewport[y * (CORE.width * 2) + (x * 2)].color = color;
        CORE.viewport[y * (CORE.width * 2) + (x * 2 + 1)].ch = ch;
        CORE.viewport[y * (CORE.width * 2) + (x * 2 + 1)].color = color;
    }
}