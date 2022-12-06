#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game()
{
  pacman_attack_mode = false;
}

void Game::Run(Map &map, Controller const &controller, Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  Uint32 attackperiod;

  int frame_count = 0;
  bool running = true;

  InitGame(map);

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    controller.HandleInput(running, pacman);
    
    if(!game_over)
    {
      Update(map);
    }

    //renderer.Render(snake, food);
    renderer.Render(map); 

    frame_end = SDL_GetTicks();

    attackperiod = frame_end - attacktimestart;
    if(attackperiod>=PacmanAttackTimePeriod)
    {
      attackmodeoff();
    }

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

void Game::ghost_init(Map &map, uint8_t num)
{
    switch (num)
    {
      case 0 : ghost[0] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(0,-1), ALIVE_T::LIVE, Pacman_base::red); break;
      case 1 : ghost[1] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(2,-1), ALIVE_T::LIVE, Pacman_base::pink); break;
      case 2 : ghost[2] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(4,-1), ALIVE_T::LIVE, Pacman_base::cyan); break;
      case 3 : ghost[3] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(6,-1), ALIVE_T::LIVE, Pacman_base::orange); break;
      case 4 : ghost[4] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(0, 1), ALIVE_T::LIVE, Pacman_base::green); break;
      case 5 : ghost[5] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(6, 2), ALIVE_T::LIVE, Pacman_base::blue); break;
      default: ghost[num] = Pacman_base(NAME_T::GHOST, 1, 10, map.get_ghost_start_point(6, 2), ALIVE_T::LIVE, Pacman_base::blue); break;
    }
}

void Game::ghost_update(Map& map, uint8_t num)
{
    switch(num)
    {
      case 0 : 
      {
        ghost[num].update(map, pacman.xy); 
        break;
      }
      case 1 : 
      {
        SDL_Point g1_xy = ghost[num].find_nearest_offset(map, pacman.xy, 4*map.get_screen_to_grid_ratio(), 8*map.get_screen_to_grid_ratio(), pacman.direction_latch);
        // std::cout << " pacman[y][x] " << pacman.xy.y << " " << pacman.xy.x << " ghost[1][y][x] " << g1_xy.y << " " << g1_xy.x << std::endl;
        ghost[num].update(map, g1_xy);
        break;
      }
      case 2 :
      {
        SDL_Point g2_xy = ghost[num].find_nearest_offset(map, pacman.xy, 6*map.get_screen_to_grid_ratio(), 10*map.get_screen_to_grid_ratio(), pacman.direction_latch);
        ghost[num].update(map, g2_xy);
        break;
      }
      default :
      {
        ghost[num].rand_update(map);
        break;
      }
    }

}

void Game::InitGame(Map &map) {
    game_over = false;

    map.place_food_to_moving_objects();

    pacman = Pacman_base(NAME_T::PACMAN, 1, 10, map.get_pacman_start_point(), ALIVE_T::LIVE, Pacman_base::yellow);
    map.set_moving_object(pacman);

    for (int i = 0; i < kNumGhosts; ++i) 
    {
        ghost_init(map, i);
        map.set_moving_object(ghost[i]);
    }

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

void Game::attackmodeon()
{
    score += 50;
    
    pacman_attack_mode = true;

    attacktimestart = SDL_GetTicks(); 

    pacman.speed_factor = 2;

    for (int i = 0; i < kNumGhosts; ++i) {
      //this prevents losing the original color if pacman
      //eats superfood twice
      if (ghost[i].ghost_mode != GHOST_MODE_T::RUNAWAY)
      {
        ghost[i].set_new_color(Pacman_base::white);
        ghost[i].ghost_mode = GHOST_MODE_T::RUNAWAY;
      }
    }

}

void Game::attackmodeoff()
{
    
    pacman_attack_mode = false;

    pacman.speed_factor = 1;

    for (int i = 0; i < kNumGhosts; ++i) {
      ghost[i].ghost_mode = GHOST_MODE_T::CHASE;
      ghost[i].set_prev_color();
    }

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
    //set attack mode
    attackmodeon();
    //clear food
    map.clear_food_copy(_return.xy);
  }

  if(pacman_attack_mode)
  {
      for (int i = 0; i < kNumGhosts; ++i) {
          ghost[i].rand_update(map);
      }
  }
  else
  {
      for (int i = 0; i < kNumGhosts; ++i) {
            ghost_update(map, i);
        }
  }
  

  for (int i = 0; i < kNumGhosts; ++i) 
  {
      if(pacman.is_same_location(&ghost[i]) && pacman_attack_mode && (ghost[i].ghost_mode == GHOST_MODE_T::RUNAWAY))
      {
        score += 100;
        //std::cout << "pacman eaten ghost" << std::endl;
        map.clear_moving_object(ghost[i].prev_xy);
        //std::cout << "clear ghost " << ghost[i].xy.y << " " << ghost[i].xy.x << std::endl;
        ghost_init(map, i);
        //std::cout << "init ghost " << ghost[i].xy.y << " " << ghost[i].xy.x << std::endl;
      }
      else if(pacman.is_same_location(&ghost[i]))
      {
        game_over = true;
      }
  }

  ClearMovingObjects(map);
  SetMovingObjects(map);
  
}


int Game::GetScore() const { return score; }
//int Game::GetSize() const { return snake.size; }