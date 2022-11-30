
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
    speed_factor = 0;
    xy = SDL_Point{0,0};
    mode = ALIVE_T::DEAD;
    color = Pacman_base::yellow;
    name = NAME_T::BACKGROUND;
    size_factor = 1;
    direction = Direction::noChange;
    prev_direction = Direction::noChange;
}

Pacman_base::Pacman_base(NAME_T name_t, uint8_t speed_f, uint8_t size_f, SDL_Point ab, ALIVE_T alive_t, RGB_T rgb_t) {
    name = name_t;
    speed_factor = speed_f;
    xy = ab;
    prev_xy = ab;
    mode = alive_t; 
    color = rgb_t;
    size_factor = size_f;
    direction = Direction::noChange;
    prev_direction = Direction::noChange;
}

// bool Pacman_base::is_same_cell(SDL_Point ab) {
//     if((ab.x == xy.x) && (ab.y == xy.y)) 
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
    
// }

void Pacman_base::update(Map &map) {
    int update;
    int last_update;
    prev_xy = xy;
    
    if(prev_direction != direction)
    {
        std::cout << "===========================================================================" << std::endl;
        std::cout << "Start [" << xy.y << "][" << xy.x << "] " << std::endl;
    }

    switch (direction) {
        case Direction::kUp:
            // for (int i = 0 ; i < 5; ++i)
            // {
            //     for (int j = 0; j < 5; ++j)
            //     {
            //         std::cout << "Pacman::update " << direction << " [" << xy.y-2+i << "][" << xy.x-2+j << "] " << map.is_valid_path(SDL_Point{xy.x-2+j, xy.y-2+i}) << std::endl;
            //     }
            // }
            //for (int i = 1; i <= int(speed_factor); ++i){
            for (int i = int(speed_factor); i > 0; --i){
                update = xy.y - i;
                if(map.is_valid_path(SDL_Point{xy.x, update}))
                {
                    std::cout << "[" << update << "][" << xy.x << "] is open"  << std::endl;
                    xy.y = update;
                    break;
                }
                else
                {
                    last_update = update;
                }
            }
            break;
        case Direction::kDown:
            //for (int i = 1; i <= int(speed_factor); ++i){
            for (int i = int(speed_factor); i > 0; --i){
                update = xy.y + i;
                if(map.is_valid_path(SDL_Point{xy.x, update}))
                {
                    std::cout << "[" << update << "][" << xy.x << "] is open"  << std::endl;
                    xy.y = update;
                    break;
                }
                else 
                {
                    last_update = update;
                }
            }
            break;
        case Direction::kLeft:
            //for (int i = 1; i <= int(speed_factor); ++i){
            for (int i = int(speed_factor); i > 0; --i){
                update = xy.x - i;
                if(map.is_valid_path(SDL_Point{update, xy.y}))
                {
                    std::cout << "[" << xy.y << "][" << update << "] is open"  << std::endl;
                    xy.x = update;
                    break;
                }
                else 
                {
                    last_update = update;
                }
            }
            break;
        case Direction::kRight:
            //for (int i = 1; i <= int(speed_factor); ++i){
            for (int i = int(speed_factor); i > 0; --i){
                update = xy.x + i;
                if(map.is_valid_path(SDL_Point{update, xy.y}))
                {
                    std::cout << "[" << xy.y << "][" << update << "] is open"  << std::endl;
                    xy.x = update;
                    break;
                }
                else
                {
                    last_update = update;
                }
            }
            break;
    }

    if(prev_direction != direction)
    {
        std::cout << "End [" << xy.y << "][" << xy.x << "] " << std::endl;
    }

    prev_direction = direction;
}

// void Pacman_base::update_rand(Map &map, int pos) {
//     prev_xy = xy;
//     switch (pos) {
//         case Direction::kUp:
//             if(map.is_map_open(SDL_Point{xy.x, xy.y-1}))
//             {
//                 xy.y = xy.y - 1;
//             }
//             break;
//         case Direction::kDown:
//             if(map.is_map_open(SDL_Point{xy.x, xy.y+1}))
//             {
//                 xy.y = xy.y + 1;
//             }
//             break;
//         case Direction::kLeft:
//             if(map.is_map_open(SDL_Point{xy.x-1, xy.y}))
//             {
//                 xy.x = xy.x - 1;
//             }
//             break;
//         case Direction::kRight:
//             if(map.is_map_open(SDL_Point{xy.x+1, xy.y}))
//             {
//                 xy.x = xy.x + 1;
//             }
//             break;
//     }
// }