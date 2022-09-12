#include "engine.h"

// Get pressed key (ncurses)
int getKey() {
    return wgetch(stdscr);
}

// Flush input buffer (ncurses)
void flushInputBuf() {
    flushinp();
}