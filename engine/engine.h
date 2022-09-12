#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

typedef unsigned long ulong_t;

typedef struct Vector2 {
    int x;
    int y;
} Vector2;

typedef struct Vector3 {
    int x;
    int y;
    int z;
} Vector3;

typedef struct Viewport{
    char ch;
    int color; 
} Viewport;

typedef struct Debug {
    int line_num;
    char* title;
    char* value;
} Debug;

typedef struct Circle {
    int x;
    int y;
    int radius;
} Circle;

typedef struct Rectangle {
    int x;
    int y;
    int width;
    int height;
} Rectangle;

typedef struct CoreData{
    // Viewport
    WINDOW *viewport;                                                           // Viewport
    Viewport *viewport_data;                                                    // Viewport data
    int width, height;                                                          // Viewport width & height
    bool border;                                                                // Viewport border (Enabled/Disabled)
    int fps;                                                                    // Viewport refresh rate
    int prev_clock_time;                                                        // Previous processor clock time
    bool color;                                                                 // Color status (Enabled/Disabled)

    // Debug
    WINDOW *debug_menu;                                                         // Debug menu
    Debug *debug_data;                                                          // Debug menu data
    bool debug_enabled;                                                         // Debug menu state (Enabled/Disabled)
    int debug_height;                                                           // Debug menu height
} CoreData;

//======================================================
// Global Variables Definition
//======================================================

static CoreData CORE;

//======================================================
// Functions
//======================================================

// Initialization
void initEngine();                                                              // Init engine
void deinitEngine();                                                            // Deinit engine

// Viewport
void setViewport(int width, int height);                                     // Create viewport w/parameters
void setColor();                                                                // Enable color rendering
void setBorder();                                                               // Enable viewport border
void renderViewport();                                                          // Render viewport to terminal
void clearViewport();                                                           // Clear viewport

// Time
void setTargetFPS(int fps);                                                     // Set target refresh rate
ulong_t getClocktime();                                                         // Return clock time (milliseconds)

// Draw
void drawPixel(int precise_x, int precise_y, char ch, int color);               // Draw pixel "#"
void drawPoint(int x, int y, char ch, int color);                               // Draw point "##"
void drawLine(int x1, int y1, int x2, int y2, char ch, int color);              // Draw line
void drawCircle(int x, int y, int r, bool fill, char ch, int color);            // Draw circle
void drawCircleT(Circle circ, bool fill, char ch, int color);                   // Draw circle with circle type
void drawRectangle(int x, int y, int w, int h, bool fill, char ch, int color);  // Draw rectangle
void drawRectangleT(Rectangle rect, bool fill, char ch, int color);             // Draw rectangle with rectangle type

// Collision
bool checkCollisionPointRect(Vector2 point, Rectangle rect);                    // Check collision between point and rectangle
bool checkCollisionPointCirc(Vector2 point, Circle circ);                       // Check collision between point and circle
bool checkCollisionRects(Rectangle rect1, Rectangle rect2);                     // Check collision between two rectangles

// Debug
void showDebug();                                                               // Show debug menu
void hideDebug();                                                               // Hide debug menu
void quitDebug();                                                               // Quit debug menu
void updateDebugAttrib(int line_num, char* title, char* value);                 // Update debug attributes

#endif