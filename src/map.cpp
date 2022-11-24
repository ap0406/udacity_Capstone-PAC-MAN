#include "map.h"
#include <iostream>
#include "SDL.h"

Map::Map() {
    conv_background_to_objects();
}

GRID_T Map::blueprint[kGridHeight][kGridWidth] = 
    {
        { GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block }
    };

// SDL_Rect Map::sr(SDL_Point xy, float size_factor) {
//     SDL_Rect r;
//     r.x = (xy.x * get_grid_width_size()) + int(get_grid_width_size() / 2) - int(get_grid_width_size() * size_factor / 2 );
//     r.y = (xy.y * get_grid_height_size()) + int(get_grid_height_size() / 2) - int(get_grid_height_size() * size_factor / 2 );
//     r.w = int(1 * get_grid_width_size() * size_factor);
//     r.h = int(1 * get_grid_height_size() * size_factor);
//     return r;
// }
SDL_Rect Map::sr(SDL_Point xy, float size_factor) {
    SDL_Rect r;
    int x = int( xy.x / get_grid_width_size() ) * get_grid_width_size();
    int y = int( xy.y / get_grid_height_size() ) * get_grid_height_size();
    r.x = x + int(get_grid_width_size() / 2) - int(get_grid_width_size() * size_factor / 2 );
    r.y = y + int(get_grid_height_size() / 2) - int(get_grid_height_size() * size_factor / 2 );
    // r.x = xy.x + int(get_grid_width_size() / 2) - int(get_grid_width_size() * size_factor / 2 );
    // r.y = xy.y + int(get_grid_height_size() / 2) - int(get_grid_height_size() * size_factor / 2 );
    r.w = int(1 * get_grid_width_size() * size_factor);
    r.h = int(1 * get_grid_height_size() * size_factor);
    return r;
}

void Map::conv_background_to_objects() {
    
    for(int i = 0; i < kGridHeight; ++i) {
        for(int j = 0; j < kGridWidth; ++j) {
            int x_scaled = j * get_grid_width_size();
            int y_scaled = i * get_grid_height_size();
            backobj = Pacman_base(NAME_T::BACKGROUND, 0.0, 1.0, SDL_Point{x_scaled,y_scaled}, ALIVE_T::DEAD, Pacman_base::black);
            background[i][j] = backobj;
            if ( blueprint[i][j] == GRID_T::block ) {
                background[i][j].color = Pacman_base::blue;
                background[i][j].name = NAME_T::BLOCK;
            }
            else if ( blueprint[i][j] == GRID_T::nouse ) {
                background[i][j].name = NAME_T::BLOCK;
            }
            
        }
    }
}

SDL_Point Map::get_pacman_start_point() { 
    SDL_Point xy {9,11};
    int x = xy.x * get_grid_width_size();
    int y = xy.y * get_grid_height_size(); 
    return SDL_Point{x,y}; 
}

SDL_Point Map::get_ghost_start_point(int x, int y)  {
    SDL_Point xy {9+x,9+y};
    int x_scaled = xy.x * get_grid_width_size();
    int y_scaled = xy.y * get_grid_height_size();
    return SDL_Point{x_scaled,y_scaled}; 
}

SDL_Point Map::get_super_food_point(int i)  {
    int x_scaled = super_food_xy[i].x * get_grid_width_size();
    int y_scaled = super_food_xy[i].y * get_grid_height_size();
    return SDL_Point{x_scaled,y_scaled}; 
    // for (auto i = 0 ; i < 4 ; ++i)
    // {
    //     xy[i].x = xy[i].x * get_grid_width_size();
    //     xy[i].y = xy[i].y * get_grid_height_size();
    //     std::cout << "sf: " << xy[i].x << " " << xy[i].y << std::endl;
    // }
    // return xy[0]; 
}

void Map::set_moving_object(Pacman_base pb) {
    int x_scaled = int( pb.xy.x / get_grid_width_size() ); 
    int y_scaled = int( pb.xy.y / get_grid_height_size() );
    background[y_scaled][x_scaled] = pb;
    //std::cout << "setting moving object: " << y_scaled << " " << x_scaled << " " << pb.name << std::endl;
};

void Map::clear_moving_object(SDL_Point xy) {
    int x_scaled = int( xy.x / get_grid_width_size() ); 
    int y_scaled = int( xy.y / get_grid_height_size() );
    background[y_scaled][x_scaled] = backobj;
};

bool Map::is_background_food(SDL_Point xy) {
    int x_scaled = int( xy.x / get_grid_width_size() ); 
    int y_scaled = int( xy.y / get_grid_height_size() );
    return background[y_scaled][x_scaled].name == NAME_T::FOOD; 
};

bool Map::is_background_superfood(SDL_Point xy) {
    int x_scaled = int( xy.x / get_grid_width_size() ); 
    int y_scaled = int( xy.y / get_grid_height_size() );
    return background[y_scaled][x_scaled].name == NAME_T::SUPERFOOD; 
};

bool Map::is_background_ghost(SDL_Point xy) { 
    int x_scaled = int( xy.x / get_grid_width_size() ); 
    int y_scaled = int( xy.y / get_grid_height_size() );
    return background[y_scaled][x_scaled].name == NAME_T::GHOST; 
};

bool Map::is_map_open(SDL_Point xy) {
    int x_scaled = int( xy.x / get_grid_width_size() ); 
    int y_scaled = int( xy.y / get_grid_height_size() );
    return background[y_scaled][x_scaled].name != NAME_T::BLOCK; 
};
