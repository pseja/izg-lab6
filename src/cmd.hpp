#pragma once

#include<ArgumentViewer/ArgumentViewer.h>

struct Cmd{
  std::shared_ptr<argumentViewer::ArgumentViewer>args;
  std::vector<int32_t>windowSize;
  std::string         shaderFile;
  uint32_t            triangles ;
  std::string         version   ;
  bool                errorCmd  ;
  bool                printHelp ;
  Cmd(int argc,char*argv[]);
  void printHelpIfNeeded()const;
  bool shouldQuit()const;
};
