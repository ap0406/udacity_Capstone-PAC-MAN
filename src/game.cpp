#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game()
    : engine(dev()),
      random_w(0, static_cast<int>(Map::kGridSize - 1)),
      random_h(0, static_cast<int>(Map::kGridSize - 1)),
      random_dir(0, 4) {
  pacman_attack_mode = false;
}

void Game::Run(Map &map, Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  InitGame(map);

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    controller.HandleInput(running, pacman);
    
    //running = false;
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

void Game::InitGame(Map &map) {
    map.place_food_to_moving_objects();

    pacman = Pacman_base(NAME_T::PACMAN, 1, 10, map.get_pacman_start_point(), ALIVE_T::LIVE, Pacman_base::yellow);
    map.set_moving_object(pacman);

    ghost[0] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(0,-1), ALIVE_T::LIVE, Pacman_base::red);
    ghost[0].set_direction(Direction::kDown);
    map.set_moving_object(ghost[0]);

    ghost[1] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(2,-1), ALIVE_T::LIVE, Pacman_base::pink);
    ghost[1].set_direction(Direction::kDown);
    map.set_moving_object(ghost[1]);
    // ghost[1] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(0,0), ALIVE_T::LIVE, Pacman_base::cyan);
    // ghost[2] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(1,0), ALIVE_T::LIVE, Pacman_base::orange);
    // ghost[3] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(-1,0), ALIVE_T::LIVE, Pacman_base::pink);

}

void Game::SetMovingObjects(Map &map) {

    map.place_food_to_moving_objects();
    map.set_moving_object(pacman);

    for (int i = 0; i < kNumGhosts; ++i) {
      map.set_moving_object(ghost[i]);
    }

}

void Game::ClearMovingObjects(Map &map) {

    for (int i = 0; i < kNumGhosts; ++i) {
      map.clear_moving_object(ghost[i].prev_xy);
    }

    map.clear_moving_object(pacman.prev_xy);
}

void Game::Update(Map &map) {

  //update pacman to new location
  pacman.update(map);

  STRUCT_RET _return = map.is_background(pacman.prev_xy, pacman.xy, NAME_T::FOOD);
  if (_return.result) {
    score++;
    //clear food
    map.clear_food_copy(_return.xy);
  }

  _return = map.is_background(pacman.prev_xy, pacman.xy, NAME_T::SUPERFOOD);
  if (_return.result) {
    score += 50;
    //set attack mode
    pacman_attack_mode = true;
    //change pacman speed to fast
    pacman.speed_factor = 2;
    //clear food
    map.clear_food_copy(_return.xy);
  }

  ghost[0].update(map, pacman.xy, GHOST_MODE_T::CHASE);
  SDL_Point g1_xy = ghost[1].find_nearest_offset(map, pacman.xy, map.get_screen_to_grid_ratio(), 3*map.get_screen_to_grid_ratio(), pacman.direction_latch);
  // std::cout << " pacman[y][x] " << pacman.xy.y << " " << pacman.xy.x << " ghost[1][y][x] " << g1_xy.y << " " << g1_xy.x << std::endl;
  ghost[1].update(map, g1_xy, GHOST_MODE_T::CHASE);

  // ghost[0].update_rand(map, random_dir(engine));
  // //clear ghost from old location
  // map.clear_moving_object(ghost[0].prev_xy);
  // //set ghost to new location
  // map.set_moving_object(ghost[0]);
  
  
  // ghost[1].update_rand(map, random_dir(engine));
  // ghost[2].update_rand(map, random_dir(engine));
  // ghost[3].update_rand(map, random_dir(engine));

  ClearMovingObjects(map);
  SetMovingObjects(map);
  
}


int Game::GetScore() const { return score; }
//int Game::GetSize() const { return snake.size; }