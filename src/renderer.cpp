#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}
/*
void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}
*/
void Renderer::Render(Map &map) {

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x0, 0x0, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render background
  // Pacman_base* background = map.get_background();
  // Pacman_base* start = background;
  // Pacman_base* end = start+2; //*sizeof(Pacman_base));
  for (int i=0; i<grid_height; ++i) {
      for (int j=0; j<grid_width; ++j) {
          Pacman_base* pb = map.get_background(i,j);
          //std::cout << "rendered: " << pb << std::endl;
          SDL_Rect rect = map.get_sdl_rect(pb);
          SDL_SetRenderDrawColor(sdl_renderer, pb->color.r, pb->color.g, pb->color.b, 0xFF);
          // if ( i->name == NAME_T::PACMAN ) {
          //   std::cout << "background: " << i->xy.x << " " << i->xy.y << " " << i->name << std::endl;
          //   std::cout << "background: " << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << std::endl;
          // }
          SDL_RenderFillRect(sdl_renderer, &rect);
      }
  }
  for (int i = 0; i < int(screen_height); ++i) {
      for(int j = 0; j < int(screen_width); ++j) {
          Pacman_base* pb = map.get_moving_objects() + ( i * screen_height ) + j;
          if(pb->mode == ALIVE_T::LIVE) {
              SDL_Rect rect = map.get_sdl_rect(pb);
              SDL_SetRenderDrawColor(sdl_renderer, pb->color.r, pb->color.g, pb->color.b, 0xFF);
              SDL_RenderFillRect(sdl_renderer, &rect);
          }
          //std::cout << "moving objects offset" << addr_offset << std::endl;
      }
  }

  // for (int i = 0; i < screen_height; ++i){
  //     for (int j = 0; j < screen_height; ++j){
  //         bool* add_offset = map.get_open_path() + ( i * screen_height ) + j;
  //         if(*add_offset) {
  //         //if (map.is_path(SDL_Point{j,i})) {
  //             //std::cout << "renderer open_path[" << i << "][" << j << "]: " << std::endl;
  //             Pacman_base pb = Pacman_base(NAME_T::BACKGROUND, 0.0, 1.0, SDL_Point{j,i}, ALIVE_T::DEAD, Pacman_base::white);
  //             SDL_Rect rect = map.get_sdl_rect(&pb);
  //             SDL_SetRenderDrawColor(sdl_renderer, pb.color.r, pb.color.g, pb.color.b, 0xFF);
  //             SDL_RenderFillRect(sdl_renderer, &rect);
  //         }
  //     }
  // }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Pacman Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
