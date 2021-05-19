/**
 * @file main.h
 * @authors Peter Zdravecký, Eduard Frlička
 * @brief Header file for simulator
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <fstream>
#include <iostream>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>

#include "device.h"

#define CONF "examples/simulator.conf"

using namespace std;

/**
 * @brief Reads file and returns plain binary data in string
 * Function copied from internet
 *
 * @param path path to file
 *
 * @see https://stackoverflow.com/questions/18816126/c-read-the-whole-file-in-buffer
 */

inline std::string slurp(const std::string &path) {
    std::ostringstream buf;
    std::ifstream input(path.c_str());
    buf << input.rdbuf();
    return buf.str();
}