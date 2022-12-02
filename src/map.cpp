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

    open_path = new bool[kScreenSize*kScreenSize];
    moving_objects = new Pacman_base[kScreenSize*kScreenSize];
    food_copy = new Pacman_base[kGridSize*kGridSize];
    // std::cout << "background start " << background[0][0] << std::endl;
    // std::cout << "background end " << background[kGridSize-1][kGridSize-1] << std::endl;

    init();
    conv_background_to_objects();
    generate_food();
}

Map::~Map() {
    for (int i=0; i<kGridSize; ++i) {
        for (int j=0; j<kGridSize; ++j) {
            delete background[i][j];
        }
    }
    delete open_path;
    delete moving_objects;
    delete food_copy;
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

FOOD_T Map::blueprint_food[kGridSize][kGridSize] = 
    {
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::superfood, FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::superfood, FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::superfood, FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::food,      FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::superfood, FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood,    FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::food,      FOOD_T::nofood   },
        { FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood,    FOOD_T::nofood   }
    };

void Map::init() {

    for (int i = 0; i < kScreenSize; ++i) {
        for(int j = 0; j < kScreenSize; ++j) {
            bool* addr_offset = open_path + ( i * kScreenSize ) + j;
            *addr_offset = false;
            //open_path[i][j] = false;
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

            //initialize food storage with background obj
            Pacman_base* addr_offset = food_copy + ( i * kGridSize ) + j;
            *addr_offset = get_background_obj(SDL_Point{j,i}, 1); 
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

//convert screen xy -> grid xy
SDL_Point Map::get_screen_to_grid_xy(SDL_Point xy)
{
    int x = int(xy.x / get_screen_to_grid_ratio());
    int y = int(xy.y / get_screen_to_grid_ratio());
    return SDL_Point{x,y};
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

void Map::generate_food() {
    for(int i = 0; i < kGridSize; ++i) {
        for(int j = 0; j < kGridSize; ++j) {
            if ( blueprint_food[i][j] == FOOD_T::food ) {
                Pacman_base food = get_background_obj_centered(SDL_Point{j,i});
                food.size_factor = 2;
                food.color = Pacman_base::white;
                food.mode = ALIVE_T::LIVE;
                food.name = NAME_T::FOOD;
                set_food_copy(food);
            }
            else if ( blueprint_food[i][j] == FOOD_T::superfood ) {
                Pacman_base food = get_background_obj_centered(SDL_Point{j,i});
                food.size_factor = 5;
                food.color = Pacman_base::white;
                food.mode = ALIVE_T::LIVE;
                food.name = NAME_T::SUPERFOOD;
                set_food_copy(food);
            }  
        }
    }
}

void Map::place_food_to_moving_objects() {
    for(int i = 0; i < kGridSize; ++i) {
        for(int j = 0; j < kGridSize; ++j) {
            Pacman_base* food = food_copy + ( i * kGridSize ) + j;
            if ( food->name == NAME_T::FOOD ) {
                set_moving_object(*food);
            }
            else if ( food->name == NAME_T::SUPERFOOD ) {
                set_moving_object(*food);
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
            for(int i=start; i <= end; ++i)
            {
                //open_path[fixed][i] = true;
                bool* addr_offset = open_path + ( fixed * kScreenSize ) + i;
                *addr_offset = true;
                //std::cout << "open_path[" << xy.y << "][" << i << "] " << std::endl;
            }
            // std::cout << "open_path_start[" << fixed << "][" << start << "] " << std::endl;
            // std::cout << "open_path_end  [" << fixed << "][" << end << "] " << std::endl;
        }
    }

    if((xy.y+1)<=kGridSize) {
        if((blueprint[xy.y][xy.x] == GRID_T::open) && (blueprint[xy.y+1][xy.x] == GRID_T::open)) {
            int start = (xy.y*get_screen_to_grid_ratio()) + int(get_screen_to_grid_ratio()/2);
            int end = start+get_screen_to_grid_ratio();
            int fixed = (xy.x*get_screen_to_grid_ratio()) + + int(get_screen_to_grid_ratio()/2);
            for(int j=start; j <= end; ++j)
            {
                //open_path[j][fixed] = true;
                bool* addr_offset = open_path + ( j * kScreenSize ) + fixed;
                *addr_offset = true;
                //std::cout << "open_path[" << j << "][" << xy.x << "] " << std::endl;
            }
            // std::cout << "open_path_start[" << start << "][" << fixed << "] " << std::endl;
            // std::cout << "open_path_end  [" << end << "][" << fixed << "] " << std::endl;
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

void Map::set_moving_object(Pacman_base pb) 
{
    Pacman_base* addr_offset = moving_objects + ( pb.xy.y * kScreenSize ) + pb.xy.x;
    // if( addr_offset->name != NAME_T::BACKGROUND)
    // {
    //     std::cout << "setting moving object old: "  << addr_offset->name << std::endl;
    //     std::cout << "setting moving object new: "  << pb.name << std::endl;
    // }
    //over write memory with new object
    *addr_offset = pb;
};

void Map::clear_moving_object(SDL_Point xy) 
{
    Pacman_base* addr_offset = moving_objects + ( xy.y * kScreenSize ) + xy.x;
    //std::cout << "clear moving object old: " << addr_offset << " " << addr_offset->name << std::endl;
    //over write memory with new object
    *addr_offset = get_background_obj(xy, 1); ;
    //std::cout << "clear moving object new: " << addr_offset << " " << addr_offset->name << std::endl;
};

void Map::set_food_copy(Pacman_base pb) 
{
    SDL_Point xy_grid {get_screen_to_grid_xy(pb.xy)};
    // std::cout << "Setting food copy xy1[x][y] " << pb.xy.x << " " << pb.xy.y << " converted to xy2[x][y] " << xy_grid.x << " " << xy_grid.y << std::endl;

    Pacman_base* addr_offset = food_copy + ( xy_grid.y * kGridSize ) + xy_grid.x;
    // if( (addr_offset->name != NAME_T::FOOD) || (addr_offset->name != NAME_T::SUPERFOOD) )
    // {
    //     std::cout << "setting food object old: "  << addr_offset->name << std::endl;
    //     std::cout << "setting food object new: "  << pb.name << std::endl;
    // }
    //over write memory with new object
    *addr_offset = pb;
};

//xy in [screenwidth] x [screenwidth]
void Map::clear_food_copy(SDL_Point xy) 
{
    SDL_Point xy_grid {get_screen_to_grid_xy(xy)};
    // std::cout << "Clearing food copy xy1[x][y] " << xy.x << " " << xy.y << " converted to xy2[x][y] " << xy_grid.x << " " << xy_grid.y << std::endl;

    Pacman_base* addr_offset = food_copy + ( xy_grid.y * kGridSize ) + xy_grid.x;
    // if( (addr_offset->name != NAME_T::FOOD) || (addr_offset->name != NAME_T::SUPERFOOD) )
    // {
    //     std::cout << "Clearing food object old: "  << addr_offset->name << std::endl;
    // }
    //over write memory with new object
    *addr_offset = get_background_obj(xy_grid, 1);

    //also clear the moving object, otherwise it may keep food pixel
    clear_moving_object(xy);
};


STRUCT_RET Map::is_background_x_fix_y_var( int x, int y1, int y2, NAME_T name)
{
    int y_delta;
    int y_start;

    if(y1 > y2)
    {
        y_delta = y1 -y2;
        y_start = y2;
    }
    else 
    {
        y_delta = y2 - y1;
        y_start = y1;
    }

    for(int i = 0; i <= y_delta; ++i)
    {
        int this_y = y_start + i;
        if(is_valid_path(SDL_Point{x, this_y}))
        {
            Pacman_base* addr_offset = moving_objects + ( this_y * kScreenSize ) + x;
            if( addr_offset->name == name )
                return STRUCT_RET{true, SDL_Point{x, this_y}};
        }
    }
    return STRUCT_RET{false, SDL_Point{0,0}};
}

STRUCT_RET Map::is_background_x_var_y_fix( int x1, int x2, int y, NAME_T name)
{
    int x_delta;
    int x_start;

    if(x1 > x2)
    {
        x_delta = x1 -x2;
        x_start = x2;
    }
    else 
    {
        x_delta = x2 - x1;
        x_start = x1;
    }

    for(int i = 0; i <= x_delta; ++i)
    {
        int this_x = x_start + i;
        if(is_valid_path(SDL_Point{this_x, y}))
        {
            Pacman_base* addr_offset = moving_objects + ( y * kScreenSize ) + this_x;
            if( addr_offset->name == name )
                return STRUCT_RET{true, SDL_Point{this_x, y}};
        }
    }

    return STRUCT_RET{false, SDL_Point{0,0}}; 
}

STRUCT_RET Map::is_background(SDL_Point xy1, SDL_Point xy2, NAME_T name) {
    STRUCT_RET _return;
    _return = is_background_x_fix_y_var(xy1.x, xy1.y, xy2.y, name);
    if(_return.result)
    {
        std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
        return _return;
    }
    _return = is_background_x_var_y_fix(xy1.x, xy2.x, xy1.y, name);
    if(_return.result)
    {
        std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
        return _return;
    }
    _return = is_background_x_fix_y_var(xy2.x, xy1.y, xy2.y, name);
    if(_return.result)
    {
        std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
        return _return;
    }
    _return = is_background_x_var_y_fix(xy1.x, xy2.x, xy2.y, name);
    if(_return.result)
    {
        std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
        return _return;
    }
    _return = STRUCT_RET{false, SDL_Point{0,0}};
    return _return;

    // if(is_background_x_fix_y_var(xy1.x, xy1.y, xy2.y, name)) 
    // { 
    //     //std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
    //     return true; 
    // }
    // else if(is_background_x_var_y_fix(xy1.x, xy2.x, xy1.y, name)) 
    // { 
    //     //std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
    //     return true; 
    // }
    // else if(is_background_x_fix_y_var(xy2.x, xy1.y, xy2.y, name)) 
    // { 
    //     //std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
    //     return true; 
    // }
    // else if(is_background_x_var_y_fix(xy1.x, xy2.x, xy2.y, name)) 
    // { 
    //     //std::cout << "is_background " << name << " found between xy1[x][y] " << xy1.x << " " << xy1.y << " xy2[x][y] " << xy2.x << " " << xy2.y << std::endl;
    //     return true; 
    // }
    // else 
    // {
    //     return false;
    // }
};

bool Map::is_background(SDL_Point xy, NAME_T name) {
    Pacman_base* addr_offset = moving_objects + ( xy.y * kScreenSize ) + xy.x;
    return addr_offset->name == name; 
};

bool Map::is_valid_path(SDL_Point xy) {
    bool* addr_offset = open_path + ( xy.y * kScreenSize ) + xy.x;
    return *addr_offset;
};

