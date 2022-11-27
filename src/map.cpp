#include "map.h"
#include <iostream>
#include <memory>
#include "SDL.h"

Map::Map() {

    for (int i=0; i<kGridSize; ++i) {
        for (int j=0; j<kGridSize; ++j) {
            background[i][j] = new Pacman_base(); //std::make_shared<Pacman_base>();
            // std::cout << "background  " << background[i][j] << std::endl;
        }
    }

    moving_objects = new Pacman_base[kScreenSize*kScreenSize];
    // std::cout << "background start " << background[0][0] << std::endl;
    // std::cout << "background end " << background[kGridSize-1][kGridSize-1] << std::endl;

    init();
    conv_background_to_objects();
}

Map::~Map() {
    for (int i=0; i<kGridSize; ++i) {
        for (int j=0; j<kGridSize; ++j) {
            delete background[i][j];
        }
    }
    delete moving_objects;
}

GRID_T Map::blueprint[kGridSize][kGridSize] = 
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

void Map::init() {

    for (int i = 0; i < kScreenSize; ++i) {
        for(int j = 0; j < kScreenSize; ++j) {
            open_path[i][j] = false;
        }
    }

    for (int i = 0; i < int(kScreenSize); ++i) {
        for(int j = 0; j < int(kScreenSize); ++j) {
            //int* addr_offset = int(moving_objects) + ( i * kScreenSize * sizeof(Pacman_base) ) + ( j * sizeof(Pacman_base) );
            //std::cout << "moving objects " << "offset " << moving_objects << "[" << i << "][" << j << "]" << std::endl;
            Pacman_base* addr_offset = moving_objects + ( i * kScreenSize ) + j;
            //std::cout << "moving objects offset" << addr_offset << std::endl;
            *addr_offset = get_background_obj(SDL_Point{j,i}, 1); 
        }
    }

    for(int i = 0; i < kGridSize; ++i) {
        for(int j = 0; j < kGridSize; ++j) {
            set_open_path(SDL_Point{j,i});
        }
    }
}

Pacman_base Map::get_background_obj(SDL_Point xy, uint8_t size) {
    return Pacman_base(NAME_T::BACKGROUND, 0, size, xy, ALIVE_T::DEAD, Pacman_base::black);
}

SDL_Point Map::get_grid_centered(SDL_Point xy) {
    int y_scaled = ( ( xy.y + 1 ) * get_screen_to_grid_ratio() ) - int( get_screen_to_grid_ratio() / 2 );
    int x_scaled = ( ( xy.x + 1 ) * get_screen_to_grid_ratio() ) - int( get_screen_to_grid_ratio() / 2 ) ;
    return SDL_Point{x_scaled,y_scaled};
}

Pacman_base Map::get_background_obj_centered(SDL_Point xy) {
    
    SDL_Point xy_scaled {get_grid_centered(xy)};
    uint8_t size = get_screen_to_grid_ratio() / 2; //size in pixels, from center point
    return get_background_obj(xy_scaled, size); 
}

void Map::conv_background_to_objects() {
    for(int i = 0; i < kGridSize; ++i) {
        for(int j = 0; j < kGridSize; ++j) {
            *background[i][j] = get_background_obj_centered(SDL_Point{j,i});
            //std::cout << "background: " << "[" << i << "]" << "[" << j << "]" << " changed to [" << background[i][j]->xy.y << "]" << "[" << background[i][j]->xy.x << "]" << std::endl;
            if ( blueprint[i][j] == GRID_T::block ) {
                background[i][j]->color = Pacman_base::blue;
            }   
        }
    }
}

