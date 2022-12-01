
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
uint8_t Pacman_base::update_cnt = 0;

Pacman_base::Pacman_base() {
    speed_factor = 0;
    xy = SDL_Point{0,0};
    mode = ALIVE_T::DEAD;
    color = Pacman_base::yellow;
    name = NAME_T::BACKGROUND;
    size_factor = 1;
    direction = Direction::noChange;
    prev_direction = Direction::noChange;
    // for(uint8_t i = 0; i < kDirectionBufferSize; ++i)
    // {
    //     prev_direction[i] = Direction::noChange; 
    // }
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
    // for(uint8_t i = 0; i < kDirectionBufferSize; ++i)
    // {
    //     prev_direction[i] = Direction::noChange; 
    // }
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
    int update_x;
    int update_y;
    bool latch_valid_path = false;
    Direction lcl_dir = Direction::noChange;

    //int last_update;
    prev_xy = xy;
    
    //use of history buffer to smooth out movement
    // if(direction == Direction::noChange){
    //     lcl_dir = prev_direction[update_cnt]; //look at the last value in history bufer
    // }
    // else {
    //     lcl_dir = direction;
    // }
    if(direction == Direction::noChange)
    {
        lcl_dir = prev_direction;
    }
    else 
    {
        lcl_dir = direction;
    }

    //lcl_dir = direction;

    // if(lcl_dir != Direction::noChange)
    // {
    //     std::cout << "===========================================================================" << std::endl;
    //     std::cout << lcl_dir << " Start [" << xy.y << "][" << xy.x << "] " << std::endl;
    // }


    switch (lcl_dir) {
        case Direction::kUp:
            for (int y = int(speed_factor); y > 0; --y)
            {
                update_y = xy.y - y;
                if(map.is_valid_path(SDL_Point{xy.x, update_y}))
                {
                    // std::cout << "[" << update_y << "][" << xy.x << "] is open"  << std::endl;
                    xy.y = update_y;
                    latch_valid_path = true;
                    break;
                }
            }
            //smooth out the control, align with next possible horizontal point
            if (!latch_valid_path)
            {
                update_y = xy.y - 1; //find the next pixel up
                for(int x = int(kAlignFactor * speed_factor); x > -int(kAlignFactor * speed_factor); --x)
                {
                    update_x = xy.x + x;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        xy.x = update_x;
                        xy.y = update_y;
                        break;
                    }
                }
            }
            break;

        case Direction::kDown:
            for (int y = int(speed_factor); y > 0; --y)
            {
                update_y = xy.y + y;
                if(map.is_valid_path(SDL_Point{xy.x, update_y}))
                {
                    // std::cout << "[" << update_y << "][" << xy.x << "] is open"  << std::endl;
                    xy.y = update_y;
                    latch_valid_path = true;
                    break;
                }
            }
            //smooth out the control, align with next possible horizontal point
            if (!latch_valid_path)
            {
                update_y = xy.y + 1; //find the next pixel down
                for(int x = int(kAlignFactor * speed_factor); x > -int(kAlignFactor * speed_factor); --x)
                {
                    update_x = xy.x + x;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        xy.x = update_x; 
                        xy.y = update_y;
                        break;
                    }
                }
            }
            break;
        case Direction::kLeft:
            for (int x = int(speed_factor); x > 0; --x)
            {
                update_x = xy.x - x;
                if(map.is_valid_path(SDL_Point{update_x, xy.y}))
                {
                    // std::cout << "[" << xy.y << "][" << update_x << "] is open"  << std::endl;
                    xy.x = update_x;
                    latch_valid_path = true;
                    break;
                }
            }
            if (!latch_valid_path)
            {
                update_x = xy.x - 1; //find the next pixel left
                for(int y = int(kAlignFactor * speed_factor); y > -int(kAlignFactor * speed_factor); --y)
                {
                    update_y = xy.y + y;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        xy.y = update_y; 
                        xy.x = update_x;
                        break;
                    }
                }
            }
            break;
        case Direction::kRight:
            for (int x = int(speed_factor); x > 0; --x)
            {
                update_x = xy.x + x; //find the next pixel right
                if(map.is_valid_path(SDL_Point{update_x, xy.y}))
                {
                    // std::cout << "[" << xy.y << "][" << update_x << "] is open"  << std::endl;
                    xy.x = update_x;
                    latch_valid_path = true;
                    break;
                }
            }
            if (!latch_valid_path)
            {
                update_x = xy.x + 1;
                for(int y = int(kAlignFactor * speed_factor); y > -int(kAlignFactor * speed_factor); --y)
                {
                    update_y = xy.y + y;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        xy.y = update_y;
                        xy.x = update_x;
                        break;
                    }
                }
            }
            break;
    }

    // if(lcl_dir != Direction::noChange)
    // {
    //     std::cout << lcl_dir << " End [" << xy.y << "][" << xy.x << "] " << std::endl;
    // }

    prev_direction = direction;
    // prev_direction[update_cnt] = direction;
    // ++update_cnt;
    // if(update_cnt == kDirectionBufferSize) { update_cnt = 0; }
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