
#include "pacman.h"
#include "map.h"
#include <iostream>

RGB_T Pacman_base::red {0xcc, 0x00, 0x00};
RGB_T Pacman_base::purple {0x33, 0x00, 0x66};
RGB_T Pacman_base::blue {0x00, 0x00, 0xcc};
RGB_T Pacman_base::green {0x00, 0xcc, 0x00};
RGB_T Pacman_base::cyan {0x00, 0xcc, 0xcc}; 
RGB_T Pacman_base::yellow {0xcc, 0xcc, 0x00}; 
RGB_T Pacman_base::white {0xff, 0xff, 0xff}; 
RGB_T Pacman_base::black {0x00, 0x00, 0x00}; 
RGB_T Pacman_base::pink {0xcc, 0x00, 0xcc};
RGB_T Pacman_base::orange {0xcc, 0x66, 0x00};
uint8_t Pacman_base::update_cnt = 0;

Pacman_base::Pacman_base()  
{
    speed_factor = 0;
    xy = SDL_Point{0,0};
    mode = ALIVE_T::DEAD;
    color = Pacman_base::yellow;
    name = NAME_T::BACKGROUND;
    size_factor = 1;
    direction = Direction::noChange;
    direction_latch = Direction::noChange;
}

Pacman_base::Pacman_base(NAME_T name_t, uint8_t speed_f, uint8_t size_f, SDL_Point ab, ALIVE_T alive_t, RGB_T rgb_t) 
{
    name = name_t;
    speed_factor = speed_f;
    xy = ab;
    prev_xy = ab;
    mode = alive_t; 
    ghost_mode = CHASE;
    color = rgb_t;
    prev_color = rgb_t;
    size_factor = size_f;
    direction = Direction::noChange;
    direction_latch = Direction::noChange;

    /* initialize random seed: */
    srand (time(NULL));
}

bool Pacman_base::is_same_location(Pacman_base *pb) {
    int x_delta = abs(xy.x-pb->xy.x);
    int y_delta = abs(xy.y-pb->xy.y);
    if((x_delta == 0) && ((y_delta < this->size_factor) || (y_delta < pb->size_factor)))
    {
        return true;
    }
    else if((y_delta == 0) && ((x_delta < this->size_factor) || (x_delta < pb->size_factor)))
    {
         return true;
    }
    return false;
}

void Pacman_base::set_direction(Direction dir) {
    if(dir != Direction::noChange)
    {
        direction = dir;

        if(direction_latch == Direction::noChange){
            direction_latch = direction;
        }
    }
}

STRUCT_RET Pacman_base::check_if_pacman_is_near_intersection(SDL_Point ab, Direction dir, Map& map)
{
    STRUCT_RET _return {false, SDL_Point{0,0}};
    int x_update;
    int y_update;

    // std::cout << "==================================================================" <<std::endl;
    // std::cout << "Check Intersection [" << ab.y << "][" << ab.x << "] is intersection"  << std::endl;

    switch (dir) {
        case Direction::kUp:
            for(int x = -1; x <= 1; x++)
            {
                if(x==0) continue;
                x_update = ab.x + x;
                for(int y = 0; y >= -speed_factor; --y)
                {
                    y_update = ab.y + y;
                    if(map.is_valid_path(SDL_Point{x_update, y_update}))
                    {
                        _return.result = true;
                        _return.xy.x = ab.x; //do not update, just bring xy at interesection
                        _return.xy.y = y_update;
                        //std::cout << "[" << _return.xy.y << "][" << _return.xy.x << "] is intersection"  << std::endl;
                        return _return;
                    }
                }
            }
            break;
        case Direction::kDown:
            for(int x = -1; x <= 1; x++)
            {
                if(x==0) continue;
                x_update = ab.x + x;
                for(int y = 0; y <= speed_factor; y++)
                {
                    y_update = ab.y + y;
                    if(map.is_valid_path(SDL_Point{x_update, y_update}))
                    {
                        _return.result = true;
                        _return.xy.x = ab.x; //do not update, just bring xy at interesection
                        _return.xy.y = y_update;
                        //std::cout << "[" << _return.xy.y << "][" << _return.xy.x << "] is intersection"  << std::endl;
                        return _return;
                    }
                }
            }
            break;
        case Direction::kLeft:
            for(int y = -1; y <= 1; y++)
            {
                if(y==0) continue; 
                y_update = ab.y + y;
                for(int x = 0; x >= -speed_factor; --x)
                {
                    x_update = ab.x + x;
                    if(map.is_valid_path(SDL_Point{x_update, y_update}))
                    {
                        _return.result = true;
                        _return.xy.x = x_update;
                        _return.xy.y = ab.y; //do not update, just bring xy at interesection
                        //std::cout << "[" << _return.xy.y << "][" << _return.xy.x << "] is intersection"  << std::endl;
                        return _return;
                    }
                }
            }
            break;
        case Direction::kRight:
            for(int y = -1; y <= 1; y++)
            {
                if(y==0) continue; 
                y_update = ab.y + y;
                for(int x = 0; x <= speed_factor; x++)
                {
                    x_update = ab.x + x;
                    if(map.is_valid_path(SDL_Point{x_update, y_update}))
                    {
                        _return.result = true;
                        _return.xy.x = x_update;
                        _return.xy.y = ab.y; //do not update, just bring xy at interesection
                        //std::cout << "[" << _return.xy.y << "][" << _return.xy.x << "] is intersection"  << std::endl;
                        return _return;
                    }
                }
            }
            break;
    }
    return _return;
}

