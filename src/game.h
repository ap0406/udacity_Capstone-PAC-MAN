#ifndef GAME_H
#define GAME_H

//#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
//#include "snake.h"
#include "map.h"
#include "pacman.h"

class Game {
 public:
  static constexpr int kNumGhosts{6};
  static constexpr std::size_t PacmanAttackTimePeriod{10000};
  Game();
  // void Run(Controller const &controller, Renderer &renderer,
  //          std::size_t target_frame_duration);
  void Run(Map &map, Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  //int GetSize() const;

 private:
  //Snake snake;
  Pacman_base pacman;
  Pacman_base ghost[kNumGhosts];

  bool pacman_attack_mode;
  bool game_over;
  uint32_t attacktimestart;
  uint32_t attacktimeend;

  int score{0};

  void InitGame(Map &map);
  void Update(Map &map);
  void SetMovingObjects(Map &map);
  void ClearMovingObjects(Map &map);
  void attackmodeon();
  void attackmodeoff();
  void ghost_init(Map&, uint8_t);
  void ghost_update(Map&, uint8_t);
};

#endif