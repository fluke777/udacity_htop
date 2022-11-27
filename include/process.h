#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>
// #include "system.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class System;
class Process {
 public:
  Process(int pid, System* system) : pid_{pid}, system_{system}  {
  };
  std::string Path();
  int Pid() const;                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  // bool operator<(Process& a);  // TODO: See src/process.cpp

 private:
   int pid_{0};
   System* system_{};
};

#endif