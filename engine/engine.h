#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

//======================================================
// Structures Definition
//======================================================

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
    int target_fps;                                                             // Viewport target refresh rate
    double curr_fps;                                                            // Current fps
    int prev_clock_time;                                                        // Previous processor clock time
    bool color_enabled;                                                         // Enable color (Enabled/Disabled)

    // Debug
    WINDOW *debug_menu;                                                         // Debug menu
    Debug *debug_data;                                                          // Debug menu data
    bool debug_enabled;                                                         // Enable debug menu (Enabled/Disabled)
    int debug_height;                                                           // Debug menu height
} CoreData;

//======================================================
// Enumeration Definition
//======================================================
typedef enum {
    KEY_ESC             = 27,        // Key: <Esc>
    KEY_SPACE           = 32,        // Key: <Space>
    KEY_APOSTROPHE      = 39,        // Key: '
    KEY_COMMA           = 44,        // Key: ,
    KEY_MINUS           = 45,        // Key: -
    KEY_PERIOD          = 46,        // Key: .
    KEY_SLASH           = 47,        // Key: /
    KEY_ZERO            = 48,        // Key: 0
    KEY_ONE             = 49,        // Key: 1
    KEY_TWO             = 50,        // Key: 2
    KEY_THREE           = 51,        // Key: 3
    KEY_FOUR            = 52,        // Key: 4
    KEY_FIVE            = 53,        // Key: 5
    KEY_SIX             = 54,        // Key: 6
    KEY_SEVEN           = 55,        // Key: 7
    KEY_EIGHT           = 56,        // Key: 8
    KEY_NINE            = 57,        // Key: 9
    KEY_SEMICOLON       = 59,        // Key: ;
    KEY_EQUAL           = 61,        // Key: =
    KEY_A               = 97,        // Key: A | a
    KEY_B               = 98,        // Key: B | b
    KEY_C               = 99,        // Key: C | c
    KEY_D               = 100,       // Key: D | d
    KEY_E               = 101,       // Key: E | e
    KEY_F               = 102,       // Key: F | f
    KEY_G               = 103,       // Key: G | g
    KEY_H               = 104,       // Key: H | h
    KEY_I               = 105,       // Key: I | i
    KEY_J               = 106,       // Key: J | j
    KEY_K               = 107,       // Key: K | k
    KEY_L               = 108,       // Key: L | l
    KEY_M               = 109,       // Key: M | m
    KEY_N               = 110,       // Key: N | n
    KEY_O               = 111,       // Key: O | o
    KEY_P               = 112,       // Key: P | p
    KEY_Q               = 113,       // Key: Q | q
    KEY_R               = 114,       // Key: R | r
    KEY_S               = 115,       // Key: S | s
    KEY_T               = 116,       // Key: T | t
    KEY_U               = 117,       // Key: U | u
    KEY_V               = 118,       // Key: V | v
    KEY_W               = 119,       // Key: W | w
    KEY_X               = 120,       // Key: X | x
    KEY_Y               = 121,       // Key: Y | y
    KEY_Z               = 122,       // Key: Z | z
    KEY_LEFT_BRACKET    = 91,        // Key: [
    KEY_BACKSLASH       = 92,        // Key: '\'
    KEY_RIGHT_BRACKET   = 93,        // Key: ]
    KEY_GRAVE           = 96,        // Key: `
} KeyboardKey;

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

void setViewport(int width, int height);                                        // Create viewport w/parameters
void setColor();                                                                // Enable color rendering
void setBorder();                                                               // Enable viewport border
void renderViewport();                                                          // Render viewport to terminal
void clearViewport();                                                           // Clear viewport

// Time

void setTargetFPS(uint16_t fps);                                                // Set target refresh rate
//! double getFPS();                                                             // Get current refresh rate
uint16_t getClocktime();                                                        // Return clock time (milliseconds)

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

// Input

int getKey();                                                                   // Get pressed key (ncurses)
void flushInputBuf();                                                           // Flush input buffer (ncurses)

// Debug

void showDebug();                                                               // Show debug menu
void hideDebug();                                                               // Hide debug menu
void quitDebug();                                                               // Quit debug menu
void updateDebugAttrib(int line_num, char* title, char* value);                 // Update/Add debug attributes

// todo: add shaders

#endif