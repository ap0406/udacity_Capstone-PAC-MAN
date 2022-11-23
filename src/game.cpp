#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game()
    : engine(dev()),
      random_w(0, static_cast<int>(Map::kGridWidth - 1)),
      random_h(0, static_cast<int>(Map::kGridHeight - 1)) {
  //PlaceFood();
}


void Game::Run(Map &map, Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  map.conv_background_to_objects();

  InitGame(map);

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

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    controller.HandleInput(running);
    
    //Update();
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
            if(map.is_map_open(xy) && !(pacman.is_same_cell(xy) || ghost[0].is_same_cell(xy) || ghost[1].is_same_cell(xy) || ghost[2].is_same_cell(xy) || ghost[3].is_same_cell(xy) || super_food[0].is_same_cell(xy) || super_food[1].is_same_cell(xy) || super_food[2].is_same_cell(xy) || super_food[3].is_same_cell(xy))) {
                Pacman_base pb;
                pb.xy.x = j;
                pb.xy.y = i;
                pb.size_factor = 0.25;
                //std::cout << "food[x] " << j << " food[y] "<< i << std::endl;
                pb.mode = ALIVE_T::DEAD;
                pb.color = Pacman_base::white;
                food.emplace_back(pb);
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
      if(map.is_map_open(xy) && !(pacman.is_same_cell(xy) || ghost[0].is_same_cell(xy) || ghost[1].is_same_cell(xy) || ghost[2].is_same_cell(xy) || ghost[3].is_same_cell(xy))) {
        Pacman_base pb;
        pb.xy = xy;
        pb.size_factor = 0.5;
        std::cout << "super food[x] " << xy.x << " super_food[y] "<< xy.y << std::endl;
        pb.mode = ALIVE_T::DEAD;
        pb.color = Pacman_base::white;
        super_food[placed] = pb;
        
        ++placed;
        if (placed == 4) 
          return;
      }
    }
}

void Game::InitGame(Map &map) {
    pacman.xy = map.get_pacman_start_point();
    pacman.mode = ALIVE_T::LIVE;
    pacman.color = Pacman_base::yellow;
    pacman.speed_factor = 0.75;
    pacman.size_factor = 0.75;

    ghost[0].xy = map.get_ghost_start_point(0,-1);
    ghost[0].mode = ALIVE_T::LIVE;
    ghost[0].color = Pacman_base::red;
    ghost[0].speed_factor = 1.0;
    ghost[0].size_factor = 0.75;

    ghost[1].xy = map.get_ghost_start_point();
    ghost[1].mode = ALIVE_T::LIVE;
    ghost[1].color = Pacman_base::cyan;
    ghost[1].speed_factor = 0.9;
    ghost[1].size_factor = 0.75;

    ghost[2].xy = map.get_ghost_start_point(1,0);
    ghost[2].mode = ALIVE_T::LIVE;
    ghost[2].color = Pacman_base::orange;
    ghost[2].speed_factor = 0.75;
    ghost[2].size_factor = 0.75;

    ghost[3].xy = map.get_ghost_start_point(-1,0);
    ghost[3].mode = ALIVE_T::LIVE;
    ghost[3].color = Pacman_base::pink;
    ghost[3].speed_factor = 0.5;
    ghost[3].size_factor = 0.75;

    PlaceSuperFood(map);

    PlaceFood(map);
}
/*
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}
*/

void Game::Update() {
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