#include "engine.h"
#include "unistd.h"

#define DEFAULT_CORE_BORDER             false
#define DEFAULT_CORE_FPS                30
#define DEFAULT_CORE_COLOR              false
#define DEFAULT_CORE_PREV_CLOCK_TIME    clock()
#define DEFAULT_CORE_DEBUG_ENABLED      false
#define DEFAULT_CORE_DEBUG_HEIGHT       3

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
    nodelay(stdscr, TRUE);
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    // defaults
    CORE.border = DEFAULT_CORE_BORDER;
    CORE.fps    = DEFAULT_CORE_FPS;
    CORE.color  = DEFAULT_CORE_COLOR;
    CORE.prev_clock_time    = DEFAULT_CORE_PREV_CLOCK_TIME;
    CORE.debug_enabled      = DEFAULT_CORE_DEBUG_ENABLED;
    CORE.debug_height       = DEFAULT_CORE_DEBUG_HEIGHT;
}

// Deinitialize Engine
void deinitEngine() {
    curs_set(1);
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
void setViewport(int width, int height) {
    CORE.width = width;
    CORE.height = height;

    CORE.viewport = newwin(CORE.height, CORE.width * 2, 0, 0);
    nodelay(CORE.viewport, TRUE);

    // Viewport layout = [char | int][char | int][char | int] ···
    // One pixel       = "##"
    CORE.viewport_data = (Viewport*)malloc((CORE.width * 2) * CORE.height * sizeof(Viewport));
    for (int i=0; i < (CORE.width * 2) * CORE.height; i++) {
        CORE.viewport_data[i].ch = 0;
        CORE.viewport_data[i].color = 0;
    }
}

// Enable color rendering
void setColor() {
    if (has_colors()) {
        CORE.color = true;

        start_color();
        init_pair(COLOR_BLACK      , COLOR_BLACK      , COLOR_BLACK);
        init_pair(COLOR_RED        , COLOR_RED        , COLOR_BLACK);
        init_pair(COLOR_GREEN      , COLOR_GREEN      , COLOR_BLACK);
        init_pair(COLOR_YELLOW     , COLOR_YELLOW     , COLOR_BLACK);
        init_pair(COLOR_BLUE       , COLOR_BLUE       , COLOR_BLACK);
        init_pair(COLOR_MAGENTA    , COLOR_MAGENTA    , COLOR_BLACK);
        init_pair(COLOR_CYAN       , COLOR_CYAN       , COLOR_BLACK);
        init_pair(COLOR_WHITE      , COLOR_WHITE      , COLOR_BLACK);
    } else {
        CORE.color = false;
    }
}

void setBorder() {
    CORE.border = true;
    wresize(CORE.viewport, CORE.height + 2, (CORE.width * 2) + 2);

    // Render border
    if (CORE.border) {
        box(CORE.viewport, 0, 0);
    }
}

// Render viewport to terminal
void renderViewport() {
    CORE.prev_clock_time = clock();
    
    int fv_height = CORE.height;        // rendered full viewport height
    int border_padding = 0;             // border padding
    int border_padding_amt = 0;         // border padding amount
    int w_width, w_height;              // terminal window size

    getmaxyx(stdscr, w_height, w_width);

    // Add border padding if border is enabled
    if (CORE.border) {
        border_padding = 1;

        border_padding_amt = 2;
        if (CORE.debug_enabled) {
            border_padding_amt += 2;
        }
    }

    // Full viewport height depending if debug menu is enabled
    if (CORE.debug_enabled) {
        fv_height += CORE.debug_height;
    }

    // Exits if viewport is smaller than needed area
    if ((w_height <= fv_height + (border_padding * border_padding_amt)) || (w_width <= CORE.width * 2 + (border_padding * border_padding_amt))) {
        deinitEngine();
        printf("Exited: Window is smaller than viewport size!");
        exit(0);
    }

    // Render viewport
    for (int i = 0; i < (CORE.width * 2) * CORE.height; i++) {
        if (CORE.viewport_data[i].ch != 0) {
            if (CORE.color) {
                wattron(CORE.viewport, COLOR_PAIR(CORE.viewport_data[i].color));
            }

            mvwaddch(CORE.viewport, i / (CORE.width * 2) + border_padding, i % (CORE.width * 2) + border_padding, CORE.viewport_data[i].ch);

            if (CORE.color) {
                wattroff(CORE.viewport, COLOR_PAIR(CORE.viewport_data[i].color));
            }
        }
    }
    wrefresh(CORE.viewport);

    // Render debug
    if (CORE.debug_enabled) {
        if (CORE.border) {
            box(CORE.debug_menu, 0, 0);
        }
        for (int i = 0; i < CORE.debug_height; i++) {
            if (CORE.debug_data[i].title != 0) {
                mvwprintw(CORE.debug_menu, i + border_padding, 0 + border_padding, "%s: %s", CORE.debug_data[i].title, CORE.debug_data[i].value);
            }
        }
        wrefresh(CORE.debug_menu);
    }

    // Delay to match fps
    // Method 1 (Original Method)
    // while (clock() - CORE.prev_clock_time < CLOCKS_PER_SEC / CORE.fps);
    // Method 2 (A lot less processor intensive)
    if ((CLOCKS_PER_SEC / CORE.fps) - (clock() - CORE.prev_clock_time) > 0) {
        usleep((CLOCKS_PER_SEC / CORE.fps) - (clock() - CORE.prev_clock_time));
    } 
}

// Clear viewport
void clearViewport() {
    clear();
    for (int i=0; i < (CORE.width * 2) * CORE.height; i++) {
        CORE.viewport_data[i].ch = 0;
        CORE.viewport_data[i].color = 0;
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
        CORE.viewport_data[py * (CORE.width * 2) + px].ch = ch;
        CORE.viewport_data[py * (CORE.width * 2) + px].color = color;
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
        CORE.viewport_data[y * (CORE.width * 2) + (x * 2)].ch = ch;
        CORE.viewport_data[y * (CORE.width * 2) + (x * 2)].color = color;
        CORE.viewport_data[y * (CORE.width * 2) + (x * 2 + 1)].ch = ch;
        CORE.viewport_data[y * (CORE.width * 2) + (x * 2 + 1)].color = color;
    }
}


//======================================================
// Debugging
//======================================================

void showDebug() {
    CORE.debug_enabled = true;

    int border_padding = 0;
    if (CORE.border) {
        border_padding = 2;
    }
    CORE.debug_menu = newwin(CORE.debug_height + border_padding, (CORE.width * 2) + border_padding, CORE.height + border_padding, 0);
    nodelay(CORE.debug_menu, TRUE);

    CORE.debug_data = (Debug*)malloc(CORE.debug_height * sizeof(Debug));
}

void hideDebug() {
    CORE.debug_enabled = false;
}

void quitDebug() {
    CORE.debug_enabled = false;

    free(CORE.debug_data);
    delwin(CORE.debug_menu);
}

void updateDebugAttrib(int line_num, char* title, char* value) {
    // Reset max height so empty space can be removed
    int max_height = DEFAULT_CORE_DEBUG_HEIGHT;
    for (int i = 0; i < CORE.debug_height; i++) {
        if (CORE.debug_data[i].line_num > max_height) {
            max_height = CORE.debug_data[i].line_num;
        }
    }
    if (line_num >= CORE.debug_height) {
        CORE.debug_height = line_num;
    }
    // Resize debug_data for new data
    CORE.debug_data = (Debug*)realloc(CORE.debug_data, CORE.debug_height * sizeof(Debug));


    CORE.debug_data[line_num].line_num = line_num;
    CORE.debug_data[line_num].title = title;
    CORE.debug_data[line_num].value = value;
}