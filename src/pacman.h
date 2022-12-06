#ifndef PACMAN_H
#define PACMAN_H

#include <vector>
#include "SDL.h"
#include <random>

struct RGB_T { uint8_t r,g,b; };
enum ALIVE_T { LIVE, DEAD };
enum Direction { kUp, kDown, kLeft, kRight, noChange };
enum NAME_T { FOOD, SUPERFOOD, PACMAN, GHOST, BACKGROUND };
enum GHOST_MODE_T { CHASE, RUNAWAY };

struct STRUCT_RET
{
    bool result;
    SDL_Point xy;

};

//forward declaration
class Map;

class Pacman_base {
    public:

        static RGB_T red;
        static RGB_T purple;
        static RGB_T blue;
        static RGB_T green;
        static RGB_T cyan;
        static RGB_T yellow;
        static RGB_T white;
        static RGB_T black;
        static RGB_T pink;
        static RGB_T orange;
        static uint8_t update_cnt;
        static constexpr uint8_t kDirectionBufferSize {5};
        static constexpr uint8_t kAlignFactor {5};

        uint8_t speed_factor;
        uint8_t size_factor;
        SDL_Point xy;
        SDL_Point prev_xy; 
        ALIVE_T mode;
        GHOST_MODE_T ghost_mode;
        RGB_T color;
        NAME_T name;
        Direction direction;
        //Direction prev_direction[kDirectionBufferSize];
        Direction direction_latch;
        void set_direction(Direction);
        Pacman_base();
        Pacman_base(NAME_T name_t, uint8_t speed_f, uint8_t size_f, SDL_Point ab, ALIVE_T alive_t, RGB_T rgb_t);
        bool is_same_location(Pacman_base*);
        void update(Map&);
        void update(Map&, SDL_Point);
        void rand_update(Map&);
        SDL_Point find_nearest_offset (Map&, SDL_Point, int, int, Direction);
        void set_new_color(RGB_T rgb) { prev_color = color; color = rgb; }
        void set_prev_color() { color = prev_color; }
        
    private:
        STRUCT_RET check_if_pacman_can_move(SDL_Point, Direction, Map&);
        STRUCT_RET check_if_pacman_is_near_intersection(SDL_Point, Direction, Map&);
        RGB_T prev_color;
        
};

#endif