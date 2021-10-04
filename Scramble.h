#pragma once
#include <string>
#include "puzzle.h"

class Scramble {
public:
    std::string name;
    Puzzle* state;
    int startDepth;
    int maxDepth;
    int slack;
    int maxSolutions;
};