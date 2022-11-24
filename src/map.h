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
        static GRID_T blueprint[kGridHeight][kGridWidth];
        Map();
    
        int get_grid_width_size() { return static_cast<uint32_t> (kScreenWidth/kGridWidth); }
        int get_grid_height_size() { return static_cast<uint32_t> (kScreenHeight/kGridHeight); }
        void set_moving_object(Pacman_base);
        void clear_moving_object(SDL_Point);
        SDL_Rect sr(SDL_Point, float);
        bool is_map_open(SDL_Point xy);
        //bool is_map_nouse(SDL_Point xy) {return blueprint[xy.y][xy.x] == GRID_T::nouse; };
        bool is_background_food(SDL_Point); 
        bool is_background_superfood(SDL_Point);
        bool is_background_ghost(SDL_Point);
        Pacman_base* get_background() { return background[0]; }
        SDL_Point get_pacman_start_point();
        SDL_Point get_ghost_start_point(int,int);
        SDL_Point get_super_food_point(int);

    private:
        Pacman_base background[kGridHeight][kGridWidth];
        std::vector<Pacman_base> moving_objects;
        void conv_background_to_objects();
        Pacman_base backobj;
        SDL_Point super_food_xy[4] { {1,3}, {17,3}, {1, 15}, {17,15}};

};

#endif