void Map::set_open_path(SDL_Point xy) {
    if((xy.x+1)<=kGridSize) {
        if((blueprint[xy.y][xy.x] == GRID_T::open) && (blueprint[xy.y][xy.x+1] == GRID_T::open)) {
            int start = (xy.x*get_screen_to_grid_ratio()) + int(get_screen_to_grid_ratio()/2);
            int end = start+get_screen_to_grid_ratio();
            int fixed = (xy.y*get_screen_to_grid_ratio()) + int(get_screen_to_grid_ratio()/2);
            for(int i=start; i < end; ++i)
            {
                open_path[fixed][i] = true;
                //std::cout << "open_path[" << xy.y << "][" << i << "]: " << std::endl;
            }
        }
    }

    if((xy.y+1)<=kGridSize) {
        if((blueprint[xy.y][xy.x] == GRID_T::open) && (blueprint[xy.y+1][xy.x] == GRID_T::open)) {
            int start = (xy.y*get_screen_to_grid_ratio()) + int(get_screen_to_grid_ratio()/2);
            int end = start+get_screen_to_grid_ratio();
            int fixed = (xy.x*get_screen_to_grid_ratio()) + + int(get_screen_to_grid_ratio()/2);
            for(int j=start; j < end; ++j)
            {
                open_path[j][fixed] = true;
                //std::cout << "open_path[" << j << "][" << xy.x << "]: " << std::endl;
            }
        }
    }
}

SDL_Rect Map::get_sdl_rect(Pacman_base* pb) {
    SDL_Rect r;
    r.x = pb->xy.x - int(pb->size_factor);
    r.y = pb->xy.y - int(pb->size_factor);
    r.w = int(2 * pb->size_factor);
    r.h = int(2 * pb->size_factor);
    return r;
}

SDL_Point Map::get_pacman_start_point() { 
    SDL_Point xy {9,11}; //grid xy coordinates
    // int x = xy.x * get_screen_to_grid_ratio();
    // int y = xy.y * get_screen_to_grid_ratio(); 
    return get_grid_centered(xy);
}

// SDL_Point Map::get_ghost_start_point(int x, int y)  {
//     SDL_Point xy {9+x,9+y};
//     int x_scaled = xy.x * get_screen_to_grid_ratio();
//     int y_scaled = xy.y * get_screen_to_grid_ratio();
//     return SDL_Point{x_scaled,y_scaled}; 
// }

// SDL_Point Map::get_super_food_point(int i)  {
//     int x_scaled = super_food_xy[i].x * get_screen_to_grid_ratio();
//     int y_scaled = super_food_xy[i].y * get_screen_to_grid_ratio();
//     return SDL_Point{x_scaled,y_scaled}; 
//     // for (auto i = 0 ; i < 4 ; ++i)
//     // {
//     //     xy[i].x = xy[i].x * get_screen_to_grid_ratio();
//     //     xy[i].y = xy[i].y * get_screen_to_grid_ratio();
//     //     std::cout << "sf: " << xy[i].x << " " << xy[i].y << std::endl;
//     // }
//     // return xy[0]; 
// }

void Map::set_moving_object(Pacman_base pb) {
    Pacman_base* addr_offset = moving_objects + ( pb.xy.y * kScreenSize ) + pb.xy.x;
    //std::cout << "setting moving object old: " << addr_offset << " " << addr_offset->name << std::endl;
    //over write memory with new object
    *addr_offset = pb;
    //std::cout << "setting moving object new: " << addr_offset << " " << addr_offset->name << std::endl;
};

void Map::clear_moving_object(SDL_Point xy) {
    Pacman_base* addr_offset = moving_objects + ( xy.y * kScreenSize ) + xy.x;
    //std::cout << "clear moving object old: " << addr_offset << " " << addr_offset->name << std::endl;
    //over write memory with new object
    *addr_offset = get_background_obj(xy, 1); ;
    //std::cout << "clear moving object new: " << addr_offset << " " << addr_offset->name << std::endl;
};

bool Map::is_background_food(SDL_Point xy) {
    Pacman_base* addr_offset = moving_objects + ( xy.y * kScreenSize ) + xy.x;
    return addr_offset->name == NAME_T::FOOD; 
};

bool Map::is_background_superfood(SDL_Point xy) {
    Pacman_base* addr_offset = moving_objects + ( xy.y * kScreenSize ) + xy.x;
    return addr_offset->name == NAME_T::SUPERFOOD; 
};

bool Map::is_background_ghost(SDL_Point xy) { 
    Pacman_base* addr_offset = moving_objects + ( xy.y * kScreenSize ) + xy.x;
    return addr_offset->name == NAME_T::GHOST; 
};

bool Map::is_path(SDL_Point xy) {
    return open_path[xy.y][xy.x];
};

// bool Map::is_map_open(SDL_Point xy) {
//     return open_path[xy.y][xy.x];
// };
