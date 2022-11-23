#ifndef PACMAN_H
#define PACMAN_H

#include <vector>
#include "SDL.h"

struct RGB_T { uint8_t r,g,b; };
enum ALIVE_T { LIVE, DEAD };

class Pacman_base {
    public:

        static RGB_T red;
        static RGB_T blue;
        static RGB_T green;
        static RGB_T cyan;
        static RGB_T yellow;
        static RGB_T white;
        static RGB_T black;
        static RGB_T pink;
        static RGB_T orange;

        float speed_factor;
        float size_factor;
        SDL_Point xy; 
        ALIVE_T mode;
        RGB_T color;
        Pacman_base();
        bool is_same_cell(SDL_Point);
};

#endif