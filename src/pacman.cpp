
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
    direction_latch = Direction::noChange;
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
    direction_latch = Direction::noChange;
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

void Pacman_base::set_direction(Direction dir) {
    if(dir != Direction::noChange)
    {
        direction = dir;

        if(direction_latch == Direction::noChange){
            direction_latch = direction;
        }
    }
}

STRUCT_RET Pacman_base::check_if_pacman_can_change_direction(Direction dir, Map& map)
{
    STRUCT_RET _return {false, SDL_Point{0,0}};
    int update_x;
    int update_y;

    switch (dir) {
        case Direction::kUp:
            for (int y = int(speed_factor); y > 0; --y)
            {
                update_y = xy.y - y;
                if(map.is_valid_path(SDL_Point{xy.x, update_y}))
                {
                    // std::cout << "[" << update_y << "][" << xy.x << "] is open"  << std::endl;
                    _return.result = true;
                    _return.xy.x = xy.x;
                    _return.xy.y = update_y;
                    break;
                }
            }
            //smooth out the control, align with next possible horizontal point
            if (!_return.result)
            {
                update_y = xy.y - 1; //find the next pixel up
                for(int x = int(kAlignFactor * speed_factor); x > -int(kAlignFactor * speed_factor); --x)
                {
                    update_x = xy.x + x;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        _return.result = true;
                        _return.xy.x = update_x;
                        _return.xy.y = update_y;
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
                    //std::cout << "[" << update_y << "][" << xy.x << "] is open"  << std::endl;
                    _return.result = true;
                    _return.xy.x = xy.x;
                    _return.xy.y = update_y;
                    break;
                }
            }
            //smooth out the control, align with next possible horizontal point
            if (!_return.result)
            {
                update_y = xy.y + 1; //find the next pixel down
                for(int x = int(kAlignFactor * speed_factor); x > -int(kAlignFactor * speed_factor); --x)
                {
                    update_x = xy.x + x;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        //std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        _return.result = true;
                        _return.xy.x = update_x;
                        _return.xy.y = update_y;
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
                    _return.result = true;
                    _return.xy.x = update_x;
                    _return.xy.y = xy.y;
                    break;
                }
            }
            if (!_return.result)
            {
                update_x = xy.x - 1; //find the next pixel left
                for(int y = int(kAlignFactor * speed_factor); y > -int(kAlignFactor * speed_factor); --y)
                {
                    update_y = xy.y + y;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        _return.result = true;
                        _return.xy.x = update_x;
                        _return.xy.y = update_y;
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
                    _return.result = true;
                    _return.xy.x = update_x;
                    _return.xy.y = xy.y;
                    break;
                }
            }
            if (!_return.result)
            {
                update_x = xy.x + 1;
                for(int y = int(kAlignFactor * speed_factor); y > -int(kAlignFactor * speed_factor); --y)
                {
                    update_y = xy.y + y;
                    if(map.is_valid_path(SDL_Point{update_x, update_y}))
                    {
                        // std::cout << "[" << update_y << "][" << update_x << "] is open"  << std::endl;
                        _return.result = true;
                        _return.xy.x = update_x;
                        _return.xy.y = update_y;
                        break;
                    }
                }
            }
            break;
    }
    return _return;
}

void Pacman_base::update(Map &map) {

    //int last_update;
    prev_xy = xy;

    //     std::cout << "===========================================================================" << std::endl;
    //     std::cout << lcl_dir << " Start [" << xy.y << "][" << xy.x << "] " << std::endl;

    STRUCT_RET result = check_if_pacman_can_change_direction(direction, map);

    if(result.result)
    {
        direction_latch = direction;
        xy = result.xy;
    }
    else 
    {

        result = check_if_pacman_can_change_direction(direction_latch, map);
        if(result.result)
        {
            xy = result.xy;
        }
    }

    //     std::cout << lcl_dir << " End [" << xy.y << "][" << xy.x << "] " << std::endl;

}

void Pacman_base::update(Map &map, SDL_Point pacman_xy, GHOST_MODE_T mode) {

    //int last_update;
    prev_xy = xy;

    //std::cout << "===========================================================================" << std::endl;
    //std::cout << "Ghost Start " <<  direction_latch << "[" << xy.y << "][" << xy.x << "] " << std::endl;

    STRUCT_RET result = check_if_pacman_can_change_direction(direction_latch, map);

    if(result.result)
    {
        //std::cout << "ghost bouncing back: " << direction_latch << "[" << result.xy.y << "][" << result.xy.x << "] " << std::endl;
        xy = result.xy;
    }
    else 
    {
        direction_latch = map.calc_shortest_dist(xy, pacman_xy);
        result = check_if_pacman_can_change_direction(direction_latch, map);
        if(result.result)
        {
            //std::cout << "ghost found shortest dist: " << direction_latch << "[" << result.xy.y << "][" << result.xy.x << "] "<< std::endl;
            xy = result.xy;
        }
        else{
            std::cout << "Error: " << result.result << " " << direction_latch << std::endl;
        }
    }

    //     std::cout << lcl_dir << " End [" << xy.y << "][" << xy.x << "] " << std::endl;
}

// void Pacman_base::update(Map &map) {

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