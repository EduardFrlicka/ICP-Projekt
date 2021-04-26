#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <unistd.h>

#include "device.h"

#define CONF "/mnt/Programing/ICP/Projekt/src/simulator/sim.conf"



using namespace std;


inline std::string slurp (const std::string& path) { // borrowed from https://stackoverflow.com/questions/18816126/c-read-the-whole-file-in-buffer
  std::ostringstream buf; 
  std::ifstream input (path.c_str()); 
  buf << input.rdbuf(); 
  return buf.str();
}