STRUCT_RET Pacman_base::check_if_pacman_can_move(SDL_Point ab, Direction dir, Map& map)
{
    STRUCT_RET _return {false, SDL_Point{0,0}};
    int update_x;
    int update_y;

    // std::cout << "==================================================================" <<std::endl;
    // std::cout << "Check if pacman can move [" << ab.y << "][" << ab.x << "]"  << std::endl;

    switch (dir) {
        case Direction::kUp:
            for (int y = int(speed_factor); y > 0; --y)
            {
                update_y = ab.y - y;
                if(map.is_valid_path(SDL_Point{ab.x, update_y}))
                {
                    _return.result = true;
                    _return.xy.x = ab.x;
                    _return.xy.y = update_y;
                    //std::cout << "Pacman moving [" << _return.xy.y << "][" << _return.xy.x << "]"  << std::endl;
                    break;
                }
            }
            break;

        case Direction::kDown:
            for (int y = int(speed_factor); y > 0; --y)
            {
                update_y = ab.y + y;
                if(map.is_valid_path(SDL_Point{ab.x, update_y}))
                {
                    _return.result = true;
                    _return.xy.x = ab.x;
                    _return.xy.y = update_y;
                    //std::cout << "Pacman moving [" << _return.xy.y << "][" << _return.xy.x << "]"  << std::endl;
                    break;
                }
            }
            break;
        case Direction::kLeft:
            for (int x = int(speed_factor); x > 0; --x)
            {
                update_x = ab.x - x;
                if(map.is_valid_path(SDL_Point{update_x, ab.y}))
                {
                    // std::cout << "[" << ab.y << "][" << update_x << "] is open"  << std::endl;
                    _return.result = true;
                    _return.xy.x = update_x;
                    _return.xy.y = ab.y;
                    //std::cout << "Pacman moving [" << _return.xy.y << "][" << _return.xy.x << "]"  << std::endl;
                    break;
                }
            }
            break;
        case Direction::kRight:
            for (int x = int(speed_factor); x > 0; --x)
            {
                update_x = ab.x + x; //find the next pixel right
                if(map.is_valid_path(SDL_Point{update_x, ab.y}))
                {
                    // std::cout << "[" << ab.y << "][" << update_x << "] is open"  << std::endl;
                    _return.result = true;
                    _return.xy.x = update_x;
                    _return.xy.y = ab.y;
                    //std::cout << "Pacman moving [" << _return.xy.y << "][" << _return.xy.x << "]"  << std::endl;
                    break;
                }
            }
            break;
    }
    return _return;
}

