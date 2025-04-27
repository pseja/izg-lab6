#include<iostream>

#include<SDL.h>

#include<error.hpp>

extern bool errorCmd;

void error(std::string const&name,std::string const&msg){
  if(errorCmd){
    std::cerr << name << std::endl;
    std::cerr << msg << std::endl;
  }else{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,name.c_str(),msg.c_str(),nullptr);
  }
}

