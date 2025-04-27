#include<sdl.hpp>

SDL::SDL(uint32_t w,uint32_t h){
  SDL_LogSetPriority(
      SDL_LOG_CATEGORY_APPLICATION,
      SDL_LOG_PRIORITY_INFO);

  if(SDL_Init(SDL_INIT_VIDEO)<0)
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_Init fail: %s\n",
        SDL_GetError());

  window = SDL_CreateWindow(
      "izg_lab06",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      w,
      h,
      SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  if (!window)
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_CreateWindow fail: %s\n",
        SDL_GetError());

  context = SDL_GL_CreateContext(window);
  if (!context)
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION,
        "SDL_GL_CreateContext fail: %s\n",
        SDL_GetError());
}

SDL::~SDL(){
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void SDL::mainloop(){
  running = true;
  while(running){
    eventloop();
    if(draw)draw();
    SDL_GL_SwapWindow(window);
  }
}

void SDL::eventloop(){
  SDL_Event event;
  while(SDL_PollEvent(&event))
    handleEvent(event);
}

void SDL::handleEvent(SDL_Event const&event){
  if(event.type==SDL_QUIT)running=false;
  if(handlers.count(event.type))handlers[event.type](event);
}