void Pacman_base::update(Map &map) {

    //store last xy
    prev_xy = xy;

    //     std::cout << "===========================================================================" << std::endl;
    //     std::cout << lcl_dir << " Start [" << xy.y << "][" << xy.x << "] " << std::endl;

    if(direction != direction_latch)
    {
        STRUCT_RET result1 = check_if_pacman_is_near_intersection(xy, direction_latch, map);
        if(result1.result)
        {
            //check if pacman can move to new direction
            STRUCT_RET result2 = check_if_pacman_can_move(result1.xy, direction, map);
            if(result2.result)
            {
                xy = result2.xy;
                direction_latch = direction;
                return;
            }
        }
        else 
        {
            STRUCT_RET result3 = check_if_pacman_can_move(xy, direction, map);
            if(result3.result)
            {
                xy = result3.xy;
                direction_latch = direction;
                return;
            }

        }
    }

    STRUCT_RET result = check_if_pacman_can_move(xy, direction_latch, map);
    if(result.result)
    {
        xy = result.xy;
    }
    else 
    {
        direction_latch = Direction::noChange;
    }

}

SDL_Point Pacman_base::find_nearest_offset(Map &map, SDL_Point ab, int offset_min, int offset_max, Direction dir)
{
    //check if the manhuttan distance is less than offset, just goto ab
    if((abs(xy.x-ab.x)+abs(xy.y-ab.y)) < offset_min )
    {
        return ab;
    }

    switch (dir)
    {
        case Direction::kRight:
        {
            for (int i = offset_min; i <= offset_max; ++i)
            {
                int update_x = ( ab.x+i ) % map.kScreenSize;
                SDL_Point ab_offset {update_x, ab.y};
                if (map.is_valid_path(ab_offset))
                {
                    return ab_offset;
                }
            }
            break;
        }
        case Direction::kLeft:
        {
            for (int i = offset_min; i <= offset_max; ++i)
            {
                int update_x = abs(ab.x - i);
                SDL_Point ab_offset {ab.x-i, ab.y};
                if (map.is_valid_path(ab_offset))
                {
                    return ab_offset;
                }
            }
            break;
        }
        case Direction::kUp:
        {
            for (int i = offset_min; i <= offset_max; ++i)
            {
                int update_y = abs(ab.y - i);
                SDL_Point ab_offset {ab.x, update_y};
                if (map.is_valid_path(ab_offset))
                {
                    return ab_offset;
                }
            }
            break;
        }
        case Direction::kDown:
        {
            for (int i = offset_min; i <= offset_max; ++i)
            {
                int update_y = ( ab.y + i ) % map.kScreenSize;
                SDL_Point ab_offset {ab.x, update_y};
                if (map.is_valid_path(ab_offset))
                {
                    return ab_offset;
                }
            }
            break;
        }
    }
    //std::cout << "nearest offset " << ab.y << " " << ab.x << std::endl;
    return ab;

}

void Pacman_base::update(Map &map, SDL_Point pacman_xy) {

    //int last_update;
    prev_xy = xy;

    // std::cout << "===========================================================================" << std::endl;
    // std::cout << "Ghost Start " <<  direction_latch << "[" << xy.y << "][" << xy.x << "] " << std::endl;

    STRUCT_RET result_intersect = check_if_pacman_is_near_intersection(xy, direction_latch, map);
    // std::cout << "Ghost intersection check " <<  result_intersect.result << "[" << result_intersect.xy.y << "][" << result_intersect.xy.x << "] " << std::endl;

    bool intesect_dir_change = false;

    if(result_intersect.result)
    {
        Direction temp_dir = map.calc_shortest_dist(result_intersect.xy, pacman_xy);
        // std::cout << "Ghost shortest distance check " <<  temp_dir << "[" << result_intersect.xy.y << "][" << result_intersect.xy.x << "] " << std::endl;

        switch (direction_latch)
        {
            case Direction::kDown:
            case Direction::kUp:
                if((temp_dir == Direction::kLeft) || (temp_dir == Direction::kRight))
                {
                    direction_latch = temp_dir;
                    xy = result_intersect.xy;
                    intesect_dir_change = true;
                }
                break;
            case Direction::kLeft:
            case Direction::kRight:
                if((temp_dir == Direction::kUp) || (temp_dir == Direction::kDown))
                {
                    direction_latch = temp_dir;
                    xy = result_intersect.xy;
                    intesect_dir_change = true;
                }
                break;

        }
    }

    STRUCT_RET result_move = check_if_pacman_can_move(xy, direction_latch, map);
    // std::cout << "Ghost move check " <<  result_move.result << "[" << result_move.xy.y << "][" << result_move.xy.x << "] " << std::endl;

    if(result_move.result && !intesect_dir_change)
    {
        xy = result_move.xy;
    }
    else 
    {
        direction_latch = map.calc_shortest_dist(xy, pacman_xy);
        // std::cout << "Ghost shortest distance2 " <<  direction_latch << std::endl;
        STRUCT_RET result_move2 = check_if_pacman_can_move(xy, direction_latch, map);
        // std::cout << "Ghost can move result2 " <<  result_move2.result << "[" << result_move2.xy.y << "][" << result_move2.xy.x << "] " << std::endl;
        if(result_move2.result)
        {
            xy = result_move2.xy;
        }
        else{
            std::cout << "Error: " << result_move.result << " " << direction_latch << std::endl;
        }
    }
}

