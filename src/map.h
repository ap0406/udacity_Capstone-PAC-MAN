#ifndef map_H
#define map_H

#include <iostream>
#include "SDL.h"
#include <vector>
#include "pacman.h"

enum GRID_T { block, open, nouse };

class Map {
    public:

        static constexpr std::size_t kScreenWidth{608};
        static constexpr std::size_t kScreenHeight{608};
        static constexpr std::size_t kGridWidth{19};
        static constexpr std::size_t kGridHeight{19};
        static GRID_T static_background[kGridHeight][kGridWidth];
        //Map();
        void conv_background_to_objects();
        uint32_t get_grid_width_size() { return static_cast<uint32_t> (kScreenWidth/kGridWidth); }
        uint32_t get_grid_height_size() { return static_cast<uint32_t> (kScreenHeight/kGridHeight); }
        void set_moving_object(Pacman_base);
        void clear_moving_object();
        SDL_Rect sr(SDL_Point, float);
        bool is_map_open(SDL_Point);
        std::vector<Pacman_base>& get_background() { return background; }
        std::vector<Pacman_base>& get_moving_objects() { return moving_objects; }
        SDL_Point get_pacman_start_point() { return SDL_Point{9,11}; }
        SDL_Point get_ghost_start_point(int x=0, int y=0)  { return SDL_Point{9+x,9+y}; }

    private:
        std::vector<Pacman_base> background;
        std::vector<Pacman_base> moving_objects;

};

#endif