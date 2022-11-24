#ifndef CONTROLLER_H
#define CONTROLLER_H

//#include "snake.h"
#include "pacman.h"

class Controller {
 public:
  void HandleInput(bool &running, Pacman_base &pb) const;
  //void HandleInput(bool &running) const;

 private:
  //void ChangeDirection(Snake &snake, Snake::Direction input,
  //                     Snake::Direction opposite) const;
};

#endif