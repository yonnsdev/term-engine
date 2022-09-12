<div align="center"> 
  <h3>Terminal Game Engine</h3>
  <h1>Term Engine</h1>

[![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)](#)
[![Terminal](https://img.shields.io/badge/Terminal-%234D4D4D.svg?style=for-the-badge&logo=windows-terminal&logoColor=white)](#)

</div>

## Features
- Terminal color support
- (Not much yet)

## Basic example
```c
#include "engine.h"

int main() {
    initEngine();
    
    setViewport(60, 30);
    setTargetFPS(12);
    setColor();
    setBorder();
    
    // Replace with custom loop ending condition
    while (true) {
        clearViewport();
        
        drawCircleT(30, 15, 5 false, '#', COLOR_CYAN);

        renderViewport();
    }
    
    deinitEngine();
    return 0;
}
```

## Installation
1. Clone repository
```
git clone https://github.com/yonnsdev/term-engine.git
```
2. Move "engine" folder to project directory
3. Build using `make`
4. Include to project
```
(example makefile)

INCFLAGS = -Iengine
LDFLAGS  = -lncurses
LDFLAGS += engine/ecore.o
LDFLAGS += engine/eshapes.o
```
## Cheatsheet
```c
// Initialization
void initEngine();                            // Init engine
void deinitEngine();                          // Deinit engine

// Viewport
void setViewport(int width, int height);      // Create viewport w/parameters
void setColor();                              // Enable color rendering
void setBorder();                             // Enable viewport border
void renderViewport();                        // Render viewport to terminal
void clearViewport();                         // Clear viewport

// Time
void setTargetFPS(int fps);                   // Set target refresh rate
ulong_t getClocktime();                       // Return clock time (milliseconds)

// Draw
void drawPixel(int precise_x, int precise_y, char ch, int color);               // Draw pixel "#"
void drawPoint(int x, int y, char ch, int color);                               // Draw point "##"
void drawLine(int x1, int y1, int x2, int y2, char ch, int color);              // Draw line
void drawCircle(int x, int y, int r, bool fill, char ch, int color);            // Draw circle
void drawCircleT(Circle circ, bool fill, char ch, int color);                   // Draw circle with circle type
void drawRectangle(int x, int y, int w, int h, bool fill, char ch, int color);  // Draw rectangle
void drawRectangleT(Rectangle rect, bool fill, char ch, int color);             // Draw rectangle with rectangle type

// Collision
bool checkCollisionPointRect(Vector2 point, Rectangle rect);      // Check collision between point and rectangle
bool checkCollisionPointCirc(Vector2 point, Circle circ);         // Check collision between point and circle
bool checkCollisionRects(Rectangle rect1, Rectangle rect2);       // Check collision between two rectangles

// Debug
void showDebug();                                                 // Show debug menu
void hideDebug();                                                 // Hide debug menu
void quitDebug();                                                 // Quit debug menu
void updateDebugAttrib(int line_num, char* title, char* value);   // Update/Add debug attributes
```
