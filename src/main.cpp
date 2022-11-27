#include <iostream>
#include "controller.h"
#include "game.h"
#include "map.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};

  //Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  //Controller controller;
  //Game game(kGridWidth, kGridHeight);
  //game.Run(controller, renderer, kMsPerFrame);

  Map map;
  Renderer renderer(Map::kScreenSize, Map::kScreenSize, Map::kGridSize, Map::kGridSize);
  Game game;
  Controller controller;
  game.Run(map, controller, renderer, kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  //std::cout << "Score: " << game.GetScore() << "\n";
  //std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}