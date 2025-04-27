#include<cmd.hpp>

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif

Cmd::Cmd(int argc,char*argv[]){
  args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  windowSize  = args->geti32v  ("--window-size",{500,500}                                          ,"size of the window"     );
  shaderFile  = args->gets     ("--shader"     ,std::string(CMAKE_ROOT_DIR)+"/shaders/student.glsl","shader file"            );
  triangles   = args->getu32   ("--triangles"  ,10000                                              ,"number of triangles"    );
  version     = args->gets     ("--version"    ,"330"                                              ,"GLSL version, try \"330\", \"300 es\", \"450\", ...");
  errorCmd         = args->isPresent("--cmd-error"  ,"prints error to console");
  printHelp   = args->isPresent("-h"                                                                    ,"prints help"            );
  printHelp       |= args->isPresent("--help"                                                                ,"prints help"            );
}

void Cmd::printHelpIfNeeded()const{
  if(!shouldQuit())return;
  std::cerr << args->toStr() << std::endl;
}

bool Cmd::shouldQuit()const{
  if(!printHelp && args->validate())return false;
  return true;
}
