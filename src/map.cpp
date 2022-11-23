#include "map.h"
#include <iostream>
#include "SDL.h"

GRID_T Map::static_background[kGridHeight][kGridWidth] = 
    {
        { GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block },
        { GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::block, GRID_T::block },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::nouse, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::block, GRID_T::nouse, GRID_T::block, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
        { GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::open,  GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse, GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::open,  GRID_T::block, GRID_T::nouse, GRID_T::nouse, GRID_T::nouse },
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

SDL_Rect Map::sr(SDL_Point xy, float size_factor) {
    SDL_Rect r;
    r.x = (xy.x * get_grid_width_size()) + int(get_grid_width_size() / 2) - int(get_grid_width_size() * size_factor / 2 );
    r.y = (xy.y * get_grid_height_size()) + int(get_grid_height_size() / 2) - int(get_grid_height_size() * size_factor / 2 );
    r.w = int(1 * get_grid_width_size() * size_factor);
    r.h = int(1 * get_grid_height_size() * size_factor);
    return r;
}
bool Map::is_map_open(SDL_Point xy) {
    if ( static_background[xy.y][xy.x] == GRID_T::open ) 
    {
        return true;
    }
    else
    {
        return false;
    }
}
void Map::conv_background_to_objects() {
    
    for(int i = 0; i < kGridHeight; ++i) {
        for(int j = 0; j < kGridWidth; ++j) {
            Pacman_base pb;
            pb.xy.x = j;
            pb.xy.y = i;
            pb.mode = ALIVE_T::DEAD;
            if ( static_background[i][j] == GRID_T::block ) {
                pb.color = Pacman_base::blue;
            }
            else {
                pb.color = Pacman_base::black;
            }
            background.emplace_back(pb);
        }
    }
}
void Map::clear_moving_object() {
    moving_objects.clear();
}
void Map::set_moving_object(Pacman_base pb) {
    moving_objects.emplace_back(pb);
}