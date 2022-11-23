
#include "pacman.h"
#include <iostream>

RGB_T Pacman_base::red {0xcc, 0x00, 0x00};
RGB_T Pacman_base::blue {0x00, 0x00, 0xcc};
RGB_T Pacman_base::green {0x00, 0xcc, 0x00};
RGB_T Pacman_base::cyan {0x00, 0xcc, 0xcc}; 
RGB_T Pacman_base::yellow {0xcc, 0xcc, 0x00}; 
RGB_T Pacman_base::white {0xff, 0xff, 0xff}; 
RGB_T Pacman_base::black {0x00, 0x00, 0x00}; 
RGB_T Pacman_base::pink {0xcc, 0x00, 0xcc};
RGB_T Pacman_base::orange {0xcc, 0x66, 0x00};

Pacman_base::Pacman_base() {
    speed_factor = 1.0;
    xy.x = 0;
    xy.y = 0;
    mode = ALIVE_T::DEAD;
    color = Pacman_base::yellow;
    size_factor = 1.0;
}

bool Pacman_base::is_same_cell(SDL_Point ab) {
    if((ab.x == xy.x) && (ab.y == xy.y)) 
    {
        return true;
    }
    else
    {
        return false;
    }
    
}