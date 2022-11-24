#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game()
    : engine(dev()),
      random_w(0, static_cast<int>(Map::kGridWidth - 1)),
      random_h(0, static_cast<int>(Map::kGridHeight - 1)),
      random_dir(0, 4) {
  //PlaceFood();
}


void Game::Run(Map &map, Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  InitGame(map);

  SetMovingObjects(map);

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    controller.HandleInput(running, pacman);
    
    Update(map);
    //renderer.Render(snake, food);
    renderer.Render(map); 

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}
void Game::PlaceFood(Map &map) {   
    for(int i = 0; i < Map::kGridHeight; ++i) {
        for(int j = 0; j < Map::kGridWidth; ++j) {
            SDL_Point xy = {j, i};
            if(map.is_map_open(xy) && !(pacman.is_same_cell(xy) || ghost[0].is_same_cell(xy) || ghost[1].is_same_cell(xy) || ghost[2].is_same_cell(xy) || ghost[3].is_same_cell(xy) || super_food[0].is_same_cell(xy) || super_food[1].is_same_cell(xy) || super_food[2].is_same_cell(xy) || super_food[3].is_same_cell(xy))) 
            {

                //std::cout << "food[x] " << j << " food[y] "<< i << std::endl;
                food.emplace_back(Pacman_base(NAME_T::FOOD, 0.0, 0.25, xy, ALIVE_T::LIVE, Pacman_base::white));
            }
        }
    }
}
void Game::PlaceSuperFood(Map &map) {
    int placed{0};
    while (true) {
      SDL_Point xy{random_w(engine), random_h(engine)};
      // Check that the location is not occupied by oridinary food or pacman item before placing
      // Super food.
      if(map.is_map_open(xy) && !(pacman.is_same_cell(xy) || ghost[0].is_same_cell(xy) || ghost[1].is_same_cell(xy) || ghost[2].is_same_cell(xy) || ghost[3].is_same_cell(xy))) 
      {
        //Pacman_base pb (0.0, 0.5, xy, ALIVE_T::DEAD, Pacman_base::white);
        //std::cout << "super food[x] " << xy.x << " super_food[y] "<< xy.y << std::endl;
        super_food[placed] = Pacman_base(NAME_T::SUPERFOOD, 0.0, 0.5, xy, ALIVE_T::LIVE, Pacman_base::white);
        
        ++placed;
        if (placed == 4) 
          return;
      }
    }
}

void Game::InitGame(Map &map) {
    pacman = Pacman_base(NAME_T::PACMAN, 0.01, 0.75, map.get_pacman_start_point(), ALIVE_T::LIVE, Pacman_base::yellow);

    ghost[0] = Pacman_base(NAME_T::GHOST, 1.0, 0.75, map.get_ghost_start_point(0,-1), ALIVE_T::LIVE, Pacman_base::red);
    ghost[1] = Pacman_base(NAME_T::GHOST, 0.9, 0.75, map.get_ghost_start_point(), ALIVE_T::LIVE, Pacman_base::cyan);
    ghost[2] = Pacman_base(NAME_T::GHOST, 0.75, 0.75, map.get_ghost_start_point(1,0), ALIVE_T::LIVE, Pacman_base::orange);
    ghost[3] = Pacman_base(NAME_T::GHOST, 0.5, 0.75, map.get_ghost_start_point(-1,0), ALIVE_T::LIVE, Pacman_base::pink);

    PlaceSuperFood(map);

    PlaceFood(map);
}

void Game::SetMovingObjects(Map &map) {

    for (auto const i : food) {
      map.set_moving_object(i);
    }

    for (auto i = 0; i < 4; ++i) {
      map.set_moving_object(super_food[i]);
    }

    for (auto i = 0; i < 4; ++i) {
      map.set_moving_object(ghost[i]);
    }

    map.set_moving_object(pacman);
}

void Game::ClearMovingObjects(Map &map) {

    for (auto const i : food) {
      map.clear_moving_object(i.xy);
    }

    for (auto i = 0; i < 4; ++i) {
      map.clear_moving_object(super_food[i].xy);
    }

    for (auto i = 0; i < 4; ++i) {
      map.clear_moving_object(ghost[i].xy);
    }

    map.clear_moving_object(pacman.xy);
}

void Game::Update(Map &map) {

  //ClearMovingObjects(map);

  //update pacman to new location
  pacman.update(map);

  if (map.is_background_food(pacman.xy)) {
    score++;
    //clear food
    map.clear_moving_object(pacman.xy);
  }
  //clear pacman from old location
  map.clear_moving_object(pacman.prev_xy);
  //set pacman to new location
  map.set_moving_object(pacman);

  ghost[0].update_rand(map, random_dir(engine));
  //clear ghost from old location
  map.clear_moving_object(ghost[0].prev_xy);
  //set ghost to new location
  map.set_moving_object(ghost[0]);
  
  
  ghost[1].update_rand(map, random_dir(engine));
  ghost[2].update_rand(map, random_dir(engine));
  ghost[3].update_rand(map, random_dir(engine));

  //UpdateMovingObjects(map);
  /*
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  */
  
}

int Game::GetScore() const { return score; }
//int Game::GetSize() const { return snake.size; }