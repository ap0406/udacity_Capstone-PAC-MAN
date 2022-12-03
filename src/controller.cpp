#include "controller.h"
#include <iostream>
#include "SDL.h"
//#include "snake.h"

// void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
//                                  Snake::Direction opposite) const {
//   if (snake.direction != opposite || snake.size == 1) snake.direction = input;
//   return;
// }

void Controller::HandleInput(bool &running, Pacman_base &pb) const {
  SDL_Event e;
  //pb.direction = Direction::noChange;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          pb.set_direction(Direction::kUp);
          break;

        case SDLK_DOWN:
          pb.set_direction(Direction::kDown);
          break;

        case SDLK_LEFT:
          pb.set_direction(Direction::kLeft);
          break;

        case SDLK_RIGHT:
          pb.set_direction(Direction::kRight);
          break;
      }
    }
  }
}

// void Controller::HandleInput(bool &running) const {
//   SDL_Event e;
//   while (SDL_PollEvent(&e)) {
//     if (e.type == SDL_QUIT) {
//       running = false;
//     } 
//   }
// }