void Pacman_base::rand_update(Map &map) 
{
    //int last_update;
    prev_xy = xy;

    // std::cout << "===========================================================================" << std::endl;
    // std::cout << "Ghost Start " <<  direction_latch << "[" << xy.y << "][" << xy.x << "] " << std::endl;

    while(direction_latch == noChange)
    {
        Direction temp_dir = static_cast<Direction> (rand()%noChange); //(Direction) random_dir(engine);

        STRUCT_RET result_move_init = check_if_pacman_can_move(xy, temp_dir, map);
        // std::cout << "Ghost move init check " <<  result_move_init.result << "[" << result_move_init.xy.y << "][" << result_move_init.xy.x << "] " << std::endl;

        if(result_move_init.result)
        {
            // std::cout << " changing directions from " << direction_latch << " to " << temp_dir << std::endl;
            direction_latch = temp_dir;
            xy = result_move_init.xy;
        }
        // std::cout << "ghost init direction " << direction_latch << std::endl;
    }

    STRUCT_RET result_intersect = check_if_pacman_is_near_intersection(xy, direction_latch, map);
    // std::cout << "Ghost intersection check " <<  result_intersect.result << "[" << result_intersect.xy.y << "][" << result_intersect.xy.x << "] " << std::endl;

    bool intersect_dir_change = false;

    if(result_intersect.result)
    {
        while(!intersect_dir_change)
        {
            Direction temp_dir = static_cast<Direction> (rand()%noChange);

            if((direction_latch == Direction::kDown) && (temp_dir == Direction::kUp)) { continue; }
            if((direction_latch == Direction::kUp) && (temp_dir == Direction::kDown)) { continue; }
            if((direction_latch == Direction::kLeft) && (temp_dir == Direction::kRight)) { continue; }
            if((direction_latch == Direction::kRight) && (temp_dir == Direction::kLeft)) { continue; }

            STRUCT_RET result_move = check_if_pacman_can_move(result_intersect.xy, temp_dir, map);
            // std::cout << "Ghost move check " <<  result_move.result << "[" << result_move.xy.y << "][" << result_move.xy.x << "] " << std::endl;

            if(result_move.result)
            {
                // std::cout << " changing directions from " << direction_latch << " to " << temp_dir << std::endl;
                direction_latch = temp_dir;
                xy = result_move.xy;
                intersect_dir_change = true;
            }
            // std::cout << "ghost rand direction " << temp_dir << std::endl;
        }
    }

    STRUCT_RET result_move2 = check_if_pacman_can_move(xy, direction_latch, map);
    // std::cout << "Ghost move2 check " <<  result_move2.result << "[" << result_move2.xy.y << "][" << result_move2.xy.x << "] " << std::endl;

    if(result_move2.result && !intersect_dir_change)
    {
        xy = result_move2.xy;
    }
    else if (!intersect_dir_change)
    {
        std::cout << "Error: " << result_move2.result << " " << direction_latch << std::endl;
    }
    // std::cout << "leaving with [" << xy.y << "][" << xy.x << "] " << std::endl;

}