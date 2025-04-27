#pragma once

#include<map>
#include<functional>

#include<SDL.h>

struct SDL{
  SDL_Window*   window         ;
  SDL_GLContext context        ;
  bool          running = false;
  SDL(uint32_t w,uint32_t h);
  ~SDL();
  using EventCall = std::function<void(SDL_Event const&)>;
  std::map<uint32_t,EventCall>handlers;
  void handleEvent(SDL_Event const&);
  std::function<void()>draw = nullptr;
  void eventloop();
  void mainloop();
};


