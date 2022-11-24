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
    
        uint32_t get_grid_width_size() { return static_cast<uint32_t> (kScreenWidth/kGridWidth); }
        uint32_t get_grid_height_size() { return static_cast<uint32_t> (kScreenHeight/kGridHeight); }
        void set_moving_object(Pacman_base pb) {background[pb.xy.y][pb.xy.x] = pb;};
        void clear_moving_object(SDL_Point xy) {background[xy.y][xy.x] = backobj;};
        SDL_Rect sr(SDL_Point, float);
        bool is_map_open(SDL_Point xy) {return blueprint[xy.y][xy.x] == GRID_T::open; };
        bool is_map_nouse(SDL_Point xy) {return blueprint[xy.y][xy.x] == GRID_T::nouse; };
        bool is_background_food(SDL_Point xy) { return background[xy.y][xy.x].name == NAME_T::FOOD; };
        bool is_background_superfood(SDL_Point xy) { return background[xy.y][xy.x].name == NAME_T::SUPERFOOD; };
        bool is_background_ghost(SDL_Point xy) { return background[xy.y][xy.x].name == NAME_T::GHOST; };
        Pacman_base* get_background() { return background[0]; }
        std::vector<Pacman_base>& get_moving_objects() { return moving_objects; }
        SDL_Point get_pacman_start_point() { return SDL_Point{9,11}; }
        SDL_Point get_ghost_start_point(int x=0, int y=0)  { return SDL_Point{9+x,9+y}; }

    private:
        Pacman_base background[kGridHeight][kGridWidth];
        std::vector<Pacman_base> moving_objects;
        void conv_background_to_objects();
        Pacman_base backobj;

};

#endif