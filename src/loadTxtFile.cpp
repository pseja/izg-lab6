#include<fstream>

#include<error.hpp>

#include<loadTxtFile.hpp>

std::string loadTxtFile(std::string const&fileName){
  auto file = std::ifstream(fileName);
  if(!file.is_open()){
    error("cannot open file: "+fileName,"text file: "+fileName+" cannot be opened");
    return "";
  }
  std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
  return str;
}

