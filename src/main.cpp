#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <functional>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BasicCamera/OrbitCamera.h>
#include <BasicCamera/PerspectiveCamera.h>

#include <ArgumentViewer/ArgumentViewer.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#include <timer.hpp>
#include <main.hpp>

#include <sdl.hpp>
#include <cmd.hpp>

using namespace ge::gl;

bool errorCmd=false;

struct Prg{

};

int main(int argc,char*argv[]){
  auto cmd = Cmd(argc,argv);
  cmd.printHelpIfNeeded();
  if(cmd.shouldQuit())return EXIT_SUCCESS;
  errorCmd = cmd.errorCmd;

  auto sdl = SDL(cmd.windowSize[0],cmd.windowSize[1]);

  ge::gl::init(SDL_GL_GetProcAddress);

  basicCamera::OrbitCamera       orbitCamera      ;
  basicCamera::PerspectiveCamera perspectiveCamera;

  auto initCamera = [&](){
    orbitCamera = basicCamera::OrbitCamera{};
    orbitCamera.addDistance(3.f);
    orbitCamera.addXAngle(glm::radians(30.f));
    orbitCamera.addYAngle(glm::radians(-30.f));
  };

  initCamera();

  perspectiveCamera.setNear(0.01f);
  perspectiveCamera.setFar(1000.f);
  perspectiveCamera.setFovy(glm::radians(90.f));
  perspectiveCamera.setAspect(1.f);
   
  float sensitivity = 0.01f;
  float zoomSpeed = 0.02f;

  glEnable(GL_DEPTH_TEST);

  auto src = loadTxtFile(cmd.shaderFile);

  std::map<GLenum,GLuint>shaders;
  GLuint prg   =  0;
  GLint  viewL = -1;
  GLint  projL = -1;
  GLint  resL  = -1;
  GLint  timeL = -1;
  GLenum mode = GL_TRIANGLES;

  auto deleteProgram = [&](){
    if(glIsProgram(prg))glDeleteProgram(prg);
    prg = 0;
    for(auto&s:shaders)
      if(glIsShader(s.second)){
        glDeleteShader(s.second);
        s.second = 0;
      }
  };


  auto reCreateProgram = [&](){
    deleteProgram();

    for(auto&s:shaders){
      auto const&name = shaderTypeToName(s.first);
      if(!hasSubstring(src,name))continue;
      shaders[s.first] = createShader(s.first,
          std::string("")+
          "#version "+cmd.version+
          "\n#define "+name+"\n"+src);
    }

    if(glIsShader(shaders[GL_TESS_CONTROL_SHADER])){
      mode = GL_PATCHES;
    }else{
      if(glIsShader(shaders[GL_GEOMETRY_SHADER])){
        GLint m;
        glGetProgramiv(prg, GL_GEOMETRY_INPUT_TYPE,&m);
        mode = m;
      }else
        mode = GL_TRIANGLES;
    }

    prg   = createProgram(shaders);
    viewL = glGetUniformLocation(prg,"view");
    projL = glGetUniformLocation(prg,"proj");
    resL  = glGetUniformLocation(prg,"iResolution");
    timeL = glGetUniformLocation(prg,"iTime");
  };

  shaders[GL_VERTEX_SHADER] = 0;
  shaders[GL_FRAGMENT_SHADER] = 0;
  shaders[GL_GEOMETRY_SHADER] = 0;
  shaders[GL_TESS_CONTROL_SHADER] = 0;
  shaders[GL_TESS_EVALUATION_SHADER] = 0;
  reCreateProgram();


  GLuint vao;
  glCreateVertexArrays(1,&vao);

  auto timer = Timer<float>();
  float lastLoadingTime = timer.elapsedFromStart();

  sdl.handlers[SDL_MOUSEMOTION] = [&](SDL_Event const&event){
    auto xrel=event.motion.xrel;
    auto yrel=event.motion.yrel;
    if(event.motion.state & SDL_BUTTON_LMASK){
      orbitCamera.addXAngle(yrel * sensitivity);
      orbitCamera.addYAngle(xrel * sensitivity);
    }
    if(event.motion.state & SDL_BUTTON_RMASK){
      orbitCamera.addDistance(yrel * zoomSpeed);
    }
    if(event.motion.state & SDL_BUTTON_MMASK){
      int width,height;
      SDL_GetWindowSize(sdl.window,&width,&height);
      orbitCamera.addXPosition(+orbitCamera.getDistance() * xrel /
                                float(width) * 2.f);
      orbitCamera.addYPosition(-orbitCamera.getDistance() * yrel /
                                float(height) * 2.f);
    }
  };

  sdl.handlers[SDL_KEYDOWN] = [&](SDL_Event const&event){
    if(event.key.keysym.sym == SDLK_r){
      std::cerr << "shader reloaded" << std::endl;
      src = loadTxtFile(cmd.shaderFile);
      reCreateProgram();
    }
    if(event.key.keysym.sym == SDLK_c){
      initCamera();
    }
    if(event.key.keysym.sym == SDLK_f){
      auto flags = SDL_GetWindowFlags(sdl.window);
      if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
        SDL_SetWindowFullscreen(sdl.window, 0);
      else
        SDL_SetWindowFullscreen(sdl.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
  };

  sdl.handlers[SDL_WINDOWEVENT] = [&](SDL_Event const&event){
    if(event.window.event== SDL_WINDOWEVENT_RESIZED){
      auto const&w=event.window;
      glViewport(0,0,w.data1,w.data2);
      perspectiveCamera.setAspect((float)w.data1/(float)w.data2);
    }
  };

  sdl.draw = [&](){
    auto time = timer.elapsedFromStart();
    if(time - lastLoadingTime>1){
      std::cerr << "load" << std::endl;
      auto newSrc = loadTxtFile(cmd.shaderFile);
      if(newSrc != src){
        src = newSrc;
        reCreateProgram();
      }
      lastLoadingTime = time;
    }

    glClearColor(0.5,0.5,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    auto viewMatrix = orbitCamera.getView();
    auto projMatrix = perspectiveCamera.getProjection();


    glUseProgram(prg);
    glBindVertexArray(vao);

    glUniformMatrix4fv(viewL,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projL,1,GL_FALSE,glm::value_ptr(projMatrix));
    glUniform1f(timeL,timer.elapsedFromStart());

    int w,h;
    SDL_GetWindowSize(sdl.window,&w,&h);
    glUniform2f(resL,w,h);

    glPointSize(4);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(mode,0,cmd.triangles*3);


  };


  sdl.mainloop();


  deleteProgram();
  glDeleteVertexArrays(1,&vao);


  return 0;
}


std::string shaderTypeToName(GLuint type){
  if(type==GL_VERTEX_SHADER         )return "VERTEX_SHADER"         ;
  if(type==GL_FRAGMENT_SHADER       )return "FRAGMENT_SHADER"       ;
  if(type==GL_TESS_CONTROL_SHADER   )return "TESS_CONTROL_SHADER"   ;
  if(type==GL_TESS_EVALUATION_SHADER)return "TESS_EVALUATION_SHADER";
  if(type==GL_GEOMETRY_SHADER       )return "GEOMETRY_SHADER"       ;
  return "unknown";
}

GLuint createShader(GLuint type,std::string const&src){
  GLuint vs = glCreateShader(type);
  char const*vsSrc[1]={
    src.c_str()
  };

  glShaderSource(vs,1,vsSrc,nullptr);
  glCompileShader(vs);
  int compileStatus;
  glGetShaderiv(vs,GL_COMPILE_STATUS,&compileStatus);
  if(compileStatus != GL_TRUE){
    uint32_t const msgLen = 1<<11;
    char msg[msgLen];
    glGetShaderInfoLog(vs,msgLen,nullptr,msg);
    error(shaderTypeToName(type)+" shader compilation error",msg);
  }
  return vs;
}

GLuint createProgram(std::map<GLenum,GLuint>const&shaders){
  GLuint prg = glCreateProgram();

  for(auto const&s:shaders)
    glAttachShader(prg,s.second);

  glLinkProgram(prg);
  GLint linkStatus;
  glGetProgramiv(prg,GL_LINK_STATUS,&linkStatus);
  if(linkStatus != GL_TRUE){
    uint32_t const msgLen = 1<<11;
    char msg[msgLen];
    glGetProgramInfoLog(prg,msgLen,nullptr,msg);
    error("program linking error",msg);
  }
  return prg;
}

bool hasSubstring(std::string const&txt,std::string w){
  return txt.find(w) != std::string::npos;
}


