#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

typedef struct viewport_t {
    char ch;
    int color; 
} viewport_t;

/*
*  ╔═══════════════════════════════════════════════════╗
*  ║                    Variables                      ║
*  ╚═══════════════════════════════════════════════════╝
*/

viewport_t* viewport;       // Viewport
int v_width, v_height;      // Viewport width & height
int v_fps;                  // Viewport refresh rate
clock_t prev_ct;            // Previous processor clock time


/*
*  ╔═══════════════════════════════════════════════════╗
*  ║                    Functions                      ║
*  ╚═══════════════════════════════════════════════════╝
*/

// ||---------------|| Initialization ||--------------||

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
    prev_ct = clock();
}

// Deinitialize Engine
void deinitEngine() {
    endwin();
}

// ||---------------||    Viewport    ||--------------||

/**
* Set viewport parameters
* @param width  Width of viewport
* @param height  Height of viewport
* @param fc Fill character
*/
void setViewport(int width, int height, char fc) {
    v_width = width;
    v_height = height;

    // Viewport layout = [char | int][char | int][char | int] ···
    // One pixel       = "##"
    viewport = (viewport_t*)malloc((v_width * 2) * v_height * (sizeof(char) + sizeof(int)));
    for (int i=0; i < (v_width * 2) * v_height; i++) {
        viewport[i].ch = 0;
        viewport[i].color = -1;
    }
}

// Render viewport to terminal
void renderViewport() {
    static int tick_count = 0; // ! For debugging

    // Check if window is bigger than rendering viewport
    int w_width, w_height; 
    getmaxyx(stdscr, w_height, w_width);
    if (w_height <= v_height || w_width <= v_width * 2) {
        deinitEngine();
        printf("Exited: Window is smaller than viewport size!");
        exit(0);
    }

    // Render viewport
    for (int i=0; i < (v_width * 2) * v_height; i++) {
        if (viewport[i].ch != 0) {
            mvaddch((int)i / (v_width * 2), i % (v_width * 2), viewport[i].ch);
        }
    }

    mvaddstr(v_height, 0, "tick: "); // ! For debugging
    printw("%d", tick_count++);      // ! For debugging
    prev_ct = clock();
    refresh();

    // Delay to match fps
    while (clock() - prev_ct < CLOCKS_PER_SEC / v_fps);

    // Clear viewport
    clear();
    for (int i=0; i < (v_width * 2) * v_height; i++) {
        viewport[i].ch = 0;
        viewport[i].color = -1;
    }
}

// ||---------------||      Draw      ||--------------||
/**
* (USE CAREFULLY)
* Draw pixel "#"
* @param precise_x Precise x position
* @param precise_y Presice y poisiton
* @param ch Character
* @param color Foreground color
*/
void drawPixel(int precise_x, int precise_y, char ch, int color) {
    viewport[precise_y * (v_width * 2) + precise_x].ch = ch;
    viewport[precise_y * (v_width * 2) + precise_x].color = color;
}

/**
* Draw point "##"
* @param x X position
* @param y Y poisiton
* @param ch Character
* @param color Foreground color
*/
void drawPoint(int x, int y, char ch, int color) {
    viewport[y * (v_width * 2) + (x * 2)].ch = ch;
    viewport[y * (v_width * 2) + (x * 2)].color = color;
    viewport[y * (v_width * 2) + (x * 2 + 1)].ch = ch;
    viewport[y * (v_width * 2) + (x * 2 + 1)].color = color;
}

/**
* Draw line
* @param sx Start x position
* @param sy Start y poisiton
* @param ex End x position
* @param ey End y poisiton
* @param ch Character
* @param color Foreground color
*/
void drawLine(int sx, int sy, int ex, int ey, char ch, int color);

// ||---------------||      Time      ||--------------||

/**
* Set refresh rate
* @param fps FPS
*/
void setFPS(int fps) {
    v_fps = fps;
}

// Return clock time (milliseconds)
unsigned long getClocktime() {
    return clock() / 1000;
}

#endif