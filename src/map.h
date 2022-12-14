#ifndef map_H
#define map_H

#include <iostream>
#include <memory>
#include "SDL.h"
#include <vector>
#include "pacman.h"

enum GRID_T { block, open, nouse };
enum FOOD_T { food, superfood, nofood };

class Map {
    public:

        static constexpr std::size_t kScreenSize{608};
        static constexpr std::size_t kGridSize{19};
        static GRID_T blueprint[kGridSize][kGridSize];
        static FOOD_T blueprint_food[kGridSize][kGridSize];

        Map();
        ~Map();
        int get_screen_to_grid_ratio() { return static_cast<int> (kScreenSize/kGridSize); }
        SDL_Point get_screen_to_grid_xy(SDL_Point);
        void set_moving_object(Pacman_base);
        void clear_moving_object(SDL_Point);
        void set_food_copy(Pacman_base);
        void clear_food_copy(SDL_Point);
        void place_food_to_moving_objects();
        SDL_Rect get_sdl_rect(Pacman_base*);
        bool is_valid_path(SDL_Point);
        bool* get_open_path() { return open_path; };
        STRUCT_RET is_background(SDL_Point, SDL_Point, NAME_T);
        bool is_background(SDL_Point, NAME_T); 
        Pacman_base* get_background(int i, int j) { return background[i][j]; }
        Pacman_base* get_moving_objects() { return moving_objects; }
        SDL_Point get_pacman_start_point();
        SDL_Point get_ghost_start_point(int, int);
        Direction calc_shortest_dist(SDL_Point, SDL_Point);

    private:
        Pacman_base* background[kGridSize][kGridSize];
        bool* open_path; //
        Pacman_base* food_copy; //store food information here, so that you can set it again when ghosts move over it
        Pacman_base* moving_objects;
        //Pacman_base moving_objects[kScreenSize];
        
        void init();
        void set_open_path(SDL_Point);
        void generate_food();
        Pacman_base get_background_obj(SDL_Point, uint8_t);
        SDL_Point get_grid_centered(SDL_Point);
        Pacman_base get_background_obj_centered(SDL_Point);
        void conv_background_to_objects();
        STRUCT_RET is_background_x_fix_y_var(int, int, int, NAME_T);
        STRUCT_RET is_background_x_var_y_fix(int, int, int, NAME_T);


};

#endif