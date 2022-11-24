
#include "pacman.h"
#include "map.h"
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
    speed_factor = 0.0;
    xy = SDL_Point{0,0};
    mode = ALIVE_T::DEAD;
    color = Pacman_base::yellow;
    name = NAME_T::BACKGROUND;
    size_factor = 1.0;
    direction = Direction::noChange;
}

Pacman_base::Pacman_base(NAME_T name_t, float speed_f, float size_f, SDL_Point ab, ALIVE_T alive_t, RGB_T rgb_t) {
    name = name_t;
    speed_factor = speed_f;
    xy = ab;
    prev_xy = ab;
    mode = alive_t; 
    color = rgb_t;
    size_factor = size_f;
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

void Pacman_base::update(Map &map) {
    prev_xy = xy;
    switch (direction) {
        case Direction::kUp:
            if(map.is_map_open(SDL_Point{xy.x, xy.y-1}))
            {
                xy.y = xy.y - 1;
            }
            break;
        case Direction::kDown:
            if(map.is_map_open(SDL_Point{xy.x, xy.y+1}))
            {
                xy.y = xy.y + 1;
            }
            break;
        case Direction::kLeft:
            if(map.is_map_open(SDL_Point{xy.x-1, xy.y}))
            {
                xy.x = xy.x - 1;
            }
            break;
        case Direction::kRight:
            if(map.is_map_open(SDL_Point{xy.x+1, xy.y}))
            {
                xy.x = xy.x + 1;
            }
            break;
    }
}

void Pacman_base::update_rand(Map &map, int pos) {
    prev_xy = xy;
    switch (pos) {
        case Direction::kUp:
            if(map.is_map_open(SDL_Point{xy.x, xy.y-1}))
            {
                xy.y = xy.y - 1;
            }
            break;
        case Direction::kDown:
            if(map.is_map_open(SDL_Point{xy.x, xy.y+1}))
            {
                xy.y = xy.y + 1;
            }
            break;
        case Direction::kLeft:
            if(map.is_map_open(SDL_Point{xy.x-1, xy.y}))
            {
                xy.x = xy.x - 1;
            }
            break;
        case Direction::kRight:
            if(map.is_map_open(SDL_Point{xy.x+1, xy.y}))
            {
                xy.x = xy.x + 1;
            }
            break;
    }
}