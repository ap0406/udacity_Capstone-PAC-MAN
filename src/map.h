#ifndef map_H
#define map_H

#include <iostream>
#include <memory>
#include "SDL.h"
#include <vector>
#include "pacman.h"

enum GRID_T { block, open, nouse };

class Map {
    public:

        static constexpr std::size_t kScreenSize{608};
        static constexpr std::size_t kGridSize{19};
        static GRID_T blueprint[kGridSize][kGridSize];

        Map();
        ~Map();
        int get_screen_to_grid_ratio() { return static_cast<int> (kScreenSize/kGridSize); }
        void set_moving_object(Pacman_base);
        void clear_moving_object(SDL_Point);
        SDL_Rect get_sdl_rect(Pacman_base*);
        bool is_valid_path(SDL_Point);
        bool* get_open_path() { return open_path; };
        bool is_background_food(SDL_Point); 
        bool is_background_superfood(SDL_Point);
        bool is_background_ghost(SDL_Point);
        Pacman_base* get_background(int i, int j) { return background[i][j]; }
        Pacman_base* get_moving_objects() { return moving_objects; }
        SDL_Point get_pacman_start_point();
        // SDL_Point get_ghost_start_point(int,int);
        // SDL_Point get_super_food_point(int);

    private:
        Pacman_base* background[kGridSize][kGridSize];
        bool* open_path; //[kScreenSize][kScreenSize];
        Pacman_base* moving_objects;
        //Pacman_base moving_objects[kScreenSize];
        
        void init();
        void set_open_path(SDL_Point);
        Pacman_base get_background_obj(SDL_Point, uint8_t);
        SDL_Point get_grid_centered(SDL_Point);
        Pacman_base get_background_obj_centered(SDL_Point);
        void conv_background_to_objects();
        // Pacman_base backobj;
        // SDL_Point super_food_xy[4] { {1,3}, {17,3}, {1, 15}, {17,15}};

};

#endif