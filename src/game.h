#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
//#include "snake.h"
#include "map.h"
#include "pacman.h"

class Game {
 public:
  Game();
  // void Run(Controller const &controller, Renderer &renderer,
  //          std::size_t target_frame_duration);
  void Run(Map &map, Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  //int GetSize() const;

 private:
  //Snake snake;
  //SDL_Point food;
  std::vector<Pacman_base> food;
  Pacman_base super_food[4];
  Pacman_base pacman;
  Pacman_base ghost[4];

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> random_dir;

  int score{0};

  void InitGame(Map &map);
  void Update(Map &map);
  void SetMovingObjects(Map &map);
  void ClearMovingObjects(Map &map);
};

